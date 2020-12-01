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
const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE HTML>
    <html>
        <head>
            <meta http-equiv="Access-Control-Allow-Origin" name="viewport" content="width=device-width, initial-scale=1.0 , user-scalable=no">
        
        <!--title is Quantum Optoelectronics Lab Control Center-->
        <title>Quantum Optoelectronics Lab Control Center</title>
        <script>
          function toggleChiller() {
            if(document.getElementById("Chiller").checked === true){
                window.location = "http://192.168.0.192:5000/D1/1";
            } else {
                window.location = "http://192.168.0.192:5000/D1/0";
            }
            
          }
          function togglePump() {
            if(document.getElementById("Pump").checked === true){
              window.location = "http://192.168.0.192:5000/D2/1";
            } else {
              window.location = "http://192.168.0.192:5000/D2/0";
            }
          }
          function toggleLaser() {
            if(document.getElementById("Laser").checked === true){
              window.location = "http://192.168.0.192:5000/D6/1";
            } else {
              window.location = "http://192.168.0.192:5000/D6/0";
            }
          }
          function toggleLight() {
            if(document.getElementById("Light").checked === true){
              window.location = "http://192.168.0.192:5000/D7/1";
            } else {
              window.location = "http://192.168.0.192:5000/D7/0";
            }
          }
        </script>
    
        <!-- Customizing ON (kikigreen) and OFF (kikired) Buttons and lining them -->
        <style> 
        body {
        
        background-repeat: no-repeat;
        background-size: cover;
        background-attachment: fixed; 
        }
    
    .switch {
      position: relative;
      display: inline-block;
      width: 60px;
      height: 34px;
    }
    
    .switch input {
      display: none;
    }
    
    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #ccc;
      -webkit-transition: .4s;
      transition: .4s;
    }
    
    .slider:before {
      position: absolute;
      content: "";
      height: 26px;
      width: 26px;
      left: 4px;
      bottom: 4px;
      background-color: white;
      -webkit-transition: .4s;
      transition: .4s;
    }
    
    input:checked+.slider {
      background-color: #4CAF50;
    }
    
    input:focus+.slider {
      box-shadow: 0 0 1px #4CAF50;
    }
    
    input:checked+.slider:before {
      -webkit-transform: translateX(26px);
      -ms-transform: translateX(26px);
      transform: translateX(26px);
    }
    
    /* Rounded sliders */
    
    .slider.round {
      border-radius: 34px;
    }
    
    .slider.round:before {
      border-radius: 50%;
    }
    
    h2 { 
        font-size: 300%; 
        text-decoration: none;
        }
        
    </style>
    </head>
    
        <!-- Body -->
        <body>
        <div style="color:white; font-size: 125%; text-align: center;"><b>
        
        <!-- Title -->
        <h1>QuantOpt Lab Control Center</h1>
        
        <!-- Chiller -->
        <h2 style="margin-right:60px;  display: inline-block;" >Chiller: </h2>
    
            <label class="switch">
                <input type="checkbox" id="Chiller" onchange="toggleChiller()" {{ChillerStatus}}>
            <span class="slider round"></span>
            </label>
          <br>
        
        <!-- Pump -->
        <h2 style="margin-right:60px;  display: inline-block;" >Pump: </h2>
    
          <label class="switch">
          <input type="checkbox" id="Pump" onchange="togglePump()" {{PumpStatus}}>
          <span class="slider round"></span>
          </label><br>
    
        <!-- Laser -->
        <h2 style="margin-right:60px;  display: inline-block;" >Laser: </h2>
    
          <label class="switch">
              <input type="checkbox" id="Laser" onchange="toggleLaser()" {{LaserStatus}}>
          <span class="slider round"></span>
          </label><br>
    
        <!-- Light -->
        <h2 style="margin-right:60px;  display: inline-block;" >Light: </h2>
    
          <label class="switch">
              <input type="checkbox" id="Light" onchange="toggleLight()" {{LightStatus}}>
          <span class="slider round"></span>
          </label>
    
        <!-- Author -->
        <h3>Danae Savvidi </h3>
        </b></div>
        </body>
        </html>
        )rawliteral";
  }
