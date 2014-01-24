<?php




function webform_views_filter_views_data() {

  $data = array();

  $data['webform_component']['table']['group']  = t('Webform Component');
  $data['webform_component']['table']['join'] = array(
    'node' => array (
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  $data['webform_component']['nid'] = array(
    'title' => t('Webform data filter'),
    'group' => t('Webform'),
    'help' => t('Filter content that has webform attached.'),
    'filter' => array(
    'handler' => 'webform_views_filter_has_components',
    'label' => t('Has Webform'),
    'type' => 'yes-no',
    'use equal' => TRUE, // Use status = 1 instead of status <> 0 in WHERE statment
  ),
  );

  return $data;
}