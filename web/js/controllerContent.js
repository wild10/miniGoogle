var http = new XMLHttpRequest();
var url = 'http://localhost:8090/content?id={0}';
var params = 'orem=ipsum&name=binny';



String.prototype.format = function() {
    var formatted = this;
    for (var i = 0; i < arguments.length; i++) {
        var regexp = new RegExp('\\{'+i+'\\}', 'gi');
        formatted = formatted.replace(regexp, arguments[i]);
    }
    return formatted;
};

function getParameterByName(name) {
    var url = window.location.href;
    name = name.replace(/[\[\]]/g, '\\$&');
    var regex = new RegExp('[?&]' + name + '(=([^&#]*)|&|#|$)'),
        results = regex.exec(url);
    if (!results) return null;
    if (!results[2]) return '';
    return decodeURIComponent(results[2].replace(/\+/g, ' '));
}

/* variables */
var id_parameter = getParameterByName('id');

window.onload = function() {

    url = url.format(id_parameter);

    http.open('GET', url, true);

    http.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    http.onreadystatechange = function() {
        if(http.readyState == 4 && http.status == 200) {
            json_ans = JSON.parse(http.responseText);
            document.getElementById("title").innerHTML = json_ans["title"];
            document.getElementById("content").innerHTML = json_ans["content"];
        }else{
            //alert("Fail");
        }
    }
    http.send(params);
  
}
