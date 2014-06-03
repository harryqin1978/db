jQuery(document).ready(function($) {

  // alert('x');
  
  if ($.browser.msie && parseFloat($.browser.version) >= 6.0 && parseFloat($.browser.version) < 7.0) {
    DD_belatedPNG.fix('#footer-wrapper');
  }

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
    if ($('#edit-commerce-shipping-shipping-service input[type=radio]').length <= 1) {
      $('#edit-commerce-shipping-shipping-service').hide();
    }
  }

  $("#block-block-4 .cart-number").mouseenter(function(e) {
    $(".region-header #block-commerce-cart-cart").show();
  }).mouseleave(function(e) {
    // alert(e.pageX + ' | ' + e.pageY + ' | ' + $(".region-header #block-commerce-cart-cart").offset().left + ' | ' + $(".region-header #block-commerce-cart-cart").offset().top + ' | ' + $(".region-header #block-commerce-cart-cart").width() + ' | ' + $(".region-header #block-commerce-cart-cart").height());
    var _cart = $(".region-header #block-commerce-cart-cart");
    if (e.pageX >= _cart.offset().left && e.pageX <= _cart.offset().left + _cart.width() && e.pageY >= _cart.offset(100).top && e.pageY <= _cart.offset().top + _cart.height()) {
    } else {
      _cart.hide();
    }
  });

  $(".region-header #block-commerce-cart-cart").mouseleave(function(e) {
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
