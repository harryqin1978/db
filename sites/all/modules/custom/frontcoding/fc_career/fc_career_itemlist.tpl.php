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
<div class="fc-career-itemlist">
	
  <div class="fc-career-item-filter">
  <?php print render($items['filter']);?>
  </div>
	
	<?php 
	if (isset($items['title'])){
	foreach ($items['title'] as $value){?>
  <div class="fc-career-item-title">
  <?php print $value;?>
  </div>
  <?php 
  }
	}else {
  ?>
  <div class="fc-career-item-title">
  <?php print t("NO ITEM");?>
  </div>

	<?php 
	}
	?>
</div>