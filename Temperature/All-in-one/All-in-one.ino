#include <ESP8266WebServer.h>
#include "DHTesp.h"

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

DHTesp dht;

  float tempSensor1;
  float humidSensor1;
  
  
  /*Put your SSID & Password*/
  //const char* ssid = "ICP";  // Enter SSID here
  //const char* password = "xihu@123";  //Enter Password here
  //define IP fixed adderess
  const char *ssid = "your-ssid";
const char *password = "your-password";
int IP_1 = 192; int IP_2 = 168; int IP_3 = x; int IP_4 = x2;  int log_size = 300;
  float TempData[3][300];
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
    
    dht.setup(5, DHTesp::DHT22);
    humidSensor1 = dht.getHumidity();
    tempSensor1 = dht.getTemperature();

    for (int i = 0; i <= log_size-1; i++) {
      TempData[0][i] = tempSensor1;
      TempData[1][i] = 0;
      TempData[2][i] = humidSensor1;
    }
    server.begin();
    Serial.println("HTTP server started");
  }
  
  void TempHumid_Measure() {
    int i = 0;
    do {
      int j = 0;
      do {
        TempData[i][log_size - 1 - j] = TempData[i][log_size - 2 - j];
  
        j += 1;
      } while (j < log_size - 1);
      i += 1;
    } while (i < 3);
    
    humidSensor1 = dht.getHumidity();
    tempSensor1 = dht.getTemperature();
    meas_time = millis() - start_time;
    TempData[1][0] = meas_time;
    TempData[0][0] = tempSensor1;
    TempData[2][0] = humidSensor1;
  
  }
  
  void loop() {
    Serial.println(TempData[2][0]);
    Serial.println(TempData[0][0]);
    TempHumid_Measure();
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
    ptr +="<meta http-equiv=\"refresh\" content=\"300\"><meta http-equiv=\"content-type\" content=\"text/html\" charset=\"UTF-8\"> <title>QuantOpt Temperature Lab</title> <meta http-equiv=\"content-type\" content=\"text/html\" charset=\"UTF-8\"> <meta name=\"robots\" content=\"noindex, nofollow\"> <meta name=\"googlebot\" content=\"noindex, nofollow\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <script type=\"text/javascript\" src=\"/js/lib/dummy.js\"></script> <link rel=\"stylesheet\" type=\"text/css\" href=\"/css/result-light.css\"> <style id=\"compiled-css\" type=\"text/css\"> .highcharts-figure, .highcharts-data-table table { min-width: 310px; max-width: 1300px; margin: 1em auto;} #container { display: flex; height: 90vh; } .highcharts-data-table table { font-family: Verdana, sans-serif; border-collapse: collapse; border: 1px solid #EBEBEB; margin: 10px auto; text-align: center; width: 100%; /* max-width: 500px; */ max-width: 1000px; } .highcharts-data-table caption { padding: 1em 0; font-size: 1.2em; color: #555; } .highcharts-data-table th { font-weight: 600; padding: 0.5em; } .highcharts-data-table td, .highcharts-data-table th, .highcharts-data-table caption { padding: 0.5em; } .highcharts-data-table thead tr, .highcharts-data-table tr:nth-child(even) { background: #f8f8f8; } .highcharts-data-table tr:hover { background: rgb(118, 124, 226); } </style>";
    ptr +="<script id=\"insert\"></script> </head>";
    ptr +="<body>";
    ptr +="<script src=\"https://code.highcharts.com/highcharts.js\"></script> <script src=\"https://code.highcharts.com/modules/exporting.js\"></script> <script src=\"https://code.highcharts.com/modules/export-data.js\"></script> <script src=\"https://code.highcharts.com/modules/accessibility.js\"></script> <figure class=\"highcharts-figure\"><div id=\"container\"></div></figure> <script type=\"text/javascript\"> Highcharts.chart(\"container\", { credits: { enabled: false }, time: { timezoneOffset: -2 * 60 }, chart: { zoomType: \"xy\" }, title: { text: \"QuantOpt Temp\", style: { fontSize:\"25px\", color: Highcharts.getOptions().colors[7]}}, subtitle: { text: \"Made by Danae Savvidi © :)\" }, xAxis: [{ formatter: function() { return Highcharts.dateFormat(\"%Y-%m-%d\", this.value); }, type: \"datetime\",  labels: {style: { fontSize:\"13px\", color: Highcharts.getOptions().colors[7] } }, title: { text: \"Time\", style: {  fontSize:\"16px\", color: Highcharts.getOptions().colors[7] } }, crosshair: true }], yAxis: [{ labels: { format: \"{value}°C\", style: { fontSize:\"16px\",color: Highcharts.getOptions().colors[5] } }, title: { text: \"Temperature\", style: { fontSize:\"20px\",color: Highcharts.getOptions().colors[5] } } }, { title: { text: \"Humidity\", style: { fontSize:\"20px\", color: Highcharts.getOptions().colors[4] } }, labels: { format: \"{value} %\", style: { fontSize:\"16px\",color: Highcharts.getOptions().colors[4] } }, opposite: true }], tooltip: { shared: true }, legend: { layout: \"vertical\", align: \"left\", x: 120, verticalAlign: \"top\", y: 100, floating: true, backgroundColor: Highcharts.defaultOptions.legend.backgroundColor || \"rgba(255,255,255,0.25)\" }, series: [{ color: Highcharts.getOptions().colors[4], name: \"Humidity\", type: \"spline\", yAxis: 1, ";
    ptr +="data: [ ";
    int j=log_size-1;
        do {
        ptr +="\n[(new Date()).getTime()+"; ptr +=TempData[1][j]-TempData[1][0]; ptr +=", "; ptr +=TempData[2][j]; ptr +="],";
      j -= 1;
        } while (j > -1);
    ptr +="\n],";
    
    ptr +="tooltip: { valueSuffix: \" %\" }, }, { color: Highcharts.getOptions().colors[5],name: \"Temperature\", type: \"spline\",";
    ptr +="data: [ ";
    j=log_size-1;
        do {
        ptr +="\n[(new Date()).getTime()+"; ptr +=TempData[1][j]-TempData[1][0]; ptr +=", "; ptr +=TempData[0][j]; ptr +="],";
      j -= 1;
        } while (j > -1); 
    ptr +="\n],";
    ptr +="tooltip: { valueSuffix: \"°C\" } }] }); </script>";
    ptr +="<script> \n if (window.parent && window.parent.parent){ window.parent.parent.postMessage([\"resultsFrame\", { height: document.body.getBoundingClientRect().height, slug: \"\" }], \"*\") }  ";
    ptr +="\n window.name = \"result\" </script> ";
    ptr +="</body> </html> ";
      return ptr;
    }
