(function($) {
	$(document).ready(function() {
		var develsearch = new Object();
		var devel = $(this).find('.krumo-root');
		var ranSearch = false;
		if (devel) {
			var i = 1;
			var loadDevel = $('.krumo-root').each(function() {
				$(this).addClass('krumo-root-' + i + '');
				loadDevelSearch(i);
				i++;
			});
			var foundElementsParent;
			$('.search-devel').click(function(i) {
				var dataIdentifier = $(this).attr('data-value');
				var SearchForm = $(this).parent('.devel-search-wrapper-' + dataIdentifier + '');
				searchDevel(SearchForm, dataIdentifier);
			});
		}

		function findSearchItems(searchInput, elementClass) {
			var isFound = $("." + elementClass + ":contains(" + searchInput + ")").each(function() {
				if ($(this).parents(".krumo-root-" + develsearch.identifier).length == 1) {
					switch (elementClass) {
					case 'krumo-name':
						if (!$(this).parent('.krumo-element').hasClass('krumo-expand')) {
							$(this).parent('.krumo-element').addClass('found-devel-search-item-' + develsearch.identifier);
						} else {
							$(this).parent('.krumo-element').addClass('found-devel-parent-item-' + develsearch.identifier);
						}
						break;
					case 'krumo-string':
						$(this).parent('.krumo-element').addClass('found-devel-search-item-' + develsearch.identifier);
						break;
					}
					$(this).parents('.krumo-nest').prev().addClass('found-devel-parent-item-' + develsearch.identifier);
				}
			});
		}

		function searchDevel(element, dataIdentifier) {
			if (develsearch.identifier != '' && develsearch.identifier != dataIdentifier) {
				ranSearch = false;
			}
			if (ranSearch == true) {
				resetSearchResults(dataIdentifier);
				ranSearch = false;
			} else {
				develsearch.identifier = dataIdentifier;
				develsearch.searchElements = [];
				develsearch.searchInput = element.find('.devel-search-input').attr('value');
				develsearch.searchType = element.find('.search-devel-value-type').attr('value');
				switch (develsearch.searchType) {
				case 'krumo-string':
					develsearch.searchElements['array_values'] = 'krumo-string';
					break;
				case 'krumo-name':
					develsearch.searchElements['array_keys'] = 'krumo-name';
					break;
				default:
					develsearch.searchElements['array_keys'] = 'krumo-name';
					develsearch.searchElements['array_values'] = 'krumo-string';
					break;
				}
				for (var index in develsearch.searchElements) {
					findSearchItems(develsearch.searchInput, develsearch.searchElements[index]);
				}
				showSearchResults();
				highlightSearchResults();
				ranSearch = true;
			}
		}

		function showSearchResults() {
			$('.found-devel-parent-item-' + develsearch.identifier).trigger('click');
		}

		function highlightSearchResults() {
			$('.found-devel-parent-item-' + develsearch.identifier).css({
				background: '#666',
				color: 'white'
			}).children().css({
				background: '#666',
				color: 'white'
			});
			$('.found-devel-search-item-' + develsearch.identifier).css({
				background: '#999',
				color: 'white'
			}).children().css({
				background: '#999',
				color: 'white'
			});
		}

		function resetSearchResults(dataIdentifier) {
			$('.found-devel-parent-item-' + develsearch.identifier).css({
				background: '#FADB61',
				color: 'black'
			}).children().css({
				background: '#FADB61',
				color: 'black'
			});
			$('.found-devel-search-item-' + develsearch.identifier).css({
				background: '#FCEBA9',
				color: 'black'
			}).children().css({
				background: '#FCEBA9',
				color: 'black'
			});
			$('.found-devel-parent-item-' + develsearch.identifier).trigger('click');
			$('.found-devel-parent-item-' + develsearch.identifier).removeClass('found-devel-parent-item-' + develsearch.identifier);
			$('.found-devel-search-item-' + develsearch.identifier).removeClass('found-devel-search-item-' + develsearch.identifier);
		}

		function loadDevelSearch(i) {
			develsearch.searchDevel = '<div class="devel-search-wrapper-' + i + '"><input type="text" class="devel-search-input" name="search-devel"/><input type="submit" class="search-devel" value="search" data-value="' + i + '"/><select class="search-devel-value-type" style="width:15%;"><option value="krumo-string">Values</option><option value="krumo-name">Keys</option><option value="bolth">All</option></select></div>';
			$('.krumo-root-' + i + '').prepend(develsearch.searchDevel);
		}
	});
})(jQuery);