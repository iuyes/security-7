 (function($) {
 	$(document).ready(function() {
 		$('#remove_managed_images').click(function() {
 			var images = fileManager('edit-field-vt-tour-files-und-0-current-files-0', 'managed_image_files');
 		});
 		$('#remove_managed_videos').click(function() {
 			var video = fileManager('edit-field-vt-tour-files-und-0-current-files-1', 'managed_video_files');
 		});

 		function fileManager(id, checkboxes) {
 			var nodes = $("." + checkboxes).map(function() {
 				if ($(this).is(':checked')) {
 					return $(this).attr("value");
 				}
 			}).get();
 			var conf = confirm('Are you sure you want to permanently delete File(s):' + nodes + '?');
 			if (conf == true) {
 				var staying = $("." + checkboxes).map(function() {
 					if (!$(this).is(':checked')) {
 						return $(this).attr("value");
 					}
 				}).get();
 				for (var i = 0; i < nodes.length; i++) {
 					$("#" + nodes[i]).hide();
 				}
 				$("#" + id).val(staying);
 			}
 		}
 	});
 })(jQuery);