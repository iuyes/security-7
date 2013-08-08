<?php

class IpdatatelScrapper{

	private $curl_default_settings = array();
	private $cookie_file = NULL;
	public $url_arguments;
	public $remote_url;
	public $remote_post_url;
	public $browser_user_agent;
	public $cookie;
	public $execute;
	const ERROR_URL_ARGUMENTS = 'ERROR: Url is not an array or is empty';
	const ERROR_REMOTE_URL = 'ERROR: You must set the remote url and or the remot post url!';
	const ERROR_EXECUTE = 'Curl cannot be executed due to errors. Fix the errors and try again';
	public $returned_content;


	public function __construct(){
		error_reporting(-1);
		$this->curl = curl_init();
	}

	public function setUrlPerameters($base_url, $post_url, $arguments, $browser = NULL){
		$this->remote_url = $base_url;
		$this->remote_post_url = !empty($post_url) ? $post_url : '';
		$this->browser_user_agent = empty($browser) ? "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US) AppleWebKit/534.16 (KHTML, like Gecko) Chrome/10.0.648.204 Safari/534.16" : $browser;
		$this->url_arguments = is_array($arguments) ? self::urlArguments($arguments) : self::ERROR_URL_ARGUMENTS;
	}

	//helper function
	private function urlArguments($arguments){
		$post_url_arguments = '';
		$i = 0;
		foreach($arguments as $arg => $value){
			$prefix = $i > 0 ? '&' : '';
			$post_url_arguments .= $prefix . $arg .'='. $value;
			$i++;
		}
		return $post_url_arguments;
	}


	public function curlSetCookieLocation($cookie){
		if(file_exists($cookie)){
			$this->cookie = $cookie;
			curl_setopt($this->curl, CURLOPT_COOKIEJAR, $cookie);
			curl_setopt($this->curl, CURLOPT_COOKIEFILE, $cookie);
		}
	}


	public function curlSettings($settings){
		$errors = array();
		!empty($this->remote_url) || !empty($this->remote_post_url) ? curl_setopt($this->curl, CURLOPT_URL, $this->remote_url.$this->remote_post_url) : $errors[] = self::ERROR_REMOTE_URL;
		!empty($this->remote_url) ? curl_setopt($this->curl, CURLOPT_REFERER, $this->remote_url) : $errors[] = self::ERROR_REMOTE_URL;
		!empty($this->url_arguments) ? curl_setopt($this->curl, CURLOPT_POSTFIELDS, $this->url_arguments) : $errors[] = self::ERROR_URL_ARGUMENTS;
		curl_setopt($this->curl, CURLOPT_USERAGENT, $this->browser_user_agent);
		if(is_array($settings)){
			foreach($settings as $key => $value){
				switch($key){
				case'CURLOPT_POST':
					curl_setopt($this->curl, CURLOPT_POST, $value);
					break;
				case'CURLOPT_RETURNTRANSFER':
					curl_setopt($this->curl, CURLOPT_RETURNTRANSFER, $value);
					break;
				case'CURLOPT_FOLLOWLOCATION':
					curl_setopt($this->curl, CURLOPT_FOLLOWLOCATION, $value);
					break;
				case'CURLOPT_COOKIEFILE':
					curl_setopt($this->curl, CURLOPT_COOKIEFILE, $value);
					break;
				case'CURLOPT_URL':
					curl_setopt($this->curl, CURLOPT_URL, $value);
					break;
				}
			}
		}
		if(!empty($errors)){
			$this->execute = FALSE;
			return $errors;
		}else{
			$this->execute = TRUE;
		}
	}



	public function curlExecute($redirect = FALSE){
		if(!$redirect){
			$content = ($this->execute == TRUE) ? curl_exec($this->curl) . curl_close($this->curl) : self::ERROR_EXECUTE;
			return $content;
		}elseif($this->execute == TRUE){
			$content = curl_exec($this->curl);
			curl_close($this->curl);
			unset($this->curl);
			self::__construct();
			$this->curlSettings($redirect);
			$result = curl_exec($this->curl);
			curl_close($this->curl);
			return $result;
		}
	}
}

class Scraper{

	public $results = array();
	
	public function __construct($document){
		$this->document = $document;
	}
	public function whichScrapper($which){
		$results = '';
		foreach($which as $function => $value){
			switch($function){
			case'filterZoneNames':
				$this->results['zone_names'] = $this->filterZoneNames($this->document);
				break;
			case'filterZoneNumbers':
				$this->results['zone_numbers'] = $this->filterZoneNumbers($this->document);
				break;
			}
		}
		return $this->results;
	}
	
	public function mergeZones($zone_names, $zone_numbers){
		$alarm_zones = array();
		foreach($zone_names as $key => $zone_name){
			foreach($zone_numbers as $delta => $zone_num){
				$alarm_zones[$zone_name] = $zone_numbers[$key];
			}
		}
		return $alarm_zones;
	}

	private function filterZoneNames($result){
		preg_match_all("|(<td.*>([^>]*)</td>)|U", $result, $scrape, PREG_SET_ORDER);
		$zones = array();
		foreach($scrape as $key => $value){
			preg_match_all("/(<td.*>)([^><]*)/", $value[0], $filter, PREG_SET_ORDER);
			$fix = strtolower(str_replace('Zone', '', $filter[0][2]));
			if(strlen($fix) > 1){
				$zones[] = $fix;
			}
		}
		return $zones;
	}

	private function filterZoneNumbers($result){
		preg_match_all("|((<a.*?)(?P<didget>\d+)([^>]*))|U", $result, $scrape, PREG_SET_ORDER);
		$zones = array();
		foreach($scrape as $key => $value){
			preg_match_all("/(<a.*>)([^><]*)/", $value[0], $filter, PREG_SET_ORDER);
			$fix = !empty($filter[0][2]) ? $filter[0][2] : '';
			if(!empty($fix) && $fix > 1){
				$zones[] = $fix;	
			}
		}
		return $zones;
	}

}