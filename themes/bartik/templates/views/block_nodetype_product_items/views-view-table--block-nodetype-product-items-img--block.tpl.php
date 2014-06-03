<ul id="thumblist" class="jcarousel-skin-tango" >
	<?php foreach ($rows as $count => $row): ?>
	<li <?php if ($row_classes[$count]) { print 'class="' . implode(' ', $row_classes[$count]).' "';  } ?>><a href='javascript:void(0);' rel="{gallery: 'gal1', smallimage: '<?php print $row['field_image']; ?>',largeimage: '<?php print $row['field_image_1']; ?>'}" <?php if($count ==0) { ?> class="zoomThumbActive" <?php } ?> ><img src='<?php print $row['field_image_2']; ?>'></a></li>
	<?php endforeach; ?>
</ul>