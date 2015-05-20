jQuery(document).ready(function($) {

$("select#edit-category option[value='All']").text(Drupal.t('Category'));
$("select#edit-color option[value='All']").text(Drupal.t('Color'));
$("select#edit-country option[value='All']").text(Drupal.t('Country'));
$("select#edit-taste option[value='All']").text(Drupal.t('Taste'));
$("select#edit-specials option[value='All']").text(Drupal.t('Specials'));
$("select#edit-pricing option[value='All']").text(Drupal.t('Pricing range'));

$(".views-exposed-widgets select[id*='edit-']").chosen({disable_search_threshold: 10});

$("ul.chzn-results [id*='_0']").css("display", "none");

});
