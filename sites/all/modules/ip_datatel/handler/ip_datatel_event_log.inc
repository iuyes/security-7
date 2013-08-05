<?php
class IpdatatelEventLogs extends IpdatatelJson {

  public $event_logs = array();
  
  public function constructAccountInformation($account_information, $username){
    $render_events = NULL;
    $status = array();
    foreach($account_information as $key => $value){
      foreach($value as $delta => $account_data){
        if($account_data['username'] == $username){
          $this->requested_name = $username;
          $this->customer_requested_mac_addr = $account_data['mac_addr'];
        }
      }
    }
  }
  
  public $logs = array();
  public function structorEventRequest($args){
      $username = $this->requested_name;
      $device_mac = $this->customer_requested_mac_addr;
      $utility = 'event_log';
      $structure_event_data = array('device' => $device_mac);
      if(!empty($args) && is_array($args)){
        $structure_event_data = array_merge($args, $structure_event_data);
      }
      $event_data_build_request = $this->jsonData($utility, $structure_event_data, '', '');
      //dpm($event_data_build_request);
      $event_data_send_request = $this->jsonRequest($event_data_build_request);
      
      if(!empty($event_data_send_request->logs)){
        $this->logs = $event_data_send_request->logs;
        $this->user = $event_data_send_request->name;
        $this->amount = $event_data_send_request->count;
        $status = TRUE;
      }else{
        $status[] = 'No Events Found';
      }
      return array('status' => $status, 'event_logs' => $this->logs);
  }
  
  
  
  

  public function sortLogs($logs){
    $logs = $this->logs;
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
  
  
  
  public function sortForm($title, $args){
    
    $form = array();
    $form = drupal_get_form('ip_datatel_sort_events_form');
      if($args['sort_form'] == TRUE){
      $logs = $this->logs;
      $form['wrapper']['dynamic_form_items']['event_logs'] = $this->sortLogs($logs);
    }
    return $form;
  }

}


