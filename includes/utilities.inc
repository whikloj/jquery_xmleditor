<?php
/**
 * @file
 * Utility functions
 */

/**
 * Retrieve Schema information for database.
 */
function jquery_xmleditor_get_schema_by_id($id) {
  $results = db_query("SELECT id, name, description, xml_schema, json_schema, root_element FROM {jquery_xmleditor_schemas} WHERE id = :id", array(':id' => $id));
  foreach ($results as $result) {
    $full_path = explode('/', $result->xml_schema);
    $result->xsd_filename = array_pop($full_path);
    $result->directory = implode('/', $full_path) . '/';
    $result->json_filename = current(array_slice(array_reverse(explode('/', $result->json_schema)), 0, 1));
    return $result;
  }
  return FALSE;
}
