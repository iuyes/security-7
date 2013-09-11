(function($) {
	$(document).ready(function() {
		var stop = false;
		var viewEvent;
		var IpDatatel = new Object();
		IpDatatel.settings = Drupal.settings.ip_datatel;
		if (IpDatatel.settings.processing_accounts && IpDatatel.settings.account_variables && IpDatatel.settings.uid) {
			IpDatatel.settings.session = Math.floor(Math.random() * 101);
			Drupal.settings.hoveringEvent = false;
			var startProcessing = setInterval(function() {
				var return_log = processingRequest();
			}, 10000);
		}

		function processingRequest(settings) {
			$.ajax({
				type: 'POST',
				url: '/ajax/alarm-processor',
				data: {
					settings: IpDatatel.settings,
				},
				success: function(data) {
				console.log(data.status);
					if (data.status) {
							IpDatatel.settings.start_time = data.status.start_time * 1;
							IpDatatel.settings.new_events = data.status.new_events;
							IpDatatel.settings.ran_processor = data.status.ran;
							if (IpDatatel.settings.ran_processor > 14) {
								IpDatatel.settings.ran_processor = 0;
							}
							Drupal.settings.events = data.status.new_events;
							if(!Drupal.settings.hoveringEvent){
								$('#alarm-processing-table').html(data.status.content.table);
							}
					}
				},
				complete: function(data) {}
			});
		}
		
		$(document).on('click', '.view-event', function(){
			$(this).parents('td').siblings().css('background', 'rgb(58, 209, 58)');
			var clickedEvent = $(this).attr('data-arguments');
			console.log(clickedEvent);
			console.log(IpDatatel.settings.new_events[clickedEvent])
			IpDatatel.settings.new_events[clickedEvent].clicked = 'TRUE';
		});
		
	});
})(jQuery);