<?php

function _ip_datatel_settings_form($form, $form_state) {

  $ipdatatel = new IpdatatelConfig();
  
  //dpm($profile2);
  $settings = array();
  
  $form['summary'] = array(
    '#type' => 'item',
    '#markup' => t('IPDATATEL'),  
  );
  
	$form['wrapper'] = array(
    '#type' => 'vertical_tabs',
  );
  
  
  $form['general_settings'] = array(
  	'#type' => 'fieldset',
  	'#collapsible' => TRUE,
  	'#collapsed' => TRUE,
  	'#group' => 'wrapper',
  	'#title' => t('General'),
  );
  
  //dealer section
  $form['general_settings']['dealer_info'] = array(
  	'#type' => 'fieldset',
  	'#collapsible' => TRUE,
  	'#collapsed' => FALSE,
  	'#title' => t('IpDatatel Dealer Info'),
  );
  $settings['ip_datatel_dealer'] = variable_get('ip_datatel_dealer');
  
  $form['general_settings']['dealer_info']['ip_datatel_dealer'] = array(
  	'#type' => 'radios',
  	'#default_value' => !empty($settings['ip_datatel_dealer']) ? $settings['ip_datatel_dealer'] : 0,
  	'#title' => t('Dealer'),
  	'#options' => array(
  	 1 => t('Enable Dealer Settings'),
  	 0 => t('Disable Dealer Settings'),
  	),
  	'#weight' => -100,
  );
  $settings['ip_datatel_dealer_id'] = variable_get('ip_datatel_dealer_id', '');
  $form['general_settings']['dealer_info']['ip_datatel_dealer_id'] = array(
  	'#type' => 'textfield',
  	'#title' => t('Dealer ID'),
  	'#description' => t('Your Dealer id provided by IpDatatel'),
  	'#weight' => -99,
  	'#size' => 20,
  	'#default_value' => !empty($settings['ip_datatel_dealer_id']) ? $settings['ip_datatel_dealer_id'] : '',
  	'#max_length' => 254,
  	'#states' => array(
			'visible' => array(
				':input[name="ip_datatel_dealer"]' => array('checked' => TRUE),
			),
		),
  );
  $settings['ip_datatel_dealer_pass'] = variable_get('ip_datatel_dealer_pass', '');
  $form['general_settings']['dealer_info']['ip_datatel_dealer_pass'] = array(
  	'#type' => 'password',
  	'#title' => t('Dealer Password'),
  	'#description' => t('Your Dealer password provided by IpDatatel'),
  	'#max_length' => 254,
  	'#default_value' => !empty($settings['ip_datatel_dealer_pass']) ? $settings['ip_datatel_dealer_pass'] : '',
  	'#weight' => -98,
  	'#states' => array(
			'visible' => array(
				':input[name="ip_datatel_dealer"]' => array('checked' => TRUE),
			),
		),
  );
  //profile2 section
  $form['general_settings']['profile2'] = array(
  	'#type' => 'fieldset',
  	'#collapsible' => TRUE,
  	'#collapsed' => FALSE,
  	'#title' => t('Profile2'),
  );
  $settings['ip_datatel_enable_profile2'] = variable_get('ip_datatel_enable_profile2', '');
  $form['general_settings']['profile2']['ip_datatel_enable_profile2'] = array(
  	'#type' => 'radios',
  	'#title' => t('Enable Profile2 Users'),
  	'#default_value' => !empty($settings['ip_datatel_enable_profile2']) ? $settings['ip_datatel_enable_profile2'] : 0,
  	'#options' => array(
  	 1 => t('Enable Profile2 Settings'),
  	 0 => t('Disable Profile2 Settings'),
  	),
  	'#weight' => -100,
  );
  if(!empty($settings['ip_datatel_enable_profile2']) && $settings['ip_datatel_enable_profile2'] == 1){
    $profile2 = $ipdatatel->loadProfile2();
    if(!empty($profile2)){
      $default = array(0 => 'none');
      $profiles = array_merge($default, $profile2);
      $settings['ip_datatel_profile2_type'] = variable_get('ip_datatel_profile2_type', '');
      $form['general_settings']['profile2']['ip_datatel_profile2_type'] = array(
      	'#type' => 'select',
      	'#title' => t('Profile2 Type'),
      	'#default_value' => !empty($settings['ip_datatel_profile2_type']) ? $settings['ip_datatel_profile2_type'] : 0,
      	'#options' => $profiles,
      	'#weight' => -100,
      );
    }
    $settings['ip_datatel_profile2_field'] = variable_get('ip_datatel_profile2_field', '');
    $form['general_settings']['profile2']['ip_datatel_profile2_field'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Field Containing Customers'),
    	'#description' => t('Enter the fields machine name which contains usernames of IpDatatel customers ( have to be customers not users attatched to a customer )'),
    	'#max_length' => 254,
    	'#weight' => -99,
    	'#default_value' => !empty($settings['ip_datatel_profile2_field']) ? $settings['ip_datatel_profile2_field'] : '',
    	'#states' => array(
  			'visible' => array(
  				':input[name="ip_datatel_enable_profile2"]' => array('checked' => TRUE),
  			),
  		),
    );
    $form['general_settings']['profile2']['ip_datatel_profile2_id_field'] = array(
    	'#type' => 'textfield',
    	'#title' => t("Field Containing Customers id's ( Not Required )"),
    	'#description' => t('Enter the Customer Id Field machine name ( have to be id of customers not users attatched to a customer )'),
    	'#max_length' => 254,
    	'#weight' => -98,
    	'#default_value' => !empty($settings['ip_datatel_profile2_id_field']) ? $settings['ip_datatel_profile2_id_field'] : '',
    	'#states' => array(
  			'visible' => array(
  				':input[name="ip_datatel_enable_profile2"]' => array('checked' => TRUE),
  			),
  		),
    );
  }
  //block settings
  $form['block_settings'] = array(
  	'#type' => 'fieldset',
  	'#collapsible' => TRUE,
  	'#collapsed' => FALSE,
  	'#group' => 'wrapper',
  	'#title' => t('Block Settings'),
  );
  $settings['ip_datatel_block_status'] = variable_get('ip_datatel_block_status', '');
  $form['block_settings']['ip_datatel_block_status'] = array(
  	'#type' => 'radios',
  	'#title' => t('IpDatatel Block'),
  	'#default_value' => !empty($settings['ip_datatel_block_status']) ? $settings['ip_datatel_block_status'] : 0,
  	'#options' => array(
  	 1 => t('Enable Block'),
  	 0 => t('Disable Block'),
  	),
  	'#weight' => -100,
  );
  $settings['ip_datatel_block_settings'] = variable_get('ip_datatel_block_settings', '');
  $form['block_settings']['ip_datatel_block_settings'] = array(
  	'#type' => 'checkboxes',
  	'#title' => t('To Show In Block'),
  	'#default_value' => !empty($settings['ip_datatel_block_settings']) ? $settings['ip_datatel_block_settings'] : array('attatched_users'),
  	'#options' => array(
  	 'attatched_users' => t('Users Attached To This Customer'),
  	 'edit_users' => t('Edit Users Link'),
  	 'add_users' => t('Add Users'),
  	 'delete_users' => t('Delete Users'),
  	 'events' => t('Show Events'),
  	),
  	'#weight' => -99,
  	'#states' => $ipdatatel->ipDatatelStates('visible', 'ip_datatel_block_status', array('checked' => TRUE)),
  );
  
  //page settings
  $form['page_settings'] = array(
  	'#type' => 'fieldset',
  	'#collapsible' => TRUE,
  	'#collapsed' => FALSE,
  	'#group' => 'wrapper',
  	'#title' => t('Page Settings'),
  );
  $settings['ip_datatel_page_status'] = variable_get('ip_datatel_page_status', '');
  $form['page_settings']['ip_datatel_page_status'] = array(
  	'#type' => 'radios',
  	'#title' => t('IpDatatel Page'),
  	'#default_value' => !empty($settings['ip_datatel_page_status']) ? $settings['ip_datatel_page_status'] : 0,
  	'#options' => array(
  	 1 => t('Enable Page'),
  	 0 => t('Disable Page'),
  	),
  	'#weight' => -100,
  );
  $settings['ip_datatel_page_settings'] = variable_get('ip_datatel_page_settings', '');
  $form['page_settings']['ip_datatel_page_settings'] = array(
  	'#type' => 'checkboxes',
  	'#title' => t('To Show On Page'),
  	'#default_value' => !empty($settings['ip_datatel_page_settings']) ? $settings['ip_datatel_page_settings'] : array('attatched_users'),
  	'#options' => array(
  	 'attatched_users' => t('Users Attached To This Customer'),
  	 'edit_users' => t('Edit Users Link'),
  	 'events' => t('Show Events'),
  	 'add_users' => t('Add Users'),
  	 'delete_users' => t('Delete Users'),
  	),
  	'#weight' => -99,
  	'#states' => $ipdatatel->ipDatatelStates('visible', 'ip_datatel_page_status', array('checked' => TRUE)),
  );
  return system_settings_form($form);
}


class IpdatatelConfig{

  public $profile2 = array();
  
  public function loadProfile2(){
    $result = NULL;
    try{
      $query = 	$sql = "SELECT type, label FROM {profile_type}";
      $result = db_query($sql);
      if(!empty($result)){
        foreach($result as $data){
          $this->profile2[$data->type] = $data->label;
        }
        $result = $this->profile2;
      }
    }
    catch(Exeption $e){
      throw new Exeption('No Profile2 profiles can be found');
    }
    return $result;
  }
  
  
  
  
  public function loadUsers(){
    
  }
  
  
  

  
  private function Query(){
    
  }
  
  
  
  public function ipDatatelStates($type, $field, $args){
    return array(
			$type => array(
				':input[name="'.$field.'"]' => $args,
			),
		);
  }
  
  
  
}