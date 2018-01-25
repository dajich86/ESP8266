#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <EasyDDNS.h>
#include <EEPROM.h>

ESP8266WebServer server(80);

String mainPage = "";
String setupPage = "";

int gpio0_pin = 0;
int gpio2_pin = 2;


//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
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
mainPage += "    <h3 class ='text-left'>Salida 2</h3>   ";
mainPage += "  <a href=\'socket2On\'><button type='button' class='btn btn-success btn-lg btn-block'>Encender</button></a> ";
mainPage += "  <h6 class='text-center'></h6>";
mainPage += "  <a href=\'socket2Off\'><button type='button' class='btn btn-danger btn-lg btn-block'>Apagar</button></a>   ";
mainPage += "    <h3 class ='text-left'>Salida 3</h3>   ";
mainPage += "  <a href=\'socket3On\'><button type='button' class='btn btn-success btn-lg btn-block'>Encender</button></a> ";
mainPage += "  <h6 class='text-center'></h6>";
mainPage += "  <a href=\'socket3Off\'><button type='button' class='btn btn-danger btn-lg btn-block'>Apagar</button></a>   ";
mainPage += "    <h3 class ='text-left'>Salida 4</h3>   ";
mainPage += "  <a href=\'socket4On\'><button type='button' class='btn btn-success btn-lg btn-block'>Encender</button></a> ";
mainPage += "  <h6 class='text-center'></h6>";
mainPage += "  <a href=\'socket4Off\'><button type='button' class='btn btn-danger btn-lg btn-block'>Apagar</button></a>   ";
mainPage += "    <h3 class ='text-left'>Salida 5</h3>   ";
mainPage += "  <a href=\'socket5On\'><button type='button' class='btn btn-success btn-lg btn-block'>Encender</button></a> ";
mainPage += "  <h6 class='text-center'></h6>";
mainPage += "  <a href=\'socket5Off\'><button type='button' class='btn btn-danger btn-lg btn-block'>Apagar</button></a>   ";
mainPage += "    <h3 class ='text-left'>Salida 6</h3>   ";
mainPage += "  <a href=\'socket6On\'><button type='button' class='btn btn-success btn-lg btn-block'>Encender</button></a> ";
mainPage += "  <h6 class='text-center'></h6>";
mainPage += "  <a href=\'socket6Off\'><button type='button' class='btn btn-danger btn-lg btn-block'>Apagar</button></a>   ";
mainPage += "    <h3 class ='text-left'>Salida 7</h3>   ";
mainPage += "  <a href=\'socket7On\'><button type='button' class='btn btn-success btn-lg btn-block'>Encender</button></a> ";
mainPage += "  <h6 class='text-center'></h6>";
mainPage += "  <a href=\'socket7Off\'><button type='button' class='btn btn-danger btn-lg btn-block'>Apagar</button></a>   ";
mainPage += "    <h3 class ='text-left'>Salida 8</h3>   ";
mainPage += "  <a href=\'socket8On\'><button type='button' class='btn btn-success btn-lg btn-block'>Encender</button></a> ";
mainPage += "  <h6 class='text-center'></h6>";
mainPage += "  <a href=\'socket8Off\'><button type='button' class='btn btn-danger btn-lg btn-block'>Apagar</button></a>   ";

mainPage += "    <h3 class ='text-left'>Todas</h3> ";
mainPage += "  <a href=\'allOn\'><button type='button' class='btn btn-success btn-lg btn-block'>Encender</button></a>";
mainPage += "  <h6 class='text-center'></h6>";
mainPage += "  <a href=\'allOff\'><button type='button' class='btn btn-danger btn-lg btn-block'>Apagar</button></a>";
mainPage += "    <h4 class='text-center'>www.reefacuario.com</h4>";
mainPage += "  <a href=\'setup\'><button type='button' class='btn btn-default btn-lg btn-block'>Configurar DNS</button></a>";
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
setupPage += "    <h1 class='text-center'>DNS</h1>";
setupPage += "    <h3></h3>";
setupPage += "    <form action='/' method='post'>";//aqui esta la clave para enviar a guardar los datos
setupPage += "    <div class='input-group'>";
setupPage += "      <span class='input-group-addon'><i class='glyphicon glyphicon-user'></i></span>";
setupPage += "      <input maxlength='30' type='text' class='form-control' name='domain' placeholder='daniel'>";
setupPage += "      <span class='input-group-addon'>.duckdns.org</i></span>";
setupPage += "    </div>";
setupPage += "        <h3></h3>";
setupPage += "    <div class='input-group'>";
setupPage += "      <span class='input-group-addon'><i class='glyphicon glyphicon-lock'></i></span>";
setupPage += "      <input maxlength='36' class='form-control' name='token' placeholder='token'>";
setupPage += "    </div>";
setupPage += "    <br>";
setupPage += "  <a href=\'/'><button type='submit' class='btn btn-default btn-lg btn-block'>Guardar</button></a>";
//setupPage += "  <a href=\'/'><input type='submit' value='Send'>";
setupPage += "</div>";
setupPage += "</form>";
setupPage += "</body>";
setupPage += "</html>";
  
  // preparing GPIOs
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  delay(500);
  
   server.on("/", [](){
    server.send(200, "text/html", mainPage);
  });
  //////////////////////////////////////////////
  server.on("/socket1On", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(16, LOW);
    delay(1000);
  });
  server.on("/socket1Off", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(16, HIGH);
    delay(1000); 
  });
  server.on("/socket2On", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(5, LOW);
    delay(1000);
  });
  server.on("/socket2Off", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(5, HIGH);
    delay(1000); 
  });

  server.on("/socket3On", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(4, LOW);
    delay(1000);
  });
  server.on("/socket3Off", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(4, HIGH);
    delay(1000); 
  });
  server.on("/socket4On", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(0, LOW);
    delay(1000);
  });
  server.on("/socket4Off", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(0, HIGH);
    delay(1000); 
  });
    server.on("/socket5On", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(2, LOW);
    delay(1000);
  });
  server.on("/socket5Off", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(2, HIGH);
    delay(1000); 
  });
    server.on("/socket6On", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(14, LOW);
    delay(1000);
  });
  server.on("/socket6Off", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(14, HIGH);
    delay(1000); 
  });
    server.on("/socket7On", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(12, LOW);
    delay(1000);
  });
  server.on("/socket7Off", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(12, HIGH);
    delay(1000); 
  });
    server.on("/socket8On", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(13, LOW);
    delay(1000);
  });
  server.on("/socket8Off", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(13, HIGH);
    delay(1000); 
  });

  server.on("/allOn", [](){
    server.send(200, "text/html", mainPage);   
    digitalWrite(16, LOW);
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);
    digitalWrite(0, LOW);
    digitalWrite(2, LOW);
    digitalWrite(14, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    delay(1000);
  });
  server.on("/allOff", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(16, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(0, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(14, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
    delay(1000); 
  });
  ////////////////////////////////////////////
  //setup page
  server.on("/setup", handleRoot);
  server.begin();
  //DDNS DuckDNS
  EasyDDNS.service("duckdns");    // Enter your DDNS Service Name - "duckdns" / "noip"
  EasyDDNS.client("dajich.duckdns.org","3c08f8d5-d76f-49ac-903a-67486cc1de61");    // Enter ddns Domain & Token | Example - "esp.duckdns.org","1234567"
}
 
void loop(void){
  EasyDDNS.update(300000); // 1000 = 1 segundo
  server.handleClient();
}


void write_EEPROM(String x,int pos)
{
    for(int n=pos;n<x.length()+pos;n++)
    {
      EEPROM.write(n,x[n-pos]);
    }
    EEPROM.commit();
  }

void write_to_Memory(String d,String t)
{
  d+=";";
  write_EEPROM(d,0);
  t+=";";
  write_EEPROM(t,50);
  EEPROM.commit();
  }

void handleSubmit()
  {
   write_to_Memory(String(server.arg("domain")),String(server.arg("token")));
  } 

void handleRoot() {
   if (server.hasArg("domain")&& server.hasArg("token")) {//If all form fields contain data call handelSubmit()
    handleSubmit();
  }
  else {//Redisplay the form
    server.send(200, "text/html", setupPage);
  }
}


