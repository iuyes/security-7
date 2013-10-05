(function($) {
	$(document).ready(function() {
		$('#player').hide();
		$('.activate-button').click(function() {
			$('#player').show();
			// set up player without "internal" playlists
			$f("player", "http://releases.flowplayer.org/swf/flowplayer-3.2.16.swf", {}).playlist("div.clips");
		});
		$('.downloadVideo').click(function() {
			var downloadLocation = $(this).siblings().attr('value');
			window.location = downloadLocation;
		});
		
		$('.enlarge').click(function() {
			var image_widths = 0;
			var num_of_images = $('#image-modal-ul li img').each(function() {
				var img_width = parseInt($(this).css('width'), 10);
				image_widths = (image_widths + img_width) * 1;
			});
			image_widths = image_widths - 480;
		// for enlarge images modal
		var bPosition = 0;
		$('.img_modal_right').click(function() {
			bPosition = bPosition + 250;
			if (image_widths <= bPosition) {
				bPosition = bPosition - 250;
			}
				$('#image-modal-ul').animate({
					left: '-' + bPosition,
				}, 'fast');
		});
		var cPosition = 0;
		$('.img_modal_left').click(function() {
			cPosition = bPosition - 250;
			$('#image-modal-ul').animate({
				left: '-' + cPosition,
			}, 'fast');
			bPosition = bPosition - 250;
		});
		// end for enlarge images modal
		});
	});
})(jQuery);