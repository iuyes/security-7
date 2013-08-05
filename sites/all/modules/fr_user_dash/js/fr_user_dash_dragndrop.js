(function($) {
	$(document).ready(function() {
		$('.save-file').draggable({
			cursor: "move",
			revert: function(is_valid_drop) {
				if (!is_valid_drop) {
					$('.save-files').hide();
				}
			},
			cursorAt: {
				top: -5,
				left: -10
			},
			helper: function(event) {
				$('.save-files').show();
				$('.save-files').html('<div class="save">Save</div>');
				$('.save-files').removeClass('dropped');
				return $("<div class='ui-widget-header'>Save</div>");
			}
		});
		// $( "#set div" ).draggable({ stack: "#set div" });
		$('.save-files').droppable({
			accept: ".save-file",
			activeClass: "ui-state-hover",
			hoverClass: "ui-state-active",
			drop: function(event, ui) {
				var element = ui.draggable.closest('.views-cust-data');
				$('.save-files').addClass('dropped');
				var nid = ui.draggable.attr('id');
				$.ajax({
					type: 'POST',
					url: '/fr_save_files',
					dataType: 'json',
					data: {
						files: nid
					},
					success: function(data) {
						if (data.status == 1) {
							$('.save-files').html("<div class='save'>Saved!</div>");
							$('.save-files').fadeOut('slow', function() {
								$(this).hide();
								$(element).hide();
							});
						} else {
							$('.save-files').html("Failed to save file. Please contact <a href='mailto:info@fortifiedsecurityservices.com'>Fortified</a>");
						}
					},
					complete: function(data) {}
				});
			}
		});
	});
})(jQuery);