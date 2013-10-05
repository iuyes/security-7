<?php

class getRequest{

	public function loadUser(){
		global $user;
		$this->user_account = $user;
		$this->user = $user->uid;
	}

	public function requestDownload($nid){
		drupal_add_http_header('Content-Transfer-Encoding', 'binary');
		if(!empty($nid)){
			$node = node_load($nid);
			if(!empty($node)){
				$this->customer = $node->field_cust_alarm_group_owner['und'][0]['target_id'];
				$account_info = fr_tweaks_load_user_profile2_account_id($this->user_account, 9);
				if($account_info['account_node_id'] == $this->customer){
					$get_files = explode(',', $node->field_vt_tour_files['und'][0]['upload_type']);
					if(in_array('video', $get_files)){
						$videos = $node->field_vt_tour_files['und'][0]['video_files'];
						$get_videos = explode(',', $videos);
						foreach($get_videos as $key => $video){
							$link = $this->downloadVideo($video);
							$this->status[] = $link;
						}
						return $link;
					}
				}
			}
		}
	}

	private function downloadVideo($video_fid){
		$load_file = file_load($video_fid);
		$file_name = $load_file->filename;
		$format = fr_user_dash_video_file_download($load_file);
		$stream = $format['uri'];
		$options = array();
		return $stream;
	}

	public function getView($uid, $type){
		$results = $this->queryData($uid, $type);
		$pageModal['html'] = drupal_render($results);
		return $pageModal;
	}


	private function queryData($uid, $type){
		$query = new EntityFieldQuery;
		$query->entityCondition('entity_type', 'node')
		->entityCondition('bundle', $type)
		->fieldCondition('field_vt_customer', 'target_id', $uid, '=')
		->propertyCondition('status', 1)
		->propertyOrderBy('created', 'DESC')
		->range(0, 10);
		$result = $query->execute();
		$data = array();
		$headers = array(
			'Title',
			'ID',
			'Date',
			'Edit',
			'Manage Files',
			'Preview',
		);
		if(!empty($result['node'])){
			$rows = array();
			foreach($result['node'] as $key => $values){
				$value = node_load($values->nid);
				$rows[] = array(
					'data' => array(
						array(
							'data' => '<a href="../../node/'.$value->nid.'">'.$value->title.'</a>',
							'style' => array(''),
							'class' => array(''),
						),
						array(
							'data' => $value->nid,
							'style' => array(''),
							'class' => array(''),
						),
						array(
							'data' => date('Y-m-d g:i a', $value->created),
							'style' => array(''),
							'class' => array(''),
						),
						array(
							'data' => '<a href="../../node/'.$value->nid.'/edit">Edit</a>',
							'style' => array(''),
							'class' => array(''),
						),
						array(
							'data' => '<a href="../../file-browse/file/'.$value->nid.'">Manage Files</a>',
							'style' => array(''),
							'class' => array(''),
						),
						array(
							'data' => '<input type="button" class="preview-cust-data button radius small" value="Preview" cust-nid="'.$value->nid.'"/>',
							'style' => array(''),
							'class' => array(''),
						),
					),
				);

			}
		}


		$data['virtual_tours'] = array(
			'#type' => 'container',
			'#attributes' => array(
				'class' => array('cust-data-wrapper'),
			),
		);

		$data['virtual_tours']['table'] = array(
			'#theme' => 'table',
			'#header' => $headers,
			'#rows' => $rows,
			'#empty' => t('Sorry No Data For This Customer'),
			'#group' => 'wrapper',
			'#attributes' => array(
				'class' => array('cust-data-table', 'large-6', 'collumns'),
			),
		);
		$data['virtual_tours']['preview'] = array(
			'#type' => 'markup',
			'#markup' => '',
			'#prefix' => '<div id="preview-cust-data" class="large-12 columns">',
			'#suffix' => '</div>',
		);
		return $data;

	}




	public function saveFiles($nid){
		$load = node_load($nid);
		$save = $load->field_vt_tour_saved_status;
		if(empty($load->field_vt_tour_saved_status['und'][0]['value'])){
			$load->field_vt_tour_saved_status['und'][0]['value'] = TRUE;
		}else{
			$load->field_vt_tour_saved_status['und'][0]['value'] = TRUE;
		}
		$done = node_save($load);
		if($done){
			$status = array('status' => 0);
		}else{
			$status = array('status' => 1);
		}
		return $status;
	}
	
	public function loadCustProfile($uid){
		$this->cust_profile = profile2_load_by_user($uid, $type_name = NULL);
	}
	
	
	public function loadCustDvrs($uid){
		$dvrs = new FortifiedDvrs();
		$get_dvrs = $dvrs->queryDvrs($uid);
		$this->dvr_data = $get_dvrs;
		return $get_dvrs;
	}

}