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
			Drupal.settings.processingModal = true;
			$('#processingModal').foundation('reveal', 'open');
			processingHtml.table = $('.alarm-processing-inner');
			processingHtml.processingId = $('#processingModal').attr('data-processing-id');
			processingHtml.processingRole = $('#processingModal').attr('data-processing-role');
			if(processingHtml.processingId && processingHtml.processingRole){
				 processingHtml.functions.queryProcessingDvrs(processingHtml.processingId, processingHtml.processingRole);
			}
		});
		
		
		/**
		 * @ function for putting the processing table into and dvrs into the modal
     *
     */
		processingHtml.functions.insertProcessingText = function(){
			var $newElement = $("<div>", {id: "dvr-info"});
			//$('.ctools-use-modal').trigger('click');
			$newElement.html( $(processingHtml.dvrHtml) );
			$newElement.appendTo('.processing-modal');
			processingHtml.table.appendTo('.processing-modal');
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
			$('#alarm-processing-table').removeClass('fixed');
			$('#alarm-processing-table').appendTo('.alarm-processing-inner');
			processingHtml.functions.ChangeProcessingElements();
		});
		
		
		/**
		 * @ function for putting the html back into its origional place
     *
     */
		processingHtml.functions.ChangeProcessingElements = function(){
			processingHtml.table.appendTo('#alarm-processing-inner');
			$('.alarm-activate-modal').show();
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
		processingHtml.functions.queryProcessingDvrs = function(processing_id, processing_role){
			processingHtml.ajaxData = {};
			processingHtml.ajaxData.uid = processing_id;
			processingHtml.ajaxData.role = processing_role;
			processingHtml.ajaxData.type = 'processing';
			$.ajax({
				beforeSend : function(){
					$('.processing-modal').append( $('<div class="tooltip-throbber"></div>') );
				},
				type: 'POST',
				url: '/ajax/dvr-reveal-modals',
				data: { 
					data:processingHtml.ajaxData,
				},
				success: function(data) {
					$('.tooltip-throbber').remove();
					if(data.html){
						processingHtml.dvrHtml = data.html;
						//window.setTimeout(processingHtml.functions.insertProcessingText, 1800);
						processingHtml.functions.insertProcessingText();
					}
				},
				complete: function(data) {}
			});
			
		}
		

	});
})(jQuery);