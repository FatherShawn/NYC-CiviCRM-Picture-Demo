<?php
/**
 * @file nyc_picture_demo_handler_field_image_url.php
 *
 * @author Shawn P. Duncan <sduncan@dioceseli.org>
 * @date   1/26/14,  10:13 AM
 *
 *
 * Copyright 2014 by Shawn P. Duncan.  This code is
 * released under the GNU General Public License.
 * Which means that it is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

class nyc_picture_demo_handler_field_image_url extends views_handler_field {

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $image_styles = image_style_options(FALSE);
    if (module_exists('picture')) {
      $picture_group_options = array();
      $picture_group_options[] = "- Use Fallback Image Style -";
      $picture_group_options = $picture_group_options + picture_get_mapping_options();
      $form['picture_options'] = array(
        '#type' => 'fieldset',
        '#title' => t('Set Picture Module Options'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#weight' => 80,
      );
      $form['picture_group'] = array(
        '#title' => t('Picture group'),
        '#type' => 'select',
        '#default_value' => $this->options['picture_group'],
        '#options' => $picture_group_options,
        '#fieldset' => 'picture_options',
      );
      $form['fallback_image_style'] = array(
        '#title' => t('Fallback image style'),
        '#type' => 'select',
        '#default_value' => $this->options['fallback_image_style'],
        '#empty_option' => t('Automatic'),
        '#options' => $image_styles,
        '#fieldset' => 'picture_options',
      );
      $form['alt'] = array(
        '#title' => t('Alt attribute'),
        '#description' => t('The text to use as value for the <em>img</em> tag <em>alt</em> attribute.'),
        '#type' => 'textfield',
        '#default_value' => $this->options['alt'],
        '#fieldset' => 'picture_options',
      );
      $form['title'] = array(
        '#title' => t('Title attribute'),
        '#description' => t('The text to use as value for the <em>img</em> tag <em>title</em> attribute.'),
        '#type' => 'textfield',
        '#default_value' => $this->options['title'],
        '#fieldset' => 'picture_options',
      );
    }
    else {
      $form['imagestyle_options'] = array(
        '#type' => 'fieldset',
        '#title' => t('Set Image Style Options'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#weight' => 80,
      );
      $form['image_style'] = array(
        '#title' => t('Image style'),
        '#type' => 'select',
        '#default_value' => $this->options['image_style'],
        '#empty_option' => t('None (original image)'),
        '#options' => $image_styles,
        '#fieldset' => 'imagestyle_options',
      );
      $form['alt'] = array(
        '#title' => t('Alt attribute'),
        '#description' => t('The text to use as value for the <em>img</em> tag <em>alt</em> attribute.'),
        '#type' => 'textfield',
        '#default_value' => $this->options['alt'],
        '#fieldset' => 'imagestyle_options',
      );
      $form['title'] = array(
        '#title' => t('Title attribute'),
        '#description' => t('The text to use as value for the <em>img</em> tag <em>title</em> attribute.'),
        '#type' => 'textfield',
        '#default_value' => $this->options['title'],
        '#fieldset' => 'imagestyle_options',
      );
    }
  }

  /* add our options to the options definition */
  function option_definition() {
    $options = parent::option_definition();
    if (module_exists('picture')) {
      $options['picture_group'] = array('default' => 0);
      $options['fallback_image_style'] = array('default' => '');
      $options['alt'] = array(
        'default' => '',
        'translatable' => TRUE,
      );
      $options['title'] = array(
        'default' => '',
        'translatable' => TRUE,
      );
    }
    else {
      $options['image_style'] = array('default' => '');
      $options['alt'] = array(
        'default' => '',
        'translatable' => TRUE,
      );
      $options['title'] = array(
        'default' => '',
        'translatable' => TRUE,
      );
    }
    return $options;
  }

  /* Override views_handler_field pre_render.  The parent method probably doesn't need to be called
   * as it is an empty placeholder, but that could change...
   * */
  function pre_render(&$values) {
    parent::pre_render($values);
    /* Break the url to the contact image into it's parts, find the point in the array
     * that is the files directory, slice out the remaining path elements, glue them back
     * up into a path and make a public file uri out of that.  Then turn the uri into a
     * Drupal file object and replace the result so the render function can style the image.
     * */
    foreach ($values as $key => $row) {
      $file = array();
      $img_url = $this->get_value($row);
      $url_parts = explode('/', $img_url);
      $files_key = array_search('files', $url_parts);
      $url_parts = array_slice($url_parts, $files_key + 1);
      $file_path = 'public://' . implode('/', $url_parts);
      $image_info = image_get_info($file_path);
      if ($image_info) {
        $file['width'] = $image_info['width'];
        $file['height'] = $image_info['height'];
      }
      $file['alt'] = $this->options['alt'];
      /* If picture group is "use fallback" then it has value 0, so if it isn't 0
       * or null then load breakpoints otherwise set the fallback image style into a variable as a flag and for
       * use in setting up the file array for image style themeing instead
       */
      if (module_exists('picture')) {
        if (isset ($this->options['picture_group']) && ($this->options['picture_group'] === '0')) {
          /* User has chosen to fallback to core image_style */
          $image_style = $this->options['fallback_image_style'];
        }
        elseif (isset($this->options['picture_group'])) {
          $file['uri'] = $file_path;
          $mappings = picture_mapping_load($this->options['picture_group']);
          $file['breakpoints'] = picture_get_mapping_breakpoints($mappings, $this->options['fallback_image_style']);
        }
        if (!isset($image_style) && isset($this->options['title'])) {
          $file['attributes'] = array(
            '#title' => $this->options['title'],
          );
        }
      }
      if (isset($this->options['image_style']) || isset($image_style)) {
        if (isset($image_style)) {
          $file['style_name'] = $image_style;
        }
        else {
          $file['style_name'] = $this->options['image_style'];
        }
        $file['path'] = $file_path;
        $file['title'] = $this->options['title'];
      }
      if ((!isset($file['breakpoints'])) && (!isset($file['style_name']))) {
        $file['path'] = $file_path;
        $file['title'] = $this->options['title'];
      }
      $values[$key]->{$this->field_alias} = $file;
    }
  }

  function render($values) {
    /* get the file array created in $this::prerender */
    $file = $this->get_value($values);
    /* If $file['breakpoints'] was set in prerender() then we are using Picture module,
     * otherwise use core D7 image_style or fall through to raw image.
     * */
    if (isset($file['breakpoints'])) {
      return theme('picture', $file);
    }
    elseif (isset($file['style_name'])) {
      return theme('image_style', $file);
    }
    else {
      return theme('image', $file);
    }
  }
}