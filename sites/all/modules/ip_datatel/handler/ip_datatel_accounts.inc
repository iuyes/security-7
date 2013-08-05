<?php
/*
*
*@ Load all ipdatatel alarm account
*
*/
class ipDatatel{
  
  
  
  public function loadAccountInformation($uid){
  
    global $user;
    $this->uid = $uid;
    $this->user_account = $user;
    $this->customer_account = user_load($uid);
    $this->customer_name = $this->customer_account->name;
  
  
    $customer_name = $this->customer_name;
    $account_information = array();
    $account_data = array();
    $attatched_users = array();
    $this->query = new EntityFieldQuery;
    $this->query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'alarm')
    ->fieldCondition('field_alarm_customer', 'target_id', $uid, '=');
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
        $account_information[$customer_name][$results->field_alarm_ipdatatel_username['und'][0]['value']] = array(
          'account_type' => !empty($results->field_alarm_account_type['und'][0]['value']) ? $results->field_alarm_account_type['und'][0]['value'] : NULL,
          'username' => !empty($results->field_alarm_ipdatatel_username['und'][0]['value']) ? $results->field_alarm_ipdatatel_username['und'][0]['value'] : NULL,
          'pass' => !empty($results->field_alarm_ipdatatel_password['und'][0]['value']) ? $results->field_alarm_ipdatatel_password['und'][0]['value'] : NULL,
          'mac_addr' => !empty($results->field_alarm_device_id['und'][0]['value']) ? $results->field_alarm_device_id['und'][0]['value'] : NULL,
          'attatched_users' => !empty($attatched_users) ? $attatched_users : NULL,
          'account_id' => !empty($account_id) ? $account_id : $results->field_alarm_account_id['und'][0]['value'],
          'node_id' => $node->nid,
        );
      }
    }
    return $account_information;
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