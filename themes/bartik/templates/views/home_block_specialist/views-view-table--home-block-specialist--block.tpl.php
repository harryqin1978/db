
<div id="home_special">
<?php foreach ($rows as $count => $row): ?>
<div class="s-3">
				<div class="inner">
					<div class="href_layer"><a href="<?php print $row['path'] ;?>"></a></div>
					<div class="photo"><?php print $row['field_image'] ;?></div>
					<div class="title"><?php print $row['title'] ;?></div>
					<div class="des"><?php print $row['body'] ;?></div>
					<div class="attr">
						<div class="price"><span class="orgin-price"><?php print $row['field_product_origin_price'] ;?></span><span class="sell_price"><?php print $row['commerce_price'] ;?></span></div>
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
<?php endforeach; ?>
</div>