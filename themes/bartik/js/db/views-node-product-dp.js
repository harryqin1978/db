jQuery(document).ready(function($) {

	$(document).ready(function() {
		$('.jqzoom').jqzoom({
            zoomType: 'innerzoom',
            preloadImages: false,
            alwaysOn:false
        });
	});

jQuery(document).ready(function() {
    jQuery('#thumblist').jcarousel({
        scroll: 1
    });
    jQuery('#thumblist_01').jcarousel({
        scroll: 5
    });
});

});