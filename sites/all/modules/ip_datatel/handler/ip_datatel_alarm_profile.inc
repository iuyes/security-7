<?php


class IpdatatelAlarmProfile {


	private $drupal_permissions = array();
	private $bat_settings = array();
	private $alarm_settings = array();
	public $add_users_permissions = array();
	public $account_profile;
	public $account_permissions;
	public $access = FALSE;
	public $admin = FALSE;
	public $account_owner;
	public $tools = array();

	public function __construct($account_information, $uid, $account_owner = NULL){
		$this->account_information = $account_information;
		$this->uid = $uid;
		$this->cust_block_settings = variable_get('ip_datatel_block_settings');
		$this->admin_page_settings = variable_get('ip_datatel_page_settings');
		$this->account_owner = $account_owner;
		$this->account_permissions = $this->accountPermissions();
	}


	public function accountPermissions(){
		$permissions = array();
		$this->account_type = $this->account_owner['role'];
		switch($this->account_owner['number']){
		case 3;
			$this->access = TRUE;
			$this->admin = TRUE;
			$this->account_profile = 'admin';
			$permissions['alarm'] = TRUE;
			break;
		case 4:
			$this->access = FALSE;
			$this->account_profile = profile2_load_by_user($this->uid, $type_name = NULL);
			if(!empty($this->account_profile['customer']->field_customer_alarm_permissions['und'][0]['value'])){
				$permissions['alarm'] = unserialize($this->account_profile['customer']->field_customer_alarm_permissions['und'][0]['value']);
			}
			break;
		case 9:
			$this->access = TRUE;
			$this->account_profile = profile2_load_by_user($this->uid, $type_name = NULL);
			$permissions['alarm'] = TRUE;
			break;
		case 11:
			$this->access = TRUE;
			$this->account_profile = 'central_station';
			$this->admin = TRUE;
			$permissions['alarm'] = TRUE;
			break;
		}
		return $permissions;
	}


	public function sortAlarmAccounts(){
		global $user;
		$format_account_data = array();
		$account_users = array();
		$users = array();
		$tools = array();
		$admin = FALSE;
		$recent_events = '';
		$tool_item = array();
		$account_wrapper = array();
		$roles = $user->roles;
		$admin = $this->admin;
		$account_info = $this->account_information;
		$admins = array('administrator', 'supervisor');
		drupal_add_js(drupal_get_path('module', 'ip_datatel') .'/js/ip_datatel_login.js', array('type' => 'file', 'scope' => 'header'));
		drupal_add_js('misc/form.js');
		drupal_add_js('misc/collapse.js');
		//$block_settings = $this->alarmPermissionsRout($this->cust_block_settings);
		$tool_item['user_login'] = $this->toolsLogin();
		$accounts = current($account_info);
		$account_permissions = $this->account_permissions;
		$account_wrapper['accounts_wrapper'] = $this->alarmDataWrapper('Alarm Accounts', '', TRUE, FALSE, FALSE, 100);
		$account_users['accounts_wrapper']['inner']['markup'] = '';
		$account_wrapper['tools'] = $this->alarmDataWrapper('Alarm Tools', NULL, TRUE, FALSE, FALSE, 'fs-depth-2', 100);
		//loop
		foreach($accounts as $account_name => $account_data){
			if(!empty($account_permissions['alarm'][$account_name]) && array_key_exists('view_alarm', $account_permissions['alarm'][$account_name]) || $this->access == TRUE){

				$bat_settings = $this->batSettings($account_data, 'permissions');
				$admin_permission = $this->batSettings($account_data, 'admin_permissions');
				$username = $account_data['username'];

				switch($account_data['account_type']){
				case 1:
				case 2:
				case 4:
					$gernerate_zones = NULL;
					$auth_data = array('username' => $username, 'uid' => $this->uid, 'pass' => $account_data['pass'], 'nid' => $account_data['node_id']);
					$alarm_settings = $this->newPermissionsFunction($bat_settings, $admin_permission, $auth_data, $account_permissions);
					$user_data = array();
					$account_users = $this->alarmDataWrapper($username, '', TRUE, FALSE, FALSE, 'fs-depth-2', 100);
					$account_tools = $this->toolListTools();
					$alarm_users = $this->listAlarmUsers($account_data, $username);
					if(!empty($alarm_settings['recent_events']) && $alarm_settings['recent_events'] || $this->admin || $this->access){
						$recent_events = $this->alarmDataWrapper('Recent Notifications', NULL, TRUE, FALSE, FALSE, 'fs-depth-3', 100);
						$recent_events['inner']['markup'] = drupal_render($this->recentAlarmEvents($account_data, $this->uid));
					}
					//for admins
					if(!empty($admin_permission['process_alarms']) && $admin_permission['process_alarms'] == TRUE){
						//$account_wrapper['accounts_wrapper'][$account_name] = $this->toolsProcessAlarms($this->uid);
					}
					if($this->admin){
						//$gernerate_zones = $this->alarmZonesMarkup('', 'aliase-replace', '');
					}
					if(!empty($recent_events)){
						$recent_events = drupal_render(fr_groups_accordion_fieldset($recent_events));
					}

					$account_users['inner']['markup'] .=  drupal_render($account_tools) . $alarm_users . $recent_events;

					$account_wrapper['accounts_wrapper']['inner']['markup'] .= drupal_render(fr_groups_accordion_fieldset($account_users));

					break;

				case 3:

					$auth_data = array('username' => $username, 'uid' => $this->uid, 'pass' => $account_data['pass'], 'nid' => $account_data['node_id']);
					$alarm_settings = $this->newPermissionsFunction($bat_settings, $admin_permission, $auth_data, $account_permissions);

					$user_data = array();
					if(!empty($users)){
						unset($account_data['attatched_users']);
						$account_data['attatched_users'] = $users;
					}
					$gernerate_zones = NULL;
					$user_data = array();
					$account_users = $this->alarmDataWrapper($username, '', TRUE, FALSE, FALSE, 'fs-depth-2', 100);
					$account_tools = $this->toolListTools();
					$alarm_users = $this->listAlarmUsers($account_data, $username);
					$alarm_users = !empty($alarm_users) ? $alarm_users : '';
					
					
					if(!empty($alarm_settings['recent_events']) && $alarm_settings['recent_events'] || $this->admin || $this->access){
						$recent_events = $this->alarmDataWrapper('Recent Notifications', NULL, TRUE, FALSE, FALSE, 'fs-depth-3', 100);
						$recent_events['inner']['markup'] = drupal_render($this->recentAlarmEvents($account_data, $this->uid));
					}
					//for admins
					if(!empty($admin_permission['process_alarms']) && $admin_permission['process_alarms'] == TRUE){
						//$account_wrapper['accounts_wrapper'][$account_name] = $this->toolsProcessAlarms($this->uid);
					}
					if($this->admin){
						//$gernerate_zones = $this->alarmZonesMarkup('', 'aliase-replace', '');
					}
					if(!empty($recent_events)){
						$recent_events = drupal_render(fr_groups_accordion_fieldset($recent_events));
					}
					$account_users['inner']['markup'] .=  drupal_render($account_tools) . $alarm_users . $recent_events;
					$account_wrapper['accounts_wrapper']['inner']['markup'] .= drupal_render(fr_groups_accordion_fieldset($account_users));

					break;
				}
			}
		}

		if(!empty($this->add_users_permissions)){
			$tool_item['add_users'] = $this->toolsAddUsers();
		}
		$account_wrapper['tools']['inner']['markup'] = drupal_render($this->toolListTools($tool_item, TRUE));
		$tools = drupal_render(fr_groups_accordion_fieldset($account_wrapper['tools']));
		$account_wrapper['accounts_wrapper']['inner']['markup'] .= $tools;
		$account_wrapper = drupal_render(fr_groups_accordion_fieldset($account_wrapper['accounts_wrapper']));

		$this->addToRender($account_wrapper);
	}









	public $for_render = array();
	public function addToRender($data){
		$this->for_render = $data;
	}


	public function recentAlarmEvents($account_data, $uid){
		$recent_events = ip_datatel_load_alarm_notifications($this->uid, NULL, $account_data, 5);
		return $recent_events;
	}





	private function newPermissionsFunction($bat_settings = NULL, $admin_settings = NULL, $auth_data, $account_permissions){
		$uid = $auth_data['uid'];
		$pass = $auth_data['pass'];
		$username = $auth_data['username'];
		$nid = $auth_data['nid'];
		$account_owner_role = $this->account_owner['number'];
		switch($account_owner_role){
		case 3:
			$permissions = TRUE;
			$this->defaultAdminTools($username, $pass,  $uid, $nid, TRUE);
			break;
		case 4:
			$permissions = $this->newPermissionsRout($bat_settings, $username, $uid , $account_permissions);
			//dpm($permissions);
			break;
		case 9:
			$permissions = TRUE;
			$this->defaultAdminTools($username, $pass,  $uid, $nid, FALSE);
			break;
		case 11:
			$permissions = TRUE;
			$this->defaultAdminTools($username, $pass,  $uid, $nid, FALSE);
			break;
		}
		return $permissions;
	}


	private function defaultAdminTools($username = NULL, $pass = NULL, $uid = NULL, $nid = NULL, $gen = FALSE){
		$tools->tools = array();
		$this->tools['add_users'] = $this->allowAddUsersPermissions($username);
		$this->tools['events'] = $this->toolsViewEvents($uid, $username);
		if($gen){
			$this->tools['generate_zones'] = $this->generateZones($username, $pass, $nid);
		}
		$this->tools['process_alarms'] = TRUE;
		return $tools;
	}





	function newPermissionsRout($bat_settings, $username, $uid, $account_permissions){
		$tools = array();
		foreach($bat_settings as $key => $value){
			switch($key){
			case 'add_users':
				if(!empty($bat_settings[$key]) && array_key_exists($key, $account_permissions['alarm'][$username])){
					$this->tools['add_users'] = $this->allowAddUsersPermissions($username);
					$tools['add_users'] = TRUE;
				}
				break;
			case 'events':
				if(!empty($bat_settings[$key]) && array_key_exists($key, $account_permissions['alarm'][$username])){
					$this->tools['events'] = $this->toolsViewEvents($uid, $username);
				}
				break;
			case 'recent_events':
				if(!empty($bat_settings[$key]) && array_key_exists($key, $account_permissions['alarm'][$username])){
					$tools['recent_events'] = TRUE;
				}
				break;
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


	public function alarmDataWrapper($title, $class = NULL, $collapsible = TRUE, $status = FALSE, $inner_status = FALSE, $iclass = NULL, $weight = 100){
		$collapsible =  $collapsible == TRUE ? 'collapsible' : '';
		$status = $status == FALSE ? 'closed' : 'active open';
		$inner_status = $inner_status == FALSE ? 'closed' : 'open';
		return array(
			'class' => $class,
			'title' => $title,
			'actions' => $collapsible,
			'status' => $status,
			'weight' => $weight,
			'inner' => array(
				'class' => '',
				'markup' => '',
				'status' => $inner_status,
			),
		);
	}


	private function toolListTools($alarm_tools = NULL, $admin = FALSE){
		$tools = !empty($alarm_tools) ? $alarm_tools : $this->tools;
		foreach($tools as $key => $value){
			if(is_numeric($value) || empty($value['data'])){
				unset($tools[$key]);
			}
		}
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




	private function listAlarmUsers($data, $username){
		$user_list = array();
		$block_settings = $this->cust_block_settings;
		$user_list['users_wrapper'] = $this->alarmDataWrapper('Alarm Users', NULL, TRUE, FALSE, FALSE, 100);
		if(!empty($data['attatched_users'])){
			$attatched_users = $data['attatched_users'];

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

			$account_permissions = $this->account_permissions;
			foreach($attatched_users as $key => $value){
				$options = array();
				$rows[$key] = array(
					'data' => array(
						array(
							'data' => $value,
						),
					),
				);
				//edit
				if(!empty($account_permissions['alarm'][$username]) && array_key_exists('edit_users', $account_permissions['alarm'][$username]) || $this->access == TRUE){
					$edit = !empty($access['edit']) || in_array('edit_users', $alarm_settings) ? array('data' => l('Edit', '../../alarm-settings/'.$uid.'/cust_info/'.$value.'', $options)) : 'N/A';
				}else{
					$edit = t('N/A');
				}
				array_push($rows[$key]['data'], $edit);
				//delete
				if(!empty($account_permissions['alarm'][$username]) && array_key_exists('delete_users', $account_permissions['alarm'][$username]) || $this->access == TRUE){
					$delete = !empty($access['delete']) || in_array('delete_users', $alarm_settings) ? array('data' => l('Delete', '../../alarm-settings/'.$uid.'/cust_remove/'.$value.'/'.$data['node_id'].'', $options)) : 'N/A';
				}else{
					$delete = t('N/A');
				}
				array_push($rows[$key]['data'], $delete);
			}


			$user_list_table = array(
				'#theme' => 'table',
				'#header' => $headers,
				'#rows' => $rows,
				'#attributes' => array(
					'class' => array('alarm-user-list'),
				),
				'#empty' => t('No Users Found'),
				'#weight' => 100,
			);
			$user_list['users_wrapper']['inner']['markup'] = drupal_render($user_list_table);
			$user_list = drupal_render(fr_groups_accordion_fieldset($user_list['users_wrapper']));
		}else{
			$user_list['users_wrapper']['inner']['markup'] = 'This Alarm System has no attatched users.';
			$user_list = drupal_render(fr_groups_accordion_fieldset($user_list['users_wrapper']));
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