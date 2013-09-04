(function($) {
	$(document).ready(function() {
		var stop = false;
		var viewEvent;
		var IpDatatel = new Object();
		IpDatatel.settings = Drupal.settings.ip_datatel;
		if (IpDatatel.settings.processing_accounts && IpDatatel.settings.account_variables && IpDatatel.settings.uid) {
			IpDatatel.settings.session = Math.floor(Math.random() * 101);
			IpDatatel.settings.viewingEvent = false;
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
							if(!IpDatatel.settings.viewingEvent){
								$('#alarm-processing-table').html(data.status.table);
							}
					}
				},
				complete: function(data) {}
			});
		}
		
		$(document).on('mouseenter', '.alarm-event-wrapper', function(){
			IpDatatel.settings.viewingEvent = true;
			$(this).addClass('f-tooltip top');
			Drupal.settings.events = IpDatatel.settings.new_events;
		});
		$(document).on('mouseleave', '.alarm-event-wrapper', function(){
			IpDatatel.settings.viewingEvent = false;
		});
		$(document).on('hover mouseleave', ".f-tooltip-open", function(e) {
			switch (e.type) {
			case 'mouseenter':
				IpDatatel.settings.viewingEvent = true;
				break;
			case 'mouseleave':
				IpDatatel.settings.viewingEvent = false;
				break;
			}
		});
	});
})(jQuery);