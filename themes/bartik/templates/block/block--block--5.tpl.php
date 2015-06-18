<?php global $language; ?>

<div class="content">
  <div id="home-first-content">
    <div class="col col-center">
      <?php
      $nid = db_query("SELECT nid FROM {node} WHERE status = 1 AND type = 'hmi' AND language = :language LIMIT 0, 1", array(':$language' => $language->language))->fetchField();
      if ($nid) {
        $node = node_load($nid);
        $field_hmi_image = field_get_items('node', $node, 'field_hmi_image');
        $field_hmi_link = field_get_items('node', $node, 'field_hmi_link');
      }
      if (isset($node) && $node) {
      ?>
        <h2 class="title" id="page-title"><?php print check_plain($node->title); ?></h2>
        <a href="<?php print url($field_hmi_link[0]['value']); ?>"><img src="<?php print file_create_url($field_hmi_image[0]['uri']); ?>" style="width: 100%;" /></a>
      <?php } ?>
    </div>
    <div class="col col-right">
      <h2 class="title"><?php print t('Specials'); ?></h2>
      <?php
      $view = views_get_view('home_block_specialist');
      $view->set_display('block');
      $view->execute();
      print $view->render();
      ?>
    </div>
    <div class="clear">&nbsp;</div>
  </div>
</div>