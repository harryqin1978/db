
(function($) {

/**
 * Initiate Fancybox using selector and options from the module's settings.
 */
Drupal.behaviors.initFancybox = {
  
  attach : function() {
    var settings = Drupal.settings.fancybox;

    if (settings && settings.selector.length) {
      $(settings.selector).fancybox(settings.options);
    }

    // $('a.fc-gallery-fancybox').fancybox(settings.options);

    // alert($.browser.msie);
    // alert(parseInt($.browser.version));

    $('a.fc-gallery-fancybox').fancybox({
      // 'transitionIn'		: 'none',
      // 'transitionOut'		: 'none',
      'titlePosition' 	: ($.browser.msie && parseInt($.browser.version) <= 6) ? 'inside' : 'over',
      'titleFormat'		: function(title, currentArray, currentIndex, currentOpts) {
        return ($.browser.msie && parseInt($.browser.version) <= 6) ? 
        ('<span id="fancybox-title-inside">' + (currentIndex + 1) + ' / ' + currentArray.length + (title.length ? ' &nbsp; ' + title : '') + '</span>') :
        ('<span id="fancybox-title-over">' + (currentIndex + 1) + ' / ' + currentArray.length + (title.length ? ' &nbsp; ' + title : '') + '</span>')
        ;
      }
    });

  }
}

})(jQuery);
