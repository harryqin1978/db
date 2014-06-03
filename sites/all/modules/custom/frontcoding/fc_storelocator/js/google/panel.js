google.maps.event.addDomListener(window, 'load', function() {
  
  var xcenter = parseFloat(Drupal.settings.fc_storelocator_google.xcenter);
  var ycenter = parseFloat(Drupal.settings.fc_storelocator_google.ycenter);
  
  var markerimage_url = Drupal.settings.fc_storelocator_google.markerimage_url;
  var marker_offset_x = parseInt(Drupal.settings.fc_storelocator_google.marker_offset_x);
  var marker_offset_y = parseInt(Drupal.settings.fc_storelocator_google.marker_offset_y);
  
  if (markerimage_url) {
  
    var ICON = new google.maps.MarkerImage(
      markerimage_url, 
      null, 
      null,
      (marker_offset_x && marker_offset_y) ? new google.maps.Point(marker_offset_x, marker_offset_y) : null
    );
  
  }
  
  var map = new google.maps.Map(document.getElementById('fc-storelocator-google-map-canvas'), {
    // center: new google.maps.LatLng(ycenter, xcenter),
    // zoom: 4,
    mapTypeId: google.maps.MapTypeId.ROADMAP
  });

  var panelDiv = document.getElementById('fc-storelocator-google-panel');

  var data = new MedicareDataSource;

  var view = new storeLocator.View(map, data, {
    geolocation: false,
    features: data.getFeatures()
  });

  if (typeof(ICON)=='object') {
    view.createMarker = function(store) {
      var markerOptions = {
        position: store.getLocation(),
        icon: ICON,
        title: store.getDetails().title
      };
      return new google.maps.Marker(markerOptions);
    }
  }

  new storeLocator.Panel(panelDiv, {
    view: view
  });
  
  var _count = Drupal.settings.fc_storelocator_google.xcoords.length;
  var _xcoords = Drupal.settings.fc_storelocator_google.xcoords;
  var _ycoords = Drupal.settings.fc_storelocator_google.ycoords;
  var bounds = new google.maps.LatLngBounds();
  for (var i=0; i<_count; i++) {
    var latlng = new google.maps.LatLng(
      parseFloat(_ycoords[i]),
      parseFloat(_xcoords[i])
    );
    bounds.extend(latlng);
  }
  view.getMap().fitBounds(bounds);
  
  // console.log(view.getMap());
  
});
