(function($) {
$(document).ready(function(){

	$('.form-item-profile-customer-field-cust-virtual-tours-und').hide();
	$(".field-type-fr-times").hide();
	
	$("#edit-profile-customer-field-cust-virtual-tours-und-yes").click(function(){
	 $(".field-type-fr-times").show();
	});
		$("#edit-profile-customer-field-cust-virtual-tours-und-no").click(function(){
	 $(".field-type-fr-times").hide();
	});
		$("#edit-profile-customer-field-cust-service-type-und-cam").click(function(){
		var services = $(this).map(function() {
			if ($(this).is(':checked')) {
				$(".form-item-profile-customer-field-cust-virtual-tours-und").show();
			}else{
				$(".form-item-profile-customer-field-cust-virtual-tours-und").hide();
			}
		}).get();
	});
	

		//$(".field-type-office-hours").hide();
		//$('.form-item-profile-customer-field-cust-virtual-tours-und').hide();
		var tours = "edit-profile-customer-field-cust-virtual-tours-und-yes";
		var has_tours = $("#" + tours).map(function() {
			if ($(this).is(':checked')) {
				$(".field-type-fr-times").show();
			}	
		}).get();
		var service_type = 'edit-profile-customer-field-cust-service-type-und-cam';
		var services = $("#" + service_type).map(function() {
			if ($(this).is(':checked')) {
				$(".form-item-profile-customer-field-cust-virtual-tours-und").show();
			}
		}).get();
	});
})(jQuery);