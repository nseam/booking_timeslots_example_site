<?php

/**
 * @file
 *   Form settings include file
 *
 * @version
 *   $Id:$
 *
 * @author
 *   Rafal Wieczorek <kenorb@gmail.com>
 */

/**
 *  Menu callback for the settings form.
 */
function example_booking_settings_timeslots_form($form_state) {
  /*
  $form['timeslot_length'] = array(
    '#type' => 'fieldset',
    '#title' => t('Timeslot settings'),
    '#description' => t('The default length of time of a timeslot.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  for ($i = 0; $i < 24; $i++) {
    $hour_options[] = $i;
  }

  $form['timeslot_length']['example_booking_length_hours'] = array(
    '#type' => 'select',
    '#title' => t('Hours'),
    '#default_value' => variable_get('example_booking_length_hours', 1),
    '#options' => $hour_options,
  );

  for ($i = 0; $i < 60; $i++) {
    $minute_options[] = $i;
  }

  $form['timeslot_length']['example_booking_length_minutes'] = array(
    '#type' => 'select',
    '#title' => t('Minutes'),
    '#default_value' => variable_get('example_booking_length_minutes', 0),
    '#options' => $minute_options,
  );

  for ($i = 0; $i < 10; $i++) {
    $avaliable_slots[] = $i;
  }

  $form['timeslot_length']['example_booking_available_slots'] = array(
    '#type' => 'select',
    '#title' => t('Maximum avaliable slots per timeframe'),
    '#default_value' => variable_get('example_booking_available_slots', 1),
    '#options' => $avaliable_slots,
    '#description' => t('How many slots are available to book at the same time. Choose 0 to disable checking for slot availability.'),
  );

   $form['timeslot_length']['example_booking_hour_from'] = array(
    '#type' => 'select',
    '#title' => t('Hour from'),
    '#default_value' => variable_get('example_booking_hour_from', 8),
    '#options' => $hour_options,
    '#description' => t('Default starting hour.'),
  );

  $form['timeslot_length']['example_booking_hour_to'] = array(
    '#type' => 'select',
    '#title' => t('Hour to'),
    '#default_value' => variable_get('example_booking_hour_to', 18),
    '#options' => $hour_options,
    '#description' => t('Default ending hour.'),
  );
  
  $form['timeslot_length']['example_booking_not_avaliable'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Weekdays which are not available for booking'),    
    '#default_value' => variable_get('example_booking_not_avaliable', array()),
    '#options' => array(
        'monday' => t('Monday'),
        'tuesday' => t('Tuesday'),
        'wednesday' => t('Wednesday'),
        'thursday' => t('Thursday'),
        'friday' => t('Friday'),
        'saturday' => t('Saturday'),
        'sunday' => t('Sunday')
    )
  );
    */
   
  $form['#validate'][] = 'example_booking_form_validate';
  return system_settings_form($form);
}

/**
 *  Menu callback for the settings form.
 */
function example_booking_settings_form($form_state) {

   $form['entity'] = array(
    '#type' => 'fieldset',
    '#title' => t('Entity integration'),
    '#description' => t('Here you can choose which entities you want to integrate with booking time slots.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $content_types = array(0 => '(None)');
  $content_types += drupal_map_assoc(array_keys(node_type_get_types()));
  
  $non_reference_fields = array(0 => '(None)');
  $reference_fields     = array(0 => '(None)');
  $integer_fields       = array(0 => '(None)');

  foreach(field_info_fields() as $name => $info) {
    $bundles = array();
    
    if ($info['bundles']) {
      $bundles = call_user_func_array('array_merge', $info['bundles']);
    }

    $title = '';

    if ($info['type'] == 'node_reference') {
      $targets = array();
      foreach ($info['settings']['referenceable_types'] as $type_name => $enabled) {
        if ($enabled !== 0) {
          $targets[] = $type_name;
        }
      }

      $title  .= '[' . implode(' OR ', $bundles) . '] -> [' . implode(' OR ', $targets) . ']';

      $reference_fields[$name] = $title;
    }
    else {
      if ($bundles) {
        $title  .= '[' . implode(' OR ', $bundles) . '].' . $name;
      }

      if ($info['type'] == 'number_integer') {
        $integer_fields[$name] = $title;
      }
      else {
        $non_reference_fields[$name] = $title;
      }
    }
  }
  
  //$fields += drupal_map_assoc(array_keys(field_get_items('node')));
  
  $form['entity']['example_booking_1st_content_type'] = array(
    '#type' => 'select',
    '#title' => t('Primary content type'),
    '#default_value' => variable_get('example_booking_1st_content_type', ''),
    '#options' => $content_types,
    '#description' => t('Content type which is used for booking per calendar. Select None to disable the feature. E.g. Venue'),
  );

  $form['entity']['example_booking_2nd_content_type'] = array(
    '#type' => 'select',
    '#title' => t('Secondary content type'),
    '#default_value' => variable_get('example_booking_2nd_content_type', ''),
    '#options' => $content_types,
    '#description' => t('Content type which is associated with the primary content type. Select None to disable the feature. E.g. Facility'),
  );

  // Reference from the 2nd to 1st content type
  $form['entity']['example_booking_2nd_to_1st_ref_field'] = array(
    '#type' => 'select',
    '#title' => t('Secondary to primary content type reference field'),
    '#default_value' => variable_get('example_booking_2nd_to_1st_ref_field', ''),
    '#options' => $reference_fields,
    '#description' => t('Field acting as a reference from the 2nd to 1st content type'),
  );
  
  $form['entity']['example_booking_3rd_content_type'] = array(
    '#type' => 'select',
    '#title' => t('Tertiary content type'),
    '#default_value' => variable_get('example_booking_3rd_content_type', ''),
    '#options' => $content_types,
    '#description' => t('Content type which is associated with the secondary content type. Select None to disable the feature. E.g. Class'),
  );
  
  // 3rd content type capacity field
  $form['entity']['example_booking_3rd_capacity_field'] = array(
    '#type' => 'select',
    '#title' => t('Tertiary content type capacity field'),
    '#default_value' => variable_get('example_booking_3rd_capacity_field', ''),
    '#options' => $integer_fields,
    '#description' => t('Field acting as a capacity indicator for a 3rd content type'),
  );
  

  // Reference from the 3rd to 2nd content type
  $form['entity']['example_booking_3rd_to_2nd_ref_field'] = array(
    '#type' => 'select',
    '#title' => t('Tertiary to secondary content type reference field'),
    '#default_value' => variable_get('example_booking_3rd_to_2nd_ref_field', ''),
    '#options' => $reference_fields,
    '#description' => t('Field acting as a reference from the 3rd to 2nd content type'),
  );
  
  // Reference from the 3rd to 4th content type
  $form['entity']['example_booking_3rd_to_4th_ref_field'] = array(
    '#type' => 'select',
    '#title' => t('Tertiary to quaternary content type reference field'),
    '#default_value' => variable_get('example_booking_3rd_to_4th_ref_field', ''),
    '#options' => $reference_fields,
    '#description' => t('Field acting as a reference from the 3rd to 4th content type'),
  );
  
  $form['entity']['example_booking_4th_content_type'] = array(
    '#type' => 'select',
    '#title' => t('Quaternary content type'),
    '#default_value' => variable_get('example_booking_4th_content_type', ''),
    '#options' => $content_types,
    '#description' => t('Content type which is associated with the tertiary content type. Select None to disable the feature. E.g. Instructor'),
  );
  
  

  $form['views'] = array(
    '#type' => 'fieldset',
    '#title' => t('Views integration'),
    '#description' => t('Here you can associate content type used for booking with calendar view which is used for time slots. You have to choose at least one view to use booking time slots properly.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  /* FIND ALL VIEWS WITH HAS CALENDAR AS A STYLE */
  $list_views = array();
  foreach (views_get_all_views() as $view) {
    if (!empty($view->disabled)) {
      continue;
    }
    if (strpos($view->display['default']->display_options['style_plugin'], 'calendar') !== FALSE) {
      $list_views[$view->name] = $view->name; 
    }
  }

  /* CHECK IF DEFAULT CALENDAR VIEW IS ENABLED */
  if (empty($list_views)) {
    drupal_set_message(t('Please enable at least one calendar based view i.e. <a href="@url">Calendar View.</a>', array('@url' => url('admin/build/views/list'))), 'warning');
  }
  else {
    foreach ($list_views as $key => $value) {
      $view_path = example_booking_calendar_view_path($key);
      $form['views']['example_booking_view_' . $key] = array(
          '#type' => 'select',
          '#title' => t('View: %view', array('%view' => $key)),
          '#description' => t('Choose which content type should be associated with your <a target=_blank href="@url">calendar view</a>.', array('@url' => url($view_path))),
          '#default_value' => variable_get('example_booking_view_' . $key, ''),
          '#options' => array_merge(array('' => t('- None -')), $content_types),
        );
    }
  }
  if (isset($form['views']['example_booking_view_booking_calendar']) && empty($form['views']['example_booking_view_booking_calendar']['#default_value'])) {
    $form['views']['example_booking_view_booking_calendar']['#default_value'] = array('booking_slot');
  }

  $form['config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Calendar presentation and default timezone'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['config']['date_first_day'] = array(
    '#type' => 'select',
    '#title' => t('First day of week'),
    '#default_value' => variable_get('date_first_day', 0),
    '#options' => array(0 => t('Sunday'), 1 => t('Monday'), 2 => t('Tuesday'), 3 => t('Wednesday'), 4 => t('Thursday'), 5 => t('Friday'), 6 => t('Saturday')),
    '#weight' => -2
  );
  
  $form['config']['date_default_timezone'] = array(
    '#type' => 'select',
    '#title' => t('Default time zone'),
    '#default_value' => variable_get('date_default_timezone', date_default_timezone_get()),
    '#options' => system_time_zones(),
    '#description' => t('By default, dates in this site will be displayed in the chosen time zone.'),
    '#weight' => -1,
    '#attributes' => array('class' => array('timezone-detect')),
  );
  
  $form['config']['example_booking_calendar_granularity'] = array(
    '#type' => 'select',
    '#title' => t('Calendar granularity'),
    '#default_value' => variable_get('example_booking_calendar_granularity', '15'),
    '#options' => array('15' => '15 min', '30' => '30 min', '60' => '60 min'),
    '#description' => t('Time step between each time row'),
  );

  $form['config']['example_booking_enable_booking_for_past_dates'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable booking of slots from past (apply to normal users)'),
    '#default_value' => variable_get('example_booking_enable_booking_for_past_dates', '1'),
    '#description' => t('By enabling this options you will allow normal users to book slots in past. Venue managers will we able to book such slots anyway.'),
  );
  
  
  $form['config']['example_booking_durations'] = array(
    '#type' => 'textarea',
    '#title' => t('Enabled durations'),
    '#default_value' => variable_get('example_booking_durations', "15|15 min\n30|30 min\n45|45 min\n60|60 min\n120|120 min"),
    '#description' => t("List of enabled durations. Each line should contain number of minutes and title separated by '|' character. Note that those durations must be a multiple of the calendar granularity and be greater or equal to that granularity."),
  );
  
  /*
  $form['config']['example_booking_past_booking'] = array(
    '#type' => 'checkbox',
    '#title' => t("Allow to book in the past."),
    '#description' => t('If enabled, slots in the past are available to book.'),
    '#default_value' => variable_get('example_booking_past_booking', FALSE),
  );
  */

  /*
  $form['other'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  */
  
  /*
  $form['other']['example_booking_show_warnings'] = array(
    '#type' => 'checkbox',
    '#title' => t("Don't show configuration warnings, etc. on admin pages."),
    '#description' => t('If you configured module correctly or you customized already some settings, some warnings are no longer necessary. Check to disable them.'),
    '#default_value' => variable_get('example_booking_show_warnings', FALSE),
  );
  $form['other']['example_booking_show_debug_messages'] = array(
    '#type' => 'checkbox',
    '#title' => t("Show debug messages."),
    '#description' => t('Show additional debug messages (administration permission required).'),
    '#default_value' => variable_get('example_booking_show_debug_messages', FALSE),
  );
  */

  /*
  !variable_get('example_booking_show_warnings', FALSE) ?  example_booking_check_admin_options() : NULL; // Check for configuration issues
  */
  $form['#validate'][] = 'example_booking_settings_form_validate';
  return system_settings_form($form);
}

/**
 * Get calendar view path based on view name
 */
function example_booking_calendar_view_path($view_name) {
  $view = views_get_view($view_name);
  return isset($view->display[$view_name]) ? $view->display[$view_name]->display_options['path'] : '';
}

/**
 * Form API callback to validate the settings form.
 */
function example_booking_form_validate($form, &$form_state) {
  /*
  $hour_from = $form['timeslot_length']['example_booking_hour_from']['#value'];
  $hour_to = $form['timeslot_length']['example_booking_hour_to']['#value'];

  if ($hour_from >= $hour_to) {
    form_set_error('example_booking_hour_from', t("Hour To should be greater than Hour From."));
    form_set_error('example_booking_hour_to', ' ');
  }
  */
}

/**
 * Form API callback to validate the admin settings form.
 */
function example_booking_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  /*
  // MAKE THE LIST OF ACTIVATED VIEWS
  $enabled_views = array();
  preg_match_all("/example_booking_view_\w+/", join(",", array_keys($values)), $names); // find all interval array keys from the submitted form
  foreach ((array)$names[0] as $key => $vkey) { // for each key, find the items
    $view_name = substr($vkey, strlen('example_booking_view_'));
    $content_name = $values[$vkey]; // find interval for found key from form
    $enabled_views[$view_name] = $content_name;
    if (!empty($content_name)) { // if view is associated with content type...
      module_load_include('inc', 'example_booking');
      if (!($field_name = example_booking_detect_field_from_view($view_name))) { // check if that view has proper configuration
        form_set_error($vkey,
          t("Please configure <a target='_blank' href='@view_url'>%name view</a> by <a target='_blank' href='@view_arg_url'>selecting proper field</a> which you chosen for time slots and <a target='_blank' href='@view_fields_url'>adding it</a> to the list of fields, before activating the view!",
            array(
              '@view_url' => url("admin/build/views/edit/$view_name"),
              '@view_arg_url' => url("admin/build/views/nojs/config-item/$view_name/default/argument/date_argument"),
              '@view_fields_url' => url("admin/build/views/nojs/add-item/$view_name/default/field"),
              '%name' => $view_name
             )
          )
        );
      }
    }
  }
  $enabled_views = array_filter($enabled_views); // filter empty settings
  empty($enabled_views) && !variable_get('example_booking_show_warnings', FALSE) ?  drupal_set_message(t("Proper configuration require at least one view associated with a content type!"), 'warning') : NULL; // Check for configuration issues

  // CHECK PROPER CONFIGURATION OF FIELDS
  $field_names = array_flip($values['example_booking_fields']);
  $field_names_off = array_flip($values['example_booking_excluded_dates']);
  $field_names_limited = array_flip($values['example_booking_limited_dates']);
  unset($field_names[0], $field_names_off[0], $field_names_limited[0]); // remove disabled checkboxes
  if (($wrong_fields = array_intersect($field_names, $field_names_off)) && !empty($wrong_fields)) { // search for fields, which are enabled in two places at the same time
    form_set_error('example_booking_excluded_dates', t("You can't use the same field %field for non-available dates, which is already used for time slots!", array('%field' => current($wrong_fields))));
  }
  if (($wrong_fields = array_intersect($field_names, $field_names_limited)) && !empty($wrong_fields)) { // search for fields, which are enabled in two places at the same time
    form_set_error('example_booking_limited_dates', t("You can't use the same field %field for limiting dates, which is already used for time slots!", array('%field' => current($wrong_fields))));
  }
  if (($wrong_fields = array_intersect($field_names_off, $field_names_limited)) && !empty($wrong_fields)) { // search for fields, which are enabled in two places at the same time
    form_set_error('example_booking_limited_dates', t("You can't use the same field %field for limiting dates, which is already used for non-available dates!", array('%field' => current($wrong_fields))));
  }
  */
}

/**
 * General function to check admin settings
 * Check if tpl files are in the right place
 * Note: works only with default calendar view
 */
function example_booking_check_admin_options() {
  /* CHECK IF TPL TEMPLATE HAS BEEN COPIED INTO THEME FOLDER */
  /*
  $theme_path = drupal_get_path('theme', variable_get('theme_default', NULL));
  if (($registry = theme_get_registry()) && (array_search('template_preprocess_example_booking_day', (array)$registry['calendar_day']['preprocess functions']) === FALSE)) { // check or preprocess function
    drupal_set_message(t('Please <a href="@url">clear the cache</a> to reload preprocess hooks!', array('@url' => url('admin/settings/performance'))), 'warning');
  }
  if (!file_exists($theme_path . "/calendar-month.tpl.php") || !file_exists($theme_path . "/calendar-day.tpl.php")) { // TODO: do the better checking, instead of hardcoding the files (could be: calendar-day--booking-calendar.tpl.php instead)
    drupal_set_message(t('Please copy theme files from module themes/ folder into your theme path: @theme_path to see changes in calendar view and clear the cache.', array('@theme_path' => $theme_path)), 'error');
  }
  if (module_exists('calendar_multiday')) {
    drupal_set_message(t("Currently Calendar Multiday module is not supported! <a href='@url'>Please disable it!</a> See: <a href='@issue'>#1005336</a>", array(
      '@url' => url('admin/build/modules'),
      '@issue' => 'http://drupal.org/node/1005336')), 'error');
  }
  */
}

