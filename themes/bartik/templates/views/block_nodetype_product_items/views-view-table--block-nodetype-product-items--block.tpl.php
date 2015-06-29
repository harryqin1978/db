<?php
drupal_add_css(path_to_theme() . '/css/db/views-node-product-dp.css', array('group' => CSS_THEME));
drupal_add_css(path_to_theme() . '/css/db/tango/skin.css', array('group' => CSS_THEME));
drupal_add_js(path_to_theme() . '/js/db/jquery.jqzoom-core.js', array('group' => JS_THEME));
drupal_add_js(path_to_theme() . '/js/db/jquery.jcarousel.min.js', array('group' => JS_THEME));
drupal_add_js(path_to_theme() . '/js/db/views-node-product-dp.js', array('group' => JS_THEME));
?>

<?php foreach ($rows as $count => $row): ?>
<div id="product-wrapper">
	<div class="inner">
		<div id="pro-main-content">
			<div id="zoom-gallery">
				<div class="photos_show">
					<a href="<?php print $row['field_image_1']; ?>" class="jqzoom" rel='gal1'  title="" >
           				<img src="<?php print $row['field_image']; ?>"  title="" >
        			</a>
				</div>
				<div class="photos_val">
					<?php
					$block_nodetype_product_items_img = views_embed_view('block_nodetype_product_items_img', 'block');
					print $block_nodetype_product_items_img;
					?>
				</div>
			</div>
			<div id="detials">
				<div class="t_t">
					<h2><?php print $row['title']; ?></h2>
					<div class="product-category-color">
						<span class="text"><?php print $row['field_pd_category']; ?></span>
						<span class="separator"> | </span>
						<span class="text"><?php print $row['field_pd_color']; ?></span>
					</div>
					<div class="teaser"><?php print $row['field_pd_short_description'] ? $row['field_pd_short_description'] : $row['body_1']; ?></div>
				</div>
				<!-- <div class="stock"><?php print $row['commerce_stock']; ?> items in stock</div> -->
				<div class="cart_row">
					<div class="price">
						<?php print $row['commerce_price']; ?>
						<span class="orgin-price"><?php print $row['field_product_origin_price']; ?></span>
						<div class="product-size">
							<span class="label"><?php print t('Size') ?>:</span>
							<span class="text"><?php print $row['field_pd_size']; ?></span>
							<div class="clear"></div>
						</div>	
					</div>
					<div class="cart"><?php print $row['add_to_cart_form']; ?></div>
					<div class="clear"></div>

				</div>
				<div class="attr">

				<div class="product-info">
					<div class="product-row">
						<span class="label"><?php print t('Grape varieties') ?>:</span>
						<span class="text"><?php print $row['field_pd_grape_variety']; ?></span>
					</div>

					<div class="product-row">
						<span class="label"><?php print t('Country') ?>:</span>
						<span class="text"><?php print $row['field_pd_country']; ?></span>
					</div>

					<div class="product-row">
						<span class="label"><?php print t('Region') ?>:</span>
						<span class="text"><?php print $row['field_pd_region']; ?></span>
					</div>
					<div class="product-row">
						<span class="label"><?php print t('Vintage') ?>:</span>
						<span class="text"><?php print $row['field_pd_vintage']; ?></span>
					</div>
					<div class="product-row">
						<span class="label"><?php print t('Alcohol content') ?>:</span>
						<span class="text"><?php print $row['field_pd_alcohol_content']; ?></span>
					</div>
					<div class="product-row">
						<span class="label"><?php print t('Drinking temperature') ?>:</span>
						<span class="text"><?php print $row['field_pd_drinking_temperature']; ?></span>
					</div>
					<div class="product-row">
						<span class="label"><?php print t('Cellaring potential') ?>:</span>
						<span class="text"><?php print $row['field_pd_cellaring_potential']; ?></span>
					</div>
					<div class="product-row">
						<span class="label"><?php print t('Type of closure') ?>:</span>
						<span class="text"><?php print $row['field_pd_type_of_closure']; ?></span>
					</div>
					
					<div class="product-row">
						<span class="label"><?php print t('Stock') ?>:</span>
						<span class="text"><?php print $row['commerce_stock']; ?></span>
					</div>

					<div class="product-row">
						<span class="label"><?php print t('Product code') ?>:</span>
						<span class="text"><?php print $row['sku']; ?></span>
					</div>
					<div class="clear"></div>
				</div>
					
				</div>
			</div>
			<div class="clear"></div>
		</div>
		<!--
		<div id="pro-congeneric">
			<h3>Congeneric Products</h3>
			<div class="dp-windows">
				<?php
				$block_product_specials_01 = block_load('views', 'product_specials-block_1');
				print block_output($block_product_specials_01);
				?>
			</div>
			<div class="clear"></div>
		</div>
		-->
		<div id="pro-more-info">
			<h3>More info</h3>
			<div class="dp-windows">
				<div class="body"><?php print $row['body']; ?></div>
			</div>
		</div>
		<div id="pro-accessories">
			<h3>Related Products</h3>
			<div class="dp-windows">
				<?php
				$block_product_specials_02 = block_load('views', 'product_specials-block_2');
				print block_output($block_product_specials_02);
				?>
			</div>
		</div>
		<!--
		<div id="pro-comments">
			<h3>Comments</h3>
			<div class="dp-windows"></div>
		</div>
	-->
	</div>
</div>
<?php endforeach; ?>
