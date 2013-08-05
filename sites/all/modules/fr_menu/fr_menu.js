(function($){
	$(document).ready(function(){
	$('.header-bottom-item').hover(function(){
		
			var elemWidth = $(this).width();
			var elemHeight = $(this).height();
      var location = $(this).offset();
      var x = location.left;
      var y = location.top;
     
      $('.m-hidden').css('content', '').css({
      position:'absolute',
      display:'block',
			content:"",
			position:'absolute',
			top:(y - ((elemHeight / 2) + 15)),
			left:(x + (elemWidth / 2)),
			borderStyle:'inset',
			borderWidth:'8px',
			borderColor:'white transparent transparent transparent',
      });
   }); 
	});
})(jQuery);