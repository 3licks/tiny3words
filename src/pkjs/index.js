var Clay = require('pebble-clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig, null, { autoHandleEvents: false });

var lat;
var long;
var refresh;
var messageKeys = require('message_keys');

var oldWords = "";

var locationOptions = {
  enableHighAccuracy: true, 
  maximumAge: 10000, 
  timeout: 10000
};

Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL(clay.generateUrl());
});

function locationSuccess(pos) {
  console.log("l3");
  lat = pos.coords.latitude;
  long = pos.coords.longitude;
  var key = "";
  var w3wUrl = "https://api.what3words.com/v2/reverse?coords="+lat+"%2C"+long+"&key="+key+"&display=minimal";
  var method = 'GET';
  // Create the request
  var request = new XMLHttpRequest();
  
  request.onload = function() {
    console.log("l4");
    var data = JSON.parse(this.responseText);
    var words = data.words;
    console.log("l5");
    oldWords = words;
    words = words.split(".");
    words = words[0]+". "+words[1]+". "+words[2];
    Pebble.sendAppMessage({'words': words});
    
  };
  
  // Send the request
  request.open(method, w3wUrl);
  request.send();
}

function locationError(err) {
  console.log("l2");
}

function loadLocation(){
  console.log("l1");
  navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
}

function loopRefresh(intvl){
  setTimeout(function(){
    loadLocation();
    loopRefresh(intvl);
  }, intvl);
}

Pebble.addEventListener('webviewclosed', function(e) {
  // Get the keys and values from each config item
  var claySettings = clay.getSettings(e.response);

  // In this example messageKeys.NAME is equal to 10001
  refresh = claySettings[messageKeys.enableRefresh];
  var interval = claySettings[messageKeys.interval];
  if (refresh == 1 && interval !== 0){
    
    loopRefresh(interval*60*10);
  }
});

Pebble.addEventListener('ready', function() {
  // PebbleKit JS is ready!
  loadLocation();
});

Pebble.addEventListener('appmessage', function(e) {
  console.log("reloading");
  loadLocation();
});