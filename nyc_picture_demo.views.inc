<?php
/**
 * @file nyc_ref_demo.views.inc
 * 
 * @author Shawn P. Duncan <shawn@c3design.org>
 * @date   4/3/14,  11:43 AM
 *
 * @brief auto-included views file
 *
 * Copyright 2014 by Shawn P. Duncan.  This code is
 * released under the GNU General Public License.
 * Which means that it is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

 */

function nyc_picture_demo_views_data_alter(&$data) {
  /* Change how contact images are handled
  * */
  unset($data['civicrm_contact']['image_URL']['field'][0]);
  $data['civicrm_contact']['image_URL']['field']['handler'] = 'nyc_picture_demo_handler_field_image_url';
}