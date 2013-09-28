(function($) {
$(document).ready(function(){

	$('.form-item-profile-customer-field-cust-virtual-tours-und').hide();
	$(".field-type-fr-times").hide();
	
	$("#edit-field-cust-virtual-tours-und-yes").click(function(){
	 $(".field-type-fr-times").show();
	});
		$("#edit-field-cust-virtual-tours-und-no").click(function(){
	 $(".field-type-fr-times").hide();
	 var hadTOurs = $('#edit-field-fr-vt-tour-times-und-0-tour-times-tour-amount').attr('value');
	 if(hadTOurs >= 1){
	 	$('#edit-field-fr-vt-tour-times-und-0-tour-times-tour-amount').attr('value', 1);
		 for(var i = 1; i<=hadTOurs; i++){
			 $('#edit-field-fr-vt-tour-times-und-0-tour-times-times-wrapper'+i+'-time-hour-'+i+'').attr('value', '');
			 $('#edit-field-fr-vt-tour-times-und-0-tour-times-times-wrapper'+i+'-time-minute-'+i+'').attr('value', '');
		 }
	 }
	});
	
		var tours = "edit-field-cust-virtual-tours-und-yes";
		var has_tours = $("#" + tours).map(function() {
			if ($(this).is(':checked')) {
				console.log('yes');
				$(".field-type-fr-times").show();
			}	
		}).get();
	});
})(jQuery);