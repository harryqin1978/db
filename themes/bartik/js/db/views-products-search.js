jQuery(document).ready(function($) {

$("select#edit-category option[value='All']").text(Drupal.t('All Category'));
$("select#edit-color option[value='All']").text(Drupal.t('All Wine type'));
$("select#edit-country option[value='All']").text(Drupal.t('All Country'));
$("select#edit-taste option[value='All']").text(Drupal.t('All Taste'));
$("select#edit-specials option[value='All']").text(Drupal.t('All Specials'));
$("select#edit-pricing option[value='All']").text(Drupal.t('All Pricing range'));

$(".views-exposed-widgets select[id*='edit-']").chosen({disable_search_threshold: 10});

// $("ul.chzn-results [id*='_0']").css("display", "none");

});
