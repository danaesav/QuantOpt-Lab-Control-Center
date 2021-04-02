/*  HOW TO RUN
 *  
 * STEP 1: UNCOMMENT LINE 67 AND LINE 72
 * STEP 2: RUN AND UPLOAD ONE TIME
 * STEP 3: COMMENT LINE 69 AND LINE 73
 * STEP 4: UPLOAD AGAIN AND IT WILL WORK
 * 
 * NOTES:
 * 1. DONT FORGET TO CHANGE IP, SSID AND PASS. PASS FOR HOMEKIT SET UP IS 111-111-111
 * 2. IF YOU WISH TO CHANGE THE QUANTITY OF VALUES GO TO LINES 55-56 AND CHANGE 300 TO ANY VALUE YOU LIKE
 * 3. TO CHANGE TIMEZONE GO TO LINE 33 AND EDIT 3600. FOR SHANGHAI TIME IT SHOULD BE 3600*6
 * 
 * THANKS FOR USING MY PROGRAM - DANAE
 */

#include <ESP8266WebServer.h>
#include "DHTesp.h"
#include <ESP8266WiFi.h>
#include <FS.h>
#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <arduino_homekit_server.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D2
OneWire oneWire(ONE_WIRE_BUS);  
DallasTemperature sensors(&oneWire);
#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);
 
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 0);

DHTesp dht;
  
  int IP_1 = 192; int IP_2 = 168; int IP_3 = x; int IP_4 = x2; //Change IP
  const char* ssid = "SSID";  // Enter SSID here
  const char* password = "PASS";  //Enter Password here
  ESP8266WebServer server(80);
  IPAddress local_IP(IP_1, IP_2, IP_3, IP_4);
  IPAddress gateway(IP_1, IP_2, IP_3, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress primaryDNS(8, 8, 8, 8);   //optional
  IPAddress secondaryDNS(8, 8, 4, 4); //optional

const char* timeFile = "/timeFile.txt";
const char* tempFile = "/tempFile.txt";
const char* humidFile = "/humidFile.txt";
unsigned long epochTime;

String buffer;
File f;
int i;
int log_size = 300;
float TempData[3][300];
float test;
float tempSensor1;
float humidSensor1;
uint8_t sensor1[8] = { 0x28, 0x50, 0x74, 0x8D, 0x3C, 0x19, 0x01, 0x63 };

void setup() {
  Serial.begin(115200);
  timeClient.begin();
  wifi_connect();
  initialize();  //Initialize File System
  //formatFileSystem();  //Format File System used just once
  //homekit_storage_reset(); //EXTREMELY VALUABLE DO NOT DELETE
  my_homekit_setup();

  //BEFORE SECTION - initiallize current data in temp and time tables
  //resetvalues();
  //BEFORE SECTION

  server.begin();
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
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

    timeClient.forceUpdate(); //Sync time
    epochTime = timeClient.getEpochTime(); //Get current time
    while (epochTime <= test) { // Check that it is not the same as previous one
      int z = 0;
      do {
        z += 1;
        server.handleClient();
        my_homekit_loop();
        delay(100);
      } while (z < 50);
      Serial.println("Waiting.. 2");
      timeClient.forceUpdate();
      epochTime = timeClient.getEpochTime();
    }
    dht.setup(5, DHTesp::DHT22);
    humidSensor1 = dht.getHumidity();
    tempSensor1 = dht.getTemperature();
    test = (float)epochTime;
    TempData[1][0] = test;
    TempData[0][0] = tempSensor1;
    TempData[2][0] = humidSensor1;
  
  }
  
  void loop() {
    allfilereadTime(); //Copies time data from flash memory to Time[max], max being the total number of recorded data
    allfilereadTemp(); //Copies temp data from flash memory to Temp[max], max being the total number of recorded data
    allfilereadHum();
    
    int z = 0;
    do {
      z += 1;
      server.handleClient();
      my_homekit_loop();
      delay(840);
    } while (z < 400);
    
    TempHumid_Measure();
    updateFlashData();
  }
  
  void handle_OnConnect() {
    server.send(200, "text/html", SendHTML(tempSensor1));
  }
  
  void handle_NotFound() {
    server.send(404, "text/plain", "Not found");
  }


  void updateFlashData() {
  filewriteTime(); //Updates time data on flash memory
  filewriteTemp(); //Updates temp data on flash memory
  filewriteHum();
}

void allfilereadTemp() { //Reads data from temp file in flash memory
  File f = SPIFFS.open(tempFile, "r");

  if (!f) {
    Serial.println("file open failed");
  } else {
      Serial.println("Reading Data from TEMPFile");
      i=0;
      while (f.available()) {
          char latStream[15];
         f.readBytesUntil('\n', latStream, 15);
         float neww = atof(latStream);
        TempData[0][i]=neww;
        i++;
      }
   f.close();
  }
}

void allfilereadHum() { //Reads data from temp file in flash memory
  File f = SPIFFS.open(humidFile, "r");

  if (!f) {
    Serial.println("file open failed");
  } else {
      Serial.println("Reading Data from HumFile");
      i=0;
      while (f.available()) {
          char latStream[15];
         f.readBytesUntil('\n', latStream, 15);
         float neww = atof(latStream);
        TempData[2][i]=neww;
        i++;
      }
   f.close();
  }
}

void allfilereadTime() { //Reads data from time file in flash memory
  File f = SPIFFS.open(timeFile, "r");

  if (!f) {
    Serial.println("file open failed");
  } else {
      Serial.println("Reading Data from TIMEFile");
      i=0;
      while (f.available()) {
        buffer = f.readStringUntil('\n');
        int neww = buffer.toInt();
        TempData[1][i]=neww;
        i++;
      }
   f.close();
  }
}


void filewriteTemp() { //Writes temp data in flash memory
  File f = SPIFFS.open(tempFile, "w");
  if (!f) {
    Serial.println("file open failed");
  } else {
      for (i = 0; i <= log_size-1; i++) {
        f.println(TempData[0][i]); //Copying the TempData to the file
      }
      Serial.println("Writing Data to File");
  f.close(); 
  }
}

void filewriteHum() { //Writes temp data in flash memory
  File f = SPIFFS.open(humidFile, "w");
  if (!f) {
    Serial.println("file open failed");
  } else {
      for (i = 0; i <= log_size-1; i++) {
        f.println(TempData[2][i]); //Copying the TempData to the file
      }
      Serial.println("Writing Data to File");
  f.close(); 
  }
}

void filewriteTime() { //Writes time data in flash memory
  File f = SPIFFS.open(timeFile, "w");
  if (!f) {
    Serial.println("file open failed");
  } else {
      for (i = 0; i <= log_size-1; i++) {
        f.println(TempData[1][i]); //Copying the TempData to the file
      }
      Serial.println("Writing Data to File");
  f.close(); 
  }
}


void CURwriteTemp(float tempval) { //initialize flash memory
  File f = SPIFFS.open(tempFile, "w");
  
  if (!f) {
    Serial.println("file open failed");
  } else {
    Serial.println("Writing Data to File");
    for (i = 0; i <= log_size-1; i++) {
      f.println(tempval);
    }
  f.close(); 
  }
}

void CURwriteHum(float humidval) { //initialize flash memory
  File f = SPIFFS.open(humidFile, "w");
  
  if (!f) {
    Serial.println("file open failed");
  } else {
    Serial.println("Writing Data to File");
    for (i = 0; i <= log_size-1; i++) {
      f.println(humidval);
    }
  f.close(); 
  }
}

void CURwriteTime(float epochT) { //initialize flash memory
  File f = SPIFFS.open(timeFile, "w");
  
  if (!f) {
    Serial.println("file open failed");
  } else {
    Serial.println("Writing Data to File");
    for (i = 0; i <= log_size-1; i++) {
      f.println(epochT);
    }
  f.close(); 
  }
}


void formatFileSystem() { //Formatting...
    if(SPIFFS.format()) {
    Serial.println("File System Formated");
  } else {
    Serial.println("File System Formatting Error");
  }
}

void initialize() { //Initializing...
  if(SPIFFS.begin()) {
    Serial.println("SPIFFS Initialize....ok");
  } else {
    Serial.println("SPIFFS Initialization...failed");
  }
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
        ptr +="\n["; ptr +=TempData[1][j]*1000; ptr +=", "; ptr +=TempData[2][j]; ptr +="],";
      j -= 1;
        } while (j > -1);
    ptr +="\n],";
    
    ptr +="tooltip: { valueSuffix: \" %\" }, }, { color: Highcharts.getOptions().colors[5],name: \"Temperature\", type: \"spline\",";
    ptr +="data: [ ";
    j=log_size-1;
        do {
        ptr +="\n["; ptr +=TempData[1][j]*1000; ptr +=", "; ptr +=TempData[0][j]; ptr +="],";
      j -= 1;
        } while (j > -1); 
    ptr +="\n],";
    ptr +="tooltip: { valueSuffix: \"°C\" } }] }); </script>";
    ptr +="<script> \n if (window.parent && window.parent.parent){ window.parent.parent.postMessage([\"resultsFrame\", { height: document.body.getBoundingClientRect().height, slug: \"\" }], \"*\") }  ";
    ptr +="\n window.name = \"result\" </script> ";
    ptr +="</body> </html> ";
      return ptr;
    }


void resetvalues() {
  dht.setup(5, DHTesp::DHT22);
    humidSensor1 = dht.getHumidity();
    tempSensor1 = dht.getTemperature();
    timeClient.forceUpdate();
    epochTime = timeClient.getEpochTime();
    while (epochTime < 161653056)  {
      delay(500);
      Serial.println("Waiting.. 1");
      timeClient.forceUpdate();
      epochTime = timeClient.getEpochTime();
    }
    test=(float)epochTime;
    CURwriteHum(humidSensor1);
    CURwriteTemp(tempSensor1);
    CURwriteTime(test);
}

void wifi_connect() {
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
}



//==============================
// Homekit setup and loop
//==============================

//homekit characteristics in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_current_temperature;
extern "C" homekit_characteristic_t cha_humidity;

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

void my_homekit_setup() {
  arduino_homekit_setup(&config);
}

void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();
  if (t > next_report_millis) {
    // report sensor values every 10 seconds
    next_report_millis = t + 10 * 1000;
    my_homekit_report();
  }
  if (t > next_heap_millis) {
    // show heap info every 5 seconds
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
        ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

  }
}

void my_homekit_report() {
  dht.setup(5, DHTesp::DHT22);
  humidSensor1 = dht.getHumidity();
  tempSensor1 = dht.getTemperature();
  float temperature_value = tempSensor1;
  float humid_value = humidSensor1;
  
  cha_current_temperature.value.float_value = temperature_value;
  LOG_D("Current temperature: %.1f", temperature_value);
  homekit_characteristic_notify(&cha_current_temperature, cha_current_temperature.value);

  cha_humidity.value.float_value = humid_value;
  LOG_D("Current humidity: %.1f", humid_value);
  homekit_characteristic_notify(&cha_humidity, cha_humidity.value);
}
