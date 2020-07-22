const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
body {
font-family: Helvetica;
background: black;
color: red;
}
@media (max-width: 768px) {
  body,html {
   max-width: 100%;
   min-width: 100%;
 }
}
body
.card{
    max-width: 100%;
     min-height: 250px;
     background: red;
     padding: 30px;
     box-sizing: border-box;
     color: #000;
     margin:20px;
  
}
a {
text-decoration:none;
color:black;
background: red;
}
</style>
<title>Light Metering</title>
<body>

<div class="card">
  <h2>ESP Darkroom Timer/Meter</h2><br>
  <h1><span id="Values">0</span></h1><br>
  <br>
</div>
<script>

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getData();
}, 1000); //2000mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Values").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readVals", true);
  xhttp.send();
}
</script>
<form accept-charset="UTF-8" action="setParams" autocomplete="off" method="GET">
<table border="0" style="border-collapse: collapse;  max-width: 100%;padding: 30px;margin:20px;">
<tbody>
<tr>
<td style=" text-align: center;">ISO</td>
<td style=" width:50%;"><input name="ISO" type="text" value="" /> </td>
</tr>
<tr>
<td style=" text-align: center; width:50%;">A</td>
<td style=" width:50%;"><input name="A" type="text" value="" /></td>
</tr>
<tr>
<td style="text-align: center; width:50%;">EV +-</td>
<td style=" width:50%;"><input name="EV" type="text" value="" /></td>
</tr>
<tr>
<td style="text-align: center; width:50%;">TIMER:</td>
<td style=" width:50%;"><input name="Timer" type="text" value="" /></td>
</tr>
<tr>
<td style="text-align: center"><button style="display:inline-block;
 padding:0.35em 1.2em;
 color:red;" type="submit" value="Submit">UPDATE</button></td>
<td style="">&nbsp;&nbsp;<a href="/red">&nbsp;RL&nbsp;</a>&nbsp;<a href="/white">&nbsp;WL&nbsp;</a>&nbsp;<a href="/timerLock">&nbsp;TL&nbsp;</a>&nbsp;<a href="/timerStart">&nbsp;TS&nbsp;</a> </td>
</tr>
</tbody>
</table>
</form>
</body>
</html>
)=====";
