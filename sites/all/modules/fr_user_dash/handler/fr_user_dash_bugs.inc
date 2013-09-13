<?php

	
	class FortifiedBugs{
	
		/**
		* @ Query all the bugs
		*
		*/
		public function queryBugs(){
			$data = array();
			// Create an object of type SelectQuery
			$query = db_select('bugs', 'u')
				->fields('u', array('uid', 'username', 'date', 'bug_text', 'bug_id'))
				->range(0, 15);
				$result = $query->execute();
			/*while ($return_data = $query->fetchObject()) {
				$data[] = $return_data;
			}*/
			foreach ($result as $record) {
				$data[] = $record;
			}
			$this->bugs = $data;
		}
		
		public function renderBugs(){
			drupal_add_js(drupal_get_path('module', 'fr_user_dash') .'/js/fr_user_dash_misc.js', array('type' => 'file', 'scope' => 'footer'));
			$content = array();
			if(!empty($this->bugs)){
				$content['wrapper'] = array(
					'#type' => 'container',
					'#attributes' => array(
						'class' => array('bug-wrapper'),
					),
				);
				$data = '<table class="bugs-table sticky-enabled tableheader-processed sticky-table">
					<thead>
				  	<tr class="bug-table-headers">
				  	  <th>Date</th>
				  	  <th>Username</th>
				  	  <th>View</th>
				  	  <th>Delete</th>
				  	</tr>
				  	</thead>
				  	<tbody>';
				$bug_html = '';
				foreach($this->bugs as $delta => $bdata){
				$bug_html .= '<div class="bugs-text-div bug-text-'.$delta.'" data-locate="'.$delta.'">'.$bdata->bug_text.'</div>';
					$class = 'even';
					if ($delta % 2 == 0) {
						//logic needs to be backwords... i know class should = even :)
						$class = 'odd';
					}
					$data .= '<tr class="bug-tr '.$class.'">';
					$data .= '<td class="bugs-date">'.date('Y-m-d h:i:s a', $bdata->date).'</td>';
					$data .= '<td class="bugs-username first">'.$bdata->username.'</td>';
					$data .= '<td class="bugs-view"><a data-locate="'.$delta.'" href="#" class="bugs-view-bug">View</a></td>';
					$data .= '<td class="bugs-delete"><a href="#" data-bug-id="'.$bdata->bug_id.'" class="bugs-delete-bug">Delete</a></td>';
					$data .= '</tr>';
				}
				$data .= '</tbody>';
				$data .= '</table>';
				$data = $data . $bug_html;
				$content['wrapper']['bug_table'] = array(
					'#type' => 'markup',
					'#markup' => $data,
				);
			}
			
			return drupal_render($content);
		}
	
}