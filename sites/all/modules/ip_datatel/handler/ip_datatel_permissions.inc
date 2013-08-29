<?php


class IpdatatelPermissons {

	public $account_permissions;

	public function __construct($account_data, array $permissions){
		$this->account_data = $account_data;
		$this->permissions = $permissions;
	}
	
	
	public function structureAccountPermissions($account_data, $permissions, $bool = FALSE){
		$accounts = current($account_data);
		$requested_permissions = array();
		$has_access = array();
		foreach($accounts as $account_name => $account_data){
			foreach($permissions as $permission_name => $permission){
				if($bool){
					$has_access[$account_name] = !empty($account_data[$permission_name][$permission]) ? TRUE : FALSE;
				}else{
					$has_access[$account_name] = !empty($account_data[$permission_name][$permission]) ? $accounts[$account_name] : FALSE;
				}
			}
		}
		return $has_access;
	}
	
}