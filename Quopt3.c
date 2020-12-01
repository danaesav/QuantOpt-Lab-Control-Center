void setup() {
    int pin1 = 0;
    int pin2 = 1;
    int pin6 = 3;
    int pin7 = 6;
  }
  
  void loop() {
    int pin1 = 0;
    int pin2 = 1;
    int pin6 = 3;
    int pin7 = 6;
    pin1 = digitalRead(pin1);
    pin2 = digitalRead(pin2);
    pin6 = digitalRead(pin6);
    pin7 = digitalRead(pin7);
    const char* ChillerStatus = "checked";
    const char* PumpStatus = "checked";
    const char* LaserStatus = "checked";
    const char* LightStatus = "checked";

    if (pin1 == 0) {
      const char* ChillerStatus = "checked";
    } else {
      const char* ChillerStatus = "";
    }
    
    if (pin2 == 0) {
      const char* PumpStatus = "checked";
    } else {
      const char* PumpStatus = "";
    }
    if (pin6 == 0) {
      const char* LaserStatus = "checked";
    } else {
      const char* LaserStatus = "";
    }
    if (pin7 == 0) {
      const char* LightStatus = "checked";
    } else {
      const char* LightStatus = "";
    }

    String s = "HTTP/1.1 200 OK";
s += "Content-Type: text/html";
s += "<!DOCTYPE HTML>";
s += "<html>";

s +="<head>\n";
s +="<meta http-equiv=\"Access-Control-Allow-Origin\" name=\"viewport\" content=\"width=device-width, initial-scale=1.0 , user-scalable=no\">\n";

s +="<title>Quantum Optoelectronics Lab Control Center</title>\n";

s +="<style>body {background-repeat: no-repeat; background-size: cover; background-attachment: fixed; background-color:oldlace ; }\n";
s +=".kikigeneral {padding: 5px; text-align: center; font-size: 250%; background-color: white; text-decoration: none;  border-radius: 15px; transition-duration: 0.4s; cursor: pointer; border: none;}\n";
s +=".kikigreen {color: #4CAF50; margin-right:35px; background-color: white; border: 3px solid #4CAF50;}\n";  
s +=".kikigreen:hover, .kikigreen:active { background-color: #4CAF50; color: white;}\n";
s +=".kikigreenselected { background-color: #4CAF50; color: white; margin-right:35px; border: 3px solid #4CAF50; }\n";   
s +=".kikired {color: #f44336;  background-color: white; border: 3px solid #f44336;}\n";
s +=".kikired:hover { background-color: #f44336; color: white;}\n";
s +=".kikired:focus { background-color: #f44336; color: white;}\n";
s +="h2 {font-size: 320%;  text-decoration: none; }\n";
s +=".kiki {display: inline-block;}\n";
s +="</style>";
s +="</head>";
s +="<body>";
s +="<div style=\"color:#008CBA; text-align: center; width: 100%;\"><b>";
s +="<h1 style=\"font-size: 250%;\">QuantOpt Lab Control Center</h1>";

s +="<div class= \"kiki\">";
s +="<h2 class= \"kiki\" style=\"margin-right:40px;\" >Chiller: </h2>";
s +="<form class= \"kiki\" action=\"http://192.168.0.192:5000/D1/1\"><input class=\" kikigreen kikigeneral\" type=\"submit\" value=\"ON\"/></form>";
s +="<form class= \"kiki\" action=\"http://192.168.0.192:5000/D1/0\"><input class=\" kikired kikigeneral\" type=\"submit\" value=\"OFF\"/></form>";
s +="<h3 style=\"margin-left:40px; font-size:200%; display: inline-block;\" >Status: ",ChillerStatus,"</h3>";
s +="</div><br>";
    
s +="<div class= \"kiki\">";
s +="<h2 class= \"kiki\" style=\"margin-right:40px;\" >Pump: </h2>";
s +="<form class=\"kiki\" action=\"http://192.168.0.192:5000/D2/1\"><input class=\"kikigreen kikigeneral\" type=\"submit\" value=\"ON\"/></form>";
s +="<form class=\"kiki\" action=\"http://192.168.0.192:5000/D2/0\"><input class=\"kikired kikigeneral\" type=\"submit\" value=\"OFF\"/></form>";
s +="<h3 style=\"margin-left:40px; font-size:200%; display: inline-block;\">Status: ",PumpStatus,"</h3>";
s +="</div><br>";

s +="<div class= \"kiki\">";
s +="<h2 class= \"kiki\" style=\"margin-right:40px;\" >Laser: </h2>";
s +="<form class= \"kiki\" action=\"http://192.168.0.192:5000/D6/1\"><input class=\"kikigreen kikigeneral\" type=\"submit\" value=\"ON\"/></form>";
s +="<form class= \"kiki\" action=\"http://192.168.0.192:5000/D6/0\"><input class=\"kikired kikigeneral\" type=\"submit\" value=\"OFF\"/></form>";
s +="<h3 style=\"margin-left:40px; font-size:200%; display: inline-block;\">Status: ",LaserStatus,"</h3>";
s +="</div> <br>";

s +="<div class= \"kiki\">";
s +="<h2 class= \"kiki\" style=\"margin-right:40px;\">Light: </h2>";
s +="<form class= \"kiki\" action=\"http://192.168.0.192:5000/D7/1\"><input class=\"kikigreen kikigeneral\" type=\"submit\" value=\"ON\"/></form>";
s +="<form class= \"kiki\" action=\"http://192.168.0.192:5000/D7/0\"><input class=\"kikired kikigeneral\" type=\"submit\" value=\"OFF\"/></form>";
s +="<h3 style=\"margin-left:40px; font-size:200%; display: inline-block;\">Status: ", LightStatus,"</h3>";
s +="</div>";

s +="<h3>Danae Savvidi :)</h3>";
s +="</b></div>";
s +="</body>";
}
