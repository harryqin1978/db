jQuery(document).ready(function($) {

  // alert('x');
  
  if ($.browser.msie && parseFloat($.browser.version) >= 6.0 && parseFloat($.browser.version) < 7.0) {
    DD_belatedPNG.fix('#footer-wrapper');
  }

  $('.view-home-block-featured .featured-items').each(function(index, el) {
    var _title = $(el).find('.title a').text();
    $(el).find('.href_layer a').attr('title', _title);
  });

  $('.view-products-search #page_products_search > ul > li').each(function(index, el) {
    var _title = $(el).find('.title a').text();
    $(el).find('.href_layer a').attr('title', _title);
  });

  if ($('body').hasClass('page-cart')) {
    $('#views-form-commerce-cart-form-default td.views-field-nothing').each(function(){
      var op = parseFloat($(this).parent().find('td.views-field-field-product-origin-price').html().replace('$', ''));
      var np = parseFloat($(this).parent().find('td.views-field-commerce-unit-price').html().replace('$', ''));
      if (op) {
        var per = parseInt((op - np) * 100 / op);
        $(this).html(per + '%');
      }
    });
  }

  if ($('body').hasClass('page-checkout-shipping')) {
    // if ($('#edit-commerce-shipping-shipping-service input[type=radio]').length <= 1) {
    //   $('#edit-commerce-shipping-shipping-service').hide();
    // }
    $('#edit-commerce-shipping-service-details-name').val('abcdefg');
    $('#edit-commerce-shipping-service-details').css('display', 'none');
  }

  if ($('body').hasClass('page-cart') && $('body').hasClass('not-logged-in')) {
    if (Drupal.settings.pathPrefix == 'cn/') {
      var html = '请<a href="/' + Drupal.settings.pathPrefix + 'user/register">注册</a>或<a href="/' + Drupal.settings.pathPrefix + 'user/login">登录</a>以完成购买。&nbsp;&nbsp;&nbsp;';
    } else if (Drupal.settings.pathPrefix == 'en/') {
      var html = 'Please <a href="/' + Drupal.settings.pathPrefix + 'user/register">register</a> or <a href="/' + Drupal.settings.pathPrefix + 'user/login">login</a> to continue buy.&nbsp;&nbsp;&nbsp;';
    }
    $('.commerce-line-item-actions input').before(html);
  }

  $("#block-dc-ajax-add-cart-ajax-shopping-cart-teaser").mouseenter(function(e) {
    $("#block-dc-ajax-add-cart-ajax-shopping-cart").show();
  }).mouseleave(function(e) {
    // alert(e.pageX + ' | ' + e.pageY + ' | ' + $("#block-dc-ajax-add-cart-ajax-shopping-cart").offset().left + ' | ' + $("#block-dc-ajax-add-cart-ajax-shopping-cart").offset().top + ' | ' + $("#block-dc-ajax-add-cart-ajax-shopping-cart").width() + ' | ' + $("#block-dc-ajax-add-cart-ajax-shopping-cart").height());
    var _cart = $("#block-dc-ajax-add-cart-ajax-shopping-cart");
    if (e.pageX >= _cart.offset().left && e.pageX <= _cart.offset().left + _cart.width() && e.pageY >= _cart.offset(100).top && e.pageY <= _cart.offset().top + _cart.height()) {
    } else {
      _cart.hide();
    }
  });

  $("#block-dc-ajax-add-cart-ajax-shopping-cart").mouseleave(function(e) {
    $(this).hide();
  });



$(function() {
  $('#edit-search-block-form--2').example('Search');
  $('#search-block-pro-form #edit-title').example('Search products');
  });


 if ($(document).height() > 1610) {
    //$("#main-wrapper").css("background","url('/themes/bartik/images/bg_pop_bottom.png') no-repeat 0 bottom");
    //alert($(document).height());
    $(".bg_pop_top").after("<div class='bg_pop_bottom'></div>");
    
  }
  if ($(document).width() > 4788) {
    $("#page .page_top_bg").css("background","url('/themes/bartik/images/bg_page_x5.jpg') repeat-x 0 41px");
  }



  function bgPos_top(){
        //var w = $(window).width() - $('.bg_pop_top').width();
        var w = $(window).width() - 1018;
        w1 = w/2;
        $('.bg_pop_top').css('left', w1+'px');
    }        
  $(window).on('resize', bgPos_top).trigger('resize');

  function bgPos_bottom(){
        //var w = $(window).width() - $('.bg_pop_top').width();
        var h = $(document).height() - $('.bg_pop_bottom').height() - $('#footer-wrapper').height() - $('#footer-titles').height();
        var w = $(window).width() - $('.bg_pop_bottom').width();
        w1 = w/2;
        $('.bg_pop_bottom').css('top', h+'px');
        $('.bg_pop_bottom').css('left', w1+'px');
    }        
  $(window).on('resize', bgPos_bottom).trigger('resize');



$("#block-views-home-block-category-block .content").css("background"," url('/themes/bartik/images/transparent_images_01.png') no-repeat 60px 70px");
$("#block-menu-menu-information .content").after('<div class="photo" style="position:absolute;top:12px;right:30px;"><img alt="" src="/themes/bartik/images/content/002.jpg" ></div>');



jQuery.fn.limit=function(){
    //这里的div去掉的话，就可以运用li和其他元素上了，否则就只能div使用。
    var self = $("[limit]");
      self.each(function(){
           var objString = $(this).text();
           var objLength = $(this).text().length;
           var num = $(this).attr("limit");
           if(objLength > num){
            //这里可以设置鼠标移动上去后显示标题全文的title属性，可去掉。
            //$(this).attr("title",objString);
            objString = $(this).text(objString.substring(0,num) + "...");
           }
      })
}

$(document).ready(function(){
    $(document.body).limit();
})


/*
$(function() {
    $.fn.pos_center = function() {
        var w = $(window).width();
        var h = $(window).height();
        $(this).css({
            'top':parseInt((h-$(this).height())/2),
            'left':parseInt(w/2),
            'background-position': parseInt((h-$(this).height())/2)
        })
    }
    
    $('.bg_pop_top').pos_center();
  });
*/

$(document).ready(function(){
  $("#home_featured_pro .attr label").text("qty");
  $("#home_special .attr label").text("qty");
})



    
});
