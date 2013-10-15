(function($) {

	$(document).ready(function() {	
	
	
	
	
	
		//objects & arrays
		var IpDatatel = new Object();
		var FortifiedALarms = {};
		//ipdatatel objects
		IpDatatel.functions = {};
		IpDatatel.settings = Drupal.settings.ip_datatel;
		IpDatatel.settings.alarms = [];
		IpDatatel.settings.filters = [];
		//processing objects
		var ProcessingSettings = {};
		ProcessingSettings.functions = {};
		//alarm objects
		IpDatatel.alarmNotifications = {};
		FortifiedALarms.removedEvents = [];
		//cache objects
		var processingCache = {};
		processingCache.filters = [];
		processingCache.settings = [];
		
		//bools && misc variables
		var viewEvent;
		FortifiedALarms.alert = false;
		Drupal.settings.activeAlarm = false;
		FortifiedALarms.open = false;
		FortifiedALarms.hoveringActions = false;
		//append the alarm audio element to the DOM body
		var audoElement = '<div id="audio-container"><audio id="buzzer" src="../../sites/all/themes/fortified_dev/audio/alert.mp3" type="audio/mp3"></audio></div>';
		$('body').append(audoElement);
		
		
		
		
		
		
		/**
		 * @ If we have all of the settings coming from @file ip_datatel_alarm_processor.inc
     *
     */
		if (IpDatatel.settings.processing_accounts && IpDatatel.settings.account_variables && IpDatatel.settings.uid) {
			//random session number
			IpDatatel.settings.session = Math.floor(Math.random() * 101);
			//For when you arent overing over an events tooltip
			Drupal.settings.hoveringEvent = false;
			//start the processing
			var startProcessing = setInterval(function() {
				//ajax processing request
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
					settings: IpDatatel.settings, // account data passed to use from @ ip_datatel_alarm_processor.inc
				},
				success: function(data) {
					// if we get data back
					if (data.status) {
							//the time we started the processing. this gives us the ability to only grab events after this time
							IpDatatel.settings.start_time = data.status.start_time * 1;
							//the new events returned
							IpDatatel.settings.new_events = data.status.new_events;
							//the number of times the processor has ran. after 15 times, we the processing session
							IpDatatel.settings.ran_processor = data.status.ran;
							//the user type who is running the processor
							IpDatatel.settings.type = data.status.type;
							//the dynamic filters the user has set while processing
							IpDatatel.settings.filters = ProcessingSettings.checkFilters();
							//the number of times the processor has ran. after 15 times, we the processing session
							IpDatatel.settings.ran_processor = IpDatatel.settings.ran_processor > 14 ? 0 : IpDatatel.settings.ran_processor;
							//check to see if the alarm audio filter was changed
							IpDatatel.settings.alarmAudio = ProcessingSettings.checkAlarmAudio();
							//if we have an active alarm
							if(Drupal.settings.activeAlarm && IpDatatel.settings.alarmAudio){
								//start playing the audio
								IpDatatel.functions.alarmAlertAudio();
							//else stop alarm audio
							}else{
								$('#stop-alarm-audio').remove();
							}
							//for hovering event
							Drupal.settings.events = data.status.new_events;
							//We replace the whole processing table with the new one returned from our ajax calback
							if(!Drupal.settings.hoveringEvent && !FortifiedALarms.hoveringActions){
								$('#alarm-processing-table').html(data.status.content.table);
							}
							//number of alarms in the alarm object. these get removed when a they click the dvr to view the alarm
							var alarmSize = IpDatatel.functions.checkEmptyAlarms(data.status.alarms);
							// if we have alarms
							if(alarmSize.size > 0){
								//process the new alarms
								IpDatatel.functions.processAlarms(IpDatatel.settings.type, data.status.alarms, alarmSize.size, alarmSize.key);
							}
					}
				},
				complete: function(data) {}
			});
		}
		
		
		
		
		
		/**
		 * @ Function to process alarms
     *
     */
		IpDatatel.functions.processAlarms = function(role, alarms, alarmcount, key){
			key = key * 1;
			var index;
			//We process alarms different depending on the user type which is stored in IpDatatel.settings.type
			switch(role){
				case 'account_manager':
				case 'customer':
				case 'user':
					//new alarms
				  FortifiedALarms.alert = true;
				  //array to store the new alarms
				  FortifiedALarms.alarms = [];
				  //dynamic for loop depending on number of alarms in the array and number alarm removed from the array
				  for( var i = key; i < alarmcount; i++ ){
				  	if(!alarms[i].processed){
				  	//store the alarms
				  		FortifiedALarms.alarms[i] = alarms[i];
				  		IpDatatel.settings.alarms[i] = alarms[i];
				  		//process each indavidule alarm
				  		IpDatatel.functions.processAlarmEvents(alarms[i], i);
				  	}
				  }
				  break;
				case 'admin':
				case 'central_station':
				  var parentAccount;
						FortifiedALarms.alert = true;
						FortifiedALarms.alarms = [];
						//dynamic for loop depending on number of alarms in the array and number alarm removed from the array
						for( var i = key; i < alarmcount; i++ ){
				  		if(!alarms[i].processed){
				  			//store the alarms
				  			FortifiedALarms.alarms[i] = alarms[i];
								IpDatatel.settings.alarms[i] = alarms[i];
								parentAccount = alarms[i].account_owner;
								//ajax function to return the dvrs which belong to the the alarm bat which is having an alarm
								IpDatatel.functions.ajaxCustomerDvrs(parentAccount);
								//set a delay so the ajax has time to finish. otherwise it wont toggle the dvr
								window.setTimeout(IpDatatel.functions.processAlarmEvents, 1200, alarms[i], i);
							}
						}
				  break;
				}				
			}
			
			
			

		/**
		 * @ Function to display the remove event checkbox on hover
     *
     */
		IpDatatel.functions.hoverActions = $(document).on('mouseenter', '.view-proccesed-link', function(){
			//make sure we dont update the processing table otherwise the hovering event will be lost
			FortifiedALarms.hoveringActions = true;
			//show the remove event links
			var checkboxElement = $('.processing-remove-event');
			checkboxElement.show();
		});
		
		
		
		/**
		 * @ Function remove hovering actions
     *
     */
		IpDatatel.functions.mouseleaveActions = $(document).on('mouseleave', '.view-proccesed-link', function(){
				//when the users mouse leaves the processing tbale we hide the remove event links
				FortifiedALarms.hoveringActions = false;
				$('.processing-remove-event').hide();
			});
		
		
		/**
		 * @ Function removing events on click
     *
     */
		IpDatatel.functions.removeEvents = $(document).on('click', '.processing-remove-event', function(e){
				e.preventDefault();
				$(this).parents('td').css('background', 'rgb(58, 209, 58)');
				//get the event id that needs to be removed
				var removeId = $(this).attr('data-remove-id');
				//set this events status to removed so when it gets sent to our processor it will remove it.
				IpDatatel.settings.new_events[removeId].status = 'removed';
				//update our new events array
				IpDatatel.functions.updateNewEvents();
				//remove the hovering bool
				FortifiedALarms.hoveringActions = false;
				$('.processing-remove-event').hide();
			});
		
		
			
			
			
		/**
		 * @ Functio for when an alarm comes in
     *
     */
		IpDatatel.functions.processAlarmEvents = function(notification, delta){
			//get the dvr which belongs to this alarm
			IpDatatel.alarmNotifications.dvrElement = $('[data-dvr-id="'+ notification.dvr_nid +'"]');
			//if the dvr iframe is not open
			if( !FortifiedALarms.open ){
				//this alarm has been processed
				IpDatatel.settings.alarms[delta].processed = true;
				//give the dvr which belongs to this alarm classes to notify the user
				$('.close-dvr-object').addClass('open').parent().addClass('active').removeClass('last');
				//toggle open the dvr to start the autologin function and display the dvr
				IpDatatel.alarmNotifications.dvrElement.trigger('click');
				//give the dvr which belongs to this alarm classes to notify the user
				IpDatatel.alarmNotifications.dvrElement.closest('li').addClass('active alarm');
				//so we know we had an alarm. this will false as soon as they click the dvr element
				Drupal.settings.activeAlarm = true;
				//play the alarm audio
				FortifiedALarms.alert = true;
				//if they have alarm audio enebaled
				if(IpDatatel.settings.alarmAudio){
					IpDatatel.functions.alarmAlertAudio();
				}
				//move our processing table to the top of the page out of the way
				$('#alarm-processing-table').addClass('fixed');
				$('#alarm-processing-table').appendTo('body');
				//so they can move the table where they need to
				$('#alarm-processing-table').draggable();
				//the dvr iframe is open
				FortifiedALarms.open = true;
			}else{
			//else if the dvr iframe is open we add the alarm classes to the dvr alarm match so they know there is another alarm 
				var toggleSelector = IpDatatel.alarmNotifications.dvrElement.closest('li');
				$(toggleSelector).addClass('active alarm');
				//if they have alarm audio enebaled
				if(IpDatatel.settings.alarmAudio){
					IpDatatel.functions.alarmAlertAudio();
				}
				//so we know there is another alarm
				Drupal.settings.activeAlarm = true;
				FortifiedALarms.alert = true;
			}
		}
		
		
		
		
		/**
		 * @ Function to check if our alarms arent empty
     *
     */
		IpDatatel.functions.checkEmptyAlarms = function(obj){
			//to check if our alarms array is empty or how many alarms are in it
			var alarmObjectData = [];
			var size = 0, key;
			//get number of alarms in object
	    for (key in obj) {
	    	if (obj.hasOwnProperty(key)) size++;
	    }
	    alarmObjectData['size'] = size;
	    //get the first delta in object that will be used in our for loop
	    for (var first in obj) break;
	    alarmObjectData['key'] = first;
	    return alarmObjectData;
		}
		
		
		
		
		/**
		 * @ Function for querying the customers dvrs for admins and central station
     *
     */
		IpDatatel.functions.ajaxCustomerDvrs = function(account_id){
			//ajax function to return the dvrs for the admin or central station
			$.ajax({
				type: 'POST',
				url: '/ajax/alarm-proccessor-dvr',
				data: {
					account: account_id,
				},
				success: function(data) { 
					$('.dvr-processing-markup-container').html(data.html);
				},
				complete: function(data) {},
				
			});
		}
		
		
		/**
		 * @ Function to stop the alarm audio.
     *
     */
		IpDatatel.functions.stopAlarmAudio = $(document).on('click', '#stop-alarm-audio', function(e){
			e.preventDefault();
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
				var stopAudioElement = '<a href="#" id="stop-alarm-audio">Stop Alarm Audio</a>';
				$('.dvr-processing-list').append(stopAudioElement);
			}
		}
		
		
		
		
		/**
		 * @ Function for closing the dvr object
     *
     */
		IpDatatel.functions.closeDvrObject = $(document).on('click', '.close-dvr-object', function(e){
			e.preventDefault();
			$('.dvr-processing-list li').removeClass('active alarm');
			$('#dvr-cms-iframe').removeClass('open');
			$('#dvr-cms-container').html('');
			$('.close-dvr-object').hide();
			$('#alarm-processing-table').removeClass('fixed').appendTo('.alarm-processing-inner');
			Drupal.settings.activeAlarm = false;
		});
		
		

		
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
		
		
		
		
		
		
		
		/************************************ Settings & Filters Section ************************************/
		/************************************									           ************************************/
		
		
		
		/**
		 * @ function to check to see if the processing filters have been changes
     *
     */
		ProcessingSettings.checkFilters = function(){
			var processingFilters = null;
			if( Drupal.settings.processingFilters != '' ){
				processingFilters = Drupal.settings.processingFilters;
			}
			//return the dynamic filters which will be sent to our processor
			return processingFilters;
		}
		
		
		
		/**
		 * @ function to check to see if the processing settings have changed
     *
     */
		ProcessingSettings.checkAlarmAudio = function(){
			var alarmAudio = true;
			if(Drupal.settings.processingSettings){
				alarmAudio = Drupal.settings.processingSettings.audio;
				switch(alarmAudio){
					case 1:
						alarmAudio = false;
						break;
					case 0:
						alarmAudio = true;
						break;
				}
			}
			
			//return the dynamic filters which will be sent to our processor
			return alarmAudio;
		}
		
		
		
		/**
		 * @ Timed function to let the form load in the modal before we alter the checkboxes
     *
     */
		ProcessingSettings.sleepAlter = function(){
			for (var ids in processingCache.filters.elements){
					var value = processingCache.filters.elements[ids];
					var FilterElement = $(':input[name="'+ids+'"]');
					if(FilterElement.get(0)){
						switch(value){
							case 1:
								FilterElement.attr('checked', 'checked');
								break;
							case 0:
								FilterElement.removeAttr('checked');
								break;
						}
					}
				}
		}
		
		
		
		/**
		 * @ function to return and update values to our processing filters form
     *
     */
		ProcessingSettings.filterDefaults = $('.ctools-use-modal').on('click', function(e){
			//if we have filters when the yopen the form abck up we update the checkbox elements
			if(processingCache.filters.elements){
				window.setTimeout(ProcessingSettings.sleepAlter, 1500);
			}
			//set intreval to listen for updates
			processingCache.filtersInterval = setInterval(function() {
				ProcessingSettings.filterUpdateListener();
			}, 500);
		});
		
		
		
		/**
		 * @ flisten for the processing filters to be updated
     *
     */
		ProcessingSettings.filterUpdateListener = function(){
			if(Drupal.settings.processingCache){
				if(!processingCache.filters['elements']){
				//set the elements object
					processingCache.filters['elements'] = [];
				}
				var identifier = Drupal.settings.processingCache.element.id;
				processingCache.filters['elements'][identifier] = Drupal.settings.processingCache.element.value;
			}
		}
		
		
		
		/**
		 * @ clear our interval when they close the modal form
     *
     */
		 ProcessingSettings.closeModal = $(document).on('mousedown', '.close', function(){
			clearInterval(processingCache.filtersInterval);
		});
		
		
		
		
		
		/************************************									           ************************************/
		/************************************									           ************************************/
		
		
		
		
		
	});
})(jQuery);