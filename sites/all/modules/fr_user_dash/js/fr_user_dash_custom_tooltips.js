(function($) {
	$(document).ready(function() {
	
		
		
		/**
		* @ Global Declerations
		*
		*/
		var tooltip = {};
		tooltip.ajax = {};
		tooltip.open = false;
		tooltip.MouseLeftTooltip = false;
		tooltip.triggeredEvent = false;
		var checkToCloseTooltip;		
		var tooltipFunctions = function(){}
		
		
		
		
		
		/**
		* @ Param element class .f-tooltip-open event mouseenter
		*
		*/
		tooltipFunctions.activateTooltip = $(document).on('mouseenter', '.f-tooltip', function() {
			tooltip.MouseOnTooltip = false;
			if (tooltip.open == false) {
				tooltip.MouseLeftTooltip = false;
				tooltip.ajax = {
					data: $(this).attr('data-query-data'),
					event: $(this).attr('data-event'),
					method: $(this).attr('data-method'),
					callback: $(this).attr('data-callback'),
					tooltipSize: $(this).attr('data-tooltip-size'),
					xPosition: $(this).attr('data-tooltip-xposition'),
					yPosition: $(this).attr('data-tooltip-yposition'),
					useAjax: $(this).attr('data-ajax'),
					arguments: $(this).attr('data-arguments'),
					drupalSettings: $(this).attr('data-drupal-settings'),
					drupalSetting: $(this).attr('data-drupal-setting'),
					tooltipDialog: $(this),
				};
				if(!tooltip.ajax.event){
					tooltip.ajax.event = 'mouseenter';
				}
				if(tooltip.ajax.drupalSettings){
					tooltip.ajax.drupalData = Drupal.settings[tooltip.ajax.drupalSetting];
					tooltip.ajax.drupalData = tooltip.ajax.drupalData[tooltip.ajax.arguments];
				}
				
				if(tooltip.ajax.event == 'mouseenter'){
					tooltipFunctions.activateTooltipElement(tooltip.ajax.tooltipDialog);
				}else{
					tooltipFunctions.triggeringEvent();
				}
			}
			//listener for close button
			tooltipFunctions.closeTooltipButton();
			tooltipFunctions.hoverTooltip();
		});
		
		
		
		tooltipFunctions.closeTooltipButton = function(){
			$(document).on('click', '.close-tooltip', function(){
				tooltip.ajax.tooltipDialog.parent().find('.f-tooltip-wrapper').remove();
				tooltipFunctions.updateTooltipStatus();
			});
		}
		
		
		//used to close the tooltip
			tooltipFunctions.hoverTooltip = function(){
				$(document).off('.f-tooltip').on('mouseleave', '.f-tooltip', function(){
					var openTooltipWrapper = $(this).parent().find('.f-tooltip-wrapper');
					tooltip.MouseLeftTooltip = true;
					checkToCloseTooltip = setTimeout(function() {
						tooltipFunctions.mouseLeftTooltip(openTooltipWrapper)
					}, 1500);
				});
			}
		
		
		
		/**
		* @ Set listener for disired tooltip activate event set in the triggering elements data-event
		*
		*/
		tooltipFunctions.triggeringEvent = function(){
			if(!tooltip.open && !tooltip.triggeredEvent){
				//$(document).off('.f-tooltip');
				$(document).on(tooltip.ajax.event, '.f-tooltip', function(e){
					e.preventDefault();
					if(e.type == tooltip.ajax.event){
						tooltip.triggeredEvent = true;
						Drupal.settings.hoveringEvent = true;
						tooltipFunctions.activateTooltipElement($(this));
					}
				});
			}
	}
		
		
		
		/**
		* @ start the tooltip animation
		*
		*/
		tooltipFunctions.activateTooltipElement = function(triggerElement){
			// get html if requested
				//show the tooltip
				tooltipFunctions.showFTooltip(triggerElement);
				if (tooltip.ajax.useAjax) {
					tooltipFunctions.ajaxTooltipHtml(triggerElement);
				}
		}
		
		
		
		
		/**
		* @ Param element class .f-tooltip-open event mouseleave
		*
		*/
		tooltipFunctions.mouseOnTooltipDialog = $(document).on('hover mouseleave', ".f-tooltip-open", function(e) {
			switch (e.type) {
			case 'mouseenter':
				Drupal.settings.hoveringEvent = true;
				tooltip.MouseOnTooltip = true;
				break;
			case 'mouseleave':
				Drupal.settings.hoveringEvent = false;
				tooltip.MouseOnTooltip = false;
				var openedTooltipWrapper = $(this).parent();
				var checkToCloseTooltipk = setTimeout(function() {
					tooltipFunctions.mouseOnTooltip(openedTooltipWrapper)
				}, 1500);
				break;
			}
		});
		
		
		
		
		/**
		* @ Param openTooltipWrapper
		* @ Timers for closing tooltip
		*
		*/
		tooltipFunctions.mouseLeftTooltip = function(openTooltipWrapper) {
			if (!tooltip.MouseOnTooltip) {
				openTooltipWrapper.remove();
				tooltipFunctions.updateTooltipStatus();
			}
		}
		
		
		
		/**
		* @ Param openToolTip
		*
		*/
		tooltipFunctions.mouseOnTooltip = function(openToolTip) {
			if (tooltip.MouseOnTooltip == false) {
				openToolTip.remove();
				tooltipFunctions.updateTooltipStatus();
			}
		}
		
		
		
		
		
		/**
		* set tooltip status to closed
		*
		*/
		tooltipFunctions.updateTooltipStatus = function() {
			tooltip.open = false;
			tooltip.MouseOnTooltip = false;
			tooltip.MouseLeftTooltip = true;
			Drupal.settings.hoveringEvent = false;
			tooltip.triggeredEvent = false;
		}
		
		
		
		
		/**
		* @ Param element
		*
		*/
		tooltipFunctions.showFTooltip = function(element) {
			//clode tooltip element
			tooltip.closeTooltipElement = '<div class="close-tooltip">&#215;</div>';
			// tooltip element
			tooltip.Element = '<div class="f-tooltip-wrapper">';
			tooltip.Element += '<div class="f-tooltip-open '+tooltip.ajax.tooltipSize+'">'+tooltip.closeTooltipElement+'';
			tooltip.Element += '<div style="display:inline;">Loading...</div><div class="tooltip-throbber"></div>';
			tooltip.Element += '</div>';
			tooltip.Element += '</div>';
			//trigger element width
			tooltip.triggerElement = {
				width : parseInt(element.css('width')),
			};
			
			
			
			//append the tooltip to the document
			if( !element.parent().find('.f-tooltip-wrapper').get(0) ){
				element.parent().append(tooltip.Element);
				//get the trigger element hight
				tooltip.elementHeight = parseInt(element.css('height'));
				tooltipFunctions.toolTipStyles();
				tooltip.open = true;	
			}
		}
		
		
		
		/**
		* @ function apply style attributes to the tooltip
		*
		*/
		tooltipFunctions.toolTipStyles = function(){
			if(!tooltip.ajax.yPosition){
				tooltip.ajax.yPosition = 'bottom';
			}
			//default tooltip top style
			tooltip.Style = {
				top : '2px;',
			};
			//get the tooltips height
			tooltip.toolTipHeight = parseInt($('.f-tooltip-open').css('height'));
		
			//set default ( bottom )tooltip styles
			tooltip.Style = {
				top : '2px;',
			};
			//tooltip width
			tooltip.width = $('.f-tooltip-open').css('width');
			//set the y-axis styles
			switch(tooltip.ajax.yPosition){
				case 'bottom':
					//tooltip.Style.top = (tooltip.toolTipHeight + (tooltip.elementHeight * 3)) * -1;
					$('.f-tooltip-open').css({'top' : tooltip.Style.top}).addClass('tooltip-before');
					break;
				case 'top':
					tooltip.Style.top = ((tooltip.toolTipHeight + 10) + (tooltip.elementHeight * 3)) * -1;
					$('.f-tooltip-open').css({'top' : tooltip.Style.top}).addClass('tooltip-after');
					switch(tooltip.ajax.xPosition){
						case 'center':
							//center the tooltip
							var centerTOoltip = ( parseInt(tooltip.width) / -2 ) + ( tooltip.triggerElement.width / 2);
							$('.f-tooltip-open').css('left', centerTOoltip);
							//$('.f-tooltip-open:after').css('left', ( centerTOoltip * 1 ));
							break;
					}
					break;
			}
		}
		
		
		
		
		
		/**
		* @ Param openTooltip
		*
		*/
		tooltipFunctions.ajaxTooltipHtml = function() {
			var ajaxData = {};
			// get all ajax settings and data
				if(tooltip.ajax.data){
					ajaxData.data = tooltip.ajax.data;
				}
				if(tooltip.ajax.arguments){
					ajaxData.arguments = tooltip.ajax.arguments;
				}
				if(tooltip.ajax.drupalData){
					ajaxData.drupalData = tooltip.ajax.drupalData;	
				}
				if(!tooltip.ajax.method){
					tooltip.ajax.method = 'POST';
				}
			var openTooltip = tooltip.ajax.tooltipDialog.parent().find('.f-tooltip-open');
			$.ajax({
				type: tooltip.ajax.method,
				url: tooltip.ajax.callback,
				data: { 
					arguments: ajaxData,
				},
				success: function(data) {
				//replace the tooltip html
					var closeTooltip = '<div class="close-tooltip">&#215;</div>';
					closeTooltip += data.html;
					openTooltip.html(closeTooltip);
					//openTooltip.p(tooltip.closeTooltipElement);
					tooltipFunctions.toolTipStyles();
					tooltip.open = true;
				},
				complete: function(data) {}
			});
		}
		
		
		
	});
})(jQuery);