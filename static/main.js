var p = "";
var interval = null;

function checkReady() {
    console.log('asking');
    $.ajax({
        url: '/isready',
        data: {id: p},
        success: function(data) {
            // TODO file ready
            $("#text1").text(data);
            clearInterval(interval);
        }
    });
}

$(document).ready(function(){
    const queryString = window.location.search;
    const urlParams = new URLSearchParams(queryString);
    p = urlParams.get('id');
    console.log(p);

    interval = setInterval(checkReady, 1000);
});