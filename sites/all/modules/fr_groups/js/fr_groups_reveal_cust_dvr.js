/**
 * @ file user for revealing and uto logging in DVR Camera systems
 *
 */
(function ($) {
 $(document).ready(function(){
 	//activate reveal modal function
 	var DvrPostmessage = {};
 	//the main DVR Object
	var DvrObj = {};
	DvrObj.activateDvrModal = $('.dvr-modal').click(function(e){
		e.preventDefault();
		//remove any previous dvr otherwise webclient plugin will crash
		$('.cust-dvr-modal').html('');
		// the dvr element identifier
		DvrObj.dvrId = $(this).attr('data-dvr-nid');
		//Element containg all the dvr data
		DvrObj.dataElement =  $('#myDvrModal_'+DvrObj.dvrId+'');
		//if the element exists
		if(DvrObj.dataElement){
			//dvr credentials object that will be sent in postmessage
			DvrObj.dvrCredentials = {};
			//the dvrs remote addr
			DvrObj.dvrUrl = DvrObj.dataElement.attr('data-dvr-url');
			//the dvr login username
			DvrObj.dvrCredentials.dvrUser = DvrObj.dataElement.attr('data-dvr-user');
			//the dvr login password
			DvrObj.dvrCredentials.dvrPass = DvrObj.dataElement.attr('data-dvr-pass');
			//the function to run in postMessage
			DvrObj.dvrCredentials.dvrRunFunction = DvrObj.dataElement.attr('data-dvr-function');
		}
		//if the dvr remote url is not empty
		if(DvrObj.dvrUrl != ''){
			DvrObj.dvrUrl = 'http://'+ DvrObj.dvrUrl + '';
			// iframe string
			DvrObj.dvrIframe = '<iframe seamless width="100%" height="700px" id="dvrIframe_'+ DvrObj.dvrId +'" src="'+ DvrObj.dvrUrl +'"></iframe>';
			//reveal the modal
			DvrObj.dataElement.foundation('reveal', 'open');
			// Insert the iframe into the modal
			$('.myDvrModal_'+DvrObj.dvrId+'').html(DvrObj.dvrIframe);
			//setting the DvrPostmessage object values
			DvrObj.dvrIframeElementSrc = $('#dvrIframe_'+ DvrObj.dvrId +'').attr('src');
			DvrObj.iframeContentWindow = document.querySelector("iframe").contentWindow;
			//setting the DvrPostmessage object values
			DvrPostmessage.setDefaults(DvrObj);
		}
		
	});
	

	DvrObj.closeDvrModal = $(document).on('click', '.close-dvr-modal', function(){	
		DvrObj.dataElement.foundation('reveal', 'close');
		$('#dvrIframe_' + DvrObj.dvrId + '').remove();
		$('.cust-dvr-modal').html('');
	});

	
 /**
  * @ function for setting the DvrPostmessage object values
  *
  */
	DvrPostmessage.setDefaults = function(DvrObj){
		DvrPostmessage.dvrAddr = DvrObj.dvrUrl;
		DvrPostmessage.dvrIframeElementSrc = DvrObj.dvrIframeElementSrc;
		DvrPostmessage.window = DvrObj.iframeContentWindow;
		DvrPostmessage.messageData = DvrObj.dvrCredentials;
	}
	
	
	/**
  * @ This is the actual function that posts the message to the dvr server requesting it to login
  *
  */
	DvrPostmessage.postLoginMessage = function(){
		DvrPostmessage.window.postMessage(DvrPostmessage.messageData, DvrPostmessage.dvrIframeElementSrc);
	}
	
	
	/**
  * @ adds an event listener from the DVR Camera server
  *
  */
	DvrPostmessage.EventListener = window.addEventListener("message", function(e) {
		// wait for child to signal that it's loaded.
		if ( e.data === "loaded" && e.origin == DvrPostmessage.dvrAddr && DvrPostmessage.messageData) {
			// send the child a message.
			window.setTimeout(DvrPostmessage.postLoginMessage, 1500);
		  
		}
	});
	
	
	
 });
})(jQuery);