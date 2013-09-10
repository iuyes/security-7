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
		
		processingHtml.ExpandPrevSessios = $(document).on('click', '.trigger-expand-session', function(){
			$('.expand-event-sessions').slideUp('slow');
			$('.session-wrapper').removeClass('active');
			$(this).removeClass('admin-dash-label');
			$(this).addClass('admin-dash-label');
			$(this).parent().addClass('active');
			$(this).parent().find('.expand-event-sessions').slideDown('slow').addClass('admin-dash-content');
		});
		
	});
})(jQuery);