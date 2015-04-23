<?php
/**
 * @file
 * Houses the administration form for Islandora iRODS.
 */

/**
 * Admin settings form.
 */
function islandora_irods_settings() {
  drupal_add_css(drupal_get_path('module', 'islandora_irods') . '/css/islandora_irods.css');
  $form = array();

  $form['islandora_irods_host'] = array(
    '#type' => 'textfield',
    '#title' => t('iRODS host'),
    '#default_value' => variable_get('islandora_irods_host', ''),
    '#required' => TRUE,
  );

  $form['islandora_irods_port'] = array(
    '#type' => 'textfield',
    '#title' => t('iRODS port'),
    '#default_value' => variable_get('islandora_irods_port', '1247'),
    '#required' => TRUE,
  );

  $form['islandora_irods_user'] = array(
    '#type' => 'textfield',
    '#title' => t('iRODS user'),
    '#default_value' => variable_get('islandora_irods_user', ''),
    '#required' => TRUE,
  );

  $form['islandora_irods_password'] = array(
    '#type' => 'password',
    '#title' => t('iRODS password'),
    '#default_value' => variable_get('islandora_irods_password', ''),
    '#required' => TRUE,
  );

  $form['islandora_irods_zone'] = array(
    '#type' => 'textfield',
    '#title' => t('iRODS zone'),
    '#default_value' => variable_get('islandora_irods_zone', ''),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

