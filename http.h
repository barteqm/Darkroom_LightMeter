
void handleRoot() {
  server.send(200, "text/html", MAIN_page);
}
void readVals() {
  //String message="EV:"+String(EV)+"<a>T: "+String(T)+" <a>LUX: "+String(lux);
  String Lock = "";
  if (TimerLock) {
    Lock = " L";
  }
  else {
    Lock = "";
  };
  String EV_Comp = "";
  if (EVcorr != 0) {
    if (EVcorr > 0) {
      EV_Comp = " +" + String(EVcorr) + "EV";
    }
    else {
      EV_Comp = " " + String(EVcorr) + "EV";
    }
  }
  else {
    EV_Comp = "";
  };
  String message = "";
  if (!Timer) {
    message = "<table><tbody><tr><td>Lux:</td><td>&nbsp;" + String(lux) + "</td></tr><tr><td>EV:</td><td> &nbsp;" + String(EV) + EV_Comp + "</td></tr><tr><td>T:</td><td>&nbsp;" + String(T) + Lock + "</td></tr><tr><td>A:</td><td>&nbsp;" + String(A) + "</td></tr><tr><td>ISO:</td><td>&nbsp;" + String(iso) + "</td></tr></tbody></table>";
  } else {
    float Timer_Show = TimerCounter / 1000;
    message = "TIMER ON:" + String(TimerCounter / 1000);
  }
  server.send(200, "text/html", message);

}


void setParams() {
  Serial.println("Configuring Params...");
  if (server.arg("ISO") != "") {
    iso = server.arg("ISO").toInt();
    Serial.print("ISO GOT: ");
    Serial.println(server.arg("ISO"));

  };

  if (server.arg("A") != "") {

    A = server.arg("A").toFloat();
    Serial.print("Aperture GOT: ");
    Serial.println(server.arg("A"));
  };
  if (server.arg("EV") != "") {

    EVcorr = server.arg("EV").toFloat();
    Serial.print("EVCorr GOT: ");
    Serial.println(server.arg("EV"));
  };
  if (server.arg("Timer") != "") {

    TimerCounter = server.arg("Timer").toInt() * 1000;
    TimerLock = true;
    T = TimerCounter / 1000;
    Serial.print("ExtTimer GOT: ");
    Serial.println(server.arg("Timer"));
  };

  String message = R"=====(
  <!DOCTYPE html><html><head><title>Params Updated</title>
      <meta http-equiv = "refresh" content = "1; url = /" />
   </head>
   <style>
body {
font-family: Helvetica;
background: black;
color: red;
}</style>
   <body>
      <p style="text-align:center;">Params Updated!</p>
   </body>
</html>)=====";
  server.send(200, "text/html", message);

}
void setRed() {
  RED_light = !RED_light;
  if (RED_light) {
    WHITE_light = false;

  }

  String message = R"=====(
  <!DOCTYPE html><html><head><title>Params Updated</title>
      <meta http-equiv = "refresh" content = "0.3; url = /" />
   </head>
   <style>
body {
font-family: Helvetica;
background: black;
color: red;
}</style>
   <body>
      <p style="text-align:center;">RED!</p>
   </body>
</html>)=====";
  server.send(200, "text/html", message);

}
void setWhite() {
  WHITE_light = !WHITE_light;
  if (WHITE_light) {
    RED_light = false;

  }


  String message = R"=====(
  <!DOCTYPE html><html><head><title>Params Updated</title>
      <meta http-equiv = "refresh" content = "0.3; url = /" />
   </head>
   <style>
body {
font-family: Helvetica;
background: black;
color: red;
}</style>
   <body>
      <p style="text-align:center;">WHITE!</p>
   </body>
</html>)=====";
  server.send(200, "text/html", message);

}
void setTimerLock() {
  TimerLock = !TimerLock;



  String message = R"=====(
  <!DOCTYPE html><html><head><title>Params Updated</title>
      <meta http-equiv = "refresh" content = "0.3; url = /" />
   </head>
   <style>
body {
font-family: Helvetica;
background: black;
color: red;
}</style>
   <body>
      <p style="text-align:center;">TimerLock</p>
   </body>
</html>)=====";

  server.send(200, "text/html", message);

}
void setTimerStart() {
  Timer = true;
  String message = R"=====(
  <!DOCTYPE html><html><head><title>Params Updated</title>
      <meta http-equiv = "refresh" content = "0; url = /" />
   </head>
   <style>
body {
font-family: Helvetica;
background: black;
color: red;
}</style>
   <body>
      <p style="text-align:center;">TimerLock</p>
   </body>
</html>)=====";

  server.send(200, "text/html", message);
}

