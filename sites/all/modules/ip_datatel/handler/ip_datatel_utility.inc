<?php
class ipdatatelUtility {

	private $drupal_permissions = array();
	private $bat_settings = array();
	private $alarm_settings = array();
	private $alarm_tools_link_options = array(
  	'attributes' => array(
  	  'class' => array('button', 'radius', 'small'),
  	),
  );
  public $add_users_permissions = array();
  	
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
  	global $user;
    $format_account_data = array();
    $account_users = array();
    $users = array();
    $tools = array();
    $admin = FALSE;
    $tool_item = array();
    $account_info = $this->account_information;
    $account_wrapper = array();
    $master_settings = $this->buildMasterSettings($account_info);
    /*if(in_array('add_users', $alarm_settings) || in_array('TRUE', $alarm_settings)){
      $this->addToRender($this->alarmTools($this->cust_block_settings, 'admin', $this->uid, '', 'add_users'));
    }*/
    $roles = $user->roles;
    $admins = array('administrator', 'supervisor');
    drupal_add_js(drupal_get_path('module', 'ip_datatel') .'/js/ip_datatel_login.js', array('type' => 'file', 'scope' => 'header'));
    $tools['wrapper'] = $this->alarmTitle('Alarm Tools', -100, FALSE);
    $tool_item['user_login'] = $this->toolsLogin();
		
		
    drupal_add_js('misc/form.js');
		drupal_add_js('misc/collapse.js');   
    //$block_settings = $this->alarmPermissionsRout($this->cust_block_settings);   
    foreach($account_info as $key => $value){
      foreach($value as $account_name => $account_data){
      	 $bat_settings = $this->batSettings($account_data);
      	 //for admins
      	 if(array_intersect($admins, $roles)){
	      	 $bat_settings = 'admin';
	      	 $admin = TRUE;
      	 }
      	 $username = $account_data['username'];
      	 
				 $alarm_settings = $this->alarmPermissionsRout(NULL, $bat_settings, $username, $this->uid, $account_data['pass'], $account_data['node_id']);
        switch($account_data['account_type']){
        case 1:
        case 2:
        case 4:
          $account_wrapper['wrapper'] = $this->alarmTitle($username);
          $account_wrapper['wrapper']['table'] = $this->listUsers($account_data);
					$account_wrapper['wrapper']['menu'] = $this->toolListTools();
					if($admin == TRUE){
						$account_wrapper['wrapper']['zones'] = $this->alarmZonesMarkup('', 'aliase-replace', '');
					}
          break;
        case 3:
          if(!empty($users)){
            unset($account_data['attatched_users']);
            $account_data['attatched_users'] = $users;
          }
          $account_wrapper['wrapper'] = $this->alarmTitle($username);
          $account_wrapper['wrapper']['table'] = $this->listUsers($account_data);
					$account_wrapper['wrapper']['menu'] = $this->toolListTools();
          break;
        }
        $this->addToRender($account_wrapper);
      }
    }
    if(!empty($this->add_users_permissions)){
	    $tool_item['add_users'] = $this->toolsAddUsers();
    }
		$tools['wrapper']['menu'] = $this->toolListTools($tool_item, TRUE);
		array_unshift($this->for_render, $tools);
    //$alarm_tools = build_alarm_block($this->cust_block_settings, $username, $account_name, $this->uid);
  }
  
  public $tools = array();
  
  
  private function alarmPermissionsRout($block_settings = NULL, $bat_settings = NULL, $username = NULL, $uid = NULL, $pass = NULL, $nid = NULL){
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
				  	break;
				  case 'events':
				  	$this->tools['events'] = $this->toolsViewEvents($uid, $username);
				  	break;
			  }
		  }
	 }else{
		 $this->tools['add_users'] = $this->allowAddUsersPermissions($username);
		 $this->tools['events'] = $this->toolsViewEvents($uid, $username);
		 $this->tools['generate_zones'] = $this->generateZones($username, $pass, $nid);
	 }
	  return $this->tools;
  }
 
 
  private function allowAddUsersPermissions($username){
	  $this->add_users_permissions[$username] = TRUE;
  }
  
  
  private function toolsAddUsers(){
		$path = ''.$_SERVER['REQUEST_URI'].'#';
    $tools = array(
    	'data' => l('Add User', $path, $this->alarm_tools_link_options),
    	'id' => 'add-user',
    	'data-reveal-id' => 'add_user',
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
  	  'data' => l('View Events', $path, $this->alarm_tools_link_options),
  	);
  	return $tools;
  }
  
  
  private function toolsLogin(){
		$tools = array(
  	  'data' => l('Alarm Login', 'http://'.$_SERVER['HTTP_HOST'].''.$_SERVER['REQUEST_URI'].'#', $this->alarm_tools_link_options),
  	  'class' => array('alarm_login'),
  	);
  	return $tools;
  }
  
  
  private function toolListTools($alarm_tools = NULL, $admin = FALSE){
  	$tools = !empty($alarm_tools) ? $alarm_tools : $this->tools;
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
	  	'#prefix' => '<div id="save-zones" class="button radius small">',
	  	'#suffix' => '</div>',
	  	'#weight' => 100,
  	);
  	
		return $content;
  }
  
  
  private function AlarmTitle($username, $weight = NULL, $collapsed = TRUE){
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
	      return FALSE;
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