

	<ul>
		<?php foreach ($rows as $count => $row): ?>
		<li <?php if ($row_classes[$count]) { print 'class="' . implode(' ', $row_classes[$count]).' "';  } ?>>
			<div class="box">
				<div class="inner">
					<div class="photo fl"><?php print $row['field_image'] ;?></div>
					<div class="des fl">
						<div class="title"><?php print $row['title'] ;?></div>
						<div class="teaser"><?php print $row['body'] ;?></div>
					</div>
					
					<div class="attr fl">
						<div class="inner">
							<div class="sell_price"><?php print $row['commerce_price'] ;?></div>
							<div class="add-cart"><?php print $row['add_to_cart_form'] ;?></div>
						</div>						
					</div>
					<div class="clear"></div>
				</div>				
			</div>
		</li>
		<?php endforeach; ?>
	</ul>