<?php
/**
 * @file
 * Utility functions
 */

function jquery_xmleditor_get_schemas() {
  $internal = file_scan_directory(drupal_get_path('module', 'jquery_xmleditor') . 'schemas/xsd', '*.xsd');
  $external_dir = variable_get('file_public_path', conf_path() . '/files') . '/jquery_schemas';
  $external = array();
  if (file_exists($external_dir)) {
    $external = file_scan_directory($external . '/xsd', '*.xsd');
  }
  return array_merge($internal, $external);
}

function jquery_xmleditor_get_schema_by_id($id) {
  $results = db_query("SELECT id, name, description, xml_schema, json_schema, root_element FROM {jquery_xmleditor_schemas} WHERE id = :id", array(':id' => $id));
  foreach ($results as $result) {
    $full_path = explode('/', $result->xml_schema);
    $result->xsd_filename = array_pop($full_path);
    $result->directory = implode('/', $full_path) . '/';
    $result->json_filename = current(array_slice(array_reverse(explode('/', $result->json_schema)),0,1));
    return $result;
  }
  return FALSE;
}