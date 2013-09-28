(function($){
	
	$(document).ready(function(){
	
	
		var dvrData = {};
		dvrData.functions = {};
		
		/**
		 * @ function for revealing and CMS modal and activating the first dvr
		 *
		 */
		dvrData.functions.modalTrigger = $('.dvr-cms-reveal').click(function(){
			dvrData.uid = $(this).attr('data-user-id');
			if(dvrData.uid){
				$.ajax({
				type: 'POST',
				url: '/ajax/dvr-reveal-modals',
				data: { 
					uid: dvrData.uid,
					argument: 'cms',
				},
				success: function(data, text, xhqr) {
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
		$('#dvr-cms-container').html('');
		dvrData.credentials = {};
		dvrData.firstDvr = $(selector);
		dvrData.credentials.dvrUser = dvrData.firstDvr.attr('data-dvr-user');
		dvrData.credentials.dvrPass = dvrData.firstDvr.attr('data-dvr-password');
		dvrData.dvrUrl = dvrData.firstDvr.attr('data-dvr-url');
		dvrData.credentials.dvrRunFunction = 'OnLogin';
		dvrData.dvrIframe = '<iframe seamless width="100%" height="700px" src="http://'+ dvrData.dvrUrl +'" id="dvr-cms-iframe"></iframe>';
		$('#dvr-cms-container').html(dvrData.dvrIframe);
		var trigElement = $(selector).closest('li');
		dvrData.functions.updateTriggerElement(trigElement);
		dvrData.window = document.querySelector("iframe").contentWindow;
	}
	
	
	/**
  * @ Update the trigger element class
  *
  */
	dvrData.functions.updateTriggerElement = function(element){
		$('.dvr-cms-list li').removeClass('active');
		$(element).addClass('active');
	}
	
 /**
  * @ gets the element containing all the data to change the dvr
  *
  */
	dvrData.functions.changeDvr = $(document).on('click', '.dvr-object', function(){
		//$('#alarm-processing-table').addClass('fixed');
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
		if ( e.data === "loaded" && e.origin == dvrData.dvrUrl && dvrData.credentials) {
			// send the child a message.
		  window.setTimeout(dvrData.functions.postMessage, 1500);
		}
	});
	
	
	});
	
	
})(jQuery);