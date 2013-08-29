<?php


class IpdatatelAlarmProfile {


	private $drupal_permissions = array();
	private $bat_settings = array();
	private $alarm_settings = array();
  public $add_users_permissions = array();

	
	public function __construct($account_information, $uid){
		$this->account_information = $account_information;
		$this->uid = $uid;
		$this->cust_block_settings = variable_get('ip_datatel_block_settings');
    $this->admin_page_settings = variable_get('ip_datatel_page_settings');
	}

	
	public function sortAlarmAccounts(){
  	global $user;
    $format_account_data = array();
    $account_users = array();
    $users = array();
    $tools = array();
    $admin = FALSE;
    $tool_item = array();
    $account_wrapper = array();
    $roles = $user->roles;
    $account_info = $this->account_information;
    $admins = array('administrator', 'supervisor');
    drupal_add_js(drupal_get_path('module', 'ip_datatel') .'/js/ip_datatel_login.js', array('type' => 'file', 'scope' => 'header'));
    $tools['wrapper'] = $this->alarmDataWrapper('Alarm Tools', -100, FALSE);
    $tool_item['user_login'] = $this->toolsLogin();
    drupal_add_js('misc/form.js');
		drupal_add_js('misc/collapse.js');   
    //$block_settings = $this->alarmPermissionsRout($this->cust_block_settings);
    $account_wrapper['accounts_wrapper'] = $this->alarmDataWrapper('Alarm Accounts', -100, FALSE);
    $accounts = current($account_info);
      foreach($accounts as $account_name => $account_data){
      	 $bat_settings = $this->batSettings($account_data, 'permissions');
      	 $admin_permission = $this->batSettings($account_data, 'admin_permissions');
      	 //for admins
      	 if(array_intersect($admins, $roles)){
	      	 $bat_settings = 'admin';
	      	 $admin = TRUE;
      	 }
      	 $username = $account_data['username'];
				 $alarm_settings = $this->alarmPermissionsRout(NULL, $bat_settings, $admin_permission, $username, $this->uid, $account_data['pass'], $account_data['node_id']);
        switch($account_data['account_type']){
        case 1:
        case 2:
        case 4:
          $account_wrapper['accounts_wrapper'][$account_name] = $this->alarmDataWrapper($username);
          $account_wrapper['accounts_wrapper'][$account_name]['menu'] = $this->toolListTools();
          $account_wrapper['accounts_wrapper'][$account_name]['table'] = $this->listAlarmUsers($account_data);
					if(!empty($alarm_settings['recent_events']) && $alarm_settings['recent_events'] == TRUE){
						$recent_alarm_notifications = $this->recentAlarmEvents($account_data, $this->uid);
						$account_wrapper['accounts_wrapper'][$account_name]['recent_events_wrapper'] = $this->alarmDataWrapper('Recent Notifications');
						$account_wrapper['accounts_wrapper'][$account_name]['recent_events_wrapper']['recent_events'] = $recent_alarm_notifications;
					}
					if(!empty($admin_permission['process_alarms']) && $admin_permission['process_alarms'] == TRUE){
						//$account_wrapper['accounts_wrapper'][$account_name] = $this->toolsProcessAlarms($this->uid);
					}
					if($admin == TRUE){
						$account_wrapper['accounts_wrapper'][$account_name]['zones'] = $this->alarmZonesMarkup('', 'aliase-replace', '');
					}
          break;
        case 3:
          if(!empty($users)){
            unset($account_data['attatched_users']);
            $account_data['attatched_users'] = $users;
          }
          $account_wrapper['accounts_wrapper'][$account_name] = $this->alarmDataWrapper($username);
          $account_wrapper['accounts_wrapper'][$account_name]['table'] = $this->listAlarmUsers($account_data);
					$account_wrapper['accounts_wrapper'][$account_name]['menu'] = $this->toolListTools();
          break;
        }
      }
    $this->addToRender($account_wrapper);
    if(!empty($this->add_users_permissions)){
	    $tool_item['add_users'] = $this->toolsAddUsers();
    }
		$tools['wrapper']['menu'] = $this->toolListTools($tool_item, TRUE);
		array_unshift($this->for_render, $tools);
    //$alarm_tools = build_alarm_block($this->cust_block_settings, $username, $account_name, $this->uid);
  }  
  
  
  public $for_render = array();
  public function addToRender($data){
    $this->for_render[] = $data;
  }
  
  
  public function recentAlarmEvents($account_data, $uid){
	  $recent_events = ip_datatel_load_alarm_notifications($this->uid, NULL, $account_data, 5);
		return $recent_events;
  }
  
  
  
  public $tools = array();
  private function alarmPermissionsRout($block_settings = NULL, $bat_settings = NULL, $admin_settings = NULL, $username = NULL, $uid = NULL, $pass = NULL, $nid = NULL){
  	$tools = array();
		if(!empty($block_settings)){
			foreach($block_settings as $key => $value){
				//$all_permissions['block_settings'][] = $value;
			}
		}
		if(!empty($bat_settings) && $bat_settings != 'admin'){
	  	foreach($bat_settings as $key => $value){
			  switch($key){
				  case 'add_users':
				  	$this->tools['add_users'] = $this->allowAddUsersPermissions($username);
				  	$tools['add_users'] = TRUE;
				  	break;
				  case 'events':
				  	$this->tools['events'] = $this->toolsViewEvents($uid, $username);
				  	break;
				  case 'recent_events':
				  	$tools['recent_events'] = TRUE;
				  	break;
			  }
		  }
	 }else{
		 $this->tools['add_users'] = $this->allowAddUsersPermissions($username);
		 $this->tools['events'] = $this->toolsViewEvents($uid, $username);
		 $this->tools['generate_zones'] = $this->generateZones($username, $pass, $nid);
	 }
	 if(!empty($admin_settings)){
		 foreach($admin_settings as $key => $setting){
			 switch($setting){
				 case'process_alarms':
				 		$tools['process_alarms'] = TRUE;
				 	break;
			 }
		 }
	 }
	  return $tools;
  }
 
 
  private function allowAddUsersPermissions($username){
	  $this->add_users_permissions[$username] = TRUE;
  }
  
  private function toolsProcessAlarms($uid){
  	$content = array();
		$content['alarm_process'] = array(
			'#type' => 'radios',
			'#title' => t('Process Alarms'),
			'#group' => 'wrapper',
			'#default_value' => 1,
			'#options' => array(
				'1' => 'On',
				'0' => 'Off',
			),
		);
		return $content;
  }
  
  
  private function toolsAddUsers(){
		$path = ''.$_SERVER['REQUEST_URI'].'#';
    $tools = array(
    	'data' => l('Add User', $path, array('')),
    	'id' => 'add-user',
    	'data-reveal-id' => 'add_user',
    	'class' => array('button', 'add'),
    );
    return $tools;
  }
  
  private function generateZones($user, $pass, $nid){
	  drupal_add_js(drupal_get_path('module', 'ip_datatel') .'/js/ip_datatel_gen_zones.js', array('type' => 'file', 'scope' => 'header'));
			$tools = array(
				'data' => '<div class="generate-aliases" data-utility="zone_aliase" data-user="'.$user.'" data-pass="'.$pass.'" data-nid="'.$nid.'" >
				<input type="button" class="form-submit" class="generate-zone-aliases" value="Generate Zone Aliases"/></div>',
				/*<div class="generate-aliases" data-utility="user_aliase" data-user="'.$user.'" data-pass="'.$pass.'" data-nid="'.$nid.'" >
				<input type="button" class="form-submit" class="generate-zone-aliases" value="Generate User Aliases"/>
				</div>*/
			);
			return $tools;
  }
  
  private function toolsViewEvents($uid, $username){
		$path = '../../alarm-settings/'.$uid.'/event_log/'.$username.'';
  	$tools = array(
  	  'data' => l('View Events', $path, array('')),
  	  'class' => array('button', 'next'),
  	);
  	return $tools;
  }
  
  
  private function toolsLogin(){
		$tools = array(
  	  'data' => l('Alarm Login', 'http://'.$_SERVER['HTTP_HOST'].''.$_SERVER['REQUEST_URI'].'#', array('')),
  	  'class' => array('alarm_login', 'button', 'star'),
  	);
  	return $tools;
  }
  
  
  public function alarmDataWrapper($username, $weight = NULL, $collapsed = TRUE){
  	$weight = !empty($weight) ? $weight : 100;
  	$collapse = $collapsed == FALSE ? '' : 'collapsed';
    return array(
      '#type' => 'fieldset',
      '#title' => t($username),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => $weight,
      '#attributes' => array(
        'class' => array('user-alarms-fieldset', 'large-12', 'columns', 'collapsible', $collapse),
      ),
    );
  }
  
  
  private function toolListTools($alarm_tools = NULL, $admin = FALSE){
  	$tools = !empty($alarm_tools) ? $alarm_tools : $this->tools;
	  return array(
	  	'wrapper' => array(
	  		'#type' => 'container',
	  		'#attributes' => array(
	  			'class' => array('alarm-tools-wrapper'),
	  		),
	  	'menu' => array(
	      '#theme' => 'item_list',
	      '#items' => $tools,
	      '#type' => 'ul',
	      '#attributes' => array('class' => array('alarm-menu-wrapper')),
	      '#container_id' => '',
	      '#prefix' => '<section>',
	      '#suffix' => '</section>',
	      '#weight' => -100,
	     ),
	    ),
    );
  }
  
  
  private function alarmZonesMarkup($markup, $id, $class){
  	$content = array();
  	$content['zones_wrapper'] = array(
  		'#type' => 'container',
  		'#attributes' => array(
  			'id' => 'generate-zones',
  			'class' => array('large-12 columns'),
  		),
  		'#weight' => 100,
  	);
  	$content['zones_wrapper']['zones'] = array(
  		'#type' => 'markup',
	  	'#markup' => $markup,
	  	'#prefix' => '<div id="'.$id.'" class="'.$class.'">',
	  	'#suffix' => '</div>',
	  	'#weight' => -100,
  	);
  	$content['zones_wrapper']['save'] = array(
  		'#type' => 'markup',
	  	'#markup' => 'Save Aliases',
	  	'#prefix' => '<div id="save-zones" class="button">',
	  	'#suffix' => '</div>',
	  	'#weight' => 100,
  	);
  	
		return $content;
  }



  public function renderData(){
    $renderData = $this->for_render;
    return $renderData;
  }
  
  
  
  
  public function alarmSettings(){
    global $user;
    $alarm_settings = array();
      if(in_array('administrator', $user->roles) || in_array('supervisor', $user->roles)){
        return array('TRUE');
      }else{
	      return FALSE;
      }
  }
  
  public function batSettings($account_data, $permission_type){
	  if(!empty($account_data[$permission_type])){
	 		return $account_data[$permission_type];
	  }else{
		  return array('');
	  }
  }
  
  
  
  
  private function listAlarmUsers($data){
  
    $user_list = array();
    $block_settings = $this->cust_block_settings;
    if(!empty($data['attatched_users'])){
      $attatched_users = $data['attatched_users'];
      $user_list['users_wrapper'] = $this->alarmDataWrapper('Alarm Users', -100, TRUE);
      $user_list['users_wrapper']['#attributes'] = array(
      	'class' => array('user-alarm-wrapper', 'large-12', 'columns'),
      );


      $uid = $this->uid;
      $options = array();
      $alarm_settings = $data['permissions'];
      $access = $this->checkAccess($data['permissions']);
      $headers = array();
      
      $headers[] = t('Username');
      foreach($access as $type => $value){
        if(!empty($value)){
          $headers[] = $value;
        }
      }
      
      foreach($attatched_users as $key => $value){
        $options = array();
        $rows[$key] = array(
          'data' => array(
            array(
              'data' => $value,
            ),
          ),
        );
        
        $edit = !empty($access['edit']) || in_array('edit_users', $alarm_settings) ? array('data' => l('Edit', '../../alarm-settings/'.$uid.'/cust_info/'.$value.'', $options)) : '';
        if(!empty($edit)){
          array_push($rows[$key]['data'], $edit);
        }
        $delete = !empty($access['delete']) || in_array('delete_users', $alarm_settings) ? array('data' => l('Delete', '../../alarm-settings/'.$uid.'/cust_remove/'.$value.'/'.$data['node_id'].'', $options)) : '';
        if(!empty($delete)){
          array_push($rows[$key]['data'], $delete);
        }
      }
    
    
      $user_list['users_wrapper']['table'] = array(
        '#theme' => 'table',
        '#header' => $headers,
        '#rows' => $rows,
        '#attributes' => array(
          'class' => array('alarm-user-list'),
        ),
        '#empty' => t('No Users Found'),
        '#weight' => 100,
      );
    }
    
    return $user_list;
  }
  
  public function checkAccess($alarm_settings){
    $access = array();
    $access['edit'] = user_access('edit_delete') || in_array('edit_users', $alarm_settings) ? t('Edit') : NULL;
    $access['delete'] = user_access('edit_delete') || in_array('delete_users', $alarm_settings)? t('Delete') : NULL;
    $this->drupal_permissions = $access;
    return $access;
  }

}