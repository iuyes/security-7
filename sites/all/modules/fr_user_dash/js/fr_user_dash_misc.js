(function($) {
	$(document).ready(function() {
	
		/*************** FOR BUGS *****************/
		/******************************************/
			var open = false;
			var locateBug;
			var clickedId;
			var newClick = true;
			$('.bugs-view-bug').click(function(e){
				newClick = true;
				var clickedId = $(this).attr('data-locate');
				e.preventDefault();
				if(open){
					if(locateBug != 'undefined' && locateBug == clickedId){
						newClick = false;
						$('.bug-text').slideUp('slow');
					}
					$('.bug-text').slideUp('slow').fadeOut('slow', function() {
						$(this).remove();
					});
				}
				locateBug = $(this).attr('data-locate');
					if(newClick){
					var bugText = $('.bug-text-'+locateBug+'').html();
					if(bugText){
						var parents = $(this).parents('tr').get(0);
						$(parents).after('<tr class="bug-text large-12 columns"><td>'+bugText+'</td></tr>');
						$('.bug-text').slideDown('slow');
						open = true;
					}
				}else{
					locateBug = '';
				}
			});
			//delete bugs
			$('.bugs-delete-bug').click(function(e){
				e.preventDefault();
				var hideBug =  $(this).parents('tr').get(0);
				var bugId = $(this).attr('data-bug-id');
				$.ajax({
					type: 'POST',
					url: '/ajax/fr_user_dash_delete_bug',
					dataType: 'json',
					data: {
						data: bugId,
					},
					success: function(data) {
						$(hideBug).hide();
					},
					complete: function(data) {}
				});
			});
			/******************************************/
			/*************** FOR BUGS *****************/
			
		});
		
})(jQuery);