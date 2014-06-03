
<div id="block_left_specials">
	<ul>
		<?php foreach ($rows as $count => $row): ?>
		<li <?php if ($row_classes[$count]) { print 'class="' . implode(' ', $row_classes[$count]).' "';  } ?>>
			<div class="box">
				<div class="inner">
					<div class="photo"><?php print $row['field_image'] ;?></div>
					<div class="title"><?php print $row['title'] ;?></div>
					<div class="attr">
						<div class="price">
							<span class="sell_price"><?php print $row['commerce_price'] ;?></span>
							<?php if ($row['field_product_origin_price'] !== '') { ?>
								<span class="percent_price"><?php print '-'.(round(($row['field_product_origin_price_1']-$row['commerce_price_1'])/$row['field_product_origin_price_1'], 2)*100).'%' ;?></span>							
							<?php } ?>
							<span class="origin_price"><?php print $row['field_product_origin_price'] ;?></span>
						</div>
					</div>
				</div>				
			</div>
		</li>
		<?php endforeach; ?>
	</ul>
</div>