<?php
class ipdatatelUtility {
  	
  public $account_information;
  public function __construct($account_info, $uid, $nid = NULL){
    if(!empty($nid)){
      $this->nid = $nid;
    }
    $this->account_information = $account_info;
    $this->uid = $uid;
    $this->profile = profile2_load_by_user($uid, $type_name = NULL);
    $this->ipdatateljson = new IpdatatelJson();
  }



  public function requestedUtility($utility, $arg){
    switch($utility){
    case'cust_info':
      //$get_parent_id = $this->getParentId();
      $data = $this->custInfo($utility, $arg);
      break;

    case'all':
    	$alarmProfiles = new IpdatatelAlarmProfile($this->account_information, $this->uid);
      $alarmProfiles->sortAlarmAccounts();
      $data = $alarmProfiles->renderData();
      break;

    case'modify_user':

      $data = $this->modifyUser($utility, $arg);

      break;

    case'cust_remove':

      $data = $this->deleteUser($utility, $arg);

      break;
    case'event_log':
      $data = $this->loadEvents($utility, $arg);

      break;
    }
    return $data;
  }
 



  private function loadEvents($utility, $arg){
    $event_logs = NULL;
    $accounts = current($this->account_information);
    if(array_key_exists($arg, $accounts)){
	    $EventLogs = new IpdatatelEventLogs();
	    $EventLogs->constructAccountInformation($accounts[$arg], $arg);
	    $arguments = array();
	
	    $get_logs = $EventLogs->structorEventRequest(NULL);
	    if($get_logs['status'] == TRUE){
	      $event_logs = $get_logs['event_logs'];
	    }
	    $arguments['sort_form'] = TRUE;
	    $return = $EventLogs->sortForm($arg, $arguments);
	    //$return_logs = $EventLogs->sortLogs($event_logs);
	    //$return = array_merge($form, $return_logs);
	   }
    return $return;
  }



  private function deleteUser($utility, $arg){
      $status = default_form();
      $status['wrapper']['message'] = status_message('Sorry, Something went wrong. Please contact our customer support.');
      $user = $arg;
      $data = array('user' => $user);
      $build_request = $this->ipdatateljson->jsonData($utility, $data, '', '');
      $send_request = $this->ipdatateljson->jsonRequest($build_request);
      $message = $send_request->message;
      if($send_request->status == 0){
        $status['wrapper']['message'] = status_message($send_request->message);
        $nid = $this->nid;
        $node = node_load($nid);
          if(!empty($node->field_alarm_atatched_users['und'])){
            foreach($node->field_alarm_atatched_users['und'] as $key => $value){
              if($value['value'] == $user){
                unset($node->field_alarm_atatched_users['und'][$key]);
              } 
            }
            node_save($node);
          }
        }
    return $status;
  }

  private function modifyUser($utility, $arg){
    $data = array('user' => $arg);
    $build_request = $this->ipdatateljson->jsonData($utility, $data, '', '');
    $send_request = $this->ipdatateljson->jsonRequest($build_request);
    return $send_request;
  }



  private function custInfo($utility, $arg){
    $username = $arg;
    $data = array('user' => $username);
    $build_request = $this->ipdatateljson->jsonData($utility, $data, '', '');
    $send_request = $this->ipdatateljson->jsonRequest($build_request);
    if(!empty($send_request->data)){
      $this->returned_data = $send_request->data;
      $cust_info = $this->storeData();
    }else{
      $cust_info = status_message($send_request->message);
    }
    return $cust_info;
  }

  private function storeData(){
    $content = array();
    $data = $this->returned_data;
    variable_set('alarm_customer_data', $data);
    $form = drupal_get_form('ip_datatel_mod_user_form');
    return $form;
  }

}