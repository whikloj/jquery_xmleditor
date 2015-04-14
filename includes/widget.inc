<?php
/**
 * @file
 * Functions for the Field API renderer.
 */

/**
 * Implements hook_field_widget_info().
 */
function jquery_xmleditor_field_widget_info() {
  return array(
    'jquery_xmleditor' => array(
      'label' => t('jquery.xmleditor'),
      'description' => t('Use the jquery.xmleditor to edit the XML'),
      'field types' => array('xml_field_xml'),
      'settings' => array('schema' => NULL),
      'behaviours' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default values' => FIELD_BEHAVIOR_DEFAULT,
      ),
      'weight' => 3,
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function jquery_xmleditor_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  include_once drupal_get_path('module', 'jquery_xmleditor') . '/includes/utilities.inc';
  $default_value = isset($items[$delta]['jquery_xmleditor']) ? $items[$delta]['jquery_xmleditor'] : '';

  $widget = $element;
  $widget['#delta'] = $delta;

  if ($instance['widget']['type'] == 'jquery_xmleditor' && isset($form_state['node'])) {
    $schema_id = $instance['widget']['settings']['schema'];
    $schema = jquery_xmleditor_get_schema_by_id($schema_id);
    $id = drupal_html_id('jquery_xmleditor');
    $base = base_path();

    if (FALSE !== $schema) {
      $js =<<<EOF
jQuery(document).ready(function() {
      jQuery(".edit-field-jquery-xmleditor.{$id}").xmlEditor({
              'schema' : "{$base}/jquery_xmleditor/schemas/{$schema->id}",
              'absoluteSchemaUrl' : true,
              'loadSchemaAsychronously' : false,
      });
});
EOF;
      $new_widget = array(
        '#type' => 'textarea',
//        '#prefix' => '<div id="' . $id . '" class="field-jquery-xmleditor">',
//        '#suffix' => '</div>',
        '#default_value' => '<mods:mods xmlns:mods="http://www.loc.gov/mods/v3"> </mods:mods>',
        '#attributes' => array('class' => array('edit-field-jquery-xmleditor ' . $id)),
        '#attached' => array(
          'library' => array(
            array('system', 'ui'),
          ),
          'libraries_load' => array(
            array('jquery.xmleditor'),
          ),
          'js' => array(
            array('data' => $js, 'type' => 'inline'),
          ),
        ),
      );
      $element += $new_widget;
    }
  }
  return $element;
}

/**
 * Implements hook_field_widget_settings().
 */
function jquery_xmleditor_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];

  if (is_null($settings['schema'])) {
    $results = db_query('SELECT MIN(id) as id FROM jquery_xmleditor_schemas WHERE name = :sn ORDER BY id ASC', array(':sn' => 'MODS-3.4 (pre-installed)'));
    $default_id = NULL;
    foreach ($results as $result) {
      if (is_null($default_id)) {
        $default_id = $result->id;
      }
    }
    if (is_null($default_id)) {
      drupal_set_message('Unable to find default schema for jquery.xmleditor', 'error');
    }
    else {
      $default_value = $default_id;
    }
  }
  else {
    $default_value = $settings['schema'];
  }
  $form['schema'] = array(
      '#type' => 'select',
      '#title' => 'Schema for the XML in this field.',
      '#default_value' => $default_value,
      '#options' => jquery_xmleditor_select_schemas(),
  );
  return $form;
}

/**
 * Builds select box options of available schemas
 */
function jquery_xmleditor_select_schemas() {
  $results = db_query("SELECT id, name FROM {jquery_xmleditor_schemas} ORDER BY name");
  $options = array();
  foreach ($results as $result) {
    $options[$result->id] = check_plain($result->name);
  }
  return $options;
}