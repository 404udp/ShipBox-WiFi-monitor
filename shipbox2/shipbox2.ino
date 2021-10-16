#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#ifndef APSSID
#define APSSID "combox"
#define APPSK  "combox124"
#endif //Монитор для телефона/планшета показівающий переданнв COMпорт данные
/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;
const byte DNS_PORT = 53;
const int com_speed=74880;
DNSServer dnsServer;
IPAddress apIP(192, 168, 4, 1);
String responseHTML = ""
                      "<!DOCTYPE html><html><head><title>ShipBox portal</title></head><body>"
                      "<h1>PROTAL COMBOX!</h1><p>out parametrs shipbox (comport) </p></body></html>";
String teststr="";

ESP8266WebServer server(80);
/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup() {
  delay(1000);
  Serial.begin(com_speed);
  Serial.println(); Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.mode(WIFI_AP); WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);
  //IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: "); Serial.println(apIP);
  //server.on("/", handleRoot);  //server.begin(); // replay to all requests with same HTML // server.onNotFound([]() {server.send(200, "text/html", responseHTML);}); 
  server.onNotFound([]() {server.send(200, "text/html", SendHTML("ok"));}); 
  server.begin();
  Serial.println("HTTP server started");
}
void handle_OnConnect() 
{  
  Serial.println("");
  server.send(200, "text/html", SendHTML("ok") ); 
}
void loop() {
  dnsServer.processNextRequest();   
  server.handleClient();
}
//String SendHTML(uint8_t ok)
String SendHTML(String ok)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head> <meta http-equiv='refresh' content='11'/>";
  ptr +="<title>COMBOX-data-SHIP</title>\n";
  ptr +="<body><H2>Data from COM-port</H2><HR><H1>";
   // если приходят данные из компьютера - отправим их в Wi-Fi модуль
  if (Serial.available()) {   // WIFI_SERIAL.write(Serial.read());
 teststr=Serial.readString(); Serial.println(teststr); }
  ptr +=teststr;
  ptr +="</H1></body>\n";ptr +="</html>\n";
  return ptr;
}
