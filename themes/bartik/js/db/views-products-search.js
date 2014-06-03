jQuery(document).ready(function($) {

$("select#edit-category option[value='All']").text('Category');
$("select#edit-color option[value='All']").text('Color');
$("select#edit-country option[value='All']").text('Country');
$("select#edit-taste option[value='All']").text('Taste');
$("select#edit-pricing option[value='All']").text('Pricing Range');

$("#views-exposed-form-products-search-page select[id*='edit-']").chosen({disable_search_threshold: 10});

$("ul.chzn-results [id*='_0']").css("display", "none");

});