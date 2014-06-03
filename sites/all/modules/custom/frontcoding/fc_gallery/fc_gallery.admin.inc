<?php



function fc_gallery_picture_admin() {

  $build = _fc_gallery_picture_admin_list();
  return $build;
  
}



function _fc_gallery_picture_admin_list() {

  if (isset($_GET['gnid']) && is_numeric($_GET['gnid'])) $_SESSION['fc_gallery_picture_admin_list_filter']['gnid'] = (int) $_GET['gnid'];

  $session_filter = isset($_SESSION['fc_gallery_picture_admin_list_filter']) ? $_SESSION['fc_gallery_picture_admin_list_filter'] : array();

  $multilanguage = (module_exists('translation') || db_query_range("SELECT 1 FROM {node} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());

  $languages = language_list();
  
  $header = array(
    'picture' => array('data' => t('Picture')),
    'title' => array('data' => t('Title'), 'field' => 'n.title'),
    'gallery' => array('data' => t('Gallery')),
    'author' => array('data' => t('Author')),
    'status' => array('data' => t('Status'), 'field' => 'n.status'),
    'showorder' => array('data' => t('Show Order'), 'field' => 'so.fc_picture_showorder_value', 'sort' => 'asc'),
    'created' => array('data' => t('Created'), 'field' => 'n.created'),
    'changed' => array('data' => t('Updated'), 'field' => 'n.changed'),
  );
  
  /*
  if ($multilanguage) {
    $header['language'] = array('data' => t('Language'), 'field' => 'n.language');
  }
  */
  
  $header['operations'] = array('data' => t('Operations'));

  $query = db_select('node', 'n');
  $query->join('fc_gallery_gallery_group', 'gg', 'gg.pnid=n.nid');
  $query->join('field_data_fc_picture_showorder', 'so', 'n.nid=so.entity_id AND n.vid=so.revision_id');
  $query->join('node', 'n_gallery', 'n_gallery.nid=gg.gnid');
  $query->condition('n.type', 'fc_picture', '=');
  $query->condition('so.entity_type', 'node', '=');
  $query->condition('so.bundle', 'fc_picture', '=');
  $query->condition('n_gallery.type', 'fc_gallery', '=');
  $query->condition('n_gallery.status', 1, '=');

  if (isset($session_filter['gnid'])) {
    if (is_numeric($session_filter['gnid'])) {
      $query->condition('gg.gnid', (int) $session_filter['gnid'], '=');
    }
  }

  user_build_filter_query($query);

  $count_query = clone $query;
  $count_query->addExpression('COUNT(n.nid)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('n', array('nid', 'type', 'language', 'title', 'uid', 'status', 'created', 'changed'))
    ->fields('gg', array('gnid'))
    ->fields('so', array('fc_picture_showorder_value'))
    ->addField('n_gallery', 'title', 'gallery_title');
  $query
    ->limit(50)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();
  
  $destination = drupal_get_destination();

  $status = array(t('published'), t('unpublished'));

  $options = array();
  foreach ($result as $node) {
    $_node = node_load($node->nid);
    $uri = $_node->fc_picture_picture['und'][0]['uri'];
    $picture_realurl = file_create_url($uri);
    $langcode = entity_language('node', $node);
    $l_options = $langcode != LANGUAGE_NONE && isset($languages[$langcode]) ? array('language' => $languages[$langcode]) : array();
    $gallery = check_plain($node->gallery_title);
    $album_nid = db_select('fc_gallery_album_group', 'ag')
      ->fields('ag', array('anid'))
      ->where('ag.gnid = :gnid', array(':gnid' => $node->gnid))
      ->execute()->fetchField();
    if ($album_nid) {
      $album_title = db_select('node', 'n')
      ->fields('n', array('title'))
      ->where('n.nid = :nid', array(':nid' => $album_nid))
      ->execute()->fetchField();
      if ($album_title) {
        $gallery = check_plain($album_title.' -> ').$gallery;
      }
    }
    $options[$node->nid] = array(
      /*
      'picture' => l(
        theme('image_style', array('style_name' => 'fc_gallery_mini', 'path' => $uri, 'getsize' => FALSE)),
        $picture_realurl,
        array(
          'html' => true,
          'attributes' => array(
            'class' => 'fc-gallery-fancybox',
          )
        )
      ),
      */
      'picture' => theme('image_style', array('style_name' => 'fc_gallery_mini', 'path' => $uri, 'getsize' => FALSE)),
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $node->title,
          '#href' => 'node/' . $node->nid,
          '#options' => $l_options,
          '#suffix' => ' ' . theme('mark', array('type' => node_mark($node->nid, $node->changed))),
        ),
      ),
      'gallery' => $gallery,
      'author' => theme('username', array('account' => $_node)),
      'status' => $node->status ? t('published') : t('not published'),
      'showorder' => $node->fc_picture_showorder_value,
      'created' => format_date($node->created, 'short'),
      'changed' => format_date($node->changed, 'short'),
    );
    
    /*
    if ($multilanguage) {
      if ($langcode == LANGUAGE_NONE || isset($languages[$langcode])) {
        $options[$node->nid]['language'] = $langcode == LANGUAGE_NONE ? t('Language neutral') : t($languages[$langcode]->name);
      }
      else {
        $options[$node->nid]['language'] = t('Undefined language (@langcode)', array('@langcode' => $langcode));
      }
    }
    */
    
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

  $form_filter = drupal_get_form('_fc_gallery_picture_admin_list_filter');

  $build['filter'] = array(
    '#markup' => render($form_filter),
  );
  $build['addnew'] = array(
    '#type' => 'link',
    '#title' => t('Add new picture'),
    '#href' => 'node/add/fc-picture',
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



function _fc_gallery_picture_admin_list_filter($form, &$form_state) {

  $session = isset($_SESSION['fc_gallery_picture_admin_list_filter']) ? $_SESSION['fc_gallery_picture_admin_list_filter'] : array();
  
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
    '#attributes' => array('class' => array('container-inline')),
  );

    $query = db_select('node', 'n');
    $query->join('field_data_fc_gallery_showorder', 'so', 'n.nid=so.entity_id AND n.vid=so.revision_id');
    $query->join('fc_gallery_album_group', 'ag', 'ag.gnid=n.nid');
    $query->join('node', 'n_album', 'n_album.nid=ag.anid');
    $query->join('field_data_fc_album_showorder', 'so_album', 'n_album.nid=so_album.entity_id AND n_album.vid=so_album.revision_id');
    $query->condition('n.status', 1, '=')
      ->condition('n.type', 'fc_gallery', '=')
      ->condition('so.entity_type', 'node', '=')
      ->condition('so.bundle', 'fc_gallery', '=')
      ->orderBy('so_album.fc_album_showorder_value', 'ASC')
      ->orderBy('so.fc_gallery_showorder_value', 'ASC')
      ->fields('n', array('nid', 'type', 'language', 'title', 'uid', 'status', 'created', 'changed'))
      ->fields('ag', array('anid'))
      ->addField('n_album', 'title', 'album_title');

  $result = $query->execute();

  $options = array('[any]'=>t('---All Galleries---'));
  
  foreach ($result as $row) {
    $options[$row->nid] = $row->album_title.' -> '.$row->title;
  }
  
  $form['filters']['gnid'] = array(
    '#type' => 'select',
    '#title' => t('Gallery'),
    '#options' => $options,
    '#default_value' => isset($session['gnid']) ? $session['gnid'] : '',
    '#required' => FALSE,
    '#title_display' => 'invisible',
  );
  
  $form['filters']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;

}



function _fc_gallery_picture_admin_list_filter_submit($form, &$form_state) {

  $_SESSION['fc_gallery_picture_admin_list_filter']['gnid'] = $form_state['values']['gnid'];
  
}



function fc_gallery_gallery_admin() {

  $build = _fc_gallery_gallery_admin_list();
  return $build;
  
}



function _fc_gallery_gallery_admin_list() {

  if (isset($_GET['anid']) && is_numeric($_GET['anid'])) $_SESSION['fc_gallery_gallery_admin_list_filter']['anid'] = (int) $_GET['anid'];

  $session_filter = isset($_SESSION['fc_gallery_gallery_admin_list_filter']) ? $_SESSION['fc_gallery_gallery_admin_list_filter'] : array();

  $multilanguage = (module_exists('translation') || db_query_range("SELECT 1 FROM {node} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());

  $languages = language_list();
  
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'n.title'),
    'album' => array('data' => t('Album'), 'field' => 'n_album.title'),
    'status' => array('data' => t('Status'), 'field' => 'n.status'),
    'showorder' => array('data' => t('Show Order'), 'field' => 'so.fc_gallery_showorder_value', 'sort' => 'asc'),
    'created' => array('data' => t('Created'), 'field' => 'n.created'),
    'changed' => array('data' => t('Updated'), 'field' => 'n.changed'),
  );
  
  /*
  if ($multilanguage) {
    $header['language'] = array('data' => t('Language'), 'field' => 'n.language');
  }
  */
  
  $header['operations'] = array('data' => t('Operations'));

  $query = db_select('node', 'n');
  $query->join('fc_gallery_album_group', 'ag', 'ag.gnid=n.nid');
  $query->join('field_data_fc_gallery_showorder', 'so', 'n.nid=so.entity_id AND n.vid=so.revision_id');
  $query->join('node', 'n_album', 'n_album.nid=ag.anid');
  $query->condition('n.type', 'fc_gallery', '=');
  $query->condition('so.entity_type', 'node', '=');
  $query->condition('so.bundle', 'fc_gallery', '=');
  $query->condition('n_album.type', 'fc_album', '=');
  $query->condition('n_album.status', 1, '=');

  if (isset($session_filter['anid'])) {
    if (is_numeric($session_filter['anid'])) {
      $query->condition('ag.anid', (int) $session_filter['anid'], '=');      
    }
  }

  user_build_filter_query($query);

  $count_query = clone $query;
  $count_query->addExpression('COUNT(n.nid)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('n', array('nid', 'type', 'language', 'title', 'uid', 'status', 'created', 'changed'))
    ->fields('ag', array('anid'))
    ->fields('so', array('fc_gallery_showorder_value'))
    ->addField('n_album', 'title', 'album_title');
  $query
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
    if(user_access('fc gallery admin picture')) {
      $_title = l(
        $node->title,
        'admin/config/webmaster/fc-gallery/picture-list',
        array(
          'query' => array(
            'gnid' => $node->nid,
          ),
        )
      );
    } else {
      $_title = check_plain($node->title);
    }
    $options[$node->nid] = array(
      'title' => $_title,
      'album' => check_plain($node->album_title),
      'status' => $node->status ? t('published') : t('not published'),
      'showorder' => $node->fc_gallery_showorder_value,
      'created' => format_date($node->created, 'short'),
      'changed' => format_date($node->changed, 'short'),
    );
    
    /*
    if ($multilanguage) {
      if ($langcode == LANGUAGE_NONE || isset($languages[$langcode])) {
        $options[$node->nid]['language'] = $langcode == LANGUAGE_NONE ? t('Language neutral') : t($languages[$langcode]->name);
      }
      else {
        $options[$node->nid]['language'] = t('Undefined language (@langcode)', array('@langcode' => $langcode));
      }
    }
    */
    
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

  $form_filter = drupal_get_form('_fc_gallery_gallery_admin_list_filter');

  $build['filter'] = array(
    '#markup' => render($form_filter),
  );
  $build['addnew'] = array(
    '#type' => 'link',
    '#title' => t('Add new gallery'),
    '#href' => 'node/add/fc-gallery',
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



function _fc_gallery_gallery_admin_list_filter($form, &$form_state) {

  $session = isset($_SESSION['fc_gallery_gallery_admin_list_filter']) ? $_SESSION['fc_gallery_gallery_admin_list_filter'] : array();
  
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
    '#attributes' => array('class' => array('container-inline')),
  );

  $query = db_select('node', 'n');
  $query->join('field_data_fc_album_showorder', 'so', 'n.nid=so.entity_id AND n.vid=so.revision_id');
  $query->condition('n.status', 1, '=')
    ->condition('n.type', 'fc_album', '=')
    ->condition('so.entity_type', 'node', '=')
    ->condition('so.bundle', 'fc_album', '=')
    ->orderBy('so.fc_album_showorder_value', 'ASC')
    ->fields('n', array('nid', 'type', 'language', 'title', 'uid', 'status', 'created', 'changed'));

  $result = $query->execute();

  $options = array('[any]'=>t('---All Albums---'));
  
  foreach ($result as $row) {
    $options[$row->nid] = $row->title;
  }
  
  $form['filters']['anid'] = array(
    '#type' => 'select',
    '#title' => t('Album'),
    '#options' => $options,
    '#default_value' => isset($session['anid']) ? $session['anid'] : '',
    '#required' => FALSE,
    '#title_display' => 'invisible',
  );
  
  $form['filters']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;

}



function _fc_gallery_gallery_admin_list_filter_submit($form, &$form_state) {

  $_SESSION['fc_gallery_gallery_admin_list_filter']['anid'] = $form_state['values']['anid'];
  
}



function fc_gallery_album_admin() {

  $build = _fc_gallery_album_admin_list();
  return $build;
  
}



function _fc_gallery_album_admin_list() {

  $session_filter = isset($_SESSION['fc_gallery_album_admin_list_filter']) ? $_SESSION['fc_gallery_album_admin_list_filter'] : array();

  $multilanguage = (module_exists('translation') || db_query_range("SELECT 1 FROM {node} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());

  $languages = language_list();
  
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'n.title'),
    'status' => array('data' => t('Status'), 'field' => 'n.status'),
    'showorder' => array('data' => t('Show Order'), 'field' => 'so.fc_album_showorder_value', 'sort' => 'asc'),
    'created' => array('data' => t('Created'), 'field' => 'n.created'),
    'changed' => array('data' => t('Updated'), 'field' => 'n.changed'),
  );
  
  if ($multilanguage) {
    $header['language'] = array('data' => t('Language'), 'field' => 'n.language');
  }
  
  $header['operations'] = array('data' => t('Operations'));

  $query = db_select('node', 'n');
  $query->join('field_data_fc_album_showorder', 'so', 'n.nid=so.entity_id AND n.vid=so.revision_id');
  $query->condition('n.type', 'fc_album', '=');
  $query->condition('so.entity_type', 'node', '=');
  $query->condition('so.bundle', 'fc_album', '=');

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
    ->fields('so', array('fc_album_showorder_value'))
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
    if(user_access('fc gallery admin gallery')) {
      $_title = l(
        $node->title,
        'admin/config/webmaster/fc-gallery/gallery-list',
        array(
          'query' => array(
            'anid' => $node->nid,
          ),
        )
      );
    } else {
      $_title = check_plain($node->title);
    }
    $options[$node->nid] = array(
      'title' => $_title,
      'status' => $node->status ? t('published') : t('not published'),
      'showorder' => $node->fc_album_showorder_value,
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

  $form_filter = drupal_get_form('_fc_gallery_album_admin_list_filter');

  $build['filter'] = array(
    '#markup' => render($form_filter),
  );
  $build['addnew'] = array(
    '#type' => 'link',
    '#title' => t('Add new album'),
    '#href' => 'node/add/fc-album',
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



function _fc_gallery_album_admin_list_filter($form, &$form_state) {

  $session = isset($_SESSION['fc_gallery_album_admin_list_filter']) ? $_SESSION['fc_gallery_album_admin_list_filter'] : array();
  
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



function _fc_gallery_album_admin_list_filter_submit($form, &$form_state) {

  if (isset($form_state['values']['language']))  {
    $_SESSION['fc_gallery_album_admin_list_filter']['language'] = $form_state['values']['language'];
  }

}






