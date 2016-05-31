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
        this.connectionId = "";

        document.getElementById('start-bluetooth').addEventListener('click', function () {
            var self = this;

            var led = 3;

            var intensity = document.getElementById('intensity').value;
            var frequency = document.getElementById('frequency').value;
            var time = document.getElementById('time').value;

            var protocolToSend = "[" + led + ", " + intensity + ", " + frequency + ", " + time + "]";

            log(protocolToSend);

            SerialService.getInstance().send(protocolToSend);
        });
    }

    // Global instance
    var instance;

    // Public methods:

    Serial.prototype.init = function () {
        var self = this;

        this.onReceive.addListener(function (message) {
            log(message);
            self.ui.showMessageFromSerial(message);
        });

        self.listDevices();
    };

    Serial.prototype.initConnection = function (connectionId) {
        var self = this;

        self.connectionId = connectionId;

        chrome.serial.onReceive.addListener(function (receiveInfo) {
            if (receiveInfo.connectionId == connectionId) {
                var string = arrayBufferToString(receiveInfo.data);

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
        var self = this;
        var newString = stringToArrayBuffer(message);

        chrome.serial.send(self.connectionId, newString, function () {});
    };

    Serial.prototype.getDevices = function (callback) {
        chrome.serial.getDevices(callback);
    };

    Serial.prototype.openDevice = function (path) {
        var self = this;

        chrome.serial.connect(path, { bitrate: 9600}, function (connectionInfo) {
            if (connectionInfo) {
                self.device = self.initConnection(connectionInfo.connectionId);
            }
        });
    };

    Serial.prototype.listDevices = function() {
        var self = this;

        var onLogDevices = function(ports) {
            var count = ports.length ? ports.length : 0;

            var message = "Nós encontramos " + count + " conexões seriais: ";

            for (var i = 0; i < count; i++) {
                var path = ports[i].path;

                message += path + "\r\n";

                if (path == "/dev/tty.usbmodem1411") {
                    UI.getInstance().showStatusBluetooth(true);
                    message = "Estamos conectados ao Arduino...";

                    self.openDevice(path);

                    self.ui.updateUIForOpenConnection(true);

                    break;
                }
            }

            UI.getInstance().showMessageFromSerial(message);
        };
        
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
