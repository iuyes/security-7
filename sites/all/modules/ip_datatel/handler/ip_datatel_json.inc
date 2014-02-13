<?php
class RequestBasicData extends IpdatatelJson {



  public function requestData($user_type, $username){
    $return = NULL;
    switch($user_type){
    case'parent':

      break;

    default:

      $username = $user_type;

      break;
    }
    $data = array('user' => $username);
    $ipDatatel_mod_cust = $this->jsonData('cust_info', $data, '', '');
    $send_json_data = $this->jsonRequest($ipDatatel_mod_cust);
    if(!empty($send_json_data->data)){
      $return =  $send_json_data;
    }
    return $return;
  }



  private function custData($utility, $user){
    $data = array('user' => $user);
    $build_request = $this->jsonData($utility, $data, '', '');
    $send_request = $this->jsonRequest($build_request);
    return $send_request;
  }

}





class IpdatatelJson{

  public function __construct(){
    $this->dealer_id = variable_get('ip_datatel_dealer_id');
    $this->dealer_pass = variable_get('ip_datatel_dealer_pass');
    if(empty($this->dealer_id) || empty($this->dealer_pass)){
      $this->dealer_settings = 'You Dont Have Dealer ID or Dealer Password';
    }
  }

  public function jsonData($utlility, $data, $id, $pass){
    /* IpDatatel API */
    if(empty($id)){
      $id = $this->dealer_id;
    }
    if(empty($pass)){
      $pass = $this->dealer_pass;
    }
    $data_array = array(
      'id' => $id,
      'pass' => $pass,
      'utility' => $utlility,
      'data' => $data,
    );
    return $data_array;
  }


  public function jsonRequest($data){

    //Pull the url out of the drupal settings
    $url = 'https://alarmdealer.com/api';
    $response = drupal_http_request($url, array(
        'method' => 'POST',
        'data' => json_encode($data),
        'max_redirects' => 0,
        'headers' => array('Content-Type' => 'application/json', 'Accept' => 'application/json'))
    );
    $data = !empty($response->data) ? $response->data : '';
    return json_decode($data);
  }

}