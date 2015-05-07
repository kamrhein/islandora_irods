<?php

/**
 * @file
 * Helper functions used by this module.
 */


/**
 * Produce HTML representing iRODS content.
 *
 * First, gets iRODS metadata then transforms it to HTML.
 *
 * @param AbstractObject $object
 *   A Fedora object containing an iRODS reference.
 *
 * @return string
 *   Metadata in form of HTML, as a string.
 */
function islandora_irods_show_metadata(AbstractObject $object) {
  $id = islandora_irods_get_data_object_id($object);

  $head = '<p>iRODS DataObject: ' . $id . '</p>';
 
  $variables=islandora_irods_get_variables(); 
  $output = array();
  $irods_py_path = drupal_get_path('module', 'islandora_irods') . variable_get('islandora_irodsmd_executable_path', '/py/islandorairods.py');
  $command = "python -c \"import imp; iirods = imp.load_source('islandorairods', '$irods_py_path'); iirods.getMetadata('$id', '$variables[host]', '$variables[port]', '$variables[user]', '$variables[password]', '$variables[zone]' )\" ";
  exec($command, $output, $ret); 

  $concout = "";
  for($x = 0; $x < count($output); $x++) {
    $concout .= $output[$x] . "<br/>";
  }
  return $head . $concout ;
}

/**
 * Get ID of related iRODS-Data-Object from RELS_EXT.
 *
 * @param AbstractObject $object
 *   A Fedora object containing an iRODS reference.
 *
 * @return string
 *   ID of related irods-data-object, as a string.
 */
function islandora_irods_get_data_object_id(AbstractObject $object) {
  $rels_predicate = variable_get('islandora_irods_resource_relationship', 'irodsObjectId');
  $rels = $object->relationships->get('info:fedora/fedora-system:def/relations-external#', $rels_predicate);
  $id = "";
  if (count($rels) > 0) {
    $id = $rels[0]['object']['value'];
  }
  return $id;
}


/**
 * Check existance of iRODS object.
 *
 * @param AbstractObject $object
 *   A Fedora object containing an iRODS reference.
 *
 * @return bool
 *   True if iRODS-object-ID is valid, false otherwise.
 */
function islandora_irods_check_object($id) {
  $output = array();
  $variables=islandora_irods_get_variables();
  $irods_py_path = drupal_get_path('module', 'islandora_irods') . variable_get('islandora_irodsmd_executable_path', '/py/islandorairods.py');
  $command = "python -c \"import imp; iirods = imp.load_source('islandorairods', '$irods_py_path'); iirods.checkForDataObject('$id', '$variables[host]', '$variables[port]', '$variables[user]', '$variables[password]', '$variables[zone]'   )\" ";
  exec($command, $output, $ret); 
  return !empty($output) ;
}

/**
 * Renders the irods tab.
 *
 * @pararm AbstractObject $object
 *   The object the irods belongs to.
 *
 * @return array
 *   A renderable array representing the tab.
 */
function islandora_irods_view(AbstractObject $object) {
  return array(
    'irods' => array(
      '#markup' => islandora_irods_show_metadata($object),
    ),
    'link' => array(
      '#markup' => l(t('Download AIP'), "islandora/object/{$object->id}/download_irods_resource"),
    ),
  );
}


/**
 * Get iRODS session variables.
 *
 * @return array
 *   Array including host, port, user, password, zone.
 */
function islandora_irods_get_variables() {
  $variables = array();
  $variables['host']=variable_get('islandora_irods_host', '');
  $variables['port']=variable_get('islandora_irods_port', '');
  $variables['user']=variable_get('islandora_irods_user', '');
  $variables['password']=variable_get('islandora_irods_password', ''); 
  $variables['zone']=variable_get('islandora_irods_zone', '');
  return $variables ;
}
