<?php

class DashboardTools{

	private function generateMenuList ($list_items) {
		$menu_list = array();
		foreach($list_items as $key => $item){

			switch($item['type']){
			case'refresh':
				$js = 'window.reload();';
				break;
			default:
				$js = '';
				break;
			}
			$options = array();
			$arg = !empty($item['data-reveal-id']) ? 'data-reveal-id' : 'id';

			$path = l($item['title'], $item['path'], $options);
			$list[] = array(
				'data' => $path,
				'class' => $item['class'],
				$arg => $item['data-reveal-id'],
				'onClick' => array($js),
			);
		}
		$menu = array(
			'#theme' => 'item_list',
			'#items' => $list,
			'#type' => 'ul',
			'#attributes' => array('id' => 'basic-tools'),
			'#container_id' => '',
			//'#prefix' => '<section>',
			//'#suffix' => '</section></nav>',
		);
		return $menu;
	}

	private function buildListItems($array){
		foreach($array as $key => $values){
			$type = $values['type'];
			$title = $values['title'];
			$query = $values['query'];
			$path = $values['path'];
			$class = $values['class'];
			$data_reveal = !empty($values['data-reveal-id']) ? $values['data-reveal-id'] : '';
			$list[] = array(
				'type' => $type,
				'title' => t($title),
				'path' => $path,
				'query' => $query,
				'class' => array('button', $class),
				'data-reveal-id' => $data_reveal,
			);
		}
		$buttons = $this->generateMenuList($list);
		return $buttons;
	}

	public function basicTools(){
		$tools = array();
		$type = array('user', 'users', 'refresh', 'add an alarm', 'prefrences');
		$title = array('Add User', 'Manage Users', 'Refresh Page', 'Add A Bat', 'Prefrences');
		$path = array('admin/people/create', 'admin/people', '../../user', '../../node/add/alarm', '#');
		$query = array('', '', 'window.refresh();', '', '');
		$class = array('add', 'edit', 'next', 'add', 'next');
		$id = array('', '', '', '', 'admin-prefrences-form');
		$loop = count($type) - 1;
		for($i = 0; $i<=$loop; $i++){
			$data_reveal = empty($id[$i]) ? 'none' : 'data-reveal-id';
			$tools[$i] = array(
				'type' => $type[$i],
				'title' => $title[$i],
				'path' => $path[$i],
				'query' => $query[$i],
				'class' => $class[$i],
				$data_reveal => $id[$i],
			);
		}
		$build = $this->buildListItems($tools);
		return $build;
	}
	
}