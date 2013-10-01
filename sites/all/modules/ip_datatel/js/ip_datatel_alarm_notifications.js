(function($) {

	$(document).ready(function() {
		var stop = false;
		var viewEvent;
		var IpDatatel = new Object();
		var FortifiedALarms = {};
		FortifiedALarms.alert = false;
		Drupal.settings.activeAlarm = false;
		IpDatatel.functions = {};
		IpDatatel.settings = Drupal.settings.ip_datatel;
		var audoElement = '<div id="audio-container"><audio id="buzzer" src="../../sites/all/themes/fortified_dev/audio/alert.mp3" type="audio/mp3"></audio></div>';
		$('body').append(audoElement);
				
		/**
		 * @ If we have all of the settings coming from @file ip_datatel_alarm_processor.inc
     *
     */
		if (IpDatatel.settings.processing_accounts && IpDatatel.settings.account_variables && IpDatatel.settings.uid) {
			IpDatatel.settings.session = Math.floor(Math.random() * 101);
			Drupal.settings.hoveringEvent = false;
			var startProcessing = setInterval(function() {
				var return_log = processingRequest();
			}, 10000);
		}
		
		/**
		 * @ Interval Function for sending data @file ip_datatel_alarm_processor.inc
     *
     */
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
							
							if( Drupal.settings.activeAlarm == true ){
								IpDatatel.functions.alarmAlertAudio();
							}else{
								$('#stop-alarm-audio').remove();
							}
							
							Drupal.settings.events = data.status.new_events;
							if(!Drupal.settings.hoveringEvent){
								$('#alarm-processing-table').html(data.status.content.table);
							}
							if(data.status.alarms != 'undefined' && data.status.alarms.length > 0){
								FortifiedALarms.alert = true;
								for( var i = 0; i < data.status.alarms.length; i++ ){
									if(data.status.alarms[i] != '' && data.status.alarms[i].dvr_nid){
										IpDatatel.alarmNotifications = data.status.alarms[i];
										IpDatatel.functions.processAlarmEvents(IpDatatel.alarmNotifications);
									}
								}
							}
					}
				},
				complete: function(data) {}
			});
		}
		
		
		/**
		 * @ Function to stop the alarm audio.
     *
     */
		IpDatatel.functions.stopAlarmAudio = $(document).on('click', '#stop-alarm-audio', function(){
			$('#stop-alarm-audio').remove();
			$('#audio-container').remove();
			FortifiedALarms.alert = false;
			Drupal.settings.activeAlarm = false;
		});
		
		/**
		 * @ Function to play adio when there is an alarm.
     *
     */
		IpDatatel.functions.alarmAlertAudio = function(){
			$('#stop-alarm-audio').remove();
			if(FortifiedALarms.alert){
				if( !$('#audio-container').get(0) ){
					$('body').append(audoElement);
				}
				$('#buzzer')[0].play();
				var stopAudioElement = '<div id="stop-alarm-audio">Stop Alarm Audio</div>';
				$('.dvr-processing-list').append(stopAudioElement);
			}
		}
		
		
		/**
		 * @ Function for closing the dvr object
     *
     */
		IpDatatel.functions.closeDvrObject = $(document).on('click', '.close-dvr-object', function(){
			$('.dvr-processing-list li').removeClass('active');
			$('#dvr-cms-iframe').removeClass('open');
			$('#dvr-cms-container').html('');
			$('.close-dvr-object').hide();
			$('#alarm-processing-table').removeClass('fixed');
			Drupal.settings.activeAlarm = false;
		});
		
		

		
		/**
		 * @ Functio for when an alarm comes in
     *
     */
		IpDatatel.functions.processAlarmEvents = function(notification){
			IpDatatel.alarmNotifications.dvrElement = $('[data-dvr-id="'+ notification.dvr_nid +'"]');
			if( !$('#dvr-cms-iframe').hasClass('open') ){
				$('.close-dvr-object').show().addClass('open');
				IpDatatel.alarmNotifications.dvrElement.trigger('click');
				IpDatatel.alarmNotifications.dvrElement.closest('li').addClass('active alarm');
				Drupal.settings.activeAlarm = true;
				FortifiedALarms.alert = true;
				IpDatatel.functions.alarmAlertAudio();
				$('#alarm-processing-table').addClass('fixed');
				
			}else{
				IpDatatel.functions.alarmAlertAudio();
				var toggleSelector = $('[data-dvr-id="'+ notification.dvr_nid +'"]').closest('li');
				if( $(toggleSelector).hasClass('active') ){
					$(toggleSelector).removeClass('active');
					if(  $(toggleSelector).hasClass('alarm') ){
						$(toggleSelector).removeClass('alarm');
					}
				}
				$(toggleSelector).addClass('alarm');
				IpDatatel.functions.alarmAlertAudio();
				Drupal.settings.activeAlarm = true;
				FortifiedALarms.alert = true;
			}
		}
		
		/**
		 * @ Tooltip functio nto show event details
     *
     */
		IpDatatel.functions.viewEventChangeColor = $(document).on('click', '.view-event', function(){
			var tableRow = $(this).parents('td');
			tableRow.css('background', 'rgb(58, 209, 58)').siblings().css('background', 'rgb(58, 209, 58)');
			var clickedEvent = $(this).attr('data-arguments');
			IpDatatel.settings.new_events[clickedEvent].clicked = 'TRUE';
			IpDatatel.functions.updateNewEvents();
		});
		
		
		/**
		 * @ Function for removing an event
     *
     */
		IpDatatel.functions.removeEvent = $(document).on('click', '.remove-event', function(e){
			e.preventDefault();
			var eventId = $(this).attr('data-event');
			$(this).html('Event will be removed in a few seconds!');
			IpDatatel.settings.new_events[eventId].status = 'removed';
			IpDatatel.functions.updateNewEvents();
		});
		
		
		/**
		 * @ Functio for updating the events object for customer tooltips
     *
     */
		IpDatatel.functions.updateNewEvents = function(){
			Drupal.settings.new_events = IpDatatel.settings.new_events;
		}
		
	});
})(jQuery);