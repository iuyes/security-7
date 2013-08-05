<?php

/**
 * Implements hook_views_query_alter().
 */
function fr_user_dash_views_query_alter(&$view, &$query) {
	global $user;
	switch($view->name){
		case'view_virtual_tours':
		case'view_inc_reports';
		case'view_event_reports';
			global $user;
			$uid = $user->uid;
			$query->where[1]['conditions'][2]['value'] = $uid;
		
		break;
		
		case'tours_today':
			//$day = date('w');
			//$query->where[1]['conditions'][1]['value'] = ($day);
			//dpm($view);
		break;
		
		case'saved_virtual_tours':
			$uid = $user->uid;
			$query->where[1]['conditions'][2]['value'] = $uid;	
		break;
	}
	return $query;
}

function fr_user_dash_get_form(){
	return drupal_get_form('user_prefrences_node_form');
}