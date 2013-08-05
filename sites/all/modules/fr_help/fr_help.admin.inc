<?php

function _fr_help_settings_form($form, $form_state) {
  
  $form['summary'] = array(
    '#type' => 'item',
    '#markup' => t('Help Text For User And Admin Dashboards'),  
  );
  
	$form['wrapper'] = array(
    '#type' => 'vertical_tabs',
  );

  $vars = array(
		"first_stop" => 'User Menu',
		'sec_stop' => 'User Alarm',
		'prefrences' => 'User Prefrences',
		"second_stop" => 'Incident Reports',
		'third_stop' => 'Virtual Tours',
		'fourth_stop' => 'Events',
		'fifth_stop' => 'Saved Files',
		'sixth_stop' => 'Admin Tools',
		'seventh_stop' => 'Admin Search',
		'eighth_stop' => 'Virtual Tours Today',
		'cameras' => 'Cameras ( DRV ) Login',
		'alarm' => 'Virtual Alarm Login'
		);
		

foreach($vars as $key => $var){
	 $form['help_tours']['indaviduals'][$key] = array(
  	'#type' => 'fieldset',
  	'#collapsible' => TRUE,
  	'#collapsed' => TRUE,
  	'#group' => 'wrapper',
  	'#title' => t($var),
  );
  
    $tours_text =  variable_get($key, '');
    $form['help_tours']['indaviduals'][$key][$key] = array(
    '#type' => 'text_format',
    '#title' => t($var),
    '#default_value' => !empty($tours_text['value']) ? $tours_text['value'] : 'This is default help text',
    '#format' => !empty($tours_text['format']) ? $tours_text['format'] : 'full_html',
    '#weight' => 1,
    '#group' => 'wrapper',
  );
 }
  $form['help_subjects'] = array(
      '#type' => 'textfield', 
      '#title' => t('Fortified Help Subjects'), 
      '#default_value' => variable_get('help_subjects', ''),  
      '#required' => FALSE,
      '#autocomplete_path' => '_fr_help_autocomplete',
      '#disabled' => TRUE,
    );
  
    $form['help_subjects_name'] = array(
      '#type' => 'hidden', 
      '#title' => t('Fortified Help Subjects'), 
      '#default_value' => variable_get('help_subjects_name', ''),  
      '#required' => FALSE,
    );

  // Perform our custom submit before system submit
  //$form['#submit'][] = 'fr_help_form_submit';
  
  return system_settings_form($form);
}