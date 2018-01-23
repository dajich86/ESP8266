#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

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
  
  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, LOW);
  pinMode(gpio2_pin, OUTPUT);
  digitalWrite(gpio2_pin, LOW);

  delay(500);
  
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
}
 
void loop(void){
  server.handleClient();
} 
