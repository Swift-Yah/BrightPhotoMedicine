var UI = (function () {
    // Common functions used for tweaking UI elements.
    function UI() {
        
    }

    // Global instance.
    var instance;
    
    // Public methods:
    
    UI.prototype.resetState = function() {
        setFieldValue('status-message', 'Procurando por uma conexão bluetooth com o Arduino...');

        document.getElementById('status-message').hidden = false;
        document.getElementById('setting-led').hidden = true;
        
        this.clearAllFields();  
    };

    UI.prototype.showStatusBluetooth = function(isOn) {
        var message = "Procurando por uma conexão bluetooth com o Arduino...";

        if (!isOn) {
            message = "Por favor, ligue seu bluetooth..."
        }

        setFieldValue('status-message', message);
    };

    UI.prototype.showMessageFromSerial = function(message) {
        setFieldValue('messages-from-serial').innerHTML = message;
    }

    UI.prototype.setAdapter = function (address, name) {
        var deviceInfoContainer = document.getElementById('device-info');
        var addressField = document.getElementById('adapter-address');
        var nameField = document.getElementById('adapter-name');

        deviceInfoContainer.hidden = true;

        if (address && name) {
            deviceInfoContainer.hidden = false;

            var setAdapterField = function (field, value) {
                var textNode = document.createTextNode(value);

                field.innerHTML = '';
                field.appendChild(textNode);
            };

            setAdapterField(addressField, address ? address : 'unknown');
            setAdapterField(nameField, name ? name : 'Local Adapter');
        }
    };

    UI.prototype.clearAllFields = function () {
        
    };

    // Private methods:

    function setFieldValue(id, value) {
        var div = document.getElementById(id);
        var text = (value == null) ? "-" : value;
        var textNode = document.createTextNode(text);

        div.innerHTML = '';
        div.appendChild(textNode);
    }

    return {
        getInstance: function() {
            if (!instance) {
                instance = new UI();
            }

            return instance;
        }
    };
})();