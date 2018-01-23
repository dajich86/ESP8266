#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <DNSServer.h>
#include <WiFiManager.h>  

MDNSResponder mdns;

// Replace with your network credentials
//const char* ssid = "Nito";
//const char* password = "doremifa";

ESP8266WebServer server(80);

String webPage = "";

int gpio0_pin = 0;
int gpio2_pin = 2;

void setup(void){
WiFiManager wifiManager;
wifiManager.autoConnect("REEF Controller Setup");
  
webPage += "<!DOCTYPE html>";
webPage += "<html lang='en'>";
webPage += "<head>";
webPage += "  <title>REEF Controller</title>";
webPage += "  <meta charset='utf-8'>";
webPage += "  <meta name='viewport' content='width=device-width, initial-scale=1'>";
webPage += "  <link href='https://maxcdn.bootstrapcdn.com/bootswatch/3.3.7/darkly/bootstrap.min.css' rel='stylesheet'>        ";
webPage += "  <script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>";
webPage += "  <script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>";
webPage += "</head>";
webPage += "<body>";
webPage += "<div class= 'well' 'jumbotron'>";
webPage += "    <h1 class='text-center'>REEF Controller</h1>";
webPage += "    <h3 class ='text-left'>Salida 1</h3>   ";
webPage += "  <a href=\'socket1On\'><button type='button' class='btn btn-success btn-lg btn-block'>Encender</button></a> ";
webPage += "  <h6 class='text-center'></h6>";
webPage += "  <a href=\'socket1Off\'><button type='button' class='btn btn-danger btn-lg btn-block'>Apagar</button></a>   ";
webPage += "    <h3 class ='text-left'>Salida 2</h3> ";
webPage += "  <a href=\'socket2On\'><button type='button' class='btn btn-success btn-lg btn-block'>Encender</button></a>";
webPage += "  <h6 class='text-center'></h6>";
webPage += "  <a href=\'socket2Off\'><button type='button' class='btn btn-danger btn-lg btn-block'>Apagar</button></a>";
webPage += "    <h4 class='text-center'>www.reefacuario.com</h4>";
webPage += "</div>";
webPage += "</body>";
webPage += "</html>";



  //webPage += "<h1>ESP8266 Web Server</h1><p>Socket #1 <a href=\"socket1On\"><button>ON</button></a>&nbsp;<a href=\"socket1Off\"><button>OFF</button></a></p>";
  //webPage += "<p>Socket #2 <a href=\"socket2On\"><button>ON</button></a>&nbsp;<a href=\"socket2Off\"><button>OFF</button></a></p>";
  
  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, LOW);
  pinMode(gpio2_pin, OUTPUT);
  digitalWrite(gpio2_pin, LOW);
  
  delay(1000);
  Serial.begin(115200);
  //WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  //Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  server.on("/socket1On", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio0_pin, LOW);
    delay(1000);
  });
  server.on("/socket1Off", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio0_pin, HIGH);
    delay(1000); 
  });
  server.on("/socket2On", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio2_pin, LOW);
    delay(1000);
  });
  server.on("/socket2Off", [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio2_pin, HIGH);
    delay(1000); 
  });
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 
