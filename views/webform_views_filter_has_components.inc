<?php

/**
 * @file
 * Definition of webform_views_filter_has_components.
 */

/**
 * Simple filter to handle matching of boolean values
 */
class webform_views_filter_has_components extends views_handler_filter {

  var $always_multiple = TRUE;
  var $no_operator = TRUE;
  var $accept_null = FALSE;

  /**
   * Defines the constructor for this class using a booleans as properties.
   */
  function construct() {
    $this->value_value = t('True');
    if (isset($this->definition['label'])) {
      $this->value_value = $this->definition['label'];
    }
    if (isset($this->definition['accept null'])) {
      $this->accept_null = (bool) $this->definition['accept null'];
    }
    elseif (isset($this->definition['accept_null'])) {
      $this->accept_null = (bool) $this->definition['accept_null'];
    }
    $this->value_options = NULL;
    parent::construct();
  }

  /**
   * This function returns the possible options for this filter.
   */
  function get_value_options() {
    if (isset($this->definition['type'])) {
      if ($this->definition['type'] == 'yes-no') {
        $this->value_options = array(1 => t('Yes'), 0 => t('No'));
      }
      if ($this->definition['type'] == 'on-off') {
        $this->value_options = array(1 => t('On'), 0 => t('Off'));
      }
      if ($this->definition['type'] == 'enabled-disabled') {
        $this->value_options = array(1 => t('Enabled'), 0 => t('Disabled'));
      }
    }

    // Provide a fallback if the above didn't set anything.
    if (!isset($this->value_options)) {
      $this->value_options = array(1 => t('True'), 0 => t('False'));
    }
  }

  /**
   * Defines the default option as False.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['value']['default'] = FALSE;

    return $options;
  }

  /**
   * Defines operator form.
   */
  function operator_form(&$form, &$form_state) {
    $form['operator'] = array();
  }

  /**
   * Defines the form being used in the view configuraton (true/false).
   */
  function value_form(&$form, &$form_state) {
    if (empty($this->value_options)) {
      // Initialize the array of possible values for this filter.
      $this->get_value_options();
    }
    if (!empty($form_state['exposed'])) {
      // Exposed filter: use a select box to save space.
      $filter_form_type = 'select';
    }
    else {
      // Configuring a filter: use radios for clarity.
      $filter_form_type = 'radios';
    }
    $form['value'] = array(
      '#type' => $filter_form_type,
      '#title' => $this->value_value,
      '#options' => $this->value_options,
      '#default_value' => $this->value,
    );
    if (!empty($this->options['exposed'])) {
      $identifier = $this->options['expose']['identifier'];
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $this->value;
      }
      // If we're configuring an exposed filter, add an <Any> option.
      if (empty($form_state['exposed']) || empty($this->options['expose']['required'])) {
        $any_label = variable_get('views_exposed_filter_any_label', 'new_any') == 'old_any' ? '<Any>' : t('- Any -');
        if ($form['value']['#type'] != 'select') {
          $any_label = check_plain($any_label);
        }
        $form['value']['#options'] = array('All' => $any_label) + $form['value']['#options'];
      }
    }
  }

  /**
   * Function that validates user submission in the filter configuration.
   */
  function value_validate($form, &$form_state) {
    if ($form_state['values']['options']['value'] == 'All' && !empty($form_state['values']['options']['expose']['required'])) {
      form_set_error('value', t('You must select a value unless this is an non-required exposed filter.'));
    }
  }

  /**
   * Function that returns the summary for the chosen options of the filter.
   */
  function admin_summary() {
    if ($this->is_a_group()) {
      return t('grouped');
    }
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }
    if (empty($this->value_options)) {
      $this->get_value_options();
    }
    // Now that we have the valid options for this filter, just return the
    // human-readable label based on the current value.  The value_options
    // array is keyed with either 0 or 1, so if the current value is not
    // empty, use the label for 1, and if it's empty, use the label for 0.
    return $this->value_options[!empty($this->value)];
  }

  /**
   * Configures options to be exposed.
   */
  function expose_options() {
    parent::expose_options();
    $this->options['expose']['operator_id'] = '';
    $this->options['expose']['label'] = $this->value_value;
    $this->options['expose']['required'] = TRUE;
  }

  /**
   * Adjusts the query according to the values chosen in the configuration.
   */
  function query() {
    $this->ensure_my_table();

    if($this->value == 1){
      $this->query->add_where($this->options['group'], 'webform_component.nid', '', '!=');
    }
    else{
      $this->query->add_where($this->options['group'],
        'webform_component.nid', NULL, 'is');
    }
  }
}
