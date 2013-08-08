(function($) {
	$(document).ready(function() {
		$('#generate-aliases').click(function() {
			var ipUsername = $('#edit-field-alarm-ipdatatel-username-und-0-value').attr('value');
			var ipPassword = $('#edit-field-alarm-ipdatatel-password-und-0-value').attr('value');
			var loading = '<img src="../../sites/all/themes/fortified_dev/images/loading.gif"/>';
			$('#generate-aliases').html(loading);
			if (ipUsername != '' && ipPassword != '') {
				$.ajax({
					type: 'POST',
					url: '/ip_datatel_alarm_zones',
					dataType: 'json',
					data: {
						username: ipUsername,
						password: ipPassword,
					},
					success: function(data) {
						$('#generate-aliases').html('');
						$('#aliase-replace').html(data.results);
						console.log(data.alarm_zones);
					},
				});
			} else {
				alert('IpDatatel Username or IpDatatel Password cannot be empty');
			}
		});
	});
})(jQuery);