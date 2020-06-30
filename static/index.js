var p = "";
var r = 0;
var interval = null;

function whoAsked() {
    var f = (r == 1) ? ".mp3" : ".tar";
    f = p + f;
    $.ajax({
        url: '/data/' + f,
        success: function(data) {
            // maybe need some changes
            console.log("ready");
            $('body').html('<a href="${f}">Файл готов</a>');
            clearInterval(interval);
        }
    });
}


$(document).ready(function(){
    $("#btn").click(function(){

        var fd = new FormData();
        var files = $('#file')[0].files[0];
        fd.append('file', files);
        p = $("input").val();
        if (Boolean(p)) {
            fd.append('name', p);
        } else {
            alert("Выберите имя выходного файла!");
            return;
        }
        var radioValue = $("input[name='param']:checked").val();
        if (radioValue) {
            r = radioValue;
            fd.append('forma', radioValue);
        } else {
            alert("Выберите что сделать с файлом!");
            return;
        }
         
        $.ajax({
            url: '/',
            type: 'post',
            data: fd,
            contentType: false,
            processData: false,
            success: function(response){
                $('body').html("<p>Loading...</p>");
                interval = setInterval(whoAsked, 1000);
            },
        });
    });
});