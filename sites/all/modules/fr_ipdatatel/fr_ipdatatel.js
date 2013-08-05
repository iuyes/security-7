(function ($) {
  $(document).ready(function(){
    $('.add-more-users').click(function(){
      var current = $(this).attr('attatched-value');
				$('.add-user-field-'+current+'').show();
				var new_current = (current * 1) + 1;
				$('.add-more-users').attr('attatched-value', new_current);
    });
  });
		
})(jQuery);
