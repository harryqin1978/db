<?php

function fc_banner_slider_admin($callback_arg = '') {

  $op = isset($_POST['op']) ? $_POST['op'] : $callback_arg;
  
  switch ($op) {
    case t('Create manager team'):
    case 'create':
      $build = t('This is a test.');
      break;
    default:
      $build = _fc_banner_slider_admin_list();
  }
  
  return $build;

}



function fc_banner_slider_category_admin() {

  $build = _fc_banner_slider_category_admin_list();
  return $build;

}



function _fc_banner_slider_admin_list() {

  $session_filter = isset($_SESSION['fc_banner_slider_admin_list_filter']) ? $_SESSION['fc_banner_slider_admin_list_filter'] : array();

  $multilanguage = (module_exists('translation') || db_query_range("SELECT 1 FROM {node} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());

  $languages = language_list();
  
  $header = array(
    'picture' => array('data' => t('Picture')),
    'category' => array('data' => t('Category'), 'field' => 'c.category_name'),
    'showorder' => array('data' => t('Show Order'), 'field' => 'so.fc_banner_slider_showorder_value', 'sort' => 'asc'),
    'title' => array('data' => t('Title'), 'field' => 'n.title'),
    'status' => array('data' => t('Status'), 'field' => 'n.status'),
    'created' => array('data' => t('Created'), 'field' => 'n.created'),
    'changed' => array('data' => t('Updated'), 'field' => 'n.changed'),
  );
  if ($multilanguage) {
    $header['language'] = array('data' => t('Language'), 'field' => 'n.language');
  }
  $header['operations'] = array('data' => t('Operations'));

  $query = db_select('node', 'n');
  $query->join('fc_banner_slider_nodecategory', 'nc', 'n.nid=nc.nid AND n.vid=nc.vid');
  $query->join('fc_banner_slider_category', 'c', 'nc.category_id = c.category_id');
  $query->join('field_data_fc_banner_slider_showorder', 'so', 'n.nid=so.entity_id AND n.vid=so.revision_id');
  $query->condition('n.type', 'fc_banner_slider', '=');
  $query->condition('so.entity_type', 'node', '=');
  $query->condition('so.bundle', 'fc_banner_slider', '=');

  if (isset($session_filter['category_id'])) {
    if (is_numeric($session_filter['category_id'])) {
      $query->condition('nc.category_id', (int) $session_filter['category_id'], '=');      
    }
  }

  if (isset($session_filter['language'])) {
    if ($session_filter['language'] != '[any]') {
      $query->condition('n.language', $session_filter['language'], '=');      
    }
  }

  user_build_filter_query($query);

  $count_query = clone $query;
  $count_query->addExpression('COUNT(n.nid)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('n', array('nid', 'type', 'language', 'title', 'uid', 'status', 'created', 'changed'))
    ->fields('nc', array('category_id'))
    ->fields('c', array('category_name'))
    ->fields('so', array('fc_banner_slider_showorder_value'))
    ->limit(50)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();

  $destination = drupal_get_destination();

  $status = array(t('published'), t('unpublished'));

  $options = array();
  foreach ($result as $node) {
    $_node = node_load($node->nid);
    $uri = $_node->fc_banner_slider_picture['und'][0]['uri'];
    $langcode = entity_language('node', $node);
    $l_options = $langcode != LANGUAGE_NONE && isset($languages[$langcode]) ? array('language' => $languages[$langcode]) : array();
    $options[$node->nid] = array(
      'picture' => theme('image_style', array('style_name' => 'fc_banner_slider_mini', 'path' => $uri, 'getsize' => FALSE)),
      'category' => $node->category_name,
      'showorder' => $node->fc_banner_slider_showorder_value,
      'title' => $node->title,
      'status' => $node->status ? t('published') : t('not published'),
      'created' => format_date($node->created, 'short'),
      'changed' => format_date($node->changed, 'short'),
    );
    if ($multilanguage) {
      if ($langcode == LANGUAGE_NONE || isset($languages[$langcode])) {
        $options[$node->nid]['language'] = $langcode == LANGUAGE_NONE ? t('Language neutral') : t($languages[$langcode]->name);
      }
      else {
        $options[$node->nid]['language'] = t('Undefined language (@langcode)', array('@langcode' => $langcode));
      }
    }
    // Build a list of all the accessible operations for the current node.
    $operations = array();
    if (node_access('update', $node)) {
      $operations['edit'] = array(
        'title' => t('edit'),
        'href' => 'node/' . $node->nid . '/edit',
        'query' => $destination,
      );
    }
    if (node_access('delete', $node)) {
      $operations['delete'] = array(
        'title' => t('delete'),
        'href' => 'node/' . $node->nid . '/delete',
        'query' => $destination,
      );
    }
    $options[$node->nid]['operations'] = array();
    if (count($operations) > 1) {
      // Render an unordered list of operations links.
      $options[$node->nid]['operations'] = array(
        'data' => array(
          '#theme' => 'links__node_operations',
          '#links' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
    }
    elseif (!empty($operations)) {
      // Render the first and only operation as a link.
      $link = reset($operations);
      $options[$node->nid]['operations'] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => $link['title'],
          '#href' => $link['href'],
          '#options' => array('query' => $link['query']),
        ),
      );
    }
  }

  $form_filter = drupal_get_form('_fc_banner_slider_admin_list_filter');

  $build['filter'] = array(
    '#markup' => render($form_filter),
  );
  $build['addnew'] = array(
    '#type' => 'link',
    '#title' => t('Add new banner slider'),
    '#href' => 'node/add/fc-banner-slider',
    '#options' => array(
      'query' => $destination,
    ),
  );
  $build['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#empty' => t('No item.'),
  );
  $build['pager'] = array('#theme' => 'pager');

  return $build;

}



function _fc_banner_slider_admin_list_filter($form, &$form_state) {

  $session = isset($_SESSION['fc_banner_slider_admin_list_filter']) ? $_SESSION['fc_banner_slider_admin_list_filter'] : array();
  
  $options = array('[any]'=>t('---All Categories---'));
  
  $result = db_query("SELECT * FROM {fc_banner_slider_category} ORDER BY category_id");
  foreach ($result as $row) {
    $options[$row->category_id] = $row->category_name;
  }
  
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['filters']['category_id'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#options' => $options,
    '#default_value' => isset($session['category_id']) ? $session['category_id'] : '',
    '#required' => FALSE,
    '#title_display' => 'invisible',
  );
  
  // Language filter if there is a list of languages
  if ($languages = module_invoke('locale', 'language_list')) {
    
    $options = array('[any]' => t('--All Languages---'), LANGUAGE_NONE => t('Language neutral')) + $languages;
    
    $form['filters']['language'] = array(
      '#type' => 'select',
      '#title' => t('Language'),
      '#options' => $options,
      '#default_value' => isset($session['language']) ? $session['language'] : '',
      '#required' => FALSE,
      '#title_display' => 'invisible',
    );
    
  }

  $form['filters']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;

}



function _fc_banner_slider_admin_list_filter_submit($form, &$form_state) {

  $_SESSION['fc_banner_slider_admin_list_filter']['category_id'] = $form_state['values']['category_id'];
  
  if (isset($form_state['values']['language']))  {
    $_SESSION['fc_banner_slider_admin_list_filter']['language'] = $form_state['values']['language'];
  }

}



function _fc_banner_slider_category_admin_list() {

  $header = array(
    'category_id' => array('data' => t('ID'), 'field' => 'c.category_id', 'sort' => 'asc'),
    'category_name' => array('data' => t('Name'), 'field' => 'c.category_name'),
    'image_style_name' => array('data' => t('Image style name'), 'field' => 'c.image_style_name'),
    'created' => array('data' => t('Created'), 'field' => 'c.created'),
    'operation' => array('data' => t('Operation')),
  );

  $query = db_select('fc_banner_slider_category', 'c');
  user_build_filter_query($query);

  $query = $query->extend('TableSort');
  $query
    ->fields('c', array('category_id', 'category_name', 'image_style_name', 'created'))
    ->orderByHeader($header);
  $result = $query->execute();

  $destination = drupal_get_destination();

  $options = array();

  foreach ($result as $item) {
    
    $options[$item->category_id] = array(
      'category_id' => $item->category_id,
      'category_name' => $item->category_name,
      'image_style_name' => $item->image_style_name,
      'created' => format_date($item->created, 'short'),
    );
  
    $operations = array();

    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => 'admin/config/webmaster/fc-banner-slider/category-list/' . $item->category_id . '/edit',
      'query' => $destination,
    );
    $operations['delete'] = array(
      'title' => t('delete'),
      'href' => 'admin/config/webmaster/fc-banner-slider/category-list/' . $item->category_id . '/delete',
      'query' => $destination,
    );

    $options[$item->category_id]['operations'] = array(
      'data' => array(
        '#theme' => 'links__node_operations',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline')),
      ),
    );

  }

  $build['addnew'] = array(
    '#type' => 'link',
    '#title' => t('Add new banner category'),
    '#href' => 'admin/config/webmaster/fc-banner-slider/category-list/add',
    '#options' => array(
      'query' => $destination,
    ),
  );
  $build['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#empty' => t('No item.'),
  );
  
  return $build;

}



function fc_banner_slider_category_add($form, &$form_state) {

  $form['category_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE,
    '#maxlength' => 127,
  );

  $styles = image_styles();
  
  $options = array();
  
  foreach ($styles as $name => $item) {
    $options[$name] = $name;
  }

  $form['image_style_name'] = array(
    '#type' => 'select',
    '#title' => t('Image style name'),
    '#options' => $options,
    '#default_value' => 'fc_banner_slider_large',
    '#required' => TRUE,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;

}



function fc_banner_slider_category_add_validate($form, &$form_state) {

  $found = db_query("SELECT count(category_id) FROM {fc_banner_slider_category} WHERE category_name = :category_name", array(':category_name' => $form_state['values']['category_name']))->fetchField();
  
  if ($found) {
    form_set_error('category_name', t('Category name already exists.'));
  }

}



function fc_banner_slider_category_add_submit($form, &$form_state) {

  db_insert('fc_banner_slider_category')
    ->fields(array(
      'category_name' => $form_state['values']['category_name'],
      'image_style_name' => $form_state['values']['image_style_name'],
      'created' => REQUEST_TIME,
    ))
    ->execute();
    
}



function fc_banner_slider_category_edit($form, &$form_state) {

  $record = _fc_banner_slider_category_checkexists();

  $form['category_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE,
    '#maxlength' => 127,
    '#default_value' => $record->category_name,
  );

  $styles = image_styles();
  
  $options = array();
  
  foreach ($styles as $name => $item) {
    $options[$name] = $name;
  }

  $form['image_style_name'] = array(
    '#type' => 'select',
    '#title' => t('Image style name'),
    '#options' => $options,
    '#default_value' => $record->image_style_name,
    '#required' => TRUE,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;

}



function fc_banner_slider_category_edit_validate($form, &$form_state) {

  $record = _fc_banner_slider_category_checkexists();

  $found = db_query(
    "SELECT count(category_id) FROM {fc_banner_slider_category} WHERE category_name = :category_name AND category_id <> :category_id", 
    array(':category_name' => $form_state['values']['category_name'], ':category_id' => $record->category_id)
  )->fetchField();
  
  if ($found) {
    form_set_error('category_name', t('Category name already exists.'));
  }

}



function fc_banner_slider_category_edit_submit($form, &$form_state) {

  $record = _fc_banner_slider_category_checkexists();

  db_update('fc_banner_slider_category')
    ->fields(array(
      'category_name' => $form_state['values']['category_name'],
      'image_style_name' => $form_state['values']['image_style_name'],
    ))
    ->condition('category_id', $record->category_id, '=')
    ->execute();
    
}



function fc_banner_slider_category_delete($form, &$form_state) {

  $record = _fc_banner_slider_category_checkexists();

  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $record->category_name)),
    NULL,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );

}



function fc_banner_slider_category_delete_submit($form, &$form_state) {

  $record = _fc_banner_slider_category_checkexists();

  db_delete('fc_banner_slider_category')
    ->condition('category_id', $record->category_id, '=')
    ->execute();
    
}



function _fc_banner_slider_category_checkexists() {

  $category_id = (int) arg(5);
  
  $result = db_query("SELECT * FROM {fc_banner_slider_category} WHERE category_id = :category_id",
    array(
      ':category_id' => $category_id,
    )
  );
  foreach ($result as $row) {
    $record = $row;
  }
  
  if(!isset($record)) {
    drupal_not_found();
    exit();
  } else {
    return $record;
  }

}


