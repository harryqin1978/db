<?php

/**
 * Add body classes if certain regions have content.
 */
function bartik_preprocess_html(&$variables) {
  if (!empty($variables['page']['featured'])) {
    $variables['classes_array'][] = 'featured';
  }

  if (!empty($variables['page']['triptych_first'])
    || !empty($variables['page']['triptych_middle'])
    || !empty($variables['page']['triptych_last'])) {
    $variables['classes_array'][] = 'triptych';
  }

  if (!empty($variables['page']['footer_firstcolumn'])
    || !empty($variables['page']['footer_secondcolumn'])
    || !empty($variables['page']['footer_thirdcolumn'])
    || !empty($variables['page']['footer_fourthcolumn'])) {
    $variables['classes_array'][] = 'footer-columns';
  }

  // Add conditional stylesheets for IE
  drupal_add_css(path_to_theme() . '/css/ie.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css(path_to_theme() . '/css/ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 6', '!IE' => FALSE), 'preprocess' => FALSE));
}

/**
 * Override or insert variables into the page template for HTML output.
 */
function bartik_process_html(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($variables);
  }
}

/**
 * Override or insert variables into the page template.
 */
function bartik_process_page(&$variables) {

  drupal_add_js(drupal_get_path('theme', 'bartik') . '/js/fix-ie6-png.js');
  drupal_add_js(drupal_get_path('theme', 'bartik') . '/js/DD_belatedPNG.js');
  drupal_add_js(drupal_get_path('theme', 'bartik') . '/js/db/jquery.form.min.js');
  drupal_add_js(drupal_get_path('theme', 'bartik') . '/js/db/db.js');

  if (arg(0) == 'products' && !arg(1)) {
    drupal_add_css(path_to_theme() . '/css/db/views-products-search.css', array('group' => CSS_THEME));
    drupal_add_js(path_to_theme() . '/js/db/views-products-search.js', array('group' => JS_THEME));
  }

  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($variables);
  }
  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.
  $variables['hide_site_name']   = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, the site_slogan will be empty, so we rebuild it.
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }
  // Since the title and the shortcut link are both block level elements,
  // positioning them next to each other is much simpler with a wrapper div.
  if (!empty($variables['title_suffix']['add_or_remove_shortcut']) && $variables['title']) {
    // Add a wrapper div using the title_prefix and title_suffix render elements.
    $variables['title_prefix']['shortcut_wrapper'] = array(
      '#markup' => '<div class="shortcut-wrapper clearfix">',
      '#weight' => 100,
    );
    $variables['title_suffix']['shortcut_wrapper'] = array(
      '#markup' => '</div>',
      '#weight' => -99,
    );
    // Make sure the shortcut link is the first item in title_suffix.
    $variables['title_suffix']['add_or_remove_shortcut']['#weight'] = -100;
  }

  if (module_exists('path')) {
    //allow template suggestions based on url paths.
    $alias = drupal_get_path_alias(str_replace('/edit','',$_GET['q']));
    if ($alias != $_GET['q']) {
      $suggestions = array();
      $template_filename = 'page';
      foreach (explode('/', $alias) as $path_part) {
      $template_filename = $template_filename . '__' . $path_part;
      $suggestions[] = $template_filename;
      }
      $alias_array = explode('/', $alias);

      $variables['theme_hook_suggestions'] = array_merge((array) $suggestions, $variables['theme_hook_suggestions']);

    }
    if  ($node = menu_get_object()) {
      $variables['node'] = $node;
      $suggestions = array();
      $template_filename = 'page';
      $template_filename = $template_filename . '__' . $variables['node']->type;
      $suggestions[] = $template_filename;
      $variables['theme_hook_suggestions'] = array_merge((array) $suggestions, $variables['theme_hook_suggestions']);
   }
   }
    
}

/**
 * Implements hook_preprocess_maintenance_page().
 */
function bartik_preprocess_maintenance_page(&$variables) {
  // By default, site_name is set to Drupal if no db connection is available
  // or during site installation. Setting site_name to an empty string makes
  // the site and update pages look cleaner.
  // @see template_preprocess_maintenance_page
  if (!$variables['db_is_active']) {
    $variables['site_name'] = '';
  }
  drupal_add_css(drupal_get_path('theme', 'bartik') . '/css/maintenance-page.css');
}

/**
 * Override or insert variables into the maintenance page template.
 */
function bartik_process_maintenance_page(&$variables) {
  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.
  $variables['hide_site_name']   = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, the site_slogan will be empty, so we rebuild it.
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }
}

/**
 * Override or insert variables into the node template.
 */
function bartik_preprocess_node(&$variables) {
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }
}

/**
 * Override or insert variables into the block template.
 */
function bartik_preprocess_block(&$variables) {
  // In the header region visually hide block titles.
  if ($variables['block']->region == 'header') {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
}

/**
 * Implements theme_menu_tree().
 */
function bartik_menu_tree($variables) {
  return '<ul class="menu clearfix">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_field__field_type().
 */
function bartik_field__taxonomy_term_reference($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<h3 class="field-label">' . $variables['label'] . ': </h3>';
  }

  // Render the items.
  $output .= ($variables['element']['#label_display'] == 'inline') ? '<ul class="links inline">' : '<ul class="links">';
  foreach ($variables['items'] as $delta => $item) {
    $output .= '<li class="taxonomy-term-reference-' . $delta . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }
  $output .= '</ul>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . (!in_array('clearfix', $variables['classes_array']) ? ' clearfix' : '') . '"' . $variables['attributes'] .'>' . $output . '</div>';

  return $output;
}



/* resolve views taxonomy term name expose not i18n translate problem */

function bartik_preprocess_views_exposed_form(&$vars) {
  
  $filter_field_names = array('filter-field_pd_category_tid', 'filter-field_pd_color_tid', 'filter-field_pd_country_tid', 'filter-field_pd_taste_tid');

  foreach ($filter_field_names as $filter_field_name) {
    if (isset($vars['widgets'][$filter_field_name])) {
      $_html = $vars['widgets'][$filter_field_name]->widget;
      preg_match_all('/<option.*?value="(.*?)".*?>([^<>]*?)<\/option>/u', $_html, $_matches);
      foreach ($_matches[0] as $_key => $_value) {
        if($_key){
          $_trans = i18n_string(array('taxonomy', 'term', (int) $_matches[1][$_key], 'name'), $_matches[2][$_key]);
          $_html = preg_replace('/(<option.*?value="'.$_matches[1][$_key].'".*?>)'.$_matches[2][$_key].'(<\/option>)/u', '$1'.$_trans.'$2', $_html);
          $vars['widgets'][$filter_field_name]->widget = $_html;
        }
      }
    }
  }

}


function block_output(&$vars){
 return drupal_render(_block_get_renderable_array(_block_render_blocks(array($vars))));
}
