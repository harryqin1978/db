<?php
$url = array_unique($_REQUEST);
?>

<?php $items_id = array(); ?>
<?php foreach ($rows as $count => $row): ?>

<?php if (isset($row['field_pd_category'])) $items_id[] = $row['field_pd_category']; ?>
<?php if (isset($row['field_pd_color'])) $items_id[] = $row['field_pd_color']; ?>

<?php endforeach; ?>
<?php // print_r(array_unique($items));?>
<?php  $test = array_unique($items_id) ;?>


<ul>
<?php foreach ($test as $count => $row_i): ?>
<li><?php print $row_i; ?></li>
<?php endforeach; ?>
</ul>


