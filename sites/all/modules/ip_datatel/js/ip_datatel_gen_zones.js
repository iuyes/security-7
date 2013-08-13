(function($) {
	$(document).ready(function() {
		$('#generate-zones').hide();
		$('.generate-aliases').click(function() {
			var genData = $(this);
			var ipUsername = genData.attr('data-user');
			var ipPassword = genData.attr('data-pass');
			var utility = genData.attr('data-utility');
			var nodeId = genData.attr('data-nid');
			var loading = '<img src="../../sites/all/themes/fortified_dev/images/loading.gif"/>';
			$('#save-zones').html(loading);
			$('#generate-zones').show();
			if (ipUsername != '' && ipPassword != '') {
				$.ajax({
					type: 'POST',
					url: '/ip_datatel_alarm_zones',
					dataType: 'json',
					data: {
						username: ipUsername,
						password: ipPassword,
						utility: utility,
					},
					success: function(data) {
						$('#save-zones').html('Save Zones');
						$('#aliase-replace').html(data.results);
						var zones = data.alarm_zones;
						$('#save-zones').click(function(){
							saveZones(zones, nodeId);
						});
					},
				});
			} else {
				alert('IpDatatel Username or IpDatatel Password cannot be empty');
			}
		});
			function saveZones(zones, nid){
				$.ajax({
					type: 'POST',
					url: '/ip_datatel_save_alarm_zones',
					dataType: 'json',
					data: {
						zones:zones,
						nid:nid,
					},
					success: function(data) {
						if(data.status == 2){
							$('#generate-zones').html('<div class="messages status">The zone aliases have been saved!');
						}
					},
				});
			}
	});
})(jQuery);