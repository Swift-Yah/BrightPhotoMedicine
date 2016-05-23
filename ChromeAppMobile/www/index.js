var main = (function() {
    function BrightPhotoMedicine() {
        // A mapping from device addresses to device names for found devices that expose a Led service.
        this.deviceMap_ = {};

        // The currently selected service and its characteristics.
        this.service_ = null;
        this.chrcMap_ = {};
        this.discovering_ = false;
        this.powered = false;
        this.ui = UI.getInstance();
        this.serial = SerialService.getInstance();
    }

    BrightPhotoMedicine.prototype.init = function () {
        // Store the |this| to be used by API callbacks below.
        var self = this;

        self.serial.init();

        // Request information about the local Bluetooth adapter to be displayed in the UI.
        var updateAdapterInfo = function(adapterInfo) {
            self.ui.setAdapter(adapterInfo.address, adapterInfo.name);
            self.updateDiscoveryToggleState(adapterInfo.discovering);
        };

        chrome.bluetooth.getAdapterState(function (adapterState) {
            if (chrome.runtime.lastError) {
                var message = chrome.run().lastError.message;

                self.ui.updateStatusBluetooth(message);
            }

            self.updateDiscoveryToggleState(adapterState.discovering);
            updateAdapterInfo(adapterState);
        });

        chrome.bluetooth.onAdapterStateChanged.addListener(updateAdapterInfo);
    };

    BrightPhotoMedicine.prototype.updateDiscoveryToggleState = function (discovering) {
        if (this.discovering_ != discovering) {
            this.discovering_ = discovering;
        }

        this.ui.showStatusBluetooth(discovering);
    };
    
    return {
        BrightPhotoMedicine: BrightPhotoMedicine
    };
})();

document.addEventListener('DOMContentLoaded', function () {
    var bright = new main.BrightPhotoMedicine();

    bright.init();  
});
