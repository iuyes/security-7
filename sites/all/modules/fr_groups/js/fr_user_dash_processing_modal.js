(function($){

	$(document).ready(function(){
	
		var processingHtml = {};
		processingHtml.functions = {};
		
		/**
		 * @ Function for revealing the Processing modal
     *
     */
		processingHtml.functions.revealProcessingModal = $(document).on('click', '.alarm-activate-modal', function(e){
			e.preventDefault();
			$('#processingModal').foundation('reveal', 'open');
			processingHtml.table = $('.alarm-processing-inner').html();
			$('#alarm-processing-table').remove();
			processingHtml.processingId = $('#processingModal').attr('data-processing-id');
			if(processingHtml.processingId){
				 processingHtml.functions.queryProcessingDvrs(processingHtml.processingId);
			}
			window.setTimeout(processingHtml.functions.insertProcessingText, 1500);
		});
		
		
		/**
		 * @ function for putting the processing table into and dvrs into the modal
     *
     */
		processingHtml.functions.insertProcessingText = function(){
			processingHtml.dvrHtml = processingHtml.dvrHtml + processingHtml.table;
			$('.processing-modal').html(processingHtml.dvrHtml);
			$('.alarm-activate-modal').hide();
		}
		
		
		/**
		 * @ function for closing the processing modal and putting the html back into its origional place
     *
     */
		processingHtml.functions.closeProcessingModal = $(document).on('click', '.modal-open, .close-processing-modal', function(){
			$('#processingModal').foundation('reveal', 'close');
			$('#dvr-cms-iframe').remove();
			$('.dvr-cms-wrapper').remove();
			$('#dvr-cms-container').html('');
			processingHtml.functions.ChangeProcessingElements();
		});
		
		
		/**
		 * @ function for putting the html back into its origional place
     *
     */
		processingHtml.functions.ChangeProcessingElements = function(){
			$('.alarm-processing-inner').html(processingHtml.table);
		}
		
		
		
		/**
		 * @ function for accordion previous sessions
     *
     */
		processingHtml.functions.ExpandPrevSessios = $(document).on('click', '.trigger-expand-session', function(){
			$('.expand-event-sessions').slideUp('slow');
			$('.session-wrapper').removeClass('active');
			$(this).removeClass('admin-dash-label');
			$(this).addClass('admin-dash-label');
			$(this).parent().addClass('active');
			$(this).parent().find('.expand-event-sessions').slideDown('slow').addClass('admin-dash-content');
		});
		
		
		/**
		 * @ function for getting all the dvrs related to this account
     *
     */
		processingHtml.functions.queryProcessingDvrs = function(processing_id){
			$.ajax({
				beforeSend : function(){
					$('.processing-modal').append('<div class="tooltip-throbber"</div>');
				},
				type: 'POST',
				url: '/ajax/dvr-reveal-modals',
				data: { 
					uid: processing_id,
					argument: 'processing',
				},
				success: function(data) {
					if(data.html){
						processingHtml.dvrHtml = data.html;
					}
				},
				complete: function(data) {}
			});
			
		}
		

	});
})(jQuery);