#include <ESP8266WiFi.h>

const char *ssid = "your-ssid";
const char *password = "your-password";
int IP_1 = 192; int IP_2 = 168; int IP_3 = x; int IP_4 = x2;

//const char* ssid = "ICP";
//const char* password = "xihu@123";

WiFiServer server(80);
IPAddress local_IP(IP_1,IP_2,IP_3,IP_4);
IPAddress gateway(IP_1,IP_2,IP_3,1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional
void setup() {
  Serial.begin(115200);
  delay(1000);
 WiFi.setSleepMode(WIFI_NONE_SLEEP);
// WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  digitalWrite(D1, 1);
  digitalWrite(D2, 1);
  digitalWrite(D6, 1);
  digitalWrite(D7, 1);
   
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
// Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int pin1;
  int pin2;  
  int pin6;
  int pin7;
  String m;

  if (req.indexOf("/D1/0") != -1){
    m = "Chiller OFF";
    digitalWrite(D1, 1);
  }
  else if (req.indexOf("/D1/1") != -1){
    m = "Chiller ON";
    digitalWrite(D1, 0);
  }
  else if (req.indexOf("/D2/0") != -1){
    m = "Pump OFF";
    digitalWrite(D2, 1);
  }
  else if (req.indexOf("/D2/1") != -1){
    m = "Pump ON";
    digitalWrite(D2, 0);
  }
  else if (req.indexOf("/D6/0") != -1){
    m = "Laser OFF";
    digitalWrite(D6, 1);
  }
  else if (req.indexOf("/D6/1") != -1){
    m = "Laser ON";
    digitalWrite(D6, 0);
     }
  else if (req.indexOf("/D7/0") != -1){
    m = "Light OFF";
    digitalWrite(D7, 1);
  }
   else if (req.indexOf("/D7/1") != -1){
    m = "Light ON";
    digitalWrite(D7, 0);
    }
   else if (req.indexOf("/I") != -1){
    m = "Initialized";
  }
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  pin1 = 1- digitalRead(D1);
  pin2 = 1- digitalRead(D2);
  pin6 = 1- digitalRead(D6);
  pin7 = 1- digitalRead(D7);

    String ChillerStatus ;
    String PumpStatus ;
    String LaserStatus ;
    String LightStatus ;
    
  client.flush();

  // Prepare the response
String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
    s +="<head><meta http-equiv=\"Access-Control-Allow-Origin\" name=\"viewport\" content=\"width=device-width, initial-scale=1.0 , user-scalable=no\">\n" +m+ "      Pin 1 " +pin1+"  Pin2 " + pin2+"  Pin6 "+pin6+ "  Pin7 "+pin7;
    //title is Quantum Optoelectronics Lab Control Center
    s +="<title>Quantum Optoelectronics Lab Control Center</title>\n";

    //Customizing ON (kikigreen) and OFF (kikired) Buttons and lining them
    s +="<style> .kikigreen { color: green; padding: 5px;text-align: center;margin-right:35px; font-size: 150%; background-color: white; text-decoration: none; border-radius: 12px; cursor: pointer; }\n";
    s +=".kikired { color: red; padding: 5px; text-align: center; background-color: white; font-size: 150%; text-decoration: none; border-radius: 12px; cursor: pointer;}\n";
    s +="h2 { font-size: 150%; text-decoration: none;}\n";
    s +=".kiki { display: inline-block; }\n";
    s +="</style>\n";

    //Body
    s +="<body>\n";
    s +="<div style=\"font-family: Helvetica; text-align: center; width: 100%; \"><b>\n";
    
    //Title
    s +="<h1>QuantOpt Lab Control Center</h1>\n";
    
    //er
    s +="<div class=\"kiki\">";
    s +="<h2 class=\"kiki\" style=\"margin-right:40px;\" >Chiller: </h2>";
    s +="<form class=\"kiki\" action=\"http://192.168.1.196:80/D1/1\"><input class=\"kikigreen\" type=\"submit\" value=\"ON\"/></form>";
    s +="<form class=\"kiki\" action=\"http://192.168.1.196:80/D1/0\"><input class=\"kikired\" type=\"submit\" value=\"OFF\"/></form>";
    s +="</div><br>";
    
    //Pump
    s +="<div class=\"kiki\">";
    s +="<h2 class=\"kiki\" style=\"margin-right:40px;\" >Pump: </h2>";
    s +="<form class=\"kiki\" action=\"http://192.168.1.196:80/D2/1\"><input class=\"kikigreen\" type=\"submit\" value=\"ON\"/></form>";
    s +="<form class=\"kiki\" action=\"http://192.168.1.196:80/D2/0\"><input class=\"kikired\" type=\"submit\" value=\"OFF\"/></form>";
    s +="</div><br>";

    //Laser
    s +="<div class=\"kiki\">";
    s +="<h2 class=\"kiki\" style=\"margin-right:40px;\" >Laser: </h2>";
    s +="<form class=\"kiki\" action=\"http://192.168.1.196:80/D6/1\"><input class=\"kikigreen\" type=\"submit\" value=\"ON\"/></form>";
    s +="<form class=\"kiki\" action=\"http://192.168.1.196:80/D6/0\"><input class=\"kikired\" type=\"submit\" value=\"OFF\"/></form>";
    s +="</div><br>";

    //Light
    s +="<div class=\"kiki\">";
    s +="<h2 class=\"kiki\" style=\"margin-right:40px;\" >Light: </h2>";
    s +="<form class=\"kiki\" action=\"http://192.168.1.196:80/D7/1\"><input class=\"kikigreen\" type=\"submit\" value=\"ON\"/></form>";
    s +="<form class=\"kiki\" action=\"http://192.168.1.196:80/D7/0\"><input class=\"kikired\" type=\"submit\" value=\"OFF\"/></form>";
    s +="</div><br>";
    
    //Author
    s +="<h3>Danae Savvidi :)</h3>";
    s +="</b></div>";
    s +="</body>";
    s +="</html>";
     
     // Send the response to the client
  client.print(s);
  delay(100);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
