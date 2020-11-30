String s = "HTTP/1.1 200 OK";
    s += "Content-Type: text/html";
    s += "<!DOCTYPE HTML>";
    s += "<html>";

    s +="<head>\n";
    s +="<meta http-equiv=\"Access-Control-Allow-Origin\" name=\"viewport\" content=\"width=device-width, initial-scale=1.0 , user-scalable=no\">\n";
    
    //title is Quantum Optoelectronics Lab Control Center
    s +="<title>Quantum Optoelectronics Lab Control Center</title>\n";

    //Javascript switch actions
    s +="<script>\n";
    s +="function toggleChiller() {if(document.getElementById(\"Chiller\").checked === true){window.location = \"http://192.168.0.192:5000/D1/1\";} else {window.location = \"http://192.168.0.192:5000/D1/0\";}}\n";
    s +="function togglePump() {if(document.getElementById(\"Pump\").checked === true){window.location = \"http://192.168.0.192:5000/D2/1\";} else {window.location = \"http://192.168.0.192:5000/D2/0\";}}\n";
    s +="function toggleLaser() {if(document.getElementById(\"Laser\").checked === true){window.location = \"http://192.168.0.192:5000/D6/1\";} else {window.location = \"http://192.168.0.192:5000/D6/0\";}}\n";
    s +="function toggleLight() {if(document.getElementById(\"Light\").checked === true){window.location = \"http://192.168.0.192:5000/D7/1\";}else {window.location = \"http://192.168.0.192:5000/D7/0\";}}\n";
    s +="</script>\n";

    //Styling
    s +="<style>\n";
    s +="body { background-image: url('https://i.pinimg.com/originals/f1/6d/58/f16d58dc1eb441fed4adb4435f532ad0.jpg'); background-repeat: no-repeat; background-size: cover; background-attachment: fixed;}\n";
    
    //Switch settings
    s +=".switch {position: relative; display: inline-block; width: 60px;height: 34px;}\n";
    s +=".switch input {display: none;}\n";
    
    //Slider Styling/Settings
    s +=".slider.round {border-radius: 34px;}\n";
    s +=".slider.round:before {border-radius: 50%;}\n";
    s +=".slider {position: absolute;    cursor: pointer;    top: 0;    left: 0;    right: 0;    bottom: 0;    background-color: #ccc;    -webkit-transition: .4s;    transition: .4s;}\n";
    s +=".slider:before {position: absolute;    content: "";    height: 26px;    width: 26px;    left: 4px;    bottom: 4px;    background-color: white;    -webkit-transition: .4s;    transition: .4s;  }\n";

    //Input Settings
    s +="input:checked+.slider {background-color: #4CAF50;}\n";
    s +="input:focus+.slider {ox-shadow: 0 0 1px #4CAF50;}\n";
    s +="input:checked+.slider:before {-webkit-transform: translateX(26px); -ms-transform: translateX(26px); transform: translateX(26px);  }\n";

    //Heading 2 font styling
    s +="h2 {font-size: 300%; text-decoration: none;}\n";

    s +="</style>\n";
    s +="</head>\n";

    //Body
    s +="<body>\n";
    s +="<div style=\"color:black; font-size: 125%; text-align: center; \"><b>\n";
    
    //Title
    s +="<h1>QuantOpt Lab Control Center</h1>\n";
    
    //Chiller Switch
    s +="<h2 style=\"margin-right:60px;  display: inline-block;\" >Chiller: </h2>\n";
    s +="<label class=\"switch\">\n";
    s +="<input type=\"checkbox\" id=\"Chiller\" onchange=\"toggleChiller()\">\n";
    s +=" <span class=\"slider round\"></span>\n";
    s +="</label><br>\n";
    
    //Pump Switch
    s +="<h2 style=\"margin-right:60px;  display: inline-block;\" >Pump: </h2>\n";
    s +="<label class=\"switch\">\n";
    s +="<input type=\"checkbox\" id=\"Pump\" onchange=\"togglePump()\">\n";
    s +=" <span class=\"slider round\"></span>\n";
    s +="</label><br>\n";

    //Laser Switch
    s +="<h2 style=\"margin-right:60px;  display: inline-block;\" >Laser: </h2>\n";
    s +="<label class=\"switch\">\n";
    s +="<input type=\"checkbox\" id=\"Laser\" onchange=\"toggleLaser()\">\n";
    s +=" <span class=\"slider round\"></span>\n";
    s +="</label><br>\n";

    //Light Switch
    s +="<h2 style=\"margin-right:60px;  display: inline-block;\" >Light: </h2>\n";
    s +="<label class=\"switch\">\n";
    s +="<input type=\"checkbox\" id=\"Light\" onchange=\"toggleLight()\">\n";
    s +=" <span class=\"slider round\"></span>\n";
    s +="</label><br>\n";
    
    //Author
    s +="<h3>Danae Savvidi :)</h3>";
    s +="</b></div>";
    s +="</body>";
    s +="</html>";