<?php 

	//Career
	function fc_career_admin() {
	
	  $build = _fc_career_admin_list();
	  return $build;
	  
	}

	function _fc_career_admin_list(){
	$session_filter = isset($_SESSION['fc_career_admin_list_filter']) ? $_SESSION['fc_career_admin_list_filter'] : array();
	$multilanguage = (module_exists('translation') || db_query_range("SELECT 1 FROM {node} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());
	$languages = language_list();
  $header = array(
		'picture' => array('data' => t('Picture')),
    'title' => array('data' => t('Title'), 'field' => 'n.title'),
    'department' => array('data' => t('Department'), 'field' => 'n_department.title'),
    'status' => array('data' => t('Status'), 'field' => 'n.status'),
		'showorder' => array('data' => t('Show Order'), 'field' => 'so.fc_career_showorder_value', 'sort' => 'asc'),
    'created' => array('data' => t('Created'), 'field' => 'n.created', 'sort' => 'asc'),
    'changed' => array('data' => t('Updated'), 'field' => 'n.changed'),
  );	
  $header['operations'] = array('data' => t('Operations'));

  $query = db_select('node', 'n');
  $query->join('fc_career_department_group', 'cg', 'cg.cnid=n.nid');
  $query->join('node', 'n_department', 'n_department.nid=cg.dnid');
  $query->join('field_data_fc_career_showorder', 'so', 'n.nid=so.entity_id AND n.vid=so.revision_id');
  $query->condition('n.type', 'fc_career', '=');
  $query->condition('n_department.type', 'fc_career_department', '=');
  $query->condition('n_department.status', 1, '=');

  if (isset($session_filter['dnid'])) {
    if (is_numeric($session_filter['dnid'])) {
      $query->condition('cg.dnid', (int) $session_filter['dnid'], '=');      
    }
  }

  user_build_filter_query($query);
  
  $count_query = clone $query;
  $count_query->addExpression('COUNT(n.nid)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('n', array('nid', 'type', 'language', 'title', 'uid', 'status', 'created', 'changed'))
    ->fields('cg', array('dnid'))
    ->fields('so', array('fc_career_showorder_value'))
    ->addField('n_department', 'title', 'department_title');
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
    $uri = $_node->fc_career_picture['und'][0]['uri'];
    $picture_realurl = file_create_url($uri);
    $l_options = $langcode != LANGUAGE_NONE && isset($languages[$langcode]) ? array('language' => $languages[$langcode]) : array();
    $department = CHECK_PLAIN($node->department_title);
	 	$location_nid = db_select('fc_career_location_group', 'lg')
    ->fields('lg', array('lnid'))
    ->where('lg.dnid = :dnid', array(':dnid' => $node->dnid))
    ->execute()->fetchField();
    if ($location_nid) {
      $location_title = db_select('node', 'n')
      ->fields('n', array('title'))
      ->where('n.nid = :nid', array(':nid' => $location_nid))
      ->execute()->fetchField();
      if ($location_title) {
        $department = check_plain($location_title.' -> ').$department;
      }
    }    

    $options[$node->nid] = array(
    	'picture' => theme('image_style', array('style_name' => 'fc_career_mini', 'path' => $uri, 'getsize' => FALSE)),
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $node->title,
          '#href' => 'node/' . $node->nid,
          '#options' => $l_options,
          '#suffix' => ' ' . theme('mark', array('type' => node_mark($node->nid, $node->changed))),
        ),
      ),
      'department' => $department,
      'status' => $node->status ? t('published') : t('not published'),
      'showorder' => $node->fc_career_showorder_value,
      'created' => format_date($node->created, 'short'),
      'changed' => format_date($node->changed, 'short'),
    );
    
    
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
  
  
  
  $form_filter = drupal_get_form('_fc_career_admin_list_filter');
  $build['filter'] = array(
    '#markup' => render($form_filter),
  );
  $build['addnew'] = array(
    '#type' => 'link',
    '#title' => t('Add new career'),
    '#href' => 'node/add/fc-career',
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
	
	function _fc_career_admin_list_filter($form, &$form_state) {

  $session = isset($_SESSION['fc_career_admin_list_filter']) ? $_SESSION['fc_career_admin_list_filter'] : array();

  
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
    '#attributes' => array('class' => array('container-inline')),
  );

    $query = db_select('node', 'n');
    $query->join('fc_career_location_group', 'lg', 'lg.dnid=n.nid');
    $query->join('node', 'n_location', 'n_location.nid=lg.lnid');
    $query->condition('n.status', 1, '=')
      ->condition('n.type', 'fc_career_department', '=')
      ->fields('n', array('nid', 'type', 'language', 'title', 'uid', 'status', 'created', 'changed'))
      ->fields('lg', array('lnid'))
      ->addField('n_location', 'title', 'location_title');

  $result = $query->execute();

  $options = array('[any]'=>t('---All Department---'));
  
  foreach ($result as $row) {
    $options[$row->nid] = $row->location_title.' -> '.$row->title;
  }
  
  $form['filters']['dnid'] = array(
    '#type' => 'select',
    '#title' => t('Department'),
    '#options' => $options,
    '#default_value' => isset($session['dnid']) ? $session['dnid'] : '',
    '#required' => FALSE,
    '#title_display' => 'invisible',
  );
  
  $form['filters']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;

}	
	function _fc_career_admin_list_filter_submit($form, &$form_state) {

  $_SESSION['fc_career_admin_list_filter']['dnid'] = $form_state['values']['dnid'];
  
}	
	
	
	//Career Location
	function fc_career_location_admin(){
	
	  $build = _fc_career_location_admin_list();
	  return $build;		
	}
	
	function _fc_career_location_admin_list(){
		
	$multilanguage = (module_exists('translation') || db_query_range("SELECT 1 FROM {node} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());
	$languages = language_list();
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'n.title'),
    'status' => array('data' => t('Status'), 'field' => 'n.status'),
  	'showorder' => array('data' => t('Show Order'), 'field' => 'so.fc_career_location_showorder_value', 'sort' => 'asc'),
    'created' => array('data' => t('Created'), 'field' => 'n.created', 'sort' => 'asc'),
    'changed' => array('data' => t('Updated'), 'field' => 'n.changed'),
  );	
  $header['operations'] = array('data' => t('Operations'));

  $query = db_select('node', 'n');
  $query->join('field_data_fc_career_location_showorder', 'so', 'n.nid=so.entity_id AND n.vid=so.revision_id');
  $query->condition('n.type', 'fc_career_location', '=');
  $query->condition('n.status', 1, '=');


  
  $count_query = clone $query;
  $count_query->addExpression('COUNT(n.nid)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('n', array('nid', 'type', 'language', 'title', 'uid', 'status', 'created', 'changed'))
    ->fields('so', array('fc_career_location_showorder_value'));
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

    $options[$node->nid] = array(
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $node->title,
          '#href' => 'node/' . $node->nid,
          '#options' => $l_options,
          '#suffix' => ' ' . theme('mark', array('type' => node_mark($node->nid, $node->changed))),
        ),
      ),
      'status' => $node->status ? t('published') : t('not published'),
      'showorder' => $node->fc_career_location_showorder_value,
      'created' => format_date($node->created, 'short'),
      'changed' => format_date($node->changed, 'short'),
    );
    
    
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
  
  
  
  $build['addnew'] = array(
    '#type' => 'link',
    '#title' => t('Add new location'),
    '#href' => 'node/add/fc-career-location',
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

	
	
	
	
	//Career Department
	
	function fc_career_department_admin(){
	
	  $build = _fc_career_department_admin_list();
	  return $build;		
	}
	
	function _fc_career_department_admin_list(){
	$session_filter = isset($_SESSION['fc_career_department_admin_list_filter']) ? $_SESSION['fc_career_department_admin_list_filter'] : array();
	$multilanguage = (module_exists('translation') || db_query_range("SELECT 1 FROM {node} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());
	$languages = language_list();
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'n.title'),
    'location' => array('data' => t('Location'), 'field' => 'n_location.title'),
    'status' => array('data' => t('Status'), 'field' => 'n.status'),
  	'showorder' => array('data' => t('Show Order'), 'field' => 'so.fc_career_department_showorder_value', 'sort' => 'asc'),
    'created' => array('data' => t('Created'), 'field' => 'n.created', 'sort' => 'asc'),
    'changed' => array('data' => t('Updated'), 'field' => 'n.changed'),
  );	
  $header['operations'] = array('data' => t('Operations'));

  $query = db_select('node', 'n');
  $query->join('fc_career_location_group', 'lg', 'lg.dnid=n.nid');
  $query->join('node', 'n_location', 'n_location.nid=lg.lnid');
  $query->join('field_data_fc_career_department_showorder', 'so', 'n.nid=so.entity_id AND n.vid=so.revision_id');
  $query->condition('n.type', 'fc_career_department', '=');
  $query->condition('n_location.type', 'fc_career_location', '=');
  $query->condition('n_location.status', 1, '=');

  if (isset($session_filter['lnid'])) {
    if (is_numeric($session_filter['lnid'])) {
      $query->condition('lg.lnid', (int) $session_filter['lnid'], '=');      
    }
  }

  user_build_filter_query($query);
 
  $count_query = clone $query;
  $count_query->addExpression('COUNT(n.nid)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('n', array('nid', 'type', 'language', 'title', 'uid', 'status', 'created', 'changed'))
    ->fields('lg', array('lnid'))
    ->fields('so',array('fc_career_department_showorder_value'))
    ->addField('n_location', 'title', 'location_title');
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
    $location = CHECK_PLAIN($node->location_title);


    $options[$node->nid] = array(
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $node->title,
          '#href' => 'node/' . $node->nid,
          '#options' => $l_options,
          '#suffix' => ' ' . theme('mark', array('type' => node_mark($node->nid, $node->changed))),
        ),
      ),
      'location' => $location,
      'status' => $node->status ? t('published') : t('not published'),
      'showorder' => $node->fc_career_department_showorder_value,
      'created' => format_date($node->created, 'short'),
      'changed' => format_date($node->changed, 'short'),
    );
    
    
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
  
  
  
  $form_filter = drupal_get_form('_fc_career_location_admin_list_filter');
  $build['filter'] = array(
    '#markup' => render($form_filter),
  );
  $build['addnew'] = array(
    '#type' => 'link',
    '#title' => t('Add new department'),
    '#href' => 'node/add/fc-career-department',
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
	function _fc_career_location_admin_list_filter($form, &$form_state) {

  $session = isset($_SESSION['fc_career_department_admin_list_filter']) ? $_SESSION['fc_career_department_admin_list_filter'] : array();

  
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
    '#attributes' => array('class' => array('container-inline')),
  );

    $query = db_select('node', 'n');
    $query->condition('n.status', 1, '=')
      ->condition('n.type', 'fc_career_location', '=')
      ->fields('n', array('nid', 'type', 'language', 'title', 'uid', 'status', 'created', 'changed'));

  $result = $query->execute();

  $options = array('[any]'=>t('---All Location---'));
  
  foreach ($result as $row) {
    $options[$row->nid] = $row->title;
  }
  
  $form['filters']['lnid'] = array(
    '#type' => 'select',
    '#title' => t('Location'),
    '#options' => $options,
    '#default_value' => isset($session['lnid']) ? $session['lnid'] : '',
    '#required' => FALSE,
    '#title_display' => 'invisible',
  );
  
  $form['filters']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;

}	
	function _fc_career_location_admin_list_filter_submit($form, &$form_state) {

  $_SESSION['fc_career_department_admin_list_filter']['lnid'] = $form_state['values']['lnid'];
  
}		

//career apply

	function fc_career_apply_admin() {
	
	  $build = _fc_career_apply_list();
	  return $build;
	  
	}
	
	function _fc_career_apply_list(){
		
	$session_filter = isset($_SESSION['fc_career_apply_admin_list_filter']) ? $_SESSION['fc_career_apply_admin_list_filter'] : array();
	$multilanguage = (module_exists('translation') || db_query_range("SELECT 1 FROM {node} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());
	$languages = language_list();
  $header = array(
    'title' => array('data' => t('Job Title'), 'field' => 'n.title', 'sort' => 'asc'),
    'phone' => array('data' => t('Phone'), 'field' => 'ca.phone_number'),
    'email' => array('data' => t('Email'), 'field' => 'ca.email'),
    'content' => array('data' => t('Content'), 'field' => 'ca.content'),
  );	
 // $header['operations'] = array('data' => t('Operations'));

  $query = db_select('node', 'n');
  $query->join('fc_career_apply', 'ca', 'ca.cid=n.nid');
  $query->condition('n.type', 'fc_career', '=');
  $query->condition('n.status', 1, '=');
//	print_r($session_filter);
  if (isset($session_filter['cid'])) {
    if (is_numeric($session_filter['cid'])) {
      $query->condition('ca.cid', (int) $session_filter['cid'], '=');      
    }
  }

  user_build_filter_query($query);
 
  $count_query = clone $query;
  $count_query->addExpression('COUNT(n.nid)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('ca', array('caid','cid','phone_number', 'email', 'content'))
    ->addField('n', 'title');
  $query
    ->limit(50)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute()->fetchAll();
  //print_r($result);
  $destination = drupal_get_destination();
  $status = array(t('published'), t('unpublished'));  
  $options = array();

	  foreach ($result as $node) {
   
    $langcode = entity_language('node', $node);



    $options[$node->caid] = array(
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $node->title,
          '#href' => 'node/' . $node->cid,
        ),
      ),
      'phone' => $node->phone_number,
      'email' => $node->email,
      'content' => $node->content,

    );
    
    
    // Build a list of all the accessible operations for the current node.



  }  
  
  
  $form_filter = drupal_get_form('_fc_career_apply_admin_list_filter');
  $build['filter'] = array(
    '#markup' => render($form_filter),
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



	function _fc_career_apply_admin_list_filter($form, &$form_state) {

  $session = isset($_SESSION['fc_career_apply_admin_list_filter']) ? $_SESSION['fc_career_apply_admin_list_filter'] : array();

  
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
    '#attributes' => array('class' => array('container-inline')),
  );

    $query = db_select('node', 'n');
    $query->join('fc_career_department_group', 'dg', 'dg.cnid=n.nid');
    $query->join('fc_career_location_group', 'lg', 'lg.dnid=dg.dnid');
    $query->join('node', 'n_location', 'n_location.nid=lg.lnid');
    $query->condition('n.status', 1, '=')
      ->condition('n.type', 'fc_career', '=')
      ->fields('n', array('nid', 'type', 'language', 'title', 'uid', 'status', 'created', 'changed'))
			->addField('n_location', 'title', 'location_title');
  $result = $query->execute();

  $options = array('[any]'=>t('---All Career---'));
  
  foreach ($result as $row) {
    $options[$row->nid] = $row->location_title."->".$row->title;
  }
  
  $form['filters']['cid'] = array(
    '#type' => 'select',
    '#title' => t('Career'),
    '#options' => $options,
    '#default_value' => isset($session['cid']) ? $session['cid'] : '',
    '#required' => FALSE,
    '#title_display' => 'invisible',
  );
  
  $form['filters']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;

}	
	function _fc_career_apply_admin_list_filter_submit($form, &$form_state) {

  $_SESSION['fc_career_apply_admin_list_filter']['cid'] = $form_state['values']['cid'];
  
}	
	