<?php
class ipdatatelUtility {

	private $drupal_permissions = array();
	private $bat_settings = array();
	private $alarm_settings = array();

  public $account_information;
  public function __construct($account_info, $uid, $nid = NULL){
    if(!empty($nid)){
      $this->nid = $nid;
    }
    $this->account_information = $account_info;
    $this->uid = $uid;
    $this->profile = profile2_load_by_user($uid, $type_name = NULL);
    $this->cust_block_settings = variable_get('ip_datatel_block_settings');
    $this->admin_page_settings = variable_get('ip_datatel_page_settings');
    $this->ipdatateljson = new IpdatatelJson();
  }



  public $for_render = array();
  public function addToRender($data){
    $this->for_render[] = $data;
  }



  public function sortAccounts(){
    $format_account_data = array();
    $account_users = array();
    $users = array();
    $alarm_settings = $this->alarmSettings();
    $account_info = $this->account_information;
    $account_wrapper = array();
    $master_settings = $this->buildMasterSettings();
    if(in_array('add_users', $alarm_settings) || in_array('TRUE', $alarm_settings)){
      $this->addToRender($this->alarmTools($this->cust_block_settings, 'admin', $this->uid, '', 'add_users'));
    }/*else{
	    drupal_add_js(drupal_get_path('module', 'ip_datatel') .'/js/ip_datatel_login.js', array('type' => 'file', 'scope' => 'header'));
			$this->addToRender($this->alarmTools($this->cust_block_settings, 'default', $this->uid, '', 'login'));
    }*/
    
    drupal_add_js('misc/form.js');
		drupal_add_js('misc/collapse.js');
    
    foreach($account_info as $key => $value){
      foreach($value as $account_name => $account_data){
      	$bat_settings = $this->batSettings($account_data);
        switch($account_data['account_type']){
        case 1:
          $username = $account_data['username'];
          $account_wrapper['wrapper'] = $this->alarmTitle($username);
          $account_wrapper['wrapper']['table'] = $this->listUsers($account_data);
          if(in_array('view_events', $alarm_settings) || in_array('TRUE', $alarm_settings) || in_array('events', $bat_settings)){
            $account_wrapper['wrapper']['menu'] = $this->alarmTools($this->cust_block_settings, $bat_settings, $this->uid, $username, 'events');
          }
          $this->addToRender($account_wrapper);
          break;
        case 2:
          $username = $account_data['username'];
          $account_wrapper['wrapper'] = $this->alarmTitle($username);
          $account_wrapper['wrapper']['table'] = $this->listUsers($account_data);
          $permissions = $account_data['permissions'];
          if(in_array('view_events', $alarm_settings) || in_array('TRUE', $alarm_settings) || in_array('events', $bat_settings)){
          
            $account_wrapper['wrapper']['menu'] = $this->alarmTools($this->cust_block_settings, $bat_settings, $this->uid, $username, 'events');
          }
          $this->addToRender($account_wrapper);
          break;
        case 3:
          if(!empty($users)){
            unset($account_data['attatched_users']);
            $account_data['attatched_users'] = $users;
          }
          $username = $account_data['username'];
          $account_wrapper['wrapper'] = $this->alarmTitle($username);
          $account_wrapper['wrapper']['table'] = $this->listUsers($account_data);
          if(in_array('view_events', $alarm_settings) || in_array('TRUE', $alarm_settings) || in_array('events', $bat_settings)){
            $account_wrapper['wrapper']['menu'] = $this->alarmTools($this->cust_block_settings, $bat_settings, $this->uid, $username, 'events');
          }
          $this->addToRender($account_wrapper);

          break;
        case 4:
          $username = $account_data['username'];
         $account_wrapper['wrapper'] = $this->alarmTitle($username);
          $account_wrapper['wrapper']['table'] = $this->listUsers($account_data);
          if(in_array('view_events', $alarm_settings) || in_array('TRUE', $alarm_settings) || in_array('events', $bat_settings)){
            $account_wrapper['wrapper']['menu'] = $this->alarmTools($this->cust_block_settings, $bat_settings, $this->uid, $username, 'events');
          }
          $this->addToRender($account_wrapper);

          break;
        }
      }
    }

    //$alarm_tools = build_alarm_block($this->cust_block_settings, $username, $account_name, $this->uid);
  }
  
  
  private function AlarmTitle($username){
  
    return array(
      '#type' => 'fieldset',
      '#title' => t($username),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#attributes' => array(
        'class' => array('user-alarms-fieldset', 'large-12', 'columns', 'collapsible', 'collapsed'),
      ),
    );
  }




  public function alarmTools($settings, $bat_settings, $uid, $username, $arg){
    $account_wrapper = array();
    $username = empty($username) ? 'Tools' : $username;
    $tools = array();
    $options = array(
            'attributes' => array(
              'class' => array('button', 'radius', 'small'),
            ),
          );
    foreach($settings as $key => $value){
      switch($value){
      case'add_users':
        if($arg === 'add_users' || !empty($bat_settings['add_users'])){
            $path = ''.$_SERVER['REQUEST_URI'].'#';
            $tools['add_user'] = array(
              'data' => l('Add User', $path, $options),
              'id' => 'add-user',
              'data-reveal-id' => 'add_user',
            );
        }
        break;
      case'events':
        if($arg == 'events'){
          $path = '../../alarm-settings/'.$uid.'/event_log/'.$username.'';
          $tools['events'] = array(
            'data' => l('View Events', $path, $options),
          );
        }
        break;
       case'zone_aliases':
       	break;
      }
    }
    if($arg === 'login'){
        $tools['alarm_login'] = array(
              'data' => l('Alarm Login', 'http://'.$_SERVER['HTTP_HOST'].''.$_SERVER['REQUEST_URI'].'#', $options),
              'id' => 'alarm_login',
            );
      }
    //dpm($tools);
    return array(
      '#theme' => 'item_list',
      '#items' => $tools,
      '#type' => 'ul',
      '#attributes' => array('class' => array('alarm-menu-wrapper')),
      '#container_id' => '',
      '#prefix' => '<section>',
      '#suffix' => '</section>',
      '#weight' => -100,
    );
  }




  public function renderData(){
    $renderData = $this->for_render;
    return $renderData;
  }


  public function requestedUtility($utility, $arg){
    switch($utility){
    case'cust_info':
      //$get_parent_id = $this->getParentId();
      $data = $this->custInfo($utility, $arg);

      break;

    case'all':

      $this->sortAccounts();
      $data = $this->renderData();
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
  
  
  
  
  public function alarmSettings(){
    global $user;
    $alarm_settings = array();
      if(in_array('administrator', $user->roles) || in_array('supervisor', $user->roles)){
        return array('TRUE');
      }else{
        $get_alarm_settings = $this->profile['customer']->field_cust_alarm_settings;
        if(!empty($get_alarm_settings['und'])){
          foreach($get_alarm_settings['und'] as $key => $value){
            $alarm_settings[$key] = $value['value'];
          }
        }
        $this->alarm_settings = $alarm_settings;
        return $alarm_settings; 
    }
  }
  
  public function batSettings($account_data){
	  if(!empty($account_data['permissions'])){
	 		return $account_data['permissions'];
	  }else{
		  return array('');
	  }
  }
  
  
  
  private function buildMasterSettings(){
  	$all_settings = array();
  	$all_settings['account_alarm_settings'] = !empty($this->alarm_settings) ? $this->alarm_settings : 
  	$all_settings['drupal_permissions'] = $this->drupal_permissions;
		foreach($this->account_information as $key => $value){
			foreach($value as $name => $permissions){
				$all_settings['permissions'][$name] = $permissions['permissions'];
			}
		}
		return $all_settings;
  }




  private function listUsers($data){
  
    $user_list = array();
    $block_settings = $this->cust_block_settings;
    if(!empty($data['attatched_users'])){
      $attatched_users = $data['attatched_users'];

      $user_list['users_wrapper'] = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('user-alarm-wrapper', 'large-12', 'columns'),
        ),
      );


      $uid = $this->uid;
      $options = array();
      $alarm_settings = $this->alarmSettings(); 
      $access = $this->checkAccess($alarm_settings);
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



  private function loadEvents($utility, $arg){
    $event_logs = NULL;
    $EventLogs = new IpdatatelEventLogs();
    $EventLogs->constructAccountInformation($this->account_information, $arg);
    $arguments = array();

    $get_logs = $EventLogs->structorEventRequest(NULL);
    if($get_logs['status'] == TRUE){
      $event_logs = $get_logs['event_logs'];
    }
    $arguments['sort_form'] = TRUE;
    $return = $EventLogs->sortForm($arg, $arguments);
    //$return_logs = $EventLogs->sortLogs($event_logs);
    //$return = array_merge($form, $return_logs);
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