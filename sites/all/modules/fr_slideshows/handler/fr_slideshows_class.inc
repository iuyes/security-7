<?php

class FrSlideshow {
  
  public function querySlideshowData($entity_type, $bundle, $field_conditions, $arguments){
    $this->query = new EntityFieldQuery;
    //load up the arguments
    $this->structurQueryArgs($arguments);
    //set up field conditions
    if(!empty($field_conditions)){
      $field_name = !empty($field_conditions['field_name']) ? $field_conditions['field_name'] : '';
      $target = !empty($field_conditions['target']) ? $field_conditions['target'] : '';
      $argument = !empty($field_conditions['argument']) ? $field_conditions['argument'] : '';
      $operator = !empty($field_conditions['operator']) ? $field_conditions['operator'] : '';
    }
    //basic query data
    $this->query->entityCondition('entity_type', $entity_type);
    $this->query->entityCondition('bundle', $bundle);
    $this->query->propertyOrderBy('created', 'DESC');
    //check for empty values so we can do a field condition
    if(!empty($field_name) && !empty($target) && !empty($argument) && !empty($operator)){
      $this->query->fieldCondition($field_name, $target, $argument, $operator);
    }
    //execute query
    $results = $this->query->execute();
    return $results;
  }
  
  
  
  public function getHighlightedMarkup($results, $entity_type, $args){
    $markup = array();
    foreach($results as $key => $value){
      $node = node_load($value->nid);
      if($node){
        $markup[] = node_view($node, $view_mode = 'teaser', $langcode = NULL);
      }
    }
    return $markup;
  }
  
  public function getSlideshowImages($results, $entity_type, $field_name, $captions = FALSE, $path = FALSE){
    $images = array();
    $caps = array();
    $paths = array();
    foreach($results as $key => $nodes){
      $node = node_load($nodes->nid);
			if(!empty($node->field_main_page_featured_image)){
	      $get_images = field_get_items($entity_type, $node, $field_name, $langcode = NULL);
	      if($path){
	      	$paths[] = drupal_get_path_alias('node/'.$nodes->nid);
	      }
	      if(!empty($captions)){
	        $get_caps = field_get_items($entity_type, $node, $captions, $langcode = NULL);
	        if($get_caps){
	          $caps[] = $get_caps[0]['value'];
	        }
	      }
	      if($get_images){
	        $images[] = $get_images;
	      }
	    }
	    if(!empty($caps)){
	      $images = $this->mergeData($images, 'caption', $caps);
	    }
	    if(!empty($paths)){
		    $images =  $this->mergeData($images, 'path', $paths);
	    }
	  }
    return $images;
  
 }
  
  
  private function mergeData($images, $name, $data){
    $image_cap = array();
    foreach($images as $key => $value){
      foreach($data as $delta => $values){
        $images[$key][0][$name] = $data[$key];
      }
    }
    return $images;
  }
  
  
  public function listImages($images){
    $list_items = array();
    $options = array('html' => TRUE);
    foreach($images as $delta => $delt){
      foreach($delt as $key => $image){
        $image_path = array('path' => file_create_url($image['uri']));
        $render_image = theme('image', $image_path);
        $caption = !empty($image['caption']) ? $image['caption'] : '';
        $path = !empty($image['path']) ? l($render_image, $image['path'], $options) : '';
        $render_image = !empty($path) ? $path : $render_image;
        $list_items[] = array(
          'data' => $render_image.'<div class="orbit-caption">'.$caption.'</div>',
        );
      }
    }
    return $list_items;
  }
  
  
  private function structurQueryArgs($arguments){
    if(!empty($arguments)){
      foreach($arguments as $type => $arg){
        switch($type){
          case'propertyCondition':
            if(is_array($arg)){
              foreach($arg as $key => $argument){
               $this->query->propertyCondition($key, $argument); 
              }
            }
            break;
          case'range':
            if(is_array($arg)){
              foreach($arg as $key => $argument){
                $this->query->range($key, $argument);
              }
            }
            break;
        }
      }
    } 
  }
}