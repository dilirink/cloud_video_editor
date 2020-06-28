$(document).ready(function(){
    // button id
    $("#button").click(function(){

        var fd = new FormData();
        // input id
        var files = $('#file')[0].files[0];
        fd.append('file', files);

        $.ajax({
            url: 'upload',
            type: 'post',
            data: fd,
            contentType: false,
            processData: false,
            success: function(response){
                if (response != 0) {
                    // TODO redirect
                } else {
                    // error 
                }
            },
        });
    });
});