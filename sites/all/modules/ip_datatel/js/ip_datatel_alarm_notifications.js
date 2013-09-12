(function($) {
	$(document).ready(function() {
		var stop = false;
		var viewEvent;
		var IpDatatel = new Object();
		IpDatatel.functions = {};
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
		
		IpDatatel.functions.viewEventChangeColor = $(document).on('click', '.view-event', function(){
			var tableRow = $(this).parents('td');
			tableRow.css('background', 'rgb(58, 209, 58)').siblings().css('background', 'rgb(58, 209, 58)');
			var clickedEvent = $(this).attr('data-arguments');
			IpDatatel.settings.new_events[clickedEvent].clicked = 'TRUE';
			IpDatatel.functions.updateNewEvents();
		});
		
		IpDatatel.functions.removeEvent = $(document).on('click', '.remove-event', function(e){
			e.preventDefault();
			var eventId = $(this).attr('data-event');
			$(this).html('Event will be removed in a few seconds!');
			IpDatatel.settings.new_events[eventId].status = 'removed';
			IpDatatel.functions.updateNewEvents();
		});
		
		IpDatatel.functions.updateNewEvents = function(){
			Drupal.settings.new_events = IpDatatel.settings.new_events;
		}
		
	});
})(jQuery);