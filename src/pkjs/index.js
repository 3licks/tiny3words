var lat;
var long;

var locationOptions = {
  enableHighAccuracy: true, 
  maximumAge: 10000, 
  timeout: 10000
};

function locationSuccess(pos) {
  lat = pos.coords.latitude;
  long = pos.coords.longitude;
  var w3wUrl = "http://felixhasa.website/w3w.php?lat="+lat+"&lon="+long;
  
  var method = 'GET';
  // Create the request
  var request = new XMLHttpRequest();
  
  request.onload = function() {
    var data = JSON.parse(this.responseText);
    var words = data.words;
    words = words.split(".");
    words = words[0]+". "+words[1]+". "+words[2];
    Pebble.sendAppMessage({'words': words});
  };
  
  // Send the request
  request.open(method, w3wUrl);
  request.send();
}

function locationError(err) {
  console.log('location error (' + err.code + '): ' + err.message);
}

function loadLocation(){
  navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
}

Pebble.addEventListener('ready', function() {
  // PebbleKit JS is ready!
  loadLocation();
});

Pebble.addEventListener('appmessage', function(e) {
  loadLocation();
});