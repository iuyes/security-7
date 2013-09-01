(function ($) {

	$(document).ready(function(){
		var tooltip = new Object();
		tooltip.ajax = new Object();
		tooltip.open = false;
		tooltip.MouseLeftTooltip = false;
		var checkToCloseTooltip;
		/****** ouseoff element function for tooltip *****/
	 	$(document).on('mouseenter', '.f-tooltip', function(){
	 		tooltip.MouseOnTooltip = false;
		 	if( $(this).hasClass('reveal-customer-dvr') ){
		 		if(tooltip.open == false){
				 	tooltip.ajax.data = $(this).attr('data-query-data');
				 	 tooltip.ajax.method = $(this).attr('data-method');
				 	 tooltip.ajax.callback = $(this).attr('data-callback');
				 	 showFTooltip($(this));
				 	 ajaxTooltipHtml($(this));
				}
		 	}
		 	$(document).off('.f-tooltip').on('mouseleave', '.f-tooltip', function(){
		 		var openTooltipWrapper = $(this).parent().find('.f-tooltip-wrapper');
		 		tooltip.MouseLeftTooltip = true;
		 		checkToCloseTooltip = setTimeout(function(){ mouseLeftTooltip(openTooltipWrapper) }, 2000);
		 	});
	 	});
	 	
	 	
	 	$(document).on('hover mouseleave', ".f-tooltip-open",function(e){
	 		switch(e.type){
		 		case 'mouseenter':
		 			tooltip.MouseOnTooltip = true;
		 			break;
		 		case'mouseleave':
		 			tooltip.MouseOnTooltip = false;
		 			var openedTooltipWrapper = $(this).parent();
		 			var checkToCloseTooltipk = setTimeout(function(){ mouseOnTooltip( openedTooltipWrapper ) }, 2000);
		 			break;
	 		}
	 	});
	 	
	 	// TIMERS
	 	function mouseLeftTooltip(openTooltipWrapper){
		 	if(!tooltip.MouseOnTooltip){
				openTooltipWrapper.remove();
				updateTooltipStatus();
			}
	 	}
	 	
	 	function mouseOnTooltip(openToolTip){
		 	if(tooltip.MouseOnTooltip == false){
			 	openToolTip.remove();
			 	updateTooltipStatus();
		 	}
	 	}
	 	// END TIMERS
	 	
	 	
	 	
	 	function updateTooltipStatus(){
		 	tooltip.open = false;
	 	}

	 	
	 	function showFTooltip(element){
		  tooltip.offset = element.offset();
		 	var topOffset = tooltip.offset.top;
		 		if(topOffset > 50){
			 		topOffset = topOffset - 300;
		 		}
		 		topOffset = topOffset - 300;
		 	var elementWidth = element.css('width');
		 	tooltip.Style = 'top:2px; left:5px;';
		 	tooltip.Element = '<div class="f-tooltip-wrapper">';
		 	tooltip.Element += '<div class="f-tooltip-open" style="'+tooltip.Style+'">Loading Customer Information...</div>';
		 	tooltip.Element += '</div>';
		 	element.parent().append(tooltip.Element);
		 	tooltip.open = true;
	 	}
	 	
	 	
	 	function ajaxTooltipHtml(tooltipElement){
	 		var openTooltip = tooltipElement.parent().find('.f-tooltip-open');
				$.ajax({
						type: tooltip.ajax.method,
						url: tooltip.ajax.callback,
						data: {
							variables: tooltip.ajax.data,
						},
						success: function(data) {
							openTooltip.html(data.html);
						},
						complete: function(data) {
						}
					});
				
			}
	 	
 	});
})(jQuery);