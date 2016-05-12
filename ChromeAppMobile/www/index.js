var main = (function() {
    function BrightPhotoMedicine() {
        // A mapping from device addresses to device names for found devices that expose a Battery service.
        this.deviceMap_ = {};

        // The currently selected service and its characteristics.
        this.service_ = null;
        this.chrcMap_ = {};
        this.discovering_ = false;
        this.powered = false;
    }

    BrightPhotoMedicine.prototype.init = function () {
        var self = this;


    };

    return {
        BrightPhotoMedicine: BrightPhotoMedicine
    };


})();

document.addEventListener('DOMContentLoaded', function () {
    var bright = new main.BrightPhotoMedicine();

    bright.init();  
});
