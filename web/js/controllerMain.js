/* funcion que ejecuta al momento de cargar la ventana main search:*/
window.onload = function() {

  /* hacemos el get desde el localStorage  y lo cargamos en el textfield:*/
   //document.getElementById("search").value = localStorage.getItem("searchValue");

}

var dataSuggest = [];//["poco","pocoyo","pocoyó","pocophone f1","pocoyo en español","pocoyo en espanol","Pocsi","poco a poco","poco  tarea","pocoyo para colorear"];

/* inicializar la funcion autocompletado en el elemento "myInput", y pasar los dataSuggestcomo posibles autocompletados:*/

autocomplete(document.getElementById("search"), dataSuggest);

function autocomplete(inp, arr) {

  /*la funcion autocomplete toma dos argumentos,
  el archivo de texto y una arreglo de posibles valores:*/

  var currentFocus;
  /*ejecuta una funcion cuando alguien escribe en el textfield:*/
  inp.addEventListener("input", function(e) {

      var a, b, i, val = this.value;
      // console.log(" va------> "+document.getElementById("myInput").value);

      /*cierra cualquier lista ya abierta de valores autocompletados:*/
      closeAllLists();
      //falta validar los espacios en blanco
      if (!val || val === " ") { return false;}
      var prefix = "";
      var arrVal = val.split(" ");
      if(arrVal.length > 1){
        var i=0;
        for(; i < arrVal.length-1; i++){ 
            if(arrVal[i] != "")
                prefix += arrVal[i]+" ";
            else
                prefix += arrVal[i];
        }
        val = arrVal[i];
      }
      this.value = prefix + val;
      autoResponse(prefix, val);

  });
  function closeAllLists(elmnt) {
    /*cierra todo la lista autocomplete en el documento,
    excepto el que pasó como argumento:*/
    var x = document.getElementsByClassName("autocomplete-items");
    for (var i = 0; i < x.length; i++) {
      if (elmnt != x[i] && elmnt != inp) {
        x[i].parentNode.removeChild(x[i]);
      }
    }
  }
  /* ejecuta la funcion cuando alaguien hace click en el documento:*/
  document.addEventListener("click", function (e) {
      closeAllLists(e.target);
  });
}


/* funcion para obtener respuesta del servidor para
 * cada caracter buscado y palabra buscada :*/
function autoResponse(prefix, val){

    var http = new XMLHttpRequest();
    var url = 'http://localhost:8090/altavista/getOptions?word='+ encodeURIComponent(val);
    var params = 'orem=ipsum&name=binny';
    // var arr =[];
    http.open('GET', url, true);

    http.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');

    http.onreadystatechange = function() {
        if(http.readyState == 4 && http.status == 200) {

            var arr = JSON.parse(http.responseText)["words"];
            console.log(arr);
            /*crear un elemento DIV que contendra los items con valores:*/
            a = document.createElement("DIV");
            a.setAttribute("id", this.id + "autocomplete-list");
            a.setAttribute("class", "autocomplete-items");
            /*añade el elemento DIV  como hijo del contenedor autocomplete:*/
            document.getElementById("search").parentNode.appendChild(a);

            /*recorrer para cada item en el datasuggest...*/
            for (i = 0; i < arr.length; i++) {
              /*comparar si el item empieza con la mismo caracter en el textfield :*/
              //console.log(tildes_unicode(arr[i]).substr(0, val.length).toUpperCase()+"="+val.toUpperCase());

              if (arr[i].substr(0, val.length) == val.toUpperCase()) {

                /*crea un DIV para cada elemento que hace mach:*/
                b = document.createElement("DIV");

                /*hacer en negrita las letras coencidentes:*/

                b.innerHTML = prefix + arr[i].substr(0,val.length).toLowerCase();
                b.innerHTML += "<strong>"+arr[i].substr(val.length).toLowerCase()+"</strong>";

                /*insertar un inputfield(hidden)  que contendra los valores del item actual:*/
                b.innerHTML += "<input  type='hidden' value='" +prefix+ arr[i]+ "'>";

                /*ejecutar una funcion cuando aiguien hace click en el item value(DIV):*/
                b.addEventListener("click", function(e) {
                    /*insertar el valor para autocompletar el textfield:*/

                    // console.log("aqui "+e.target.innerHTML+" -->"+ document.getElementsByTagName("input").innerHTML);
                    //cambio para buscar el selecionado con el mouse
                      var innerObj = String(this.innerHTML);
                      var index = innerObj.indexOf("value=");
                      var objValue = "";
                      if(index>0){
                              index+=7;
                          var tempStr = innerObj.substring(index,innerObj.lenght);
                          var tempIndex = tempStr.indexOf("\"");
                          tempIndex+=index;
                          objValue = innerObj.substring(index,tempIndex);
                        console.log(objValue);
                          }

//                        document.getElementById("myInput").value = document.getElementsByTagName("input")[0].value;
                    document.getElementById("search").value= objValue.toLowerCase();

                    /*cerrar la lista autocompletada de valores,
                    (o cualquier otra lista de valores autocompletados:*/
                    var x = document.getElementsByClassName("autocomplete-items");
                    for (var i = 0; i < x.length; i++) {
                      if (e.target != x[i] && e.target != val) {
                        x[i].parentNode.removeChild(x[i]);
                      }
                    }

                });
                a.appendChild(b); //añadir al DIV2
              }
            }
        }
    }
    http.send(params);
}


var message = "\
    <a href='{0}/../content.html?id={1}'>{2}</a>\
        <div class='PostBody'>\
            {3}\
        </div>\
        <div class='PostFooter'>\
        id={1}\
    </div>";

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

/* Find search */

var http = new XMLHttpRequest();
var url = 'http://localhost:8090/search?q={0}&start={1}';
var params = 'orem=ipsum&name=binny';

/* variables */
var q_parameter = getParameterByName('q');
var start_parameter = getParameterByName('start');
var next_parameter = parseInt(start_parameter)+1;
var prev_parameter = parseInt(start_parameter)-1;
/* variables */

url = url.format(q_parameter, start_parameter);

http.open('GET', url, true);

http.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
http.onreadystatechange = function() {
    if(http.readyState == 4 && http.status == 200) {
        //alert(http.responseText);
        json_ans = JSON.parse(http.responseText);
        // if(json_ans["status"] == "200"){
        console.log(json_ans);
        if(true) {
            //fill and update time and count result
            document.getElementById("time").innerHTML = json_ans["time"];
            document.getElementById("count").innerHTML = json_ans["total"];
            if(json_ans["next"] != "-1"){
                next_parameter = json_ans["next"];
            }else{
                document.getElementById("nextButton").disabled = true;
            }

            if(json_ans["prev"] != "-1"){
                prev_parameter = json_ans["prev"];
            }else{
                document.getElementById("prevButton").disabled = true;
            }

            var div_result = document.getElementById("result");

            for (var i=0; i< json_ans["results"].length; i++){
                var temp = json_ans["results"][i];
                var node = document.createElement("div");
                var id = temp["docid"];
                var dbIndex = 12;
                var title = temp["title"];
                var preview = temp["preview"];
                //node.innerHTML = message.format(id, title, findWord(body, word), dbIndex);
                node.innerHTML = message.format(window.location.href.split("?")[0], id, title, preview, dbIndex);
                node.setAttribute("class","PostResult");
                div_result.appendChild(node);
            }






            var query_op_arr = q_parameter.split(" ");
            var query_op = query_op_arr[query_op_arr.length-1];


            var http_op = new XMLHttpRequest();
            var url_op = 'http://localhost:8090/altavista/getOptions?word='+query_op;
            var params_op = 'orem=ipsum&name=binny';
            // var arr =[];
            http_op.open('GET', url_op, true);
        
            http_op.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
        
            http_op.onreadystatechange = function() {
                if(http_op.readyState == 4 && http_op.status == 200) {
                    var arr = JSON.parse(http_op.responseText)['words'];
                    var counter = 0;
                    var max = arr.length < 4 ? arr.length : 4;
                    var current_val = document.getElementById("search").value.toUpperCase();
                    for(var i=0; i<max; i++){
                        if (arr[i] != current_val){
                            counter += 1;
                            document.getElementById("suggestionRad").innerHTML += " <a href='../web/main.html?q="+arr[i]+"&start=0'>"+arr[i]+"</a>, ";
                        }
                    }
                    
                    if(counter == 0){
                        document.getElementById("wrapperSug").innerHTML = "";
                    }
                }
            }
            http_op.send(params_op);



        }else if(json_ans["status"] == "404"){
            //show json_ans["similar"]
        }
    }else{
        //alert("Fail");

        document.getElementById("search").value = q_parameter;

        //var div_result = document.getElementById("result");
//
        //for (var i=0; i<10; i++){
        //    var node = document.createElement("div");
        //    //var textnode = document.createTextNode(message.format(id, title, body, dbIndex));
        //    //var textnode = document.createTextNode(messag.format(title));
        //    //node.appendChild(textnode);
        //    node.innerHTML = message.format(id, title, findWord(body, word), dbIndex);
        //    node.setAttribute("class","PostResult");
        //    div_result.appendChild(node);
        //}

    }
}
http.send(params);

function handleClick(event){

    var http = new XMLHttpRequest();
    var url = ' http://localhost:8090/altavista/getOptions?word=wilder ';
    var params = 'orem=ipsum&name=binny';
    http.open('GET', url, true);

    http.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');

    http.onreadystatechange = function() {
        if(http.readyState == 4 && http.status == 200) {

            console.log(http.responseText);
            alert(http.responseText);

        }
    }
    http.send(params);
}

function nextPage(){
    var current_url = window.location.href.split("?")[0];
    location.href = current_url+"?q=" + q_parameter + "&start=" + next_parameter;
}

function prevPage(){
    var current_url = window.location.href.split("?")[0];
    location.href = current_url+"?q=" + q_parameter + "&start=" + prev_parameter;
}

function cutMessage(message, init, end){
    return message.substring(init, end);
}

function findWord(message, word){
    var str_split = message.split(word);
    if (message.length < maxLetters) return message.join("<b><em><u>"+word+"</u></em></b>");
    if (str_split.length == 1 ) return cutMessage(message, 0, maxLetters);
    if (str_split[0].length < maxLetters) return cutMessage(str_split.join("<b><em><u>"+word+"</u></em></b>"), 0, maxLetters+23);
    else {
        console.log(cutMessage(str_split[0],0,maxLetters/3));
        console.log(cutMessage(str_split.join("<b><em><u>"+word+"</u></em></b>"), str_split[0].length - (maxLetters/3), str_split[0].length + (maxLetters/3)));
        return cutMessage(str_split[0],0,maxLetters/3) +" ... " + cutMessage(str_split.join("<b><em><u>"+word+"</u></em></b>"), str_split[0].length - (maxLetters/3), str_split[0].length + (maxLetters/3)+24);
    }
}

function handlePosts(){
    //var elements = document.getElementById("result");
    //for (var i=0; i<elements.length; i++){
    //    var UnBoton = document.createElement("LI");
    //    document.createTextNode ('<input type="button" name="boton7" value=" 7 ">');
    //    elements.appendChild(UnBoton);
    //}

    q_parameter = document.getElementById("search").value.trim();

    if(q_parameter == ""){
        document.getElementById("search").value = "";
        return;
    }

    var current_url = window.location.href.split("?")[0];
    location.href = current_url+"?q=" + q_parameter + "&start=0";

    //var div_result = document.getElementById("result");
//
    //for (var i=0; i<10; i++){
    //    var node = document.createElement("div");
    //    //var textnode = document.createTextNode(message.format(id, title, body, dbIndex));
    //    //var textnode = document.createTextNode(messag.format(title));
    //    //node.appendChild(textnode);
    //    node.innerHTML = message.format(id, title, findWord(body, word), dbIndex);
    //    node.setAttribute("class","PostResult");
    //    div_result.appendChild(node);
    //}
}
/*enlaza y ejecuta la nueva pagina con el contenido en resumen:*/
function linkToContent(){
  /*get the tag a value :*/
    var linkName = document.getElementsByTagName("a").value;
   console.log("value of link clicked"+linkName);
  // alert("value of link clicked"+linkName);
  location.href='content.html';
}



/* display */

function querySearchKey(e){
    tecla = (document.all) ? e.keyCode : e.which;
    if (tecla==13){
      querySearch();
    }
}

function querySearch(){
    var value = document.getElementById("search").value;
    var current_url = window.location.href.split("?")[0];
    location.href = current_url+"?q=" + value + "&start=0";
}
