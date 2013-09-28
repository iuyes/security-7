(function($){
	
	$(document).ready(function(){
		var dvrAlias = {};
		dvrAlias.functions = {};
		
		/**
		 * function to get the alarm zone to modify
		 */
		dvrAlias.functions.alarmZonesFocus = $('.alarm-zone-alias :input').focus(function(){
			dvrAlias.alarmZone = $(this);
			$(this).addClass('modify-alarm-zone');
			$('.modify-alarm-zone').css('border', '2px solid blue;');
		});
		
		
		/**
		 * blur function to let the user know which alarm aliase they are to modify
		 */
		dvrAlias.functions.alarmZonesBlur = $('.alarm-zone-alias-input :input').blur(function(){
			$('.alarm-zone-alias-input :input').removeClass('modify-alarm-zone');
		});
		
		/**
		 * function to get the selected dvrs name
		 */
		dvrAlias.functions.dvrZonesChange = $('#edit-select-dvr-wrapper-dvr-zones').change(function(){
			dvrAlias.dvrZoneValue = $(this).val();
			dvrAlias.alarmZoneValue = dvrAlias.alarmZone.val();
			if(dvrAlias.dvrZoneValue != 'none' && dvrAlias.alarmZone){
				var alarmZoneValue = dvrAlias.alarmZoneValue;
				alarmZoneValue += ':'+dvrAlias.dvrZoneValue;
				$(dvrAlias.alarmZone).val(alarmZoneValue);
				$(this).val('none');
			}
		});
	});
	
})(jQuery);