(function($) {
    $(document).ready(function() {
        /*var customer_field = $('#edit-field-vt-customer-und-0-target-id');
        if (customer_field.attr('value') != '') {
           var text = customer_field.attr('value');
            getDvrs(text);
        }
        $('.reference-autocomplete').live('click', function() {
            var text = $('#edit-field-vt-customer-und-0-target-id').attr('value');
            getDvrs(text);
        });

        function getDvrs(text) {
           var strip = text.indexOf("(") + 1;
           var uid = text.slice(strip, -1);

            $.ajax({
                type: 'POST',
                url: '/fr_vitual_select_dvr',
                dataType: 'json',
                data: {
                    input: uid,
                },
                success: function(data) {
                    console.log(data);
                    var $dvrSelect = $("#edit-field-vt-dvr-und");
                    $dvrSelect.empty(); // remove old options
                    $.each(data, function(key, value) {
                        $dvrSelect.append($("<option></option>").attr("value", key).text(value));
                    });
                },
                complete: function(data) {}
            });
        }*/
    });
})(jQuery);