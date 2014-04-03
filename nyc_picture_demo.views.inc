<?php
/**
 * @file nyc_ref_demo.views.inc
 * 
 * @author Shawn P. Duncan <shawn@c3design.org>
 * @date   4/3/14,  11:43 AM
 *
 * @brief auto-included views file
 */

function nyc_picture_demo_views_data_alter(&$data) {
  /* Change how contact images are handled
  * */
  unset($data['civicrm_contact']['image_URL']['field'][0]);
  $data['civicrm_contact']['image_URL']['field']['handler'] = 'nyc_picture_demo_handler_field_image_url';
}