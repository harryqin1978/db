<?php

/**
 * @file
 * theme implementation to display a gallery items list.
 * 
 * Template variables:
 * 
 * $items => item array.
 * $items[$index] => an item.
 * $items[$index]['picture'] => picture html code, include '<img ...'.
 * $items[$index]['link'] => link to where.
 * $items[$index]['title'] => item title.
 * $items[$index]['title_native'] => item title (no check plain).
 * $fancybox = if TRUE, use fancybox.
 * $relkey = unique value to seperate different fancybox groups in one page.
 * 
 */
 
?>
<div class="fc-gallery-itemlist">

<?php foreach ($items as $index => $item) { ?>
  <div class="fc-gallery-item">
    <?php 
    $options = array(
      'html' => TRUE,
      'attributes'=>array(
        'title' => $item['title_native'], 
        'class' => 'fc-gallery-itemlink' . ($fancybox ? ' fc-gallery-fancybox' : ''),
      )
    );
    if ($fancybox && $relkey) {
      $options['attributes']['rel'] = $relkey . '-item';
    }
    print l(
      $item['picture'],
      $item['link'],
      $options
    );
    ?>
    <p class="fc-gallery-title"><?php print $item['title']; ?></p>
  </div>
  <?php if ($index%6==5) { ?>
  <div style="clear:both"></div>
  <?php } ?>
<?php } ?>

<?php if (count($items)==0) { ?>
  <div class="fc-gallery-noitem"><?php print t('No item.'); ?></div>
<?php } ?> 

</div>
