<?php

/**
 * @file
 * Definition of webform_views_filter_has_components.
 */

/**
 * Simple filter to handle matching of boolean values
 */
class webform_views_filter_has_components extends views_handler_filter_boolean_operator {


  /**
   * Redefining query to filter content based on the webform components table.
   */
  function query() {
    $this->ensure_my_table();
    $field = "$this->table.$this->real_field";

    if ($this->value == 1) {
      $this->query->add_where($this->options['group'], $field, '', '!=');
    }
    else {
      $this->query->add_where($this->options['group'], $field, NULL, 'is');
    }
  }
}
