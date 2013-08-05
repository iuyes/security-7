(function ($) {
	$(document).ready(function(){
	$('.tab_highlight').live('mouseover', function(){
		$(this).css('border-bottom', '1px dotted black')
			.css('cursor', 'pointer')
			.css("background", "url('http://fort.dev/sites/all/themes/zurb-foundation/images/arrow_green.jpg') no-repeat right transparent");
		$(this).click(function(){
			$(this).css('border-bottom', '1px dotted black')
				.css('cursor', 'pointer');
			$(this).addClass('active');
			$(this).next('.desc_wrapper').slideDown('slow')
				.siblings('.desc_wrapper').slideUp('slow');
			$(this).siblings('.tab_highlight').removeClass('active');
			return false;
		});
	});
	$('.tab_highlight').mouseout(function(){
		$(this).css('border-bottom', '1px dotted #eee')
			.css("background", "url('http://fort.dev/sites/all/themes/zurb-foundation/images/arrow.jpg') no-repeat right transparent");
		});
	});
})(jQuery);