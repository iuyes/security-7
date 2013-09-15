<?php



/**
 * Implements hook_views_query_alter().
 */
function fr_groups_views_query_alter(&$view, &$query) {
	global $user;
	switch($view->name){
		case'view_virtual_tours':
		case'view_inc_reports';
		case'view_event_reports';
			$account_id = get_account_owner();
			$query->where[1]['conditions'][3]['value'] = $account_id;
		break;
		
		case'tours_today':
			//$day = date('w');
			//$query->where[1]['conditions'][1]['value'] = ($day);
			//dpm($view);
		break;
		
		case'saved_virtual_tours':
			$account_id = get_account_owner();
			$query->where[1]['conditions'][4]['value'] = $account_id;
		break;
	}
	return $query;
}

function fr_groups_get_form(){
	return drupal_get_form('user_prefrences_node_form');
}



function get_account_owner(){
	global $user;
	$account_owner = 1;
	$profile = profile2_load_by_user($user->uid, $type_name = NULL);
	if(!empty($profile['customer']->field_cust_alarm_group_owner['und'][0]['target_id'])){
		$account_owner = $profile['customer']->field_cust_alarm_group_owner['und'][0]['target_id'];
	}
	return $account_owner;
}