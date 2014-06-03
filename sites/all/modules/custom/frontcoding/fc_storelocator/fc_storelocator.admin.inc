<?php



function fc_storelocator_admin() {

  $build = _fc_storelocator_admin_list();
  return $build;

}



function _fc_storelocator_admin_list() {

  $session_filter = isset($_SESSION['fc_storelocator_admin_list_filter']) ? $_SESSION['fc_storelocator_admin_list_filter'] : array();

  $multilanguage = (module_exists('translation') || db_query_range("SELECT 1 FROM {node} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());

  $languages = language_list();
  
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'n.title'),
    'showorder' => array('data' => t('Show Order'), 'field' => 'so.fc_storelocator_showorder_value', 'sort' => 'asc'),
    'status' => array('data' => t('Status'), 'field' => 'n.status'),
    'created' => array('data' => t('Created'), 'field' => 'n.created'),
    'changed' => array('data' => t('Updated'), 'field' => 'n.changed'),
  );
  if ($multilanguage) {
    $header['language'] = array('data' => t('Language'), 'field' => 'n.language');
  }
  $header['operations'] = array('data' => t('Operations'));

  $query = db_select('node', 'n');
  $query->join('field_data_fc_storelocator_showorder', 'so', 'n.nid=so.entity_id AND n.vid=so.revision_id');
  $query->condition('n.type', 'fc_storelocator', '=');
  $query->condition('so.entity_type', 'node', '=');
  $query->condition('so.bundle', 'fc_storelocator', '=');

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
    ->fields('so', array('fc_storelocator_showorder_value'))
    ->limit(50)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();

  $destination = drupal_get_destination();

  $status = array(t('published'), t('unpublished'));

  $options = array();
  foreach ($result as $node) {
    $_node = node_load($node->nid);
    $langcode = entity_language('node', $node);
    $l_options = $langcode != LANGUAGE_NONE && isset($languages[$langcode]) ? array('language' => $languages[$langcode]) : array();
    $options[$node->nid] = array(
      'title' => $node->title,
      'showorder' => $node->fc_storelocator_showorder_value,
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

  $form_filter = drupal_get_form('_fc_storelocator_admin_list_filter');

  $build['filter'] = array(
    '#markup' => render($form_filter),
  );
  $build['addnew'] = array(
    '#type' => 'link',
    '#title' => t('Add new store'),
    '#href' => 'node/add/fc-storelocator',
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



function _fc_storelocator_admin_list_filter($form, &$form_state) {

  $session = isset($_SESSION['fc_storelocator_admin_list_filter']) ? $_SESSION['fc_storelocator_admin_list_filter'] : array();
  
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
    '#attributes' => array('class' => array('container-inline')),
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



function _fc_storelocator_admin_list_filter_submit($form, &$form_state) {

  if (isset($form_state['values']['language']))  {
    $_SESSION['fc_storelocator_admin_list_filter']['language'] = $form_state['values']['language'];
  }

}



function fc_storelocator_google_map_setting($form, &$form_state) {

  /*
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Description above the form'),
  );
  */

  // Use the #managed_file FAPI element to upload an image file.
  $form['marker_fid'] = array(
    '#title' => t('Marker image'),
    '#type' => 'managed_file',
    '#description' => t('Upload google map marker image.'),
    '#default_value' => variable_get('fc_storelocator_google_map_marker_fid', ''),
    '#upload_location' => 'public://fc_storelocator/map/google/',
  );

  $form['marker_offset_x'] = array(
    '#type' => 'textfield',
    '#title' => t('Marker image offset x'),
    '#description' => t('Image arrow position x offset to the left-top corner.'),
    '#default_value' => variable_get('fc_storelocator_google_map_marker_offset_x', ''),
    '#size' => 5,
    '#maxlength' => 5,
  );

  $form['marker_offset_y'] = array(
    '#type' => 'textfield',
    '#title' => t('Marker image offset y'),
    '#description' => t('Image arrow position y offset to the left-top corner.'),
    '#default_value' => variable_get('fc_storelocator_google_map_marker_offset_y', ''),
    '#size' => 5,
    '#maxlength' => 5,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;

}



function fc_storelocator_google_map_setting_validate($form, &$form_state) {

  $marker_fid = file_save_upload('marker_fid', array(
    'file_validate_is_image' => array(), // Validates file is really an image.
    'file_validate_extensions' => array('png gif jpg jpeg'), // Validate extensions.
  ));
  
  if ( $form_state['values']['marker_offset_x'] && !is_numeric($form_state['values']['marker_offset_x']) ) {
    form_set_error('marker_offset_x', t('Marker image offset must be integer.'));
  }

  if ( $form_state['values']['marker_offset_y'] && !is_numeric($form_state['values']['marker_offset_y']) ) {
    form_set_error('marker_offset_y', t('Marker image offset must be integer.'));
  }

}



function fc_storelocator_google_map_setting_submit($form, &$form_state) {

  // If fid is not 0 we have a valid file.
  if ($form_state['values']['marker_fid'] != 0) {
    // The new file's status is set to 0 or temporary and in order to ensure
    // that the file is not removed after 6 hours we need to change it's status
    // to 1. Save the ID of the uploaded image for later use.
    $file = file_load($form_state['values']['marker_fid']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);

    // When a module is managing a file, it must manage the usage count.
    // Here we increment the usage count with file_usage_add().
    file_usage_add($file, 'fc_storelocator', 'google_map_marker', 1);

    // Save the fid of the file so that the module can reference it later.
    variable_set('fc_storelocator_google_map_marker_fid', $file->fid);
    drupal_set_message(t('The image @image_name was uploaded and saved with an ID of @fid.', array('@image_name' => $file->filename, '@fid' => $file->fid)));
  }
  // If the file was removed we need to remove the module's reference to the
  // removed file's fid, and remove the file.
  elseif ($form_state['values']['marker_fid'] == 0 && variable_get('fc_storelocator_google_map_marker_fid', '')) {
    // Retrieve the old file's id.
    $fid = variable_get('fc_storelocator_google_map_marker_fid', FALSE);
    $file = $fid ? file_load($fid) : FALSE;
    if ($file) {
      // When a module is managing a file, it must manage the usage count.
      // Here we decrement the usage count with file_usage_delete().
      file_usage_delete($file, 'fc_storelocator', 'google_map_marker', 1);

      // The file_delete() function takes a file object and checks to see if
      // the file is being used by any other modules. If it is the delete
      // operation is cancelled, otherwise the file is deleted.
      file_delete($file);
    }

    // Either way the module needs to update it's reference since even if the
    // file is in use by another module and not deleted we no longer want to
    // use it.
    variable_set('fc_storelocator_google_map_marker_fid', FALSE);
    drupal_set_message(t('The image @image_name was removed.', array('@image_name' => $file->filename)));
  }

  variable_set('fc_storelocator_google_map_marker_offset_x', $form_state['values']['marker_offset_x']);
  variable_set('fc_storelocator_google_map_marker_offset_y', $form_state['values']['marker_offset_y']);

  drupal_set_message(t('Update complete.'));


}

