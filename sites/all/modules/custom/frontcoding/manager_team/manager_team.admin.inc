<?php

function manager_team_admin($callback_arg = '') {

  $op = isset($_POST['op']) ? $_POST['op'] : $callback_arg;
  
  switch ($op) {
    case t('Create manager team'):
    case 'create':
      $build = t('This is a test.');
      break;
    default:
      $build = _manager_team_admin_list();
  }
  
  return $build;

}



function _manager_team_admin_list() {

  $session_filter = isset($_SESSION['manager_team_admin_list_filter']) ? $_SESSION['manager_team_admin_list_filter'] : array();
  
  $multilanguage = (module_exists('translation') || db_query_range("SELECT 1 FROM {node} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());

  $languages = language_list();
  
  $header = array(
    'picture' => array('data' => t('Picture')),
    'title' => array('data' => t('Title'), 'field' => 'n.title'),
    'status' => array('data' => t('Status'), 'field' => 'n.status'),
    'created' => array('data' => t('Created'), 'field' => 'n.created'),
    'changed' => array('data' => t('Updated'), 'field' => 'n.changed', 'sort' => 'desc'),
  );
  if ($multilanguage) {
    $header['language'] = array('data' => t('Language'), 'field' => 'n.language');
  }
  $header['operations'] = array('data' => t('Operations'));

  $query = db_select('node', 'n');
  $query->condition('n.type', 'node_manager_team', '=');

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
    ->limit(50)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();

  $destination = drupal_get_destination();

  $status = array(t('published'), t('unpublished'));

  $options = array();
  foreach ($result as $node) {
    $_node = node_load($node->nid);
    $uri = $_node->node_manager_team_avator['und'][0]['uri'];
    $langcode = entity_language('node', $node);
    $l_options = $langcode != LANGUAGE_NONE && isset($languages[$langcode]) ? array('language' => $languages[$langcode]) : array();
    $options[$node->nid] = array(
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

  $form_filter = drupal_get_form('_manager_team_admin_list_filter');

  $build['filter'] = array(
    '#markup' => render($form_filter),
  );
  $build['manager_team_addnew'] = array(
    '#type' => 'link',
    '#title' => t('Add new team member'),
    '#href' => 'node/add/node-manager-team',
    '#options' => array(
      'query' => $destination,
    ),
  );
  $build['manager_team_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#empty' => t('No item.'),
  );
  $build['manager_team_pager'] = array('#theme' => 'pager');

  return $build;

}



function _manager_team_admin_list_filter($form, &$form_state) {

  $session = isset($_SESSION['manager_team_admin_list_filter']) ? $_SESSION['manager_team_admin_list_filter'] : array();
  
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



function _manager_team_admin_list_filter_submit($form, &$form_state) {

  if (isset($form_state['values']['language']))  {
    $_SESSION['manager_team_admin_list_filter']['language'] = $form_state['values']['language'];
  }

}



