
////////////////////////////////////////////////////////
//Hardware
//No hardware connected for this scenario

////////////////////////////////////////////////////////
//Libraries

//WiFi 
//from Arduino Library "WiFi"
//Sample: https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiClient/WiFiClient.ino
#include <WiFi.h>         //für Wifi Verbindung

//Webserver
//from Arduino Library "WebServer"
//Sample: https://github.com/espressif/arduino-esp32/blob/master/libraries/WebServer/examples/WebServer/WebServer.ino
#include <WebServer.h>    //für Websever bereit stellen

////////////////////////////////////////////////////////
//Variables

//Text Strings
String Variable1 = "gelb";

//WiFi Logins
const char* WiFiSSID = "wifi name";
const char* WiFiPW = "12345678900000";

//IP Adress of WIFI
int IP_Teil1 = 192;
int IP_Teil2 = 168;
int IP_Teil3 = 1;
int IP_Teil4 = 107;

//Webserver Port
//Standard Port 80 for Webserver
WebServer server(80);

////////////////////////////////////////////////////////
//HTML Websites

//Content of main page
String HTML_fuer_Startseite(String Variable1){
 String HTML_CODE = "<!DOCTYPE html> <html>\n";
 HTML_CODE +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
 HTML_CODE +="<title>Ampelmanager</title>\n";
 HTML_CODE +="<link rel=\"stylesheet\" href=\"https://use.fontawesome.com/releases/v5.7.2/css/all.css\" integrity=\"sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr\" crossorigin=\"anonymous\">";
 HTML_CODE +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
 HTML_CODE +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
 HTML_CODE +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
 HTML_CODE +="</style>\n";
 HTML_CODE +="</head>\n";
 HTML_CODE +="<body>\n";
 HTML_CODE +="<div id=\"webpage\">\n";
 HTML_CODE +="<h1>Ich bin der Ampel Manager</h1>\n";
 HTML_CODE +="<p>Der Status der Ampel ist: ";
 HTML_CODE +=Variable1;
 HTML_CODE +="</p>";
 HTML_CODE +="</div>\n";
 HTML_CODE +="</body>\n";
 HTML_CODE +="</html>\n";
 return HTML_CODE;
}

//Content of Unterseite1
String HTML_fuer_Unterseite1(){
 String HTML_CODE = "<!DOCTYPE html> <html>\n";
 HTML_CODE +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
 HTML_CODE +="<title>Unterseite</title>\n";
 HTML_CODE +="<link rel=\"stylesheet\" href=\"https://use.fontawesome.com/releases/v5.7.2/css/all.css\" integrity=\"sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr\" crossorigin=\"anonymous\">";
 HTML_CODE +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
 HTML_CODE +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
 HTML_CODE +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
 HTML_CODE +="</style>\n";
 HTML_CODE +="</head>\n";
 HTML_CODE +="<body>\n";
 HTML_CODE +="<div id=\"webpage\">\n";
 HTML_CODE +="<h1>Unterseite zum Testen</h1>\n";
 HTML_CODE +="<p>bla bla: ";
 HTML_CODE +="</p>";
 HTML_CODE +="</div>\n";
 HTML_CODE +="</body>\n";
 HTML_CODE +="</html>\n";
 return HTML_CODE;
}

////////////////////////////////////////////////////////
//Setup Phase

void setup() {

 // Seriellen Monitor starten
 Serial.begin(115200);
 Serial.println("ESP ist gestartet");
 Serial.println();

 // WIFI vorbereiten
 // https://www.arduino.cc/en/Reference/WiFi
 // http://arduino.esp8266.com/Arduino/versions/2.1.0-rc1/doc/libraries.html#wifi-esp8266wifi-library
 // https://github.com/esp8266/Arduino/blob/4897e0006b5b0123a2fa31f67b14a3fff65ce561/doc/esp8266wifi/generic-class.md#persistent
 WiFi.mode(WIFI_STA);
 WiFi.persistent(false);
 WiFi.begin(WiFiSSID, WiFiPW);

 IPAddress ip(IP_Teil1, IP_Teil2, IP_Teil3, IP_Teil4);
 IPAddress gateway(IP_Teil1, IP_Teil2, IP_Teil3, 1);
 IPAddress subnet(255, 255, 255, 0);
 WiFi.config(ip, gateway, subnet);

 Serial.print("Verbindung wird hergestellt ...");

 while (WiFi.status() != WL_CONNECTED)
 {
   delay(500);
   Serial.print(".");
 }

 Serial.println();
 Serial.print("Verbunden als IP-Adresse: ");
 Serial.println(WiFi.localIP());

 // Webserver starten
 server.begin();
 Serial.println("Webserver wurde erfolgreich gestartet!");

 // Zur Information: HTML Status Code: https://en.wikipedia.org/wiki/List_of_HTTP_status_codes

 //Error Handling wenn "not found"
 server.onNotFound([]() {
   server.send(404, "text/plain", "Der Link wurde nicht gefunden du Dulli !");
 });

 //Server Startseite definieren
 //server.on("/", [](){
 //  server.send(200, "text/plain", "ESP8266 Startseite (tts)");
 //});
 server.on("/", handle_startseite);
 server.on("/unterseite1", handle_unterseite1);
 
}

////////////////////////////////////////////////////////
//Loop Phase

void loop() {

 //server.handleClient(); 
if (WiFi.status() == WL_CONNECTED) {

   //Webserver fragt nach, ob es Anfragen gibt
   server.handleClient();
   //Serial.println("Loop: HandleClient()");
 }
 else { 
   // kein WLAN --> nichts tun, nur seriell ausgeben
 }
  delay(1000); //Delay 1000 = 1 Sekunde
}

////////////////////////////////////////////////////////
//Website Handlers

void handle_startseite() {
 Serial.println("Webserver - Startseite wurde aufgerufen.");

 // Sende die Startseite als HTML an den Benutzer
 server.send(200, "text/html", HTML_fuer_Startseite(Variable1)); 
}

void handle_unterseite1() {
 Serial.println("Webserver - Unterseite 1 wurde aufgerufen.");

 // Sende die Startseite als HTML an den Benutzer
 server.send(200, "text/html", HTML_fuer_Unterseite1()); 
}
