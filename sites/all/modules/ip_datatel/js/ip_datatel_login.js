(function($){
	$(document).ready(function(){
		$('.alarm_login').click(function(e){
			e.preventDefault();
				$('#ipdatatel-login').remove();
			//$('.alarm-modal').html('');
			var alarmIframe = '<iframe id="ipdatatel-login" width="100%" seamless height="750px" style="border:2px solid white;" scrolling="no" src="http://fortifiedss.alarmdealer.com/index.php?mod=auth&action=login"></iframe>';
			$('.alarm-modal').append(alarmIframe);
			$('#myAlarmModal').foundation('reveal', 'open');
		});
	});
})(jQuery);
       	  
