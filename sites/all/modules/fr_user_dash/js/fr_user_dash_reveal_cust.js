(function ($) {
 $(document).ready(function(){
 /******reveal the dvr modal*****/
 	$('.customerDvrModal').html('');
 $(document).on('click', '.reveal-cust-dvr', function(e){
 		e.preventDefault();
	 	var custDvrid = $(this).attr("data-cust-uid");
	 	var custDvrUrl = $(this).attr('data-cust-url');
	 	if(custDvrid && custDvrUrl){
	 		$('#customerDvrModal').foundation('reveal', 'open');
	 		var custDvrIframe = '<iframe id="customerDvrModal" style="border:2px solid white;" seamless scroll="yes" width="100%" height="700px" src="http://'+custDvrUrl+'/WebClient.html"></iframe>';
	 		$('.customerDvrModal').html(custDvrIframe);
		}
	});
	
	$(document).on('click', '.close-dvr-modal', function(){
		$('.customerDvrModal').html('');
	});
	/******reveal the dvr modal*****/
 });
})(jQuery);