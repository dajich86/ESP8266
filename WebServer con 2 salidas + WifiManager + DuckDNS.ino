#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <EasyDDNS.h>

ESP8266WebServer server(80);

String mainPage = "";
String setupPage = "";

int gpio0_pin = 0;
int gpio2_pin = 2;

void setup(void){
WiFiManager wifiManager;
wifiManager.autoConnect("REEF Controller Setup");
  
mainPage += "<!DOCTYPE html>";
mainPage += "<html lang='en'>";
mainPage += "<head>";
mainPage += "  <title>REEF Controller</title>";
mainPage += "  <meta charset='utf-8'>";
mainPage += "  <meta name='viewport' content='width=device-width, initial-scale=1'>";
mainPage += "  <link href='https://maxcdn.bootstrapcdn.com/bootswatch/3.3.7/darkly/bootstrap.min.css' rel='stylesheet'>        ";
mainPage += "  <script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>";
mainPage += "  <script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>";
mainPage += "</head>";
mainPage += "<body>";
mainPage += "<div class= 'well' 'jumbotron'>";
mainPage += "    <h1 class='text-center'>REEF Controller</h1>";
mainPage += "    <h3 class ='text-left'>Salida 1</h3>   ";
mainPage += "  <a href=\'socket1On\'><button type='button' class='btn btn-success btn-lg btn-block'>Encender</button></a> ";
mainPage += "  <h6 class='text-center'></h6>";
mainPage += "  <a href=\'socket1Off\'><button type='button' class='btn btn-danger btn-lg btn-block'>Apagar</button></a>   ";
mainPage += "    <h3 class ='text-left'>Salida 2</h3> ";
mainPage += "  <a href=\'socket2On\'><button type='button' class='btn btn-success btn-lg btn-block'>Encender</button></a>";
mainPage += "  <h6 class='text-center'></h6>";
mainPage += "  <a href=\'socket2Off\'><button type='button' class='btn btn-danger btn-lg btn-block'>Apagar</button></a>";
mainPage += "    <h4 class='text-center'>www.reefacuario.com</h4>";
mainPage += "  <a href=\'setup\'><button type='button' class='btn btn-default btn-lg btn-block'>Configurar DUC</button></a>";
mainPage += "</div>";
mainPage += "</body>";
mainPage += "</html>";

setupPage += "<!DOCTYPE html>";
setupPage += "<html lang='en'>";
setupPage += "<head>";
setupPage += "  <title>REEF Controller</title>";
setupPage += "  <meta charset='utf-8'>";
setupPage += "  <meta name='viewport' content='width=device-width, initial-scale=1'>";
setupPage += "  <link href='https://maxcdn.bootstrapcdn.com/bootswatch/3.3.7/darkly/bootstrap.min.css' rel='stylesheet'>        ";
setupPage += "  <script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>";
setupPage += "  <script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>";
setupPage += "</head>";
setupPage += "<body>";
setupPage += "<div class= 'well' 'jumbotron'>";
setupPage += "    <h1 class='text-center'>Configuraci&oacuten del DUC</h1>";
setupPage += "    <h3></h3>";
setupPage += "    <form>";
setupPage += "    <div class='input-group'>";
setupPage += "      <span class='input-group-addon'><i class='glyphicon glyphicon-user'></i></span>";
setupPage += "      <input id='email' type='text' class='form-control' name='email' placeholder='Ejemplo juan.duckdns.org'>";
setupPage += "    </div>";
setupPage += "        <h3></h3>";
setupPage += "    <div class='input-group'>";
setupPage += "      <span class='input-group-addon'><i class='glyphicon glyphicon-lock'></i></span>";
setupPage += "      <input id='password'  class='form-control' name='password' placeholder='Token'>";
setupPage += "    </div>";
setupPage += "    <br>";
setupPage += "  <a href=\'/'><button type='button' class='btn btn-default btn-lg btn-block'>Regresar</button></a>";
setupPage += "</div>";
setupPage += "</body>";
setupPage += "</html>";
  
  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, LOW);
  pinMode(gpio2_pin, OUTPUT);
  digitalWrite(gpio2_pin, LOW);

  delay(500);
  
   server.on("/", [](){
    server.send(200, "text/html", mainPage);
  });
  server.on("/socket1On", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(gpio0_pin, LOW);
    delay(1000);
  });
  server.on("/socket1Off", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(gpio0_pin, HIGH);
    delay(1000); 
  });
  server.on("/socket2On", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(gpio2_pin, LOW);
    delay(1000);
  });
  server.on("/socket2Off", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(gpio2_pin, HIGH);
    delay(1000); 
  });
  //setup page
  server.on("/setup", [](){
    server.send(200, "text/html", setupPage);
  });
  server.begin();
  //DDNS DuckDNS
  EasyDDNS.service("duckdns");    // Enter your DDNS Service Name - "duckdns" / "noip"
  EasyDDNS.client("dajich.duckdns.org","3c08f8d5-d76f-49ac-903a-67486cc1de61");    // Enter ddns Domain & Token | Example - "esp.duckdns.org","1234567"
}
 
void loop(void){
  EasyDDNS.update(300000); // 1000 = 1 segundo
  server.handleClient();
  } 
