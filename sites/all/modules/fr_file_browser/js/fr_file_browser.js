(function($) {
    $(document).ready(function() {
        $('.view-file').click(function() {
            var file_name = $(this).attr("data-id");
            var current = this;
            $.ajax({
                type: 'POST',
                url: '/fr_file_browser',
                dataType: 'json',
                data: {
                    file: file_name,
                },
                success: function(data) {
                    $('.file-preview').html(data.status);
                    $('.file-actions').show();
                    $('.file-preview').attr('data-id', file_name);
                    $('.view-file').css('background', 'white');
                    $(current).css('background', '#d0d0d0');
                },
                complete: function(data) {},
            });
        });
        
        
        
        $('.file-actions').click(function(){
          var utility = $(this).attr('id');
          var image_file = $('.managed-image').attr('data-image-id');
          
          if(utility == 'refresh'){
            window.location.reload();
          }else if(utility == 'resize'){
            $('#resizeModal').foundation('reveal', 'open');
            $('#form-resize').click(function(){
              var class_image_file = $('.file-preview').attr('data-id');
              if(class_image_file != ''){
                image_file = class_image_file;
              }
            //alert(image_file);
              var percent = $('#resize-percent').attr('value');
              if(isInt(percent)){
                var resize = {};
                resize["file"] = image_file;
                resize['percent'] = percent;
                var file = new Array(resize.file, resize.percent);
                $('#resizeModal').foundation('reveal', 'close');
                fileActions(utility, resize);
              }else{
                alert('The height or width can only be numbers!');
              }
            });
          }else{
            fileActions(utility, file);
          }
        });
        
        
        function isInt(n) {
          return n % 1 === 0;
        }
        
        
        
        function fileActions(utility, file){
          $.ajax({
                type: 'POST',
                url: '/fr_file_browser_resize',
                dataType: 'json',
                data: {
                  type:{
                    type:utility,
                    file:file,
                  }
                },
                success: function(data) {
                    $('.file-preview').html(data.status);
                    $('.file-actions').show();
                },
                complete: function(data) {},
            });
        }
    });
})(jQuery);
/*var file = $("." + checkboxes).map(function() {
   				if ($(this).is(':checked')) {
   					return $(this).attr("value");
   				}
   			}).get();*/