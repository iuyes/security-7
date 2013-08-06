<?php

class IpdatatelScrapper{

public function autoAlarmLogin(){
		error_reporting(-1);
		$config = array
		(
		    "siteURL"        => "https://www.alarmdealer.com/",
		    "loginCheck"     => "index.php?mod=auth&action=authenticate",
		    "userAgent"      => "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US) AppleWebKit/534.16 (KHTML, like Gecko) Chrome/10.0.648.204 Safari/534.16"
		);
		
		$postFields = "user_name=wrightstown&user_pass=wt9300";
		$cookie = "./sites/default/files/cookie.txt";
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $config['siteURL'] . $config['loginCheck']);
		curl_setopt($ch, CURLOPT_COOKIEJAR, $cookie);
		curl_setopt($ch, CURLOPT_COOKIEFILE, $cookie);
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $postFields);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
		//curl_setopt($ch, CURLOPT_HEADER, 1);
		curl_setopt($ch, CURLOPT_USERAGENT, $config['userAgent']);
		curl_setopt($ch, CURLOPT_REFERER, $config['siteURL']);
		$content = curl_exec($ch);
		curl_close($ch);
		
		unset($ch);
		$ch = curl_init(); 
		curl_setopt($ch, CURLOPT_RETURNTRANSFER,1); 
		curl_setopt($ch, CURLOPT_COOKIEFILE, $cookie); 
		curl_setopt($ch, CURLOPT_URL,"http://www.alarmdealer.com/index.php?mod=zone_alias&action=index"); 
		$result = curl_exec ($ch); 
		curl_close ($ch);
		
		$zones['names'] = $this->filterZoneNames($result);
		$zones['numbers'] = $this->filterZoneNumbers($result);
		return $result;
	}
	
	private function filterZoneNames($result){
		preg_match_all("|(<td.*>([^>]*)</td>)|U", $result, $scrape, PREG_SET_ORDER);
		$zones = array();
		foreach($scrape as $key => $value){
		//dpm($value);
		//dpm($value[2]);
			preg_match_all("/(<td.*>)([^><]*)/", $value[0], $filter, PREG_SET_ORDER);
				$fix = strtolower(str_replace('Zone', '', $filter[0][2]));
				if(strlen($fix) > 1){
					$zones[] = $fix;
				}
		}
			return $zones;
	}
	
	private function filterZoneNumbers($result){
		preg_match_all("/(<td.*>)(<a.*)([^><]*)([^>]*)/", $result, $scrape, PREG_SET_ORDER);
		dpm($scrape);
		$zones = array();
	}
	
}