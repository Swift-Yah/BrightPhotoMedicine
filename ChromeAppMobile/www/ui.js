var UI = (function () {
    // Common functions used for tweaking UI elements.
    function UI() {
        
    }

    // Global instance.
    var instance;

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