"use strict";

var passmatch = () => {
    var result = document.getElementById('newpassword').value === document.getElementById('newpassword2').value;
    if (!result){
        console.log("Passwords do not match");
        $('#newpassword, #newpassword2').css({borderColor: "red"});
    return result
    }
};
$(document).ready(function(){
    
    $('#buy-link').click(() => {
        $('#transhead').html("Buy Stocks");
        if ($('#sell, #modalaccount').hasClass('active')) {
            $('#sell, #modalaccount').removeClass('active');
        }
        $('#buy').addClass('active');
    });
    $('#sell-link').click(() => {
        $('#transhead').html("Sell Stocks");
        if ($('#buy, #modalaccount').hasClass('active')){
            $('#buy, #modalaccount').removeClass('active');
        }
        $('#sell').addClass('active');
    });

    $('#forgot').click(() => {
        $('#forgotpass').addClass('active');
    });

    $('#fundaccount, #changepass').click(() => {
        $('#transhead').html("Manage Account");
        if ($('#buy, #sell').hasClass('active')){
            $('#buy, #sell').removeClass('active');
        }
        $('#modalaccount').addClass('active');
    });

    $('ul#modal-nav li a').click((event) => {
        event.preventDefault();
        // var maindiv = $('div#modal-manageacc')

        var targetdiv = event.currentTarget.hash;
        console.log(event);
        var activediv = $('div#modal-manageacc > .active');

        if (!$(targetdiv).is($(activediv))){
            $(targetdiv).addClass('active').siblings().removeClass('active');
            
            $('ul#modal-nav li a').removeClass('active');
            $(event.currentTarget).addClass('active')
        }
    })
});
