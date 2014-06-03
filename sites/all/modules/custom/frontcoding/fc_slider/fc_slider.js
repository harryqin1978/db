/* slider module functions start - base on jquery */



(function ($) {

  Drupal.behaviors.fcSlider = {
    attach: function (context, settings) {

      $('.fc_slider_container', context).each(function(){

        if ( $(this).find('.slider_item').length <= $(this).find('span.item_size').get(0).innerHTML*1 ) {
          $(this).find('.slider_navArrow').css('display', 'none');
          $(this).find('.slider_pagination').css('display', 'none');
        } else {
          $(this).find('.slider_rightArrow a').removeClass('disableme');
          fc_slider_goto($(this), 0);
        }

        if ($(this).find('span.hide_onmouseleave').get(0).innerHTML!='') {
          $(this).find('.slider_navArrow a').css('display', 'none');
          $(this).mouseenter(function() {
            $(this).find('.slider_navArrow a').fadeIn();
          }).mouseleave(function() {
            $(this).find('.slider_navArrow a').fadeOut();
          });
        }

      });
      
      $('.fc_slider_container .slider_navArrow a', context).click(function() {
        var _container = $(this).parent().parent();
        var _pos = _container.find('span.item_pos').get(0).innerHTML*1;
        var _step = _container.find('span.item_step').get(0).innerHTML*1;
        if ($(this).parent().hasClass('slider_leftArrow')) _pos -= _step;
        if ($(this).parent().hasClass('slider_rightArrow')) _pos += _step;
        fc_slider_goto(_container, _pos);
      });
      
      $('.fc_slider_container .slider_navArrow a', context).mouseenter(function() {
        if (!$(this).hasClass('disableme')) $(this).addClass('hoverme');
      }).mouseleave(function() {
        $(this).removeClass('hoverme');
      });
      
      $('.fc_slider_container .slider_pageicon a', context).mouseenter(function() {
        $(this).addClass('hoverme');
      }).mouseleave(function() {
        $(this).removeClass('hoverme');
      });

      $('.fc_slider_container .slider_pageicon a', context).click(function() {
        var _container = $(this).parent().parent().parent();
        var _parent_classes = $(this).parent().attr('class').split(' ');
        var _pos = _parent_classes[1].substr(16)*1;
        fc_slider_goto(_container, _pos);
      });

    }
  };

})(jQuery);


function fc_slider_goto(container, targetpos) {
  
  var _count = container.find('.slider_item').length;
  var _size = container.find('span.item_size').get(0).innerHTML*1;
  var _width = container.find('span.item_width').get(0).innerHTML*1;
  var _speed = container.find('span.item_speed').get(0).innerHTML*1;
  
  if ( targetpos <= 0 ) {
    targetpos = 0;
    container.find('.slider_leftArrow a').addClass('disableme');
  } else {
    container.find('.slider_leftArrow a').removeClass('disableme');
  }
  
  if ( targetpos >= _count - _size ) { 
    targetpos = _count - _size;
    container.find('.slider_rightArrow a').addClass('disableme');
  } else {
    container.find('.slider_rightArrow a').removeClass('disableme');
  }
  
  var _targetleft = _width * targetpos * (-1);
  _targetleft = _targetleft + 'px';
  container.find('.slider_holder_inner').animate({left: _targetleft}, _speed);
  container.find('span.item_pos').html(targetpos);
  
  container.find('.slider_pageicon a').removeClass('activeme');
  container.find('.slider_pageicon_'+targetpos+' a').addClass('activeme');
  
}



/* slider module functions end */
