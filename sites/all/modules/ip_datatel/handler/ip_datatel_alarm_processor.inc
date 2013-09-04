<?php

class IpdatatelProcessEvents {

	public $queryable_accounts;
	public $all_new_events = array();
	public $custom_trigger_signals = array();
	public $default_events = array();
	public $session;
	public $new_events_overflow = array();
	public $test;
	public $new_events = array();

	public function processConstruct($processing_accounts, $account_variables, $uid, $ajax = FALSE, $session = FALSE, $session_start = NULL, $ajax_new_events = array(), $ran = 0){
		$this->account_variables = $account_variables;
		$this->queryable_accounts = $processing_accounts;
		$this->uid = $uid;
		$this->session = !empty($session) ? $session : strtotime("now");
		$this->ajax = $ajax;
		$this->sessionStartTime = !empty($session_start) ? $session_start : strtotime("now");
		$this->ajax_new_events = !empty($ajax_new_events) ? $ajax_new_events : array();
		$this->ran = $ran + 1;
		$events = $this->routEventTriggers($processing_accounts);
		$return = $this->renderAllEvents();
		$this->runAfterProcesses();
		return array('start_time' => $this->sessionStartTime, 'table' => drupal_render($return), 'new_events' => $this->new_events, 'ran' => $this->ran);
	}
	
	
	public function runAfterProcesses(){
		// logic needs to be 14
		if($this->ran >= 16){
			$this->saveSession();
		}
	}
	
	
	private function saveSession(){
		$unserialize = FALSE;
		$db = new IpdatatelCustDatabaseManager();
		$uid = $this->uid;
		$check_session = db_query('SELECT * FROM {customer_variables} WHERE uid = :uid AND delta = :delta', array(':uid' => $uid, ':delta' => $this->session));
		$record = $check_session->fetchObject();
		$events = serialize(array_keys($this->new_events));
		$delta = $this->session;
		$data = '';
		if($record){
			$db->updateCustomerVariables($uid, $data, $events, $delta);
		}else{
			$db->insertNewCustomerVariables($uid, $data, $events, $delta);
		}
	}


	private function queryEvents($account_data, $trigger){
		$events = array();
		$args = NULL;
		$account_name = $account_data['username'];
		$eventapi = new IpdatatelEventLogs();
		$structure_event_api = $eventapi->constructAccountInformation($account_data, $account_name);
		if($trigger){
			$args = array('event' => $trigger);
		}
		$events = $eventapi->structorEventRequest(array('event' => $trigger));
		if(!empty($events) && $events['status'] == TRUE){
			$events = $events['event_logs'];
		}else{
			$events = NULL;
		}
		return $events;
	}


	private function buildTriggerArgs(&$account){
		$accounts = array();
		if(!empty($account['custom_triggers'])){
			foreach($account['custom_triggers'] as $trig_name => $trig_value){
				$accounts[] = array('account' => $account['username'], 'trigger' => $trig_value->event);
			}
		}
		return $accounts;
	}

	public function routEventTriggers($account_data){
		$events = array();

		if(!empty($account_data) || isset($account_data)){
			foreach($account_data as $account_name => $account_data){
				if(!empty($account_data['custom_triggers'])){
					foreach($account_data['custom_triggers'] as $delta => $trigger){
						switch($trigger->event){
						case'100':
							$events[$trigger->account_name][] =$this->buildProcessData($account_data, $trigger->event, $trigger, $delta);
							break;
						case'200':
							$events[$trigger->account_name][] = $this->buildProcessData($account_data, $trigger->event, $trigger, $delta);
							break;
						case'300':
							$events[$trigger->account_name][] = $this->buildProcessData($account_data, $trigger->event, $trigger, $delta);
							break;
						case'400':
							$events[$trigger->account_name][] = $this->buildProcessData($account_data, $trigger->event, $trigger, $delta);
							break;
						case'500':
							$events[$trigger->account_name][] = $this->buildProcessData($account_data, $trigger->event, $trigger, $delta);
							break;
						case'600':
							$events[$trigger->account_name][] = $this->buildProcessData($account_data, $trigger->event, $trigger, $delta);
							break;
						case'all':
							$events[$trigger->account_name][] = $this->buildProcessData($account_data, NULL, $trigger, $delta);
							break;
						}
					}
				}
			}
		}
		if(!empty($events)){
			$return_events = $this->firstProcessEvents($events, $this->ajax);
		}
	}


	private function buildProcessData($account_data, $trigger_event, $trigger, $delta){
		$get_events = $this->queryEvents($account_data, $trigger_event);
		$events = is_array($get_events) ? $get_events : '';
		$data = new stdClass();
		$data->events = $events;
		$data->custom_trigger = !empty($trigger) ? $trigger : NULL;
		return $data;
	}


	private function firstProcessEvents($events, $ajax = FALSE){
		$top_3 = array();
		$new_events = array();
		$processed_events = array();
		if(!empty($events)){
			foreach($events as $account_name => $account_data){
				foreach($account_data as $delta => $values){
					if(!empty($values->events) && isset($values->events)){
						$custom_trigger = $values->custom_trigger;
						$top_3 = array_slice($values->events, 0, 5, TRUE);
						$this->custom_trigger_signals[$account_name][$custom_trigger->event] = TRUE;
						foreach($top_3 as $key => $object){
							$matches = 0;
							foreach($object as $event_name => $event_value){
								switch($event_name){
								case'partition':
									if($event_value == $custom_trigger->partition || $custom_trigger->partition == 'all'){
										$matches++;
									}
									break;
								case'signal':

									if(!empty($custom_trigger->signal)){
										if($custom_trigger->signal != 'all'){
											$explode_signal = explode(':', $custom_trigger->signal);
											if(is_array($explode_signal)){
												foreach($explode_signal as $keys => $signal){
													if(strtolower($signal) == strtolower($event_value)){
														$matches++;

													}
												}
											}
										}
										else{
											$matches++;
										}
									}
									break;
								case'extra':
									if($custom_trigger->extra == 'all' || strtolower($custom_trigger->extra) == strtolower($event_value)){
										$matches++;
									}
									break;
								case'zone':
									if($custom_trigger->zone == 'all' || $event_value == $custom_trigger->zone){
										$matches++;
									}
									break;
								}
							}
							if($matches == 4 && $ajax == TRUE){
								$account_variables = &$this->account_variables;
								$event_date = new DateTime($object->datetime);
								$formated_event_date = strtotime($event_date->format('Y-m-d G:i:s'));
								$new_events[$formated_event_date] = $this->buildEventData($account_variables, $object, $custom_trigger, $ajax, $formated_event_date);
							}
						}
					}
				}
			}
		}
		if(!empty($new_events)){
			krsort($new_events, SORT_NUMERIC);
			$this->sortNewEvents($new_events);
		}
	}
	
	

	private function sortNewEvents($new_events){
		$this->new_events = $this->ajax_new_events;
		if(!empty($new_events) && !empty($this->sessionStartTime)){
			$test = array();
			foreach($new_events as $new_event_date => $new_event){
				if($new_event_date >= $this->sessionStartTime && !array_key_exists($new_event_date, $this->new_events)){
					$this->new_events[$new_event_date] = $new_event;
			}else{
				//$this->new_events->$new_event_date = $new_event;
			}
		}
		krsort($this->new_events, SORT_NUMERIC);
	}
}




	private function buildEventData(&$account_variables, $object, $custom_trigger, $ajax = FALSE, $return_data = NULL, $date = FALSE){
		$new_events = array();
		$data = new stdClass();
		$account_name = $custom_trigger->account_name;
		$object->account_name = $account_name;
		$object->session = $this->session;
		$object->type = 'new';
		$return_data = $object;
		return $return_data;
	}


	public function renderAllEvents($ajax = FALSE){
			$content = array();
			$rows = array();
			$headers = array(
				'Account',
				'Date',
				'Signal Code',
				'Signal',
				'Zone',
				'Type',
				'View',
			);
			$content['wrapper'] = array(
				'#type' => 'container',
				'#attributes' => array(
					'class' => array(''),
				),
			);
			
			if(!empty($this->new_events)){
			//drupal_add_js(drupal_get_path('module', 'ip_datatel') .'/js/ip_datatel_processing_events.js', array('type' => 'file', 'scope' => 'header'));
			foreach($this->new_events as $new_event_date => $event_data){
				$event_data = ( object ) $event_data;
							if(!empty($event_data->datetime) && $event_data->type != 'default' && $event_data->session == $this->session){
								
								$color = 'rgb(80, 201, 80)';//$this->colorAramBySignal($event_data->signal_code);
								$rows[] = array(
									'data' => array(
										array(
											'data' => $event_data->account_name,
											'style' => array($style),
											'class' => array(''),
										),
										array(
											'data' => $event_data->datetime,
											'style' => array($style),
											'class' => array(''),
										),
										array(
											'data' => $event_data->event,
											'style' => array($style),
											'class' => array(''),
										),
										array(
											'data' => $event_data->signal,
											'style' => array($style),
											'class' => array(''),
										),
										array(
											'data' => $event_data->zone,
											'style' => array($style),
											'class' => array(''),
										),
										array(
											'data' => $event_data->extra,
											'style' => array($style),
											'class' => array(''),
										),
										array(
											'data' => '<div data-callback="/ajax/alarm-processor-modal" data-arguments="'.$new_event_date.'" data-ajax="TRUE" data-drupal-settings="TRUE" data-drupal-setting="events" data-method="POST" data-tooltip-size="f-tooltip-medium" class="alarm-event-wrapper" >View</div>',
											'style' => array('background:'.$color.';'),
											'class' => array('view-proccesed-link'),
											'data-event' => $new_event_date,
										),
									),
								);
							}
						}
					}
			$content['wrapper']['table'] = array(
				'#theme' => 'table',
				'#header' => $headers,
				'#rows' => $rows,
				'#empty' => t('No data yet...'),
				'#group' => 'wrapper',
				'#attributes' => array(
					'class' => array('large-12', 'columns'),
					'id' => 'alarm-processing-table',
				),
			);
		return $content;
	}
	
	
	private function colorAramBySignal($signal){
		//@todo find the signal code from custom_triggers maybe?
		$signal = substr($signal, 0, 1);
		$color = '';
		switch($signal){
			case 1:
				$color = 'red';
				break;
			case 2:
				$color = 'orange';
				break;
			case 3:
				$color = 'yellow';
				break;
			case 4:
				$color = 'green';
				break;
			case 5:
				$color = 'green';
				break;
			case 6:
				$color = 'green';
				break;
		}
		return $signal;
	}

}


class IpdatatelCustDatabaseManager{

	public function updateCustomerVariables($uid, $data, $events, $sessions){
		$update_cust_variables = db_update('customer_variables')
		->fields(array(
				'sessions' => $events,
			))
		->condition('uid', $uid, '=')
		->condition('delta', $sessions, '=')
		->execute();
	}
	
	public function insertNewCustomerVariables($uid, $data, $events, $sessions){
		$insert = db_insert('customer_variables')
			->fields(array(
				'uid' => $uid,
				'data' => $data,
				'sessions' => $events,
				'delta' => $sessions,
			));
			$result = $insert->execute();
			return $result;
	}
}






class IpdatatelAlarmProcessor extends IpdatatelProcessEvents{

	public $processing_accounts;
	public $account_data;

	public function __construct($uid, $args){
		$this->uid = $uid;
		$this->args = $args;
	}

	public function setProcessingAccounts($account_variables = NULL){
		$ipdatatel = new ipDatatel();
		$this->account_data = $ipdatatel->loadAccountInformation($this->uid, $this->args);
		$all_accounts = !empty($this->account_data) ? current($this->account_data) : NULL;
		if($all_accounts && !empty($account_variables['processing'])){
			$this->account_variables = $account_variables;
			$accounts = array_intersect_assoc($all_accounts, $account_variables['processing']);
			$this->processing_accounts = !empty($accounts) ? $accounts : NULL;
			if(empty($this->processing_accounts) && $this->args == 'admin'){
				$this->processing_accounts = $all_accounts;
			}
		}
	}

	public function queryAlarmEvents($processing_accounts){
		$this->loadJsSettings();
		$eventProcessor = NULL;
		$account_variables = $this->account_variables;
		if($processing_accounts){
			$eventProcessor = $this->processConstruct($processing_accounts, $account_variables, $this->uid);
			drupal_add_js(array('ip_datatel' => array('processing_accounts' => serialize($processing_accounts), 'account_variables' => $account_variables, 'uid' => $this->uid)), 'setting');
		}
		return $eventProcessor;
	}


	private function loadJsSettings(){
		drupal_add_js(drupal_get_path('module', 'ip_datatel') .'/js/ip_datatel_alarm_notifications.js', array('type' => 'file', 'scope' => 'header'));
	}

}