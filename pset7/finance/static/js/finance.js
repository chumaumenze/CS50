"use strict";

$(document).ready(function(){
    $('#buy-link').click(() => {
        $('#transhead').html("Buy Stocks");
        if ($('#sell').hasClass('active')) {
            $('#sell').removeClass('active');
        }
        $('#buy').addClass('active');
    });
    $('#sell-link').click(() => {
        $('#transhead').html("Sell Stocks");
        if ($('#buy').hasClass('active')){
            $('#buy').removeClass('active');
        }
        $('#sell').addClass('active');
    });
});
