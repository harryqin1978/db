<?php drupal_add_css(path_to_theme() . '/css/db/views-news-events.css', array('group' => CSS_THEME)); ?>

<div class="news-listings events-listings">
  <ul>
    <?php foreach ($rows as $row_count => $row): ?>
    <li <?php if ($row_classes[$row_count]) { print 'class="' . implode(' ', $row_classes[$row_count]) . ' views-row-colnum-'.($row_count % 3 + 1).'"';  } ?>>
     <div class="wrapper status_<?php print $row['field_event_status_1']; ?>">
      
     <?php foreach ($row as $field => $content): ?>
          <div <?php if ($field_classes[$field][$row_count]) { print 'class="'. $field_classes[$field][$row_count] . '" '; } ?><?php print drupal_attributes($field_attributes[$field][$row_count]); ?>>
            <?php print $content; ?>
          </div>
      <?php endforeach; ?>
    </div>
      </li>
      <?php endforeach; ?>
    
  </ul>
</div>