<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */

?>


<div id="page_products_search">
	<ul>
		<?php foreach ($rows as $count => $row): ?>
		<li <?php if ($row_classes[$count]) { print 'class="' . implode(' ', $row_classes[$count]) . ' views-row-colnum-'.($count % 4 + 1).' "';  } ?>>
			<?php if ($row['field_pd_box_style'] == 1) { ?>
				<div class="box s-2">
				<div class="inner">
					<div class="href_layer"><a href="<?php print $row['path'] ;?>"></a></div>
					<div class="photo"><?php print $row['field_image_1'] ;?></div>
					<div class="title"><?php print $row['title'] ;?></div>
					<div class="des"><?php print $row['body'] ;?></div>
					<div class="attr">
						<div class="price"><span class="orgin-price"><?php print $row['field_product_origin_price'] ;?></span><?php print $row['commerce_price'] ;?></div>
						<div class="add-cart">
							<?php if($row['commerce_stock'] == 0) { ?>
							<input type="submit" value="Out of Stock" name="op" id="out-of-stock" disabled="disabled" class="out-of-stock form-submit form-button-disabled">
							<?php } else { ?>
							<?php print $row['add_to_cart_form'] ;?>
							<?php } ?>
						</div>
					</div>
				</div>				
				</div>
			<?php } elseif ($row['field_pd_box_style'] == 3) { ?>
				<div class="box s-3">
				<div class="inner">
					<div class="href_layer"><a href="<?php print $row['path'] ;?>"></a></div>
					<div class="photo"><?php print $row['field_image_2'] ;?></div>
					<div class="title"><?php print $row['title'] ;?></div>
					<div class="des"><?php print $row['body'] ;?></div>
					<div class="attr">
						<span class="orgin-price"><?php print $row['field_product_origin_price'] ;?></span><span class="price"><?php print $row['commerce_price'] ;?></span>
						<div class="add-cart">
							<?php if($row['commerce_stock'] == 0) { ?>
							<input type="submit" value="Out of Stock" name="op" id="out-of-stock" disabled="disabled" class="out-of-stock form-submit form-button-disabled">
							<?php } else { ?>
							<?php print $row['add_to_cart_form'] ;?>
							<?php } ?>
						</div>
					</div>
				</div>				
				</div>
			<?php } else { ?>
				<div class="box s-1">
				<div class="inner">
					<div class="href_layer"><a href="<?php print $row['path'] ;?>"></a></div>
					<div class="title"><?php print $row['title'] ;?></div>
					<div class="photo"><?php print $row['field_image'] ;?></div>
					<div class="attr">
						<span class="orgin-price"><?php print $row['field_product_origin_price'] ;?></span><span class="price"><?php print $row['commerce_price'] ;?></span>
						<div class="add-cart">
							<?php if($row['commerce_stock'] == 0) { ?>
							<input type="submit" value="Out of Stock" name="op" id="out-of-stock" disabled="disabled" class="out-of-stock form-submit form-button-disabled">
							<?php } else { ?>
							<?php print $row['add_to_cart_form'] ;?>
							<?php } ?>
						</div>
					</div>
				</div>				
				</div>
			<?php } ?>		
		</li>
		<?php if ($count % 4 + 1 == 4) { print '<div class="clear"></div>'; } ?>
		<?php endforeach; ?>
	</ul>
	<div class="clear"></div>
	
	
</div>