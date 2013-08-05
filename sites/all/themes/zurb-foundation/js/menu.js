

(function ($) {
	$(document).ready(function(){
	//////////////////////////////////////////////////////////////////////////
	function browserTester(browserString) {
    return navigator.userAgent.toLowerCase().indexOf(browserString) > -1;
  }


  function load_modal(){
	  $('#myDvrModal').reveal();
  }
 //////////////////////////////////////////////////////////////////////////
  if(!browserTester('chrome')) {
    $('.views-slideshow-controls-text-next.views_slideshow_controls_text_next.views-slideshow-controls-text-next-processed a').addClass('active');
    $('.views-slideshow-controls-text-previous.views_slideshow_controls_text_previous.views-slideshow-controls-text-previous-processed a').addClass('active');
   } 
	//////////////////////////////////////////////////////////////////////////
	$('.quicktabs-style-sky li').addClass('tabs-text mobile-four');
	$('.quicktabs-style-sky li a').addClass('mobile-four');
	///////////////////////////////////////////////////////////////////////////
	$('.tab_highlight').live('mouseover', function(){
		$(this).css('border-bottom', '1px dotted black')
			.css('cursor', 'pointer')
			.addClass('hovered');
		$(this).click(function(){
			$(this).css('border-bottom', '1px dotted black')
				.css('cursor', 'pointer');
			$(this).addClass('active');
			$(this).next('.desc_wrapper').slideDown('slow')
				.siblings('.desc_wrapper').slideUp('slow');
			$(this).siblings('.tab_highlight').removeClass('active');
			return false;
		});
	});
	$('.tab_highlight').mouseout(function(){
		$(this).css('border-bottom', '1px dotted #eee')
			.removeClass('hovered');
		});
	//////////////////////////////////////////////////////////////////////////
		if($('.page_menu a').hasClass('active') ){
			$('.page_menu a.active').parents().addClass('activeli');
		}
		$('.page_menu li').mouseover(function(){
			$(this).addClass('active');
		});
		$('.page_menu li').mouseout(function(){
			$(this).removeClass('active');
		});
	//////////////////////////////////////////////////////////////////////////	
	});
	//////////////////////////////////////////////////////////////////////////
	if($('.main-nav').height() < 100){
		var jump = 200;  // consider this is your 10px 
		window.scrollHeight = 0;
		$(window).scroll(function () {
			console.log($(this).scrollTop());
			var current = $(this).scrollTop();
			if(current <= 200){
	     	$('.main-nav').css('position', 'relative');
	     	$('.main-nav').removeClass('active-fixed');
     	}else if (current >= jump) {
	     	window.scrollHeight = $(this).scrollTop();
	     	console.log('reload frame');
	     	$('.main-nav').css('position', 'fixed');
	     	$('.main-nav').addClass('active-fixed');
	     	$('.main-nav').addClass('mobile-four');
	    }
    }); 
  }
  

	
	})(jQuery);