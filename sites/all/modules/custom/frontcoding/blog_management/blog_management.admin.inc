<?php

function blog_management_admin() {

  $build = _blog_management_admin_list();

  return $build;

}



function _blog_management_admin_list() {

  $multilanguage = (module_exists('translation') || db_query_range("SELECT 1 FROM {node} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());

  $header = array(
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
  $query->condition('n.type', 'blog', '=');
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

  $build['blog_management_addnew'] = array(
          '#type' => 'link',
          '#title' => t('Add new blog'),
          '#href' => 'node/add/blog',
          '#options' => array(
            'query' => $destination,
          ),
  );
  $build['blog_management_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#empty' => t('No people available.'),
  );
  $build['blog_management_pager'] = array('#theme' => 'pager');

  return $build;


}
