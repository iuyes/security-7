(function($) {
	$(document).ready(function() {
		
		/*$('.days_select').change(function(){
			var times = $(this).attr('value');
			alert(times);
			showTimes(times);
		});
		
		function showTimes(times){
			for(var t=0; t<=times; t++){
				$('.day_'+t).show();
				alert(t);
			}
		}*/
		
		$('#edit-field-fr-vt-tour-times-und-0-tour-times-tour-amount').change(function(){
				var new_times = $(this).attr('value');
				displayTimes(new_times);
		});
		
		var default_times = $('#edit-field-fr-vt-tour-times-und-0-tour-times-tour-amount').attr('value');
			if(default_times > 1){
				displayTimes(default_times);
			}
			
		function displayTimes(new_times){
			var amount = new_times;
		$.ajax({
			type: 'POST',
			url: '/fr_times_time',
			dataType: 'json',
			data: {
				quantity: amount
			},
			success: function(data) {
				$('.tour-times-wrapper').hide();
				$('.tour-times').hide();
				var returned = data.status;
				for(var i = 1; i<=returned; i++){
					//$('.times_'+i).show();
					//$('.html').hide();
					//$('.tour-times').css('display', 'block !important');
					$('.tour_times_'+i).show();
					$('.time_'+i).show();
					$('.time_'+i).css('display', 'inline');
				}
			},
			complete: function(data) {
				checkEmpty();
			}
		});
		}
		
		function checkEmpty(){
			var notify = new Array();
			for(var m=1; m<=10; m++){
				if(!$('.time_'+m).is(":visible")){
					var is_hidden = $('#edit-field-fr-vt-tour-times-und-0-tour-times-times-wrapper'+m+'-time-hour-'+m+'').attr('value');
					if(is_hidden != ''){
						notify[m] = m;
					}
				}
			}
			if(notify != ''){
				alert('You need to give Tour Times:' +notify+ 'Empty Values. You have chosen to hide a tour time but not give it empty values');
			}
		}
});
})(jQuery);