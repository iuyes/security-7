(function($) {
	$(document).ready(function() {
		$('#player').hide();
		$('.activate-button').click(function() {
			$('#player').show();
			// set up player without "internal" playlists
			$f("player", "http://releases.flowplayer.org/swf/flowplayer-3.2.16.swf", {}).playlist("div.clips");
		});
		
		$('.downloadVideo').click(function(){
			var downloadLocation = $(this).siblings().attr('value');
			window.location = downloadLocation;
		});
		
		var bPosition = 0;
		$('.img_modal_right').click(function(){
		    bPosition = bPosition + 250;
		  $('#image-modal-ul').animate({
    		left:'-'+bPosition,
  		}, 'fast');
		});
		
		var cPosition = 0;
		$('.img_modal_left').click(function(){
		    cPosition = bPosition - 250;
		  $('#image-modal-ul').animate({
    		left:'-'+cPosition,
  		}, 'fast');
  		bPosition = bPosition - 250;
		});


	});
})(jQuery);