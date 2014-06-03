<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
drupal_add_css(path_to_theme() . '/css/db/views-products-search.css', array('group' => CSS_THEME));
drupal_add_js(path_to_theme() . '/js/db/views-products-search.js', array('group' => JS_THEME));
?>
<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<?php foreach ($rows as $id => $row): ?>
  <?php
  $classes_array[$id] .= ' views-row-colnum-' . ($id % 4);
  ?>
  <div<?php if ($classes_array[$id]) { print ' class="' . $classes_array[$id] .'"';  } ?>>
    <?php print $row; ?>
  </div>
  <?php if ($id % 4 == 3) { print '<div style="clear: both;"></div>'; } ?>
<?php endforeach; ?>