<?php
global $language;

$result = db_query(
  "SELECT n.nid FROM {node} n INNER JOIN {weight_weights} ww ON n.nid = ww.entity_id WHERE n.type = 'hti' AND n.status = 1 AND ww.entity_type = 'node' AND (n.language = :language OR n.language = :langauge_none) ORDER BY ww.weight ASC, n.created DESC LIMIT 0, 5",
  array(':language' => $language->language, ':langauge_none' => LANGUAGE_NONE)
);
?>

<div class="top-images">
  <?php
  foreach ($result as $record) {
    $nid = $record->nid;
    $node = node_load($nid);
    $field_hti_image = field_get_items('node', $node, 'field_hti_image');
    $field_hti_link = field_get_items('node', $node, 'field_hti_link');
  ?>
    <div class="single-image">
      <h3><?php print check_plain($node->title); ?></h3>
      <a href="<?php print $field_hti_link[0]['value']; ?>">
        <img src="<?php print file_create_url($field_hti_image[0]['uri']); ?>" style="width: 188px; height: 210px;">
      </a>
    </div>
  <?php } ?>
  <div style="clear: both">&nbsp;</div>
</div>