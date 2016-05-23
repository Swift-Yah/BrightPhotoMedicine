var SerialService = (function () {
    var logObj = function (object) {
        console.log(object);
    };

    var log = function (message) {
        console.log(message);
    };

    function Serial() {
        this.onReceive = new chrome.Event();
        this.onError = new chrome.Event();
        this.onClose = new chrome.Event();
        this.device = null;
        this.ui = UI.getInstance();
    }

    // Global instance
    var instance;

    // Public methods:

    Serial.prototype.init = function () {
        var self = this;

        self.listDevices();
        self.openDevice();
    };

    Serial.prototype.initConnection = function (connectionId) {
        var self = this;g

        chrome.serial.onReceive.addListener(function (receiveInfo) {
            logObj(receiveInfo);

            if (receiveInfo.connectionId == connectionId) {
                var string = arrayBufferToString(receiveInfo.data);

                UI.getInstance().showMessageFromSerial(string);

                self.onReceive.dispatch(string);
            }
        });

        chrome.serial.onReceiveError.addListener(function (errorInfo) {
            logObj(errorInfo);

            if (errorInfo.connectionId == connectionId) {
                self.onError.dispatch(errorInfo.error);
            }
        });
    };

    Serial.prototype.close = function () {
        chrome.serial.disconnect(connectionId, function (success) {
            if (success) {
                self.onClose.dispatch();
            }
        })
    };

    Serial.prototype.send = function (message) {
        var newString = stringToArrayBuffer(message);

        chrome.serial.send(connectionId, newString, function () {});
    };

    Serial.prototype.getDevices = function (callback) {
        chrome.serial.getDevices(callback);
    };

    Serial.prototype.openDevice = function () {
        var self = this;

        chrome.serial.connect("/dev/tty.usbmodem1421", { bitrate: 9600}, function (connectionInfo) {
            log(connectionInfo);

            if (connectionInfo) {
                self.device = self.initConnection(connectionInfo.connectionId);
            }

            // callback(self.device);
        });
    };

    Serial.prototype.listDevices = function() {
        var onLogDevices = function(ports) {
            var count = ports.length ? ports.length : 0;

            var message = "Nós encontramos " + count + " conexões seriais: ";

            for (var i = 0; i < count; i++) {
                message += ports[i].path + "\r\n";
            }

            UI.getInstance().showMessageFromSerial(message);
        };

        this.openDevice();
        this.getDevices(onLogDevices);
    };

    // Private methods:

    // Interprets an ArrayBuffer as UTF-8 encoded string data.
    var arrayBufferToString = function (buffer) {
        var bufferView = new Uint8Array(buffer);
        var encodedString = String.fromCharCode.apply(null, bufferView);
        var escapedEncodedString = escape(encodedString);

        return decodeURIComponent(escapedEncodedString);
    };

    var stringToArrayBuffer = function (string) {
        var encodedURIComponent = encodeURIComponent(string);
        var encodedString = unescape(encodedURIComponent);
        var bytes = new Uint8Array(encodedString.length);

        for (var i = 0; i < encodedString.length; ++i) {
            bytes[i] = encodedString.charCodeAt(i);
        }

        return bytes.buffer;
    };

    return {
        getInstance: function () {
            if (!instance) {
                instance = new Serial();
            }

            return instance;
        }
    };
})();
