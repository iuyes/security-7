<?php



/**
 * Implements hook_views_query_alter().
 */
function fr_groups_views_query_alter(&$view, &$query) {
	global $user;
	switch($view->name){
		case'view_virtual_tours':
		case'view_inc_reports':
		case'view_event_reports':
			$account_id = get_account_owner();
			$query->where[1]['conditions'][3]['value'] =  ( int ) $account_id;
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
	return $view;
}

function fr_groups_get_form(){
	return drupal_get_form('user_prefrences_node_form');
}



function get_account_owner(){
	global $user;
	$account_owner = 1;
	$account_type = fr_groups_role_type($user);
	$profile = profile2_load_by_user($user->uid, $type_name = NULL);
	if(!empty($profile[$account_type]->field_cust_alarm_group_owner['und'][0]['target_id'])){
		$account_owner = $profile[$account_type]->field_cust_alarm_group_owner['und'][0]['target_id'];
	}
	return $account_owner;
}