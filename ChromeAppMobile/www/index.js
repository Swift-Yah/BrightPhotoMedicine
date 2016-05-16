var main = (function() {
    function BrightPhotoMedicine() {
        // A mapping from device addresses to device names for found devices that expose a Led service.
        this.deviceMap_ = {};

        // The currently selected service and its characteristics.
        this.service_ = null;
        this.chrcMap_ = {};
        this.discovering_ = false;
        this.powered = false;
    }

    BrightPhotoMedicine.prototype.init = function () {
        // Store the |this| to be used by API callbacks below.
        var self = this;

        // Request information about the local Bluetooth adapter to be displayed in the UI.
        var updateAdapterInfo = function(adapterInfo) {
            UI.getInstance().setAdapter(adapterInfo.address, adapterInfo.name);
            self.updateDiscoveryState(adapterInfo.discovering);
        };

        chrome.bluetooth.getAdapterState(function (adapterState) {
            if (chrome.runtime.lastError) {
                console.log(chrome.runtime.lastError.message);
            }

            self.updateDiscoveryToggleState(adapterState.discovering);
            updateAdapterInfo(adapterState);
        });

        chrome.bluetooth.onAdapterStateChanged.addListener(updateAdapterInfo);
    };

    BrightPhotoMedicine.prototype.updateDiscoveryToggleState = function (discovering) {
        if (this.discovering_ != discovering) {
            this.discovering_ = discovering;
            UI.getInstance().showStatusBluetooth(discovering);
        }
    };

    BrightPhotoMedicine.prototype.writeOnSerial = function (message) {

    };

    BrightPhotoMedicine.prototype.getDevices = function (callback) {
        chrome.serial.getDevices(callback);
    };

    BrightPhotoMedicine.prototype.openDevice = function(path, callback) {
        chrome.serial.connect(path, { bitrate: 9600 }, function (connectionInfo) {
            var device = null;

            if (connectionInfo) {
                device = new 
            }
        })
    };

    BrightPhotoMedicine.prototype.newActiveConnection

    return {
        BrightPhotoMedicine: BrightPhotoMedicine
    };


})();

document.addEventListener('DOMContentLoaded', function () {
    var bright = new main.BrightPhotoMedicine();

    bright.init();  
});
