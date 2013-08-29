<?php
class IpdatatelEventLogs extends IpdatatelJson {

  public $event_logs = array();
  const RUN_ERROR = 'RUNTIME ERROR PLEASE CHECK ACCOUNT DATA';
  public $return_event_logs = array();
  
  public function constructAccountInformation($account_information, $username = NULL){
    $render_events = NULL;
    $status = array();
	  $desired_account_events = !empty($account_information[$username]) ? $account_information[$username] : ''; 
    if(!$desired_account_events){
	    $desired_account_events = !empty($username) && $username == $account_information['username'] ? $account_information : '';
    }
    if($desired_account_events){
	  	$this->requested_name = $username;
      $this->customer_requested_mac_addr = $desired_account_events['mac_addr'];
    }
  }
  
  public $logs = array();
  public function structorEventRequest($args = NULL){
  		$errors = !empty($this->requested_name) ? FALSE : self::RUN_ERROR;
  		if(!empty($this->requested_name)){
	      $username  = $this->requested_name;
	      $device_mac = $this->customer_requested_mac_addr;
	      $utility = 'event_log';
	      $structure_event_data = array('device' => $device_mac);
	      if(!empty($args) && is_array($args)){
	        $structure_event_data = array_merge($args, $structure_event_data);
	      }
	      $event_data_build_request = $this->jsonData($utility, $structure_event_data, '', '');
	      $event_data_send_request = $this->jsonRequest($event_data_build_request);
	      
	      if(!empty($event_data_send_request->logs)){
	        $this->logs = $event_data_send_request->logs;
	        $this->user = $event_data_send_request->name;
	        $this->amount = $event_data_send_request->count;
	        $status = TRUE;
	        return $events = $errors ? $errors : array('status' => $status, 'event_logs' => $this->logs);
	      }
	    }else{
		    return array('status' => FALSE, 'error_msg' => $errors, 'event_logs' => NULL);
	    }
  }
  
  
  
  

  public function sortLogs($logs, $account = NULL){
    $logs = !empty($logs) ? $logs : $this->logs;
    $rows = array();
    $headers = array(
      'Date',
      'Event',
      'Partition',
      //'Signal Code',
      'Signal',
      'Extra',
      'zone',
    );
    if($account){
	    array_unshift($headers, 'Account');
    }
    $i = 0;
    foreach($logs as $key => $event){
      $rows[] = array(
        'data' => array(
          array(
            'data' => $event->datetime,
          ),
          array(
            'data' => $event->event,
          ),
          array(
            'data' => $event->partition,
          ),
          /*array(
            'data' => $event->signal_code,
          ),*/
          array(
            'data' => $event->signal,
          ),
          array(
            'data' => $event->extra,
          ),
          array(
            'data' => $event->zone,
          ),
        ),
      );
			if($account){
	    	array_unshift($rows[$i]['data'], array('data' => $key));
			}
			$i++;
    }
    $event_logs['table'] = array(
      '#theme' => 'table',
      '#header' => $headers,
      '#rows' => $rows,
      '#empty' => t('No Users Found'),
      '#weight' => 101,
    );
    return $event_logs;
  }
  
  public function sortFormDashboard($logs, $args){
	  $event_logs = $this->sortLogs($logs, $args['account']);
	  return $event_logs;
  }
  
  public function sortForm($title, $args){
    $form = array();
      if($args['sort_form'] == TRUE){
      $form = drupal_get_form('ip_datatel_sort_events_form');
      $logs = $this->logs;
      $form['wrapper']['dynamic_form_items']['event_logs'] = $this->sortLogs($logs);
    }
    return $form;
  }

}


