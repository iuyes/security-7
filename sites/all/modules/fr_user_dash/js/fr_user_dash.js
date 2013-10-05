(function($) {
	$(document).ready(function() {	
  	

		$('.deleteFile').live('click', function() {
			//var confirm = confirm('Are you sure you want to delete');
			var nodes = $(".manage-file").map(function() {
				if ($(this).is(':checked')) {
					return $(this).attr("value");
				}
			}).get();
			var conf = confirm('Are You Sure You Want to delete File(s):' + nodes);
			if (conf == true) {
				$.ajax({
					type: 'POST',
					url: '/fr_user_delete',
					dataType: 'json',
					data: {
						nids: nodes
					},
					success: function(data) {
						alert(data.status);
					},
					complete: function(data) {
						for (var i = 0; i <= nodes.length; i++) {
							$('.' + nodes[i]).hide();
							$('#' + nodes[i]).click();
						}
					}
				});
			}
		});
		$('.downloadFile').click(function(e) {
			e.preventDefault();
			alert('Please wait while your files are prepared for download. This may take a minute if the files are large. Press Ok to start downloading your files.');
			var nid = $(this).attr('id');
			$.ajax({
				type: 'POST',
				url: '/fr_user_download',
				dataType: 'json',
				data: {
					nids: nid,
				},
				success: function(data) {
					var amount = data.length;
					console.log(data);
					for (var i = 0; i <= amount; i++) {
						if (i == amount) {
							$.fileDownload('../../node/' + nid + '/pdf');
						} else {
							$.fileDownload(data[i]).done(function() {
								alert('All files have been downloaded!');
							});
						}
					}
				},
			});
		});
		$('.revealMyAlarmModal').live('click', function() {
			navigator.sayswho = (function() {
				var N = navigator.appName,
					ua = navigator.userAgent,
					tem;
				var M = ua.match(/(opera|chrome|safari|firefox|msie)\/?\s*(\.?\d+(\.\d+)*)/i);
				if (M && (tem = ua.match(/version\/([\.\d]+)/i)) != null) M[2] = tem[1];
				M = M ? [M[1], M[2]] : [N, navigator.appVersion, '-?'];
				return M[0];
			})();
			var browserType = navigator.sayswho;
			if (browserType != 'Firefox') {
				var browse = confirm('You may encounte problems with navigation using ' + browserType + '. Firefox is the recomended browser to view your Virtual Alarm. If you allready have Firefox installed, Click Cancel to proceed otherwise Click OK to download Firefox now.');
				if (browse == true) {
					window.open("http://www.mozilla.org/en-US/firefox/new/");
				} else {
					$('#myAlarmModal').foundation('reveal', 'open');
				}
			}else{
				$('#myAlarmModal').foundation('reveal', 'open');
			}
		});
	});
})(jQuery);