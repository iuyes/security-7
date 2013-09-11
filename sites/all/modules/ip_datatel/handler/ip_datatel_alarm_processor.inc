<?php

class IpdatatelProcessEvents {

	public $queryable_accounts;
	public $all_new_events = array();
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
		return array('start_time' => $this->sessionStartTime, 'content' => $return, 'new_events' => $this->new_events, 'ran' => $this->ran, 'test' => $this->test);
	}


	public function runAfterProcesses(){
		// logic needs to be 14
		if($this->ran >= 15){
			$this->saveSession();
		}
	}


	private function saveSession(){
		$unserialize = FALSE;
		$db = new IpdatatelCustDatabaseManager();
		$uid = $this->uid;
		$check_session = db_query('SELECT * FROM {customer_variables} WHERE uid = :uid AND delta = :delta', array(':uid' => $uid, ':delta' => $this->sessionStartTime));
		$check_num_session = fr_user_dash_query_processing_sessiosn($this->uid);
		$record = $check_session->fetchObject();
		if(!empty($check_num_session) && count($check_num_session) > 15){
			$tenth = array_values($check_num_session);
			$tenth_oldest = $tenth[14]->delta;
			$db->deleteCustomerSessions($uid, $tenth_oldest);
		}
		$events = serialize($this->new_events);
		$delta = $this->sessionStartTime;
		$data = '';
		if(isset($record) && !empty($record)){
			$db->updateCustomerVariables($uid, $data, $events, $delta);
		}elseif(!empty($this->new_events) && isset($this->new_events)){
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
				$this->test[$new_event_date] = array('event_date' => date('Y-m-d h:i:s', $new_event_date), 'start_date' => date('Y-m-d h:i:s', $this->sessionStartTime), 'diff' => $new_event_date - $this->sessionStartTime, 'now' => date('Y-m-d h:i:s'));
				krsort($this->test, SORT_NUMERIC);
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

		if(!empty($this->new_events)){
			$rows = self::buildEventTable($this->new_events, $this->session);
		}
		$content['table'] = array(
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

		//buttons for processing block
		$tools = '<div class="alarm-previous-sessions button star">';
		$tools .= '<a class="f-tooltip top" data-callback="/ajax/prev-sessions" data-tooltip-yposition="top" data-tooltip-xposition="center" data-event="click" data-ajax="TRUE" data-query-data="'.$this->uid.'" data-tooltip-size="f-tooltip-auto" href="#">Previous Sessions';
		$tools .= '</a></div>';
		$tools .= '<div class="alarm-activate-modal button next"><a href="#">Open In Modal</a></div>';
		$return_data = array('table' => drupal_render($content['table']), 'tools' => $tools);

		return $return_data;
	}
	
	
	
	public static function buildEventTable($new_events, $session, $ajax = TRUE){
		//drupal_add_js(drupal_get_path('module', 'ip_datatel') .'/js/ip_datatel_processing_events.js', array('type' => 'file', 'scope' => 'header'));
			$rows = array();
			$event = $ajax == FALSE ? '' : 'click' ;
			$colors = new IpdatatelSignalCodes();
			foreach($new_events as $new_event_date => $event_data){
				$event_data = ( object ) $event_data;
				if(!empty($event_data->datetime) && $event_data->session == $session && $ajax == TRUE || $ajax == FALSE){
					$style = '';
					if(empty($event_data->clicked)){
						$color = 'background:'.$colors->signalCodes($event_data->signal_code).'; font-weight:bold;';
					}else{
						$color = 'background:rgb(58, 209, 58); font-weight:bold;';
					}
					$rows[] = array(
						'data' => array(
							array(
								'data' => $event_data->account_name,
								'style' => array($color),
								'class' => array(''),
							),
							array(
								'data' => $event_data->datetime,
								'style' => array($color),
								'class' => array(''),
							),
							array(
								'data' => $event_data->event,
								'style' => array($color),
								'class' => array(''),
							),
							array(
								'data' => $event_data->signal,
								'style' => array($color),
								'class' => array(''),
							),
							array(
								'data' => $event_data->zone,
								'style' => array($color),
								'class' => array(''),
							),
							array(
								'data' => $event_data->extra,
								'style' => array($color),
								'class' => array(''),
							),
							array(
								'data' => '<div class="alarm-event-wrapper"><a data-callback="/ajax/alarm-processor-modal" data-arguments="'.$new_event_date.'" data-ajax="TRUE" data-drupal-settings="TRUE" data-drupal-setting="events" data-method="POST" data-tooltip-size="f-tooltip-medium" data-tooltip-yposition="top" data-tooltip-xposition="center" data-event="'.$event.'" class="f-tooltip view-event" href="#">View</a></div>',
								'style' => array($color),
								'class' => array('view-proccesed-link'),
							),
						),
					);
				}
			}
			return $rows;
	}

} //end Class


class IpdatatelCustDatabaseManager{

	public function updateCustomerVariables($uid, $data, $events, $delta){
		$update_cust_variables = db_update('customer_variables')
		->fields(array(
				'sessions' => $events,
			))
		->condition('uid', $uid, '=')
		->condition('delta', $delta, '=')
		->execute();
	}
	
	
	public function deleteCustomerSessions($uid, $delta){
		db_delete('customer_variables')
	  ->condition('delta', $delta, '<')
	  ->condition('delta', 0, '!=')
	  ->condition('uid', $uid)
	  ->execute();
	}

	public function insertNewCustomerVariables($uid, $data, $events, $delta){
		$insert = db_insert('customer_variables')
		->fields(array(
				'uid' => $uid,
				'data' => $data,
				'sessions' => $events,
				'delta' => $delta,
			));
		$result = $insert->execute();
		return $result;
	}
}


class IpdatatelSignalCodes{

	public function signalCodes($signal){
		$color = '';
		$signal_codes = array(
			'yellow' => array('E401', 'R401', 'E403', 'R403', 'E406', 'E570',),
			'rgb(255, 82, 0)' => array('R333', 'R356', 'R353', 'E301', 'R301', 'R131', 'E131', 'R383', 'E383'),
			'orange' => array('R570'),
		);
		foreach($signal_codes as $colors => $codes){
			if(in_array($signal, $codes)){
				$color = $colors;
			}
		}
		return $color;
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
		$this->account_variables = NULL;
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
		drupal_add_js(drupal_get_path('module', 'fr_user_dash') .'/js/fr_user_dash_custom_tooltips.js', array('type' => 'file', 'scope' => 'footer'));
	}

}