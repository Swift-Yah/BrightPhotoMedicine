var serial = (function () {
    var logObj = function (object) {
        console.log(object);
    };

    var log = function (message) {
        console.log(message);
    };

    function Serial() {

    }

    Serial.prototype.init = function (connectionId) {
        
    };

    // Encapsulates an active serial device connection.
    var DeviceConnection = function (connectionId) {
        var onReceive = new chrome.Event();
        var onError = new chrome.Event();
        var onClose = new chrome.Event();

        var send = function (message) {
            var newString = stringToArrayBuffer(message);

            chrome.serial.send(connectionId, newString, function () {});
        }

        var close = function () {
            chrome.serial.disconnect(connectionId, function (success) {
                if (success) {
                    onClose.dispatch();
                }
            })
        }
    };

    // Private methods

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

        return bytes.buffer
    }

    return {
        "newSerialConnection": Serial
    };
})();
