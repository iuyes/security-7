(function ($) {
 $(document).ready(function(){
	$('.dvr-modal').click(function(e){
		$('.cust-dvr-modal').html('');
		e.preventDefault();
		var dvrId = $(this).attr('data-dvr-nid');
		console.log(dvrId);
		if(dvrId != ''){
			var dvrUrl = $('#myDvrModal'+dvrId+'').attr('data-dvr-url');
			var dvrIframe = '<iframe seamless width="100%" height="700px" src="http://'+dvrUrl+'"></iframe>';
			$('#myDvrModal'+dvrId+'').foundation('reveal', 'open');
			$('.myDvrModal'+dvrId+'').html(dvrIframe);
		}
	});
 });
})(jQuery);