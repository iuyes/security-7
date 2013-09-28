<?php
/*
*
*@ Load all ipdatatel alarm account
*
*/
class ipDatatel{

	private $authenticate = array('administrator', 'supervisor', 'guard');
	public $account_type = NULL;
	public $admin = FALSE;
	public $customer_account;
	public $customer_name;
	public $user_account;
	public $user_profile;
	public $query_perams = array();
	private $entity_type;

	public function loadAccountInformation($uid = NULL, $arg = NULL, $user = NULL){

		if(empty($user)){
			global $user;
		}
		$this->uid = $user->uid;
		$this->user_account = $user;
		$this->customer_account = user_load($this->uid);
		$this->customer_name = $this->customer_account->name;
		$test = $this->authenticateUser();
		$customer_name = $this->customer_name;
		$account_information = array();
		$account_data = array();
		$attatched_users = array();
		$permissions = array();
		$admin_permissions = array();
		$custom_triggers = array();
		//set up object query
		$this->buildQueryPerams();
		if(!empty($this->entity_type)){
			$result = $this->query->execute();
			if(!empty($result[$this->entity_type])){
				foreach($result[$this->entity_type] as $key => $node){
					$results = node_load($node->nid);
					if(!empty($results->field_alarm_atatched_users['und'])){
						foreach($results->field_alarm_atatched_users['und'] as $delta => $username){
							$attatched_users[] = $results->field_alarm_atatched_users['und'][$delta]['value'];
						}
					}
					//attatched_users
					if(empty($results->field_alarm_account_id['und'][0]['value'])){
						$request_data = new RequestBasicData();
						$response_data = $request_data->requestData('parent', $results->field_alarm_ipdatatel_username['und'][0]['value']);
						$account_id = $response_data->data->user_id;
						if(!empty($response_data->data->user_id)){
							$results->field_alarm_account_id['und'][0]['value'] = $response_data->data->user_id;
							node_save($results);
						}
					}
					//permissions
					if(!empty($results->field_alarm_management['und'])){
						foreach($results->field_alarm_management['und'] as $delta => $permission){
							$permissions[$permission['value']] = $permission['value'];
						}
					}
					//permissions
					if(!empty($results->field_alarm_admin_managments['und'])){
						foreach($results->field_alarm_admin_managments['und'] as $delta => $admin_permission){
							$admin_permissions[$admin_permission['value']] = $admin_permission['value'];
						}
					}
					//custom triggers
					if(!empty($results->field_alarm_custom_bat_triggers['und'])){
						$custom_triggers = $this->buildCustomerTriggers($results->field_alarm_custom_bat_triggers['und'], $results->field_alarm_ipdatatel_username['und'][0]['value']);
					}
					//alarm zones
					if(!empty($results->field_alarm_zone_alias['und'])){
						$zone_aliases = $this->buildAlarmZoneAliases($results->field_alarm_zone_alias['und'], 'alarm');
					}
					//user aliases
					if(!empty($results->field_alarm_user_aliases['und'])){
						$user_aliases = $this->buildAlarmZoneAliases($results->field_alarm_user_aliases['und'], 'user');
					}
					$account_information[$customer_name][$results->field_alarm_ipdatatel_username['und'][0]['value']] = array(
						'account_type' => !empty($results->field_alarm_account_type['und'][0]['value']) ? $results->field_alarm_account_type['und'][0]['value'] : NULL,
						'username' => !empty($results->field_alarm_ipdatatel_username['und'][0]['value']) ? $results->field_alarm_ipdatatel_username['und'][0]['value'] : NULL,
						'pass' => !empty($results->field_alarm_ipdatatel_password['und'][0]['value']) ? $results->field_alarm_ipdatatel_password['und'][0]['value'] : NULL,
						'mac_addr' => !empty($results->field_alarm_device_id['und'][0]['value']) ? $results->field_alarm_device_id['und'][0]['value'] : NULL,
						'attatched_users' => !empty($attatched_users) ? $attatched_users : NULL,
						'account_id' => !empty($account_id) ? $account_id : $results->field_alarm_account_id['und'][0]['value'],
						'node_id' => $node->nid,
						'permissions' => !empty($permissions) ? $permissions : NULL,
						'admin_permissions' => !empty($admin_permissions) ? $admin_permissions : NULL,
						'custom_triggers' => !empty($custom_triggers) ? $custom_triggers : NULL,
						'zone_aliases' => !empty($zone_aliases) ? $zone_aliases : NULL,
						'user_aliases' => !empty($user_aliases) ? $user_aliases : NULL,
					);
				}
			}
		}
		return $account_information;
	}


	/**
	 * Build the alarm zone aliases
	 *
	 */
	private function authenticateUser(){
		$user = $this->customer_account;
		$authenticate = $this->roleType($user);
		if(!empty($authenticate)){
			$this->account_type = !empty($authenticate['role']) ? $authenticate['role'] : '';
		}
	}



	/**
	 * Build the alarm zone aliases
	 *
	 */
	public function buildAlarmZoneAliases($aliases, $type){
		$zones = array();
		foreach($aliases as $delta => $zone){
			$format_zones = explode(':', $zone['value']);
			if(count($format_zones) == 4 && $type == 'alarm'){
				$zones[$format_zones[1]] = array(
					'zone_name' => $format_zones[0],
					'dvr_nid' => $format_zones[3],
					'dvr_zone' => $format_zones[2],
				);
			}elseif(count($format_zones) == 2 && $type == 'user'){
				$zones[$format_zones[1]] = array(
					'user_aliase' => $format_zones[0],
				);
			}
		}
		return $zones;
	}



	/**
	 * Get all the dvrs attatched to Customer Group
	 *
	 */
	public function roleType($user){
		$role = '';
		$roles = array(3 => 'administrator', 4 => 'Customer', 9 => 'Account Manager', 11 => 'central station');
		$authenticate = array_intersect_assoc($roles, $user->roles);
		if(!empty($authenticate) && is_array($authenticate)){
			foreach($authenticate as $int => $role_name){
				switch($int){
				case 3:
					$this->loadAccountId('administrator');
					return array('role' => 'administrator', 'number' => 3);
					break;
				case 4:
					$this->loadAccountId('customer');
					return array('role' => 'customer', 'number' => 4);
					break;
				case 9:
					$this->loadAccountId('account_manager');
					return array('role' => 'account_manager', 'number' => 9);
					break;
				case 11:
					$this->loadAccountId('central_station');
					return array('role' => 'central_station', 'number' => 11);
					break;
				default:
					return array('role' => 'annon', 'number' => 0);
					break;
				}
			}
		}
	}



	/**
	 * Helper Functio nto get the account manaagers account id
	 *
	 */
	private function loadAccountId($type){
		$this->query_perams = new stdClass();
		$this->query_perams->db = array();
		switch($type){
		case 'customer':
		case 'account_manager':
			$this->user_profile = profile2_load_by_user($this->uid, $type_name = NULL);
			if(!empty($this->user_profile[$type]->field_cust_alarm_group_owner['und'][0]['target_id'])){
				$this->account_id = $this->user_profile[$type]->field_cust_alarm_group_owner['und'][0]['target_id'];
				$this->account_group = node_load($this->user_profile[$type]->field_cust_alarm_group_owner['und'][0]['target_id']);
				$this->query_perams->entity_type = 'node';
				$this->query_perams->bundle = 'alarm';
				$this->buildQueryParams($this->query_perams, 'field_cust_alarm_group_owner', 'target_id', $this->account_id, '=');
				$this->buildQueryParams($this->query_perams, 'field_alarm_management', 'value', 'process_alarms', '=');
			}
			break;
		case 'administrator':
			$this->account_id = 'admin';
			$this->user_profile = 'admin';
			$this->query_perams->entity_type = 'node';
			$this->query_perams->bundle = 'alarm';
			$this->buildQueryParams($this->query_perams, 'field_alarm_management', 'value', 'process_alarms', '=');
			break;
		case 'central_station':
			$this->user_profile = profile2_load_by_user($user->uid, $type_name = NULL);
			$this->account_id = $this->user_profile->pid;
			$this->query_perams->entity_type = 'profile2';
			$this->query_perams->bundle = FALSE;
			$this->buildQueryParams($this->query_perams, 'field_cust_central_station', 'target_id', $this->account_id, '=');

			break;
			default:
				return FALSE;
				break;
		}

	}

	private function buildQueryParams(&$query_perams, $table, $db_field, $value_argument, $operator){
		$query_perams->db[$table] = new stdClass();
		$query_perams->db[$table]->db_field = $db_field;
		$query_perams->db[$table]->value_argument = $value_argument;
		$query_perams->db[$table]->operator = $operator;
	}


	private function buildCustomerTriggers($triggers, $account_name){
		$custom_triggers = array();
		foreach($triggers as $delta => $trigger){
			$custom_triggers[$delta] = new stdClass();
			foreach($trigger as $trigger_name => $trigger_value){
				$custom_triggers[$delta]->account_name = $account_name;
				switch($trigger_name){
				case'event_type':
					$custom_triggers[$delta]->event = $trigger_value;
					break;
				case'partition':
					$custom_triggers[$delta]->partition = $trigger_value;
					break;
				case'signal':
					$custom_triggers[$delta]->signal = $trigger_value;
					break;
				case'extra':
					$custom_triggers[$delta]->extra = $trigger_value;
					break;
				case'zone':
					$custom_triggers[$delta]->zone = $trigger_value;
					break;
				}
			}
		}
		return $custom_triggers;
	}


	private function buildQueryPerams(){
		$account_id = !empty($this->account_id) ? $this->account_id : NULL ;
		$this->query = new EntityFieldQuery();
		$query_perams = !empty($this->query_perams) ? $this->query_perams : '';
		$this->entity_type = !empty($query_perams->entity_type) ? $query_perams->entity_type : '';
		if(!empty($query_perams->db)){
			$this->query->entityCondition('entity_type', $query_perams->entity_type);
			if($query_perams->bundle){
				$this->query->entityCondition('bundle', $query_perams->bundle);
			}
			foreach($query_perams->db as $db_field_name => $value){
				if(is_object($value)){
					$this->query->fieldCondition($db_field_name, $value->db_field, $value->value_argument, $value->operator);
				}
			}
		}
	}



	public function explodeUsers($users){
		$format_users = array();
		$users = explode(',', $users);
		foreach($users as $delta => $username){
			$format_users[$delta] = array('value' => $username);
		}
		return $format_users;
	}

}




interface Accounts{

	function buildAccounts($accounts, $user);

	function addAccount($account_name, $account_info);

	function addAccountUsername($account_name, $name);

	function addAccountPass($account_name, $pass);

	function addAccountMac($account_name, $mac);

	function addAccountAttatchedUsers($account_name, $users);

	function roleType($user);
}




class IpdatatelAccounts implements Accounts{

	public function buildAccounts($accounts, $user){
		$this->account_type = $this->roleType($user);
		$account_information = array();
		$usernames = array();
		$passwords = array();
		$mac_addrs = array();
		$account_users = array();
		$account_names = array();
		$account_id = array();
		$node_nid = array();
		foreach($accounts as $account_name => $value){
			foreach($value as $alarm_name => $data){
				$account_names[$account_name][$alarm_name] = $account_name;
				$account_information[$account_name][$alarm_name] = $data;
				$usernames[$account_name][$alarm_name] = $data['username'];
				$passwords[$account_name][$alarm_name] = $data['pass'];
				$mac_addrs[$account_name][$alarm_name] = $data['mac_addr'];
				$account_users[$account_name][$alarm_name] = $data['attatched_users'];
				$account_id[$account_name][$alarm_name] = $data['account_id'];
				$node_nid[$account_name][$alarm_name] = $data['node_id'];
			}
		}
		return array(
			'account_owner_type' => $this->account_type,
			'account_names' => $account_names,
			'account_information' => $account_information,
			'usernames' => $usernames,
			'passwords' => $passwords,
			'mac_addrs' => $mac_addrs,
			'account_users' => $account_users,
			'account_id' => $account_id,
			'node_id' => $node_nid,
		);
	}



	/**
	 * Get all the dvrs attatched to Customer Group
	 *
	 */
	public function roleType($user){
		$role = '';
		$roles = array(3 => 'administrator', 4 => 'Customer', 9 => 'Account Manager', 11 => 'central station');
		$authenticate = array_intersect_assoc($roles, $user->roles);
		if(!empty($authenticate) && is_array($authenticate)){
			foreach($authenticate as $int => $role_name){
				switch($int){
				case 3:
					return array('role' => 'administrator', 'number' => 3);
					break;
				case 4:
					return array('role' => 'customer', 'number' => 4);
					break;
				case 9:
					return array('role' => 'account_manager', 'number' => 9);
					break;
				case 11:
					return array('role' => 'central_station', 'number' => 11);
					break;
				default:
					return array('role' => 'annon', 'number' => 0);
					break;
				}
			}
		}
	}

	public $customer_accounts = array();
	public function addAccount($account_name, $account_info){
		$this->customer_accounts = $account_info;
		return $this->customer_accounts;
	}

	public $account_username = array();
	public function addAccountUsername($account_name, $name){
		$this->account_username[$account_name] = $name;
		return $this->account_username;
	}

	public $account_pass = array();
	public function addAccountPass($account_name, $pass){
		$this->account_pass[$account_name] = $pass;
		return $this->account_pass;
	}

	public $account_mac = array();
	public function addAccountMac($account_name, $mac){
		$this->account_mac[$account_name] = $mac;
		return $this->account_mac;
	}
	public $account_users = array();
	public function addAccountAttatchedUsers($account_name, $users){
		$this->account_users[$account_name] = $users;
		return $this->account_users;
	}
}