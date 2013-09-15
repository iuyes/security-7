<?php
/*
*
*@ Load all ipdatatel alarm account
*
*/
class ipDatatel{
  
  private $authenticate = array('administrator', 'supervisor', 'guard');
  
  public function loadAccountInformation($uid, $arg = NULL){
  
    global $user;
    $this->uid = $uid;
    $this->user_account = $user;
    $this->customer_account = user_load($uid);
    $this->customer_name = $this->customer_account->name;
    
    $this->user_profile = profile2_load_by_user($user->uid, $type_name = NULL);
		if(!empty($this->user_profile['customer']->field_cust_alarm_group_owner['und'][0]['target_id'])){
			$this->account_id = $this->user_profile['customer']->field_cust_alarm_group_owner['und'][0]['target_id'];
			$this->account_group = node_load($this->user_profile['customer']->field_cust_alarm_group_owner['und'][0]['target_id']);
		}
  
    $customer_name = $this->customer_name;
    $account_information = array();
    $account_data = array();
    $attatched_users = array();
    $permissions = array();
    $admin_permissions = array();
    $custom_triggers = array();
    
    $query = $this->buildQueryPerams($arg);
    $result = $this->query->execute();
    
    if(!empty($result['node'])){
      foreach($result['node'] as $key => $node){
        $results = node_load($node->nid);
        if(!empty($results->field_alarm_atatched_users['und'])){
          foreach($results->field_alarm_atatched_users['und'] as $delta => $username){
            $attatched_users[] = $results->field_alarm_atatched_users['und'][$delta]['value'];
          }
        }
        if(empty($results->field_alarm_account_id['und'][0]['value'])){
          $request_data = new RequestBasicData();
          $response_data = $request_data->requestData('parent', $results->field_alarm_ipdatatel_username['und'][0]['value']);
          $account_id = $response_data->data->user_id;
          if(!empty($response_data->data->user_id)){
            $results->field_alarm_account_id['und'][0]['value'] = $response_data->data->user_id;
            node_save($results);
          }
        }
        if(!empty($results->field_alarm_management['und'])){
	        foreach($results->field_alarm_management['und'] as $delta => $permission){
		       $permissions[$permission['value']] = $permission['value'];
	        }
        }
        if(!empty($results->field_alarm_admin_managments['und'])){
	        foreach($results->field_alarm_admin_managments['und'] as $delta => $admin_permission){
		       $admin_permissions[$admin_permission['value']] = $admin_permission['value'];
	        }
        }
        if(!empty($results->field_alarm_custom_bat_triggers['und'])){
	        $custom_triggers = $this->buildCustomerTriggers($results->field_alarm_custom_bat_triggers['und'], $results->field_alarm_ipdatatel_username['und'][0]['value']);
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
        );
      }
    }
    return $account_information;
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
  
  
  private function buildQueryPerams($arg){
  		$account_id = !empty($this->account_id) ? $this->account_id : NULL ;
	  	$uid = $this->uid;
	  	$auth = $this->authenticate;
	  	$this->query = new EntityFieldQuery;
	    $this->query->entityCondition('entity_type', 'node')
	    ->entityCondition('bundle', 'alarm');
	    switch($arg){
		    case'admin':
		    	if(array_intersect($auth, $this->user_account->roles) && $arg == 'admin'){
						$this->query->fieldCondition('field_alarm_admin_managments', 'value', 'process_alarms', '=');
					}	
		    	break;
		    case'user':
		    	$this->query->fieldCondition('field_cust_alarm_group_owner', 'target_id', $account_id, '=');
		    	$this->query->fieldCondition('field_alarm_management', 'value', 'process_alarms', '=');
		    	break;
		    default:
		    	$this->query->fieldCondition('field_cust_alarm_group_owner', 'target_id', $account_id, '=');
		    	break;
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

  function buildAccounts($accounts);

  function addAccount($account_name, $account_info);

  function addAccountUsername($account_name, $name);

  function addAccountPass($account_name, $pass);

  function addAccountMac($account_name, $mac);

  function addAccountAttatchedUsers($account_name, $users);

}




class IpdatatelAccounts implements Accounts{

  public function buildAccounts($accounts){
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