/**
 * @ file fr_groups_fieldsets.js implements custom fieldset style accordions
 *
 */
(function($) {
	$(document).ready(function() {
		var customFieldsets = {};
		customFieldsets.functions = {};
		/**
		 * @ Function Event handler for starting the accordion process
		 *
		 */
		customFieldsets.functions.activateAccordion = $('.fs-accordion-fieldset-title').click(function() {
			if ($(this).parent().hasClass('collapsible')) {
				customFieldsets.accordionElement = $(this).next('.fs-accordion-fieldset-inner');
				if (customFieldsets.accordionElement && $(customFieldsets.accordionElement).hasClass('closed')) {
					customFieldsets.functions.activateAccordionOpen(customFieldsets.accordionElement);
					$(this).addClass('active open').removeClass('closed');
				} else if (customFieldsets.accordionElement && $(customFieldsets.accordionElement).hasClass('open')) {
					customFieldsets.functions.activateAccordionClosed(customFieldsets.accordionElement);
					customFieldsets.functions.removeActive($(this));
					customFieldsets.functions.closeChildren(customFieldsets.accordionElement);
				}
			}
		});
		
		
		
		/**
		 * @ function for removing the active and open classes from the accordion titls(s)
		 *
		 */
		customFieldsets.functions.removeActive = function(element) {
			$(element).removeClass('active open').addClass('closed');
		}
		/**
		 * @ function for closing all children accordions when thier parent title is clicked
		 *
		 */
		customFieldsets.functions.closeChildren = function(element) {
			var children = element.find('.fs-accordion-fieldset.collapsible').each(function() {
				if ($(this).find('.fs-accordion-fieldset-title').hasClass('open')) {
					var accordionTitle = $(this).find('.fs-accordion-fieldset-title');
					customFieldsets.functions.removeActive(accordionTitle);
					var accordionChild = $(this).find('.fs-accordion-fieldset-inner');
					customFieldsets.functions.activateAccordionClosed(accordionChild);
				}
			});
		}
		/**
		 * @ function for openenign the accordion
		 *
		 */
		customFieldsets.functions.activateAccordionOpen = function(element) {
			$(element).slideDown('fast');
			$(element).addClass('open');
			$(element).removeClass('closed');
		}
		/**
		 * @ function for closing the accordion
		 *
		 */
		customFieldsets.functions.activateAccordionClosed = function(element) {
			$(element).slideUp('fast');
			$(element).addClass('closed');
		}
	});
})(jQuery);