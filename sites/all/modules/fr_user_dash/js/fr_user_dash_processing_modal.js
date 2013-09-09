(function($){

	$(document).ready(function(){
	
		var processingHtml = {};
		$(document).on('click', '.alarm-activate-modal', function(e){
			e.preventDefault();
			$('#processingModal').foundation('reveal', 'open');
			processingHtml.table = $('#homebox-block-fr_user_dash_alarm_processing').html();
			$('#alarm-processing-table').remove();
			$('.processing-modal').html(processingHtml.table);
			$('.alarm-activate-modal').hide();
		});
		
		$(document).on('click', '.modal-open, .close-processing-modal', function(){
			processingHtml.ChangeProcessingElements();
		});
		
		processingHtml.ChangeProcessingElements = function(){
			$('#homebox-block-fr_user_dash_alarm_processing').html(processingHtml.table);
		}
		
	});
	
})(jQuery);