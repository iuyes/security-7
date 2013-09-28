(function($){
  $(document).ready(function(){
  
  window.addEventListener("DOMContentLoaded", function() {
  	var src = $('#iframe').attr('src');
  	console.log(src);

    var iframe = document.querySelector("iframe")
      , _window = iframe.contentWindow
			console.log(iframe);
    window.addEventListener("message", function(e) {
        // wait for child to signal that it's loaded.
        if ( e.data === "loaded" && e.origin == src) {
            // send the child a message.
            _window.postMessage("Test", iframe.src)
        }
    })

}, false)

  
  });
})(jQuery);