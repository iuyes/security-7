(function($){
	$(document).ready(function(){
		$('#alarm_login').click(function(){
			var alarmIframe = '<iframe seamless width="100%" height="700px" src="http://alarmdealer.com/index.php?mod=devices&action=keypad"></iframe>';
			$('.alarm-modal').append(alarmIframe);
			$('#myAlarmModal').foundation('reveal', 'open');
		});
	});
})(jQuery);
       	  
