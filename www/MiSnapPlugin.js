module.exports = {
captureCheckFront: function(success, fail) {
    cordova.exec(success,
                 fail,
                 "MiSnapPlugin",
                 "cordovaCallMiSnap",
                 []);
}
};