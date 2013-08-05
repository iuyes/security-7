(function($) {
    if (!jQuery.viewTours) {
        jQuery.extend({
            viewTours: function(uid, type) {
                var bundle;
                if (type == 1) {
                    bundle = 'fortified_virtual_tours';
                }
                if (type == 2) {
                    bundle = 'fortified_events';
                }
                if (type == 3) {
                    bundle = 'fortified_incident_reports';
                }
                $.ajax({
                    type: 'POST',
                    url: '/fr_user_data',
                    dataType: 'json',
                    data: {
                        uids: uid,
                        types: bundle
                    },
                    success: function(data) {
                        //alert(data);
                        $('.custData').html(data.html);
                        $('#custDataModal').foundation('reveal', 'open');
                        $('.custData').show();
                        $('.preview-cust-data').click(function() {
                            var tr_parent = $(this).closest('tr');
                            $(tr_parent).addClass('preview');
                            $(tr_parent).siblings().removeClass('preview');
                            var nid = $(this).attr('cust-nid');
                            var preview = previewNid(nid);
                        });
                    },
                    complete: function(data) {},
                });

                function previewNid(nid) {
                    $.ajax({
                        type: 'POST',
                        url: '/fr_preview_cust_data',
                        dataType: 'json',
                        data: {
                            cust_nid: nid,
                        },
                        success: function(data) {
                            $('#preview-cust-data').html(data.node_data);
                        },
                    });
                }
            }
        });
    }
})(jQuery);