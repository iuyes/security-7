(function($) {
    $(document).ready(function() {
        //$.browser.device = (/android|webos|iphone|ipad|ipod|blackberry/i.test(navigator.userAgent.toLowerCase()));
        var $is_mobile = false;
        var $is_tablet = false;
        if ($('.featured-content-wrapper').css('display') == 'inline-block') {
            $is_mobile = true;
        }
        if ($('.featured-content-wrapper').css('vertical-align') == 'middle') {
            $is_tablet = true;
        }
        // set padding to make all items equal.
        var setHegiht;
        var averageHieght;
        var elementHeights = [];
        var items = $('.group_front_page_markup').map(function() {
            var thisPxHeight = $(this).find('.front-featured-body').css('height');
            var thisHeight = parseInt(thisPxHeight, 10);
            elementHeights.push(thisHeight);
        }).get();
        var sum = 0;
        for (var i = 0; i < elementHeights.length; i++) {
            sum += parseInt(elementHeights[i]);
        }
        var avg = sum / elementHeights.length;
        var changeHeight = $('.group_front_page_markup').map(function() {
            var thisPxHeight = $(this).find('.front-featured-body').css('height');
            var thisHeight = parseInt(thisPxHeight, 10);
            var needHeight = avg - thisHeight;
            if (avg > thisHeight) {
                if (!$(this).find('.field-name-field-help-images').css('height')) {
                    $(this).find('.front-page-header').css('padding-bottom', '25px');
                    $(this).find('.front-featured-body').css({
                        minHeight: ((thisHeight + needHeight) - 25) + 'px'
                    });
                } else {
                    $(this).find('.front-featured-body').css({
                        minHeight: (thisHeight + needHeight) + 'px'
                    });
                }
            } else if (!$(this).find('.field-name-field-help-images').css('height')) {
                $(this).find('.front-page-header').css('padding-bottom', '25px');
                $(this).find('.front-featured-body').css('padding-bottom', '25px');
            }
        }).get();
        var maxWidth;
        var eventType = 'mouseenter mouseleave';
        var slideDistace = 22;
        var currentWidth;
        var current_postion;
        var moveright;
        var windowWidth = window.innerWidth;
        currentWidth = $('.featured-content-item-container').attr('data-max');
        var dataTool = $('.featured-content-item-container').attr('data-tool');
        var num_of_items = (currentWidth / dataTool);
        if ($is_mobile) {
            //$('.li-featured-content').css({maxWidth:windowWidth+'px'});
            eventType = 'click mouseleave';
            num_of_items = currentWidth / 600; //600%
            var dataWidth = (6 * windowWidth);
            var math = (17.666667 / 100) * (dataWidth) - (num_of_items * 25.5);
            maxWidth = math * num_of_items;
            $('.li-featured-content').css({
                maxWidth: (windowWidth - 25) + 'px'
            });
            currentWidth = (17.666667 / 100) * (dataWidth) - (num_of_items * 25.5);
            //slideDistace = (17.666667 / 100) * (dataWidth) - (num_of_items * (num_of_items * 4));
            slideDistace = windowWidth - 25;
        } else if ($is_tablet) {
            eventType = 'click mouseleave';
            num_of_items = currentWidth / 600; //600%
            var dataWidth = (6 * windowWidth);
            var math = (8.44444 / 100) * (dataWidth) - (num_of_items * 25.5);
            maxWidth = math * num_of_items;
            $('.li-featured-content').css({
                maxWidth: (windowWidth - 50) + 'px'
            });
            currentWidth = (16.8888 / 100) * (dataWidth) - (num_of_items * 20.5);
            slideDistace = (16.8888 / 100) * (dataWidth) - (num_of_items * 1);
        } else if (num_of_items >= 4) {
            var over = (num_of_items - 3) * 550; //435
            maxWidth = (currentWidth * 1) + over;
        } else {
            maxWidth = currentWidth;
        }

        function setRightIntreval() {
            moveright = setInterval(function() {
                moveRight();
            }, 100);
        }
        var bPosition = 0;
        var bActivate;
        $('.front-featured-right').on(eventType, function(event) {
            switch (event.type) {
            case 'click':
            case 'mouseenter':
                setRightIntreval();
                break;
            case 'mouseleave':
                clearInterval(moveright);
                break;
            }
        });

        function moveRight() {
            current_postion = (currentWidth * 1) + bPosition;
            if (maxWidth <= current_postion) {
                bPosition = bPosition - slideDistace;
            }
            bPosition = bPosition + slideDistace;
            $('.featured-content').animate({
                left: '-' + bPosition,
            }, 100);
            if ($is_mobile || $is_tablet) {
                currentWidth
                clearInterval(moveright);
            }
        }
        //moveleft
        var moveleft;

        function setLeftIntreval() {
            moveleft = setInterval(function() {
                moveLeft();
            }, 100);
        }
        var cPosition = 0;
        $('.front-featured-left').on(eventType, function(event) {
            switch (event.type) {
            case 'click':
            case 'mouseenter':
                setLeftIntreval();
                break;
            case 'mouseleave':
                clearInterval(moveleft);
                break;
            }
        });

        function moveLeft() {
            cPosition = bPosition - slideDistace;
            $('.featured-content').animate({
                left: '-' + cPosition,
            }, 100);
            current_postion = (currentWidth * 1) - bPosition;
            if (current_postion == currentWidth) {
                bPosition = cPosition + slideDistace;
            } else {
                bPosition = cPosition;
            }
            if ($is_mobile || $is_tablet) {
                clearInterval(moveleft);
            }
        }
    });
})(jQuery);