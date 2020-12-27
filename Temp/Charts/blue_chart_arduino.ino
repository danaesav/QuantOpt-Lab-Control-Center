#include <ESP8266WebServer.h>
  #include <OneWire.h>
  #include <DallasTemperature.h>
  
  // Data wire is plugged into port D2 on the ESP8266
  #define ONE_WIRE_BUS D2
  
  // Setup a oneWire instance to communicate with any OneWire devices
  OneWire oneWire(ONE_WIRE_BUS);
  
  // Pass our oneWire reference to Dallas Temperature.
  DallasTemperature sensors(&oneWire);
  
  float tempSensor1;
  
  uint8_t sensor1[8] = { 0x28, 0x50, 0x74, 0x8D, 0x3C, 0x19, 0x01, 0x63 };
  
  /*Put your SSID & Password*/
  //const char* ssid = "ICP";  // Enter SSID here
  //const char* password = "xihu@123";  //Enter Password here
  //define IP fixed adderess
  int IP_1 = 192; int IP_2 = 168; int IP_3 = 1; int IP_4 = 216;
  const char* ssid = "Crete";  // Enter SSID here
  const char* password = "1234567890123";  //Enter Password here
  int log_size = 480;
  float TempData[2][480];
  float meas_time;
  float start_time;
  ESP8266WebServer server(80);
  
  
  IPAddress local_IP(IP_1, IP_2, IP_3, IP_4);
  IPAddress gateway(IP_1, IP_2, IP_3, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress primaryDNS(8, 8, 8, 8);   //optional
  IPAddress secondaryDNS(8, 8, 4, 4); //optional
  
  void setup() {
    Serial.begin(115200);
    start_time = millis();
    delay(1000);
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    Serial.begin(115200);
    delay(100);
    
    sensors.begin();
    //TempData[0][0,log_size-1]=millis()-start_time;
    // Connect to WiFi network
    Serial.println();
  
    // Configures static IP address
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
      Serial.println("STA Failed to configure");
    }
  
    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
    server.on("/", handle_OnConnect);
    server.onNotFound(handle_NotFound);
    
    sensors.requestTemperatures();
    tempSensor1 = sensors.getTempC(sensor1);
    for (int i = 0; i <= log_size-1; i++) {
      TempData[0][i] = tempSensor1;
      TempData[1][i] = 0;
    }
    server.begin();
    Serial.println("HTTP server started");
  }
  
  void TempMeasure() {
    int i = 0;
    do {
      int j = 0;
      do {
        TempData[i][log_size - 1 - j] = TempData[i][log_size - 2 - j];
  
        j += 1;
      } while (j < log_size - 1);
      i += 1;
    } while (i < 2);
    sensors.requestTemperatures();
    tempSensor1 = sensors.getTempC(sensor1); // Gets the values of the temperature
    meas_time = millis() - start_time;
    TempData[1][0] = meas_time;
    TempData[0][0] = tempSensor1;
  
  }
  
  void loop() {
    TempMeasure();
    int z = 0;
    do {
      z += 1;
      server.handleClient();
      delay(500);
    } while (z < 360);
  }
  
  void handle_OnConnect() {
  
    server.send(200, "text/html", SendHTML(tempSensor1));
  }
  
  void handle_NotFound() {
    server.send(404, "text/plain", "Not found");
  }
  
  
  String SendHTML(float tempSensor1) {
    String ptr = "<!DOCTYPE html><html><head>\n";
    ptr +="<meta http-equiv=\"refresh\" content=\"300\">";
    ptr +="<title>QuantOpt Temperature Lab</title> <meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\"> <meta name=\"robots\" content=\"noindex, nofollow\"> <meta name=\"googlebot\" content=\"noindex, nofollow\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> ";
    ptr +="<script type=\"text/javascript\" src=\"/js/lib/dummy.js\" ></script>";
    ptr +="<link rel=\"stylesheet\" type=\"text/css\" href=\"/css/result-light.css\"> ";
    ptr +="<style id=\"compiled-css\" type=\"text/css\"> .highcharts-figure, .highcharts-data-table table { min-width: 360px; max-width: 800px; margin: 1em auto; }";
    ptr +=".highcharts-data-table table { font-family: Verdana, sans-serif; border-collapse: collapse; border: 1px solid #EBEBEB; margin: 10px auto; text-align: center; width: 100%; max-width: 500px; } .highcharts-data-table caption { padding: 1em 0; font-size: 1.2em; color: #555; } .highcharts-data-table th { font-weight: 600; padding: 0.5em; } .highcharts-data-table td, .highcharts-data-table th, .highcharts-data-table caption { padding: 0.5em; } .highcharts-data-table thead tr, .highcharts-data-table tr:nth-child(even) { background: #f8f8f8; } .highcharts-data-table tr:hover { background: #f1f7ff; }";
    ptr +=".ld-label { width:200px; display: inline-block; }";
    ptr +=".ld-url-input { width: 500px; }";
    ptr +=".ld-time-input { width: 40px; } </style>";
    ptr +="<script id=\"insert\"></script>";
    ptr +="<script src=\"/js/stringify.js?d9f56d5679216d1bc5914c91196af39bb1710f2e\" charset=\"utf-8\"></script> <script> const customConsole = (w) => { \n const pushToConsole = (payload, type) => { w.parent.postMessage({ console: { payload: stringify(payload), type: type } }, \"*\") }";
    ptr +="\n w.onerror = (message, url, line, column) => { ";
    ptr +="\n line = line - 70 ";
    ptr +="\n if (line < 0){ \n pushToConsole(message, \"error\") } else { \n pushToConsole(`[${line}:${column}] ${message}`, \"error\") } }";
    ptr +="let console = (function(systemConsole){ return { log: function(){ let args = Array.from(arguments) ";
    ptr +="\n pushToConsole(args, \"log\") ";
    ptr +="\n systemConsole.log.apply(this, args) }, info: function(){ let args = Array.from(arguments) ";
    ptr +="\n pushToConsole(args, \"info\") ";
    ptr +="\n systemConsole.info.apply(this, args) }, warn: function(){ let args = Array.from(arguments) ";
    ptr +="\n pushToConsole(args, \"warn\") ";
    ptr +="\n systemConsole.warn.apply(this, args) }, error: function(){ let args = Array.from(arguments) ";
    ptr +="\n pushToConsole(args, \"error\") ";
    ptr +="\n systemConsole.error.apply(this, args) }, system: function(arg){ pushToConsole(arg, \"system\") }, clear: function(){ \n systemConsole.clear.apply(this, {}) }, time: function(label){ pushToConsole(label, \"log\") ";
    ptr +="\n systemConsole.time.apply(this, label) }, assert: function(arg){ let args = Array.from(arguments) ";
    ptr +="\n pushToConsole(args, \"log\") ";
    ptr +="\n systemConsole.assert.apply(this, args) }, dir: function(object){ pushToConsole(object, \"log\") ";
    ptr +="\n systemConsole.dir.apply(this, object) } } }(window.console))";
    ptr +="\n window.console = console";
    ptr +="console.system(\"Running fiddle\") }";
    ptr +="\n if (window.parent){ customConsole(window) } </script> </head> <body> <script src=\"https://code.highcharts.com/highcharts.js\"></script> <script src=\"https://code.highcharts.com/modules/data.js\"></script> <script src=\"https://code.highcharts.com/modules/exporting.js\"></script> <script src=\"https://code.highcharts.com/modules/export-data.js\"></script> <script src=\"https://code.highcharts.com/modules/accessibility.js\"></script>";
    ptr +="<figure class=\"highcharts-figure\"> <div id=\"container\"></div> </figure>";
    ptr +="<script type=\"text/javascript\">";
    ptr +="function createChart() { Highcharts.chart(\"container\", { time: {timezoneOffset: -2 * 60}, tooltip: {formatter: function () { var point = this.points[0]; return \"<b>\" + \"Temperature: \" + \"</b>\" + Highcharts.numberFormat(point.y, 2) + \" °C <br/><b> Time:</b> \" + Highcharts.dateFormat(\"%A %e %b %Y %H:%M:%S\", this.x);}, shared: true}, chart: { type: \"spline\" }, credits: { enabled: false }, xAxis: { title: { text: \"Time\" }, type: \"datetime\", tickPixelInterval: 150, labels: { style: { fontSize:\"15px\" } }}, yAxis: { title: { text: \"Temperature (C)\" }, labels: { style: { fontSize:\"18px\" } }}, title: { text: \"Temperature Data\" }, series: [{ marker: {enabledThreshold: 0 },name: \"Temperature\", data: (function () { ";
    ptr +="\n var data = [], time = (new Date()).getTime();";
    int j=0;
    do {
        ptr +="data.push({ x: time +"; ptr +=TempData[1][j]-TempData[1][0];
        ptr +=", y: "; ptr +=TempData[0][j];
        ptr +="});";
        j += 1;
    } while (j < log_size);
    ptr +="return data; }()) }] }); }";
    ptr +="createChart();";
    ptr +="</script> <script> if (window.parent && window.parent.parent){ window.parent.parent.postMessage([\"resultsFrame\", { height: document.body.getBoundingClientRect().height, slug: \"\" }], \"*\") }";
    ptr +="window.name = \"result\" </script>";
    ptr +="<script> let allLines = []";
    ptr +="window.addEventListener(\"message\", (message) => { if (message.data.console){ let insert = document.querySelector(\"#insert\")";
    ptr +="allLines.push(message.data.console.payload) ";
    ptr +="insert.innerHTML = allLines.join(\";\r\")";
    ptr +="let result = eval.call(null, message.data.console.payload) ";
    ptr +="if (result !== undefined){ console.log(result) } } }) </script>";
    ptr +="</body></html> ";
    return ptr;
  }
