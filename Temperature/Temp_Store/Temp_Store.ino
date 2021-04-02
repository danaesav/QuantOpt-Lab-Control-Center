/*  HOW TO RUN
 *  
 * STEP 1: GO TO LINE 69 AND UNCOMMENT IT
 * STEP 2: GO TO LINES 73-87 AND UNCOMMENT THEM
 * STEP 3: UPLOAD ONE TIME TILL A TABLE WITH THE SAME VALUES APPEARS IN SERIAL PORT
 * STEP 4: GO TO LINES 73-87 AND COMMENT THEM
 * STEP 5: GO TO LINE 69 AND COMMENT IT
 * STEP 6: UPLOAD AGAIN AND IT WILL WORK
 * 
 * NOTES:
 * 1. DONT FORGET TO CHANGE IP, SSID AND PASS
 * 2. IF YOU WISH TO CHANGE THE QUANTITY OF VALUES GO TO LINES 52-54 AND CHANGE 100 TO ANY VALUE YOU LIKE
 * 3. IF YOU WISH TO CHANGE THE TIME INBETWEEN VALUE READINGS GOOD LUCK :)
 * 
 * THANKS FOR USING MY PROGRAM - DANAE
 */


#include <ESP8266WiFi.h>
#include <FS.h>
#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D2
OneWire oneWire(ONE_WIRE_BUS);  
DallasTemperature sensors(&oneWire);
#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);
 
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

int IP_1 = 192; int IP_2 = 168; int IP_3 = 0; int IP_4 = 188;
const char *ssid = "TP_Crete";
const char *password = "1234567890123";
IPAddress local_IP(IP_1, IP_2, IP_3, IP_4);
IPAddress gateway(IP_1, IP_2, IP_3, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

const char* timeFile = "/timeFile.txt";
const char* tempFile = "/tempFile.txt";
unsigned long epochTime;

String buffer;
File f;
ESP8266WebServer server(80);
int i;
int log_size = 100;
float Temp[100];
float Time[100];
float test;
float tempSensor1;
uint8_t sensor1[8] = { 0x28, 0x50, 0x74, 0x8D, 0x3C, 0x19, 0x01, 0x63 };
  
void setup() {
  delay(1000);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  Serial.begin(115200);
  sensors.begin();
  timeClient.begin();
  timeClient.update();
  wifi_connect();
  initialize();  //Initialize File System
  //formatFileSystem();  //Format File System used just once

  //BEFORE SECTION - initiallize current data in temp and time tables

  /* 
  sensors.requestTemperatures();
  tempSensor1 = sensors.getTempC(sensor1);
  timeClient.update();
  epochTime = timeClient.getEpochTime();
  while (epochTime < 161653056) {
    Serial.println("Waiting.. 1");
    timeClient.update();
    epochTime = timeClient.getEpochTime();
    delay(500);
  }
  test=(float)epochTime;
  CURwriteTemp(tempSensor1);
  CURwriteTime((float)epochTime);
   */

  //BEFORE SECTION

  server.begin();
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  Serial.println("HTTP server started");
  
}

void loop() {
  allfilereadTime(); //Copies time data from flash memory to Time[max], max being the total number of recorded data
  allfilereadTemp(); //Copies temp data from flash memory to Temp[max], max being the total number of recorded data

  int z = 0;
    do {
      z += 1;
      server.handleClient();
      delay(500);
    } while (z < 60);

  TempMeasure(); // Get new values
  updateFlashData(); // Write table data to flash memory
  checkifworks(); // Printing stuff
}

void TempMeasure() {
    int j = 0;
    do {
      Time[log_size - 1 - j] = Time[log_size - 2 - j];
      Temp[log_size - 1 - j] = Temp[log_size - 2 - j];
      j += 1;
    } while (j < log_size - 1);
      i += 1;
    timeClient.update(); //Sync time
    epochTime = timeClient.getEpochTime(); //Get current time
    while (epochTime <= test) { // Check that it is not the same as previous one
      Serial.println("Waiting.. 2");
      timeClient.update();
      epochTime = timeClient.getEpochTime();
      server.handleClient();
      delay(5000);
    }
    sensors.requestTemperatures();
    tempSensor1 = sensors.getTempC(sensor1); // Gets the values of the temperature
    Time[0] = (float)epochTime; //Update tables
    Temp[0] = tempSensor1;
    test = Time[0];
}

void updateFlashData() {
  filewriteTime(); //Updates time data on flash memory
  filewriteTemp(); //Updates temp data on flash memory
}

void allfilereadTemp() { //Reads data from temp file in flash memory
  File f = SPIFFS.open(tempFile, "r");

  if (!f) {
    Serial.println("file open failed");
  } else {
      Serial.println("Reading Data from TEMPFile");
      i=0;
      while (f.available()) {
          char latStream[10];
         f.readBytesUntil('\n', latStream, 10);
         float neww = atof(latStream);
        Temp[i]=neww;
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
        Time[i]=neww;
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
        f.println(Temp[i]); //Copying the TempData to the file
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
        f.println(Time[i]); //Copying the TempData to the file
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


void checkifworks() { //Printing stuff
  Serial.println("");
  Serial.println("Current RAM data");
  Serial.println("");
  for (i = 0; i <= log_size-1; i++) {
    Serial.print("Temp: ");
    Serial.print(Temp[i]);
    Serial.print("\t");
    Serial.print("Time: ");
    Serial.println(Time[i]);
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

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(tempSensor1));
}
  
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

//WEBSITE
  String SendHTML(float tempSensor1) {
    String ptr ="<!DOCTYPE html> <html> <head> <meta http-equiv=\"refresh\" content=\"300\"> <meta http-equiv=\"content-type\" content=\"text/html\" charset=\"UTF-8\"> <meta http-equiv=\"content-type\" content=\"text/html\" charset=\"UTF-8\"> <meta name=\"robots\" content=\"noindex, nofollow\"> <meta name=\"googlebot\" content=\"noindex, nofollow\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <script type=\"text/javascript\" src=\"/js/lib/dummy.js\"></script> <script src=\"https://code.highcharts.com/highcharts.js\"></script> <script src=\"https://code.highcharts.com/modules/exporting.js\"></script> <script src=\"https://code.highcharts.com/modules/export-data.js\"></script> <script src=\"https://code.highcharts.com/modules/accessibility.js\"></script> <link rel=\"stylesheet\" type=\"text/css\" href=\"/css/result-light.css\"> <title>QuantOpt Temperature Lab</title> <style id=\"compiled-css\" type=\"text/css\"> .highcharts-figure, .highcharts-data-table table { min-width: 310px; max-width: 1300px; margin: 1em auto; }";
    ptr +="#container { display: flex; height: 90vh; }";
    ptr +=".highcharts-data-table table { font-family: Verdana, sans-serif; border-collapse: collapse; border: 1px solid #EBEBEB; margin: 10px auto; text-align: center; width: 100%; max-width: 1000px; }";
    ptr +=".highcharts-data-table caption { padding: 1em 0; font-size: 1.2em; color: #555; }";
    ptr +=".highcharts-data-table th { font-weight: 600; padding: 0.5em; }";
    ptr +=".highcharts-data-table td, .highcharts-data-table th, .highcharts-data-table caption { padding: 0.5em; }";
    ptr +=".highcharts-data-table thead tr, .highcharts-data-table tr:nth-child(even) { background: #f8f8f8; }";
    ptr +=".highcharts-data-table tr:hover { background: rgb(118, 124, 226); } </style> </head>";
    ptr +="<body> <figure class=\"highcharts-figure\"> <div id=\"container\"></div> </figure>";
    ptr +="<script type=\"text/javascript\"> Highcharts.chart(\"container\", { credits: { enabled: false }, time: { timezoneOffset: -2 * 60 }, chart: { zoomType: \"xy\" }, title: { text: \"QuantOpt Temp\", style: { fontSize: \"25px\", color: Highcharts.getOptions().colors[7] } }, subtitle: { text: \"Made by Danae Savvidi © :)\" }, xAxis: [{ formatter: function () { return Highcharts.dateFormat(\"%Y-%m-%d\", this.value); }, type: \"datetime\", labels: { style: { fontSize: \"13px\", color: Highcharts.getOptions().colors[7] } }, title: { text: \"Time\", style: { fontSize: \"16px\", color: Highcharts.getOptions().colors[7] } }, crosshair: true }], yAxis: [{ labels: { format: \"{value}°C\", style: { fontSize: \"16px\", color: Highcharts.getOptions().colors[5] } }, title: { text: \"Temperature\", style: { fontSize: \"20px\", color: Highcharts.getOptions().colors[5] } } }], ";
    ptr +="tooltip: { style: { color: Highcharts.getOptions().colors[5] }, xDateFormat: \"%e %B %Y %H:%M:%S\",";
    ptr +="shared: true }, ";
    
    ptr +="legend: { layout: \"vertical\", align: \"left\", x: 120, verticalAlign: \"top\", y: 100, floating: true, backgroundColor: Highcharts.defaultOptions.legend.backgroundColor || \"rgba(255,255,255,0.25)\" }, series: [{ color: Highcharts.getOptions().colors[5], name: \"Temperature\", type: \"spline\", ";
    
    ptr +="data: [ ";
    int j=0;
    do {
      ptr +="\n["; ptr +=Time[j]*1000;
        ptr +=","; ptr +=Temp[j]; ptr +="], ";
        j += 1;
    } while (j < log_size);
    ptr +="], ";
    ptr +="tooltip: { valueSuffix: \"°C\" } }] }); </script> </body> </html>";
    return ptr;
  }
//WEBSITE

//WIFI
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
//WIFI