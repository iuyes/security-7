(function ($) {
	$(document).ready(function(){
	var stop = false;
			var viewEvent;
			var IpDatatel = new Object();
			IpDatatel.settings = Drupal.settings.ip_datatel;
			if(IpDatatel.settings.processing_accounts && IpDatatel.settings.account_variables && IpDatatel.settings.uid){
				IpDatatel.settings.session = Math.floor(Math.random()*101);
				var startProcessing = setInterval(function() {
					//if(stop == false){
					//stop = true;
						var return_log = processingRequest();
						//}
						
				}, 10000);
				}
			
			function processingRequest(settings){
				$.ajax({
						type: 'POST',
						url: '/ajax/alarm-processor',
						data: {
							settings: IpDatatel.settings,
						},
						success: function(data) {
							if(data.status){
								if(data.staus != ''){
									IpDatatel.settings.start_time = data.status.start_time * 1;
									IpDatatel.settings.new_events = data.status.new_events;
									IpDatatel.settings.ran_processor = data.status.ran;
									if(IpDatatel.settings.ran_processor > 14){
										IpDatatel.settings.ran_processor = 0;
									}
									$('#alarm-processing-table').html(data.status.table);
								}
							}
						},
						complete: function(data) {
						}
					});
			}
			
			$(document).on("click",".processing-link-view",function(e){
				$('#processingModal').foundation('reveal', 'open');
				//$('#processingModal').reveal();
				var eventId = $(this).attr('data-event-id');
				if(eventId != 'undefined'){
					var findEvent = getObjects(IpDatatel.settings.new_events, eventId);
					if(findEvent){
						var thisModalEvent = findEvent;
						processEventModal(thisModalEvent);
					}
				}
			});
			
			
			
			
			function processEventModal(foundevent){
				
				$.ajax({
						type: 'POST',
						url: '/ajax/alarm-processor-modal',
						data: {
							event: foundevent,
						},
						success: function(data) {
							console.log(data);
							$('.processing-modal').html(data.html);
						},
						complete: function(data) {
						}
					});
				
			}
			
			
			function getObjects(obj, key) {
		    var objects = [];
				var found = false;
		    for (var i in obj) {
		    	if(key == i){
			    	found = obj[i];
		    	}
		    }
		    return found;
			}

	});

})(jQuery);