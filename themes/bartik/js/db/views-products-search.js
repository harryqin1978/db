jQuery(document).ready(function($) {

  $("select#edit-category option[value='All']").text(Drupal.t('Category'));
  $("select#edit-color option[value='All']").text(Drupal.t('Wine type'));
  $("select#edit-country option[value='All']").text(Drupal.t('Country'));
  $("select#edit-taste option[value='All']").text(Drupal.t('Taste'));
  $("select#edit-specials option[value='All']").text(Drupal.t('Specials'));
  $("select#edit-pricing option[value='All']").text(Drupal.t('Pricing range'));

  if ($("select#edit-category option:selected").eq(0).val() != 'All') {
    $("select#edit-category option").eq(1).before($("<option></option>").val('All').html(Drupal.t('All')));
  }
  if ($("select#edit-color option:selected").eq(0).val() != 'All') {
    $("select#edit-color option").eq(1).before($("<option></option>").val('All').html(Drupal.t('All')));
  }
  if ($("select#edit-country option:selected").eq(0).val() != 'All') {
    $("select#edit-country option").eq(1).before($("<option></option>").val('All').html(Drupal.t('All')));
  }
  if ($("select#edit-taste option:selected").eq(0).val() != 'All') {
    $("select#edit-taste option").eq(1).before($("<option></option>").val('All').html(Drupal.t('All')));
  }
  if ($("select#edit-specials option:selected").eq(0).val() != 'All') {
    $("select#edit-specials option").eq(1).before($("<option></option>").val('All').html(Drupal.t('All')));
  }
  if ($("select#edit-pricing option:selected").eq(0).val() != 'All') {
    $("select#edit-pricing option").eq(1).before($("<option></option>").val('All').html(Drupal.t('All')));
  }

  $(".views-exposed-widgets select[id*='edit-']").chosen({disable_search_threshold: 10});

  $(".views-exposed-widgets select[id*='edit-']").change(function(event) {
    $('#edit-submit-products-search').click();
  });

  $("ul.chzn-results [id*='_0']").css("display", "none");

  $("input#edit-title").attr('placeholder', Drupal.t('Keyword search'));

});
