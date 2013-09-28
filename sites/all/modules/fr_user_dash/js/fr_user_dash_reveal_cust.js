(function ($) {
 $(document).ready(function(){
 
 
 
 /******reveal the dvr modal*****/
 	var adminDVR = {};
 	var adminPostMessage = {};
 	
 	
 	
 $(document).on('click', '.reveal-cust-dvr', function(e){
 		adminDVR.dvrCredentials = {};
 			e.preventDefault();
 			adminDVR.dataElement = $(this);
			adminDVR.dvrUrl = adminDVR.dataElement.attr('data-dvr-url');
			//the dvr login username
			adminDVR.dvrCredentials.dvrUser = adminDVR.dataElement.attr('data-dvr-user');
			//the dvr login password
			adminDVR.dvrCredentials.dvrPass = adminDVR.dataElement.attr('data-dvr-pass');
			//the function to run in postMessage
			adminDVR.dvrCredentials.dvrRunFunction = adminDVR.dataElement.attr('data-dvr-function');
	 	if(adminDVR.dvrUrl != ''){
	 		var DVRMODAL = '<div id="customerDvrModal" class="reveal-modal xlarge"><div class="close-dvr-modal close-modal">&#215;</div><div class="customerDvrModal"></div></div>';
	 		var custDvrIframe = '<iframe id="customerDvrIframe" style="border:2px solid white;" seamless width="100%" height="700px" src="'+adminDVR.dvrUrl+'"></iframe>';
	 		$('body').after(DVRMODAL);
	 		adminDVR.dvrIframeElementSrc = adminDVR.dvrUrl;
	 		$('#customerDvrModal').foundation('reveal', 'open');
	 		$('.customerDvrModal').html(custDvrIframe);
			adminDVR.iframeContentWindow = document.querySelector("iframe").contentWindow;
			//setting the DvrPostmessage object values
			adminPostMessage.setDefaults(adminDVR);
		}
	});
	
	
	
	
 /**
  * @ function for setting the adminPostmessage object values
  *
  */
	adminPostMessage.setDefaults = function(adminDVR){
		console.log(adminDVR);
		adminPostMessage.dvrAddr = adminDVR.dvrUrl;
		adminPostMessage.dvrIframeElementSrc = adminDVR.dvrIframeElementSrc;
		adminPostMessage.window = adminDVR.iframeContentWindow;
		adminPostMessage.messageData = adminDVR.dvrCredentials;
	}
	
	
	
	
	/**
  * @ This is the actual function that posts the message to the dvr server requesting it to login
  *
  */
	adminPostMessage.postLoginMessage = function(){
		adminPostMessage.window.postMessage(adminPostMessage.messageData, adminPostMessage.dvrIframeElementSrc);
	}
	
	
	/**
  * @ adds an event listener from the DVR Camera server
  *
  */
	adminPostMessage.EventListener = window.addEventListener("message", function(e) {
		// wait for child to signal that it's loaded.
		if ( e.data === "loaded" && e.origin == adminPostMessage.dvrAddr && adminPostMessage.messageData) {
			// send the child a message.
			window.setTimeout(adminPostMessage.postLoginMessage, 1500);
		  
		}
	});
	
	
	$(document).on('click', '.close-dvr-modal', function(){
		$('#customerDvrModal').foundation('reveal', 'close');
		$('.customerDvrModal').html('');
		$('#customerDvrModal').remove();
	});

 });
})(jQuery);