(function($){
	
	$(document).ready(function(){
	
	
		var dvrData = {};
		dvrData.functions = {};
		dvrData.autoLogin = false;
		/**
		 * @ function for revealing and CMS modal and activating the first dvr
		 *
		 */
		dvrData.functions.modalTrigger = $('.dvr-cms-reveal').click(function(){
			dvrData.uid = $(this).attr('data-user-id');
			dvrData.ajaxData = {};
			dvrData.ajaxData.uid = dvrData.uid;
			dvrData.ajaxData.role = $(this).attr('data-cms-role');
			dvrData.ajaxData.type = 'cms';
			if(dvrData.uid){
				$.ajax({
				type: 'POST',
				url: '/ajax/dvr-reveal-modals',
				data: { 
					data: dvrData.ajaxData,
				},
				success: function(data) {
					if(data.html){
						$('body').after(data.html);
						$('#dvr-cms-modal').foundation('reveal', 'open');
						dvrData.functions.openDvr('.dvr-object:first');
					}
				},
				complete: function(data) {}
			});
			}
			
		});
		
		
		
	
	
	/**
  * @ Function for gathering all the dvr info and putting the dvr into an iframe
  *
  */
	dvrData.functions.openDvr = function(selector){
		//for the processing modal
		if( Drupal.settings.processingModal ){
			$('#alarm-processing-table').addClass('fixed').appendTo('body');
			$('#alarm-processing-table').draggable();
			$('.close-dvr-object').addClass('open').parent().addClass('active').removeClass('last');
		}
		if( $(selector).closest('li').hasClass('alarm') ){
			 $(selector).closest('li').removeClass('alarm');
			 Drupal.settings.activeAlarm = false;
		}
		$('#dvr-cms-container').html('');
		dvrData.credentials = {};
		dvrData.firstDvr = $(selector);
		dvrData.credentials.dvrUser = dvrData.firstDvr.attr('data-dvr-user');
		dvrData.credentials.dvrPass = dvrData.firstDvr.attr('data-dvr-password');
		dvrData.dvrUrl = dvrData.firstDvr.attr('data-dvr-url');
		dvrData.credentials.dvrRunFunction = 'OnLogin';
		dvrData.dvrIframe = '<iframe seamless scrolling="yes" width="100%" height="700px" src="http://'+ dvrData.dvrUrl +'" id="dvr-cms-iframe" class="loading-dvr-iframe"></iframe>';
		var loadingIframe = '<div class="loading-dvr-iframe-image"><img src="../../sites/all/themes/fortified_dev/images/loading.gif"></div>';
		$('#dvr-cms-container').html(dvrData.dvrIframe);
		$('#dvr-cms-container').prepend(loadingIframe);
		var trigElement = $(selector).closest('li');
		dvrData.functions.updateTriggerElement(trigElement);
		dvrData.window = document.querySelector("iframe").contentWindow;
		window.setTimeout(dvrData.functions.defaultDvr, 1600);
		dvrData.autoLogin = false;
	}
	
	
	
	
	/**
  * @ function to bring up the dvr login screen if it doesnt have autologin
  *
  */
	dvrData.functions.defaultDvr = function(){
		if(!dvrData.autoLogin){
			dvrData.functions.loadiframeImage();
		}else{
			dvrData.autoLogin = false;
		}
	}
	
	/**
  * @ Whiel the dvr iframe is loading
  *
  */
	dvrData.functions.loadiframeImage = function(){
		$('.loading-dvr-iframe-image').remove();
		$('#dvr-cms-iframe').removeClass('loading-dvr-iframe').addClass('iframe-loaded');
	}
	
	
	
	
	/**
  * @ Update the trigger element class
  *
  */
	dvrData.functions.updateTriggerElement = function(element){
		$('.dvr-cms-list li').removeClass('active');
		$('.dvr-processing-list li').removeClass('active');
		$(element).addClass('active');
	}
	
 /**
  * @ gets the element containing all the data to change the dvr
  *
  */
	dvrData.functions.changeDvr = $(document).on('click', '.dvr-object', function(){
		dvrData.functions.openDvr(this);
	});
	
	
 /**
  * @ This is the actual function that posts the message to the dvr server requesting it to login
  *
  */
	dvrData.functions.postMessage = function(){
		dvrData.window.postMessage(dvrData.credentials, dvrData.dvrUrl);
		$('#dvr-cms-iframe').addClass('open');
	}
	
	
	
	/**
  * @ Closes the cms modal and removes the iframe / Webclient Plugin
  *
  */
	dvrData.functions.closeCmsModal = $(document).on('click', '.close-cms-modal', function(){
		$('#dvr-cms-modal').foundation('reveal', 'close');
		$('#dvr-cms-iframe').remove();
		$('#dvr-cms-container').html('');
		$('#dvr-cms-modal').remove();
	});
	
	
	
 /**
  * @ adds an event listener from the DVR Camera server
  *
  */
	dvrData.functions.EventListener = window.addEventListener("message", function(e) {
		// wait for child to signal that it's loaded.
		if(dvrData.dvrUrl){
			dvrData.dvrUrl = 'http://'+dvrData.dvrUrl;
		}
		
		window.setTimeout(dvrData.functions.loadiframeImage, 500);
		if ( e.data === "loaded" && e.origin == dvrData.dvrUrl && dvrData.credentials) {
			dvrData.autoLogin = true;
			// send the child a message.
		  window.setTimeout(dvrData.functions.postMessage, 1600);
		}
	});
	
	
	});
	
	
})(jQuery);