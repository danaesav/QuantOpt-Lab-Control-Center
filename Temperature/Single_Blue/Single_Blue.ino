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
const char *ssid = "your-ssid";
const char *password = "your-password";
int IP_1 = 192; int IP_2 = 168; int IP_3 = x; int IP_4 = x2;
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
  TempData[1][0] = meas_time / 60000/60;
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
  String ptr = "<!DOCTYPE HTML> <html>\n";
  ptr += "<head><meta http-equiv=\"refresh\" name=\"viewport\" content=\"180\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>QuantOpt Lab Temperature</title>\n";
  ptr += "<script type=\"text/javascript\" src=\"https://www.gstatic.com/charts/loader.js\"></script>";
  ptr += "<script type=\"text/javascript\">";
  ptr += "google.charts.load(\"current\", {packages: [\"corechart\", \"line\"]});";
  ptr += "google.charts.setOnLoadCallback(drawAxisTickColors);";
  ptr += "function drawAxisTickColors() {";
  ptr += "var data = new google.visualization.DataTable();";
  ptr += "data.addColumn(\"number\", \"Time\");";
  ptr += "data.addColumn(\"number\", \"Temp\");";
  ptr += "data.addRows([";
  int j = 0;
  do {
    ptr += "["; ptr += TempData[1][j]-TempData[1][0]; ptr += ","; ptr += TempData[0][j]; ptr += "]";
    if (j < log_size - 1) {
      ptr += ", ";
    }
    j += 1;
  } while (j < log_size);
  ptr += "]);";
  ptr += "var options = {";
  ptr += "lineWidth: 6, title: \"Temp Chart\", titleTextStyle: {color: \'#1a237e\',fontName: \'Arial\',fontSize: 36, bold: true, italic: true}, width: 1200, height: 700,";
  ptr += "hAxis: {  title: \'Time (H)\',  textStyle: {color: \'#01579b\',fontSize: 20,fontName: \'Arial\', bold: true,    italic: true  },";
  ptr += "textStyle: {color: \'#1a237e\',fontSize: 26,bold: true, fontName: \'Arial\'  },";
  ptr += "titleTextStyle: {color: \'#1a237e\',fontSize: 26,fontName: \'Arial\',bold: true,italic: true  }},";
  ptr += "vAxis: {title: \'Temperature (C)\',";
  ptr += "textStyle: {color: \'#1a237e\',fontSize: 26,bold: true, fontName: \'Arial\'  },";
  ptr += "titleTextStyle: {color: \'#1a237e\',fontSize: 26,fontName: \'Arial\',bold: true,italic: true  }},colors: [\'#a52714\', \'#097138\']  };";
  
  ptr += "var chart = new google.visualization.LineChart(document.getElementById(\"chart_div\"));";
  ptr += "chart.draw(data, options);}";
  ptr += "</script>";
  ptr += "</head>";
  ptr += "<body style=\"text-align: center;\">";
//  ptr += "<h1 style=\" text-align: center; color: blue;\">Tempreture Graph</h1>";
  ptr += "<script type=\"text/javascript\" src=\"https://www.gstatic.com/charts/loader.js\"></script>";
  ptr += "<div id=\"chart_div\"></div>";
  ptr += "</body>";
  ptr += "</html>";
  return ptr;

}
