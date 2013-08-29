(function($){
	$(document).ready(funciton(){
		viewEvent = $('.processing-link-view');
									viewEvent.click(function(){
							console.log('clicked');
							$('#processingModal').foundation('reveal', 'open');
						});
	});
	
}(jQuery);