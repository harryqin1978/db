<?php drupal_add_css(path_to_theme() . '/css/db/views-news-events.css', array('group' => CSS_THEME)); ?>
<?php print $wrapper_prefix; ?>
  <?php if (!empty($title)) : ?>
    <h3><?php print $title; ?></h3>
  <?php endif; ?>
  <?php print $list_type_prefix; ?>
    <?php foreach ($rows as $id => $row): ?>
      <li class="<?php print $classes_array[$id]; ?> <?php print 'views-row-colnum-'.($id % 3 + 1) ?>">
      	<div class="wrapper">
      	<?php print $row; ?>
      	</div>
      </li>
      <?php if ($id % 3 + 1 == 3) { print '<div class="clear"></div>'; } ?>
    <?php endforeach; ?>
  <?php print $list_type_suffix; ?>
<?php print $wrapper_suffix; ?>