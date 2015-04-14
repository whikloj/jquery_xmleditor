<?php
/**
 * @file
 * Administrative forms
 */

/**
 * Administrative settings form.
 */
function jquery_xmleditor_admin($form, &$form_state) {
  global $base_path;
  $form = array();

  $path = $base_path . 'sites/all/jquery_xmleditor';

  $library = libraries_detect('jquery.xmleditor');

  if (isset($library['error'])) {
    $confirmation = format_string('<img src="@image_url"/> !message', array(
      '@image_url' => url('misc/watchdog-error.png'),
      '!message' => $library['error message'] . t(' Please ensure they are installed in the sites/all/libraries directory.'),
    ));
  }
  else {
    $confirmation = format_string('<img src="@image_url"/> @message', array(
      '@image_url' => url('misc/watchdog-ok.png'),
      '@message' => 'Found jquery.xmleditor libraries.',
    ));
  }

  // We need jQuery Update to get up to at least 1.7
  $version = variable_get('jquery_update_jquery_version', '1.10');
  if (in_array($version, array('default', '1.5'))) {
    $jq_version = format_string('<img src="@image_url"/> !message', array(
      '@image_url' => url('misc/watchdog-error.png'),
      '!message' => t('jquery.xmleditor requires a version of jQuery 1.7 or greater.'),
    ));
  }
  else {
    $jq_version = format_string('<img src="@image_url"/> !message', array(
      '@image_url' => url('misc/watchdog-ok.png'),
      '!message' => t('Found jQuery version !version.', array('!version' => $version)),
    ));
  }

  $form['jquery_xmleditor_library_check'] = array(
    '#title' => t('jquery.xmleditor Library'),
    '#type' => 'fieldset',
    'library_check' => array(
      '#type' => 'item',
      '#markup' => $confirmation,
    ),
  );

  $form['jquery_xmleditor_jq_version'] = array(
    '#title' => t('JQuery Version'),
    '#type' => 'fieldset',
    'jquery_check' => array(
      '#type' => 'item',
      '#markup' => $jq_version,
    ),
  );

  $headers = array(
    'Schema name',
    'Schema description',
    'XML Schema file',
    'JSON Schema',
  );
  $schemas = db_query("SELECT id, name, description, xml_schema, json_schema from {jquery_xmleditor_schemas} ORDER BY name ASC");

  $rows = array();
  foreach ($schemas as $record) {
    $rows[$record->id] = array(
      $record->name,
      $record->description,
      $record->xml_schema,
      $record->json_schema,
    );
  }

  $form['jquery_xmleditor_schemas'] = array(
    '#type' => 'tableselect',
    '#header' => $headers,
    '#options' => $rows,
    '#empty' => t('No schemas available'),
  );

  return $form;
}
