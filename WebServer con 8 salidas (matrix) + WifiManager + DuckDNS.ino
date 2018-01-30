#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <EasyDDNS.h>
#include <EEPROM.h>

ESP8266WebServer server(80);

String mainPage = "";
String setupPage = "";

/////////////////////////////////////////////////////////////
void setup(void){
  EEPROM.begin(512);
  // preparing GPIOs
  pinMode(16, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  init_outs();
  
WiFi.hostname("reef-controller");
WiFiManager wifiManager;
wifiManager.autoConnect("REEF Setup (IP: 192.168.4.1)");// se desborda y no pone el nombre
//wifiManager.autoConnect("REEF Controller Setup");
  
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
mainPage += "<div class= 'well'>";
mainPage += "    <h1 class ='text-center'>REEF Controller</h1> ";
mainPage += "    <hr>";
mainPage += "    <h3 class ='text-left'>Encender</h3>  ";
mainPage += "    <div class='btn-group-vertical'>";
mainPage += "      <div class='btn-group'>";
mainPage += "        <a href=\'socket1On\'><button type='button' class='btn btn-lg btn-success'>1</button>&nbsp;&nbsp;</a> ";
mainPage += "        <a href=\'socket2On\'><button type='button' class='btn btn-lg btn-success'>2</button>&nbsp;&nbsp;</a>";
mainPage += "        <a href=\'socket3On\'><button type='button' class='btn btn-lg btn-success'>3</button>&nbsp;&nbsp;</a>";
mainPage += "        <a href=\'socket4On\'><button type='button' class='btn btn-lg btn-success'>4</button></a>";
mainPage += "      </div>";
mainPage += "      <h6></h6>";
mainPage += "      <div class='btn-group'>";
mainPage += "        <a href=\'socket5On\'><button type='button' class='btn btn-lg btn-success'>5</button>&nbsp;&nbsp;</a>";
mainPage += "        <a href=\'socket6On\'><button type='button' class='btn btn-lg btn-success'>6</button>&nbsp;&nbsp;</a>";
mainPage += "        <a href=\'socket7On\'><button type='button' class='btn btn-lg btn-success'>7</button>&nbsp;&nbsp;</a>";
mainPage += "        <a href=\'socket8On\'><button type='button' class='btn btn-lg btn-success'>8</button></a>";
mainPage += "      </div>";
mainPage += "    </div>";
mainPage += "    <hr>";
mainPage += "    <h3 class ='text-left'>Apagar</h3>   ";
mainPage += "    <div class='btn-group-vertical'>";
mainPage += "      <div class='btn-group-lg'>";
mainPage += "        <a href=\'socket1Off\'><button type='button' class='btn btn-lg btn-danger'>1</button>&nbsp;&nbsp;</a> ";
mainPage += "        <a href=\'socket2Off\'><button type='button' class='btn btn-lg btn-danger'>2</button>&nbsp;&nbsp;</a>";
mainPage += "        <a href=\'socket3Off\'><button type='button' class='btn btn-lg btn-danger'>3</button>&nbsp;&nbsp;</a>";
mainPage += "        <a href=\'socket4Off\'><button type='button' class='btn btn-lg btn-danger'>4</button></a>";
mainPage += "      </div>";
mainPage += "      <h6></h6>";
mainPage += "      <div class='btn-group-lg'>";
mainPage += "        <a href=\'socket5Off\'><button type='button' class='btn btn-lg btn-danger'>5</button>&nbsp;&nbsp;</a>";
mainPage += "        <a href=\'socket6Off\'><button type='button' class='btn btn-lg btn-danger'>6</button>&nbsp;&nbsp;</a>";
mainPage += "        <a href=\'socket7Off\'><button type='button' class='btn btn-lg btn-danger'>7</button>&nbsp;&nbsp;</a>";
mainPage += "        <a href=\'socket8Off\'><button type='button' class='btn btn-lg btn-danger'>8</button></a>";
mainPage += "      </div>";
mainPage += "    </div>";
mainPage += "    <hr>";
mainPage += "  <h3 class ='text-left'>Todos</h3> ";
mainPage += "      <a href=\'allOn\'><button type='button' class='btn btn-success btn-lg btn-block'>Encender</button></a>";
mainPage += "  <h6 class='text-center'></h6>";
mainPage += "      <a href=\'allOff\'><button type='button' class='btn btn-danger btn-lg btn-block'>Apagar</button></a>";
mainPage += "    <hr>";
mainPage += "  <h4 class='text-center'>www.reefacuario.com</h4>";
mainPage += "    <hr>";
mainPage += "      <a href=\'setup\'><button type='button' class='btn btn-default btn-lg btn-block'>Configurar DNS</button></a>";
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
setupPage += "    <form action='/' method='GET'>";//aqui esta la clave para enviar a guardar los datos
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
  
   server.on("/", handleSave);
  //////////////////////////////////////////////
  server.on("/socket1On", [](){
    server.send(200, "text/html", mainPage);
    salida_on(1);
    delay(500);
  });
  server.on("/socket1Off", [](){
    server.send(200, "text/html", mainPage);
    salida_off(1);
    delay(500);
  });
  server.on("/socket2On", [](){
    server.send(200, "text/html", mainPage);
    salida_on(2);
    delay(500);
  });
  server.on("/socket2Off", [](){
    server.send(200, "text/html", mainPage);
    salida_off(2);
    delay(500);
  });

  server.on("/socket3On", [](){
    server.send(200, "text/html", mainPage);
    salida_on(3);
    delay(500);
  });
  server.on("/socket3Off", [](){
    server.send(200, "text/html", mainPage);
    salida_off(3);
    delay(500);
  });
  server.on("/socket4On", [](){
    server.send(200, "text/html", mainPage);
    salida_on(4);
    delay(500);
  });
  server.on("/socket4Off", [](){
    server.send(200, "text/html", mainPage);
    salida_off(4);
    delay(500);
  });
    server.on("/socket5On", [](){
    server.send(200, "text/html", mainPage);
    salida_on(5);
    delay(500);
  });
  server.on("/socket5Off", [](){
    server.send(200, "text/html", mainPage);
    salida_off(5);
    delay(500);
  });
    server.on("/socket6On", [](){
    server.send(200, "text/html", mainPage);
    salida_on(6);
    delay(500);
  });
  server.on("/socket6Off", [](){
    server.send(200, "text/html", mainPage);
    salida_off(6);
    delay(500); 
  });
    server.on("/socket7On", [](){
    server.send(200, "text/html", mainPage);
    salida_on(7);
    delay(500);
  });
  server.on("/socket7Off", [](){
    server.send(200, "text/html", mainPage);
    salida_off(7);
    delay(500);
  });
    server.on("/socket8On", [](){
    server.send(200, "text/html", mainPage);
    salida_on(8);
    delay(500);
  });
  server.on("/socket8Off", [](){
    server.send(200, "text/html", mainPage);
    salida_off(8);
    delay(500);
  });

  server.on("/allOn", [](){
    server.send(200, "text/html", mainPage);   
    salida_on(1);
    salida_on(2);
    salida_on(3);
    salida_on(4);
    salida_on(5);
    salida_on(6);
    salida_on(7);
    salida_on(8);
    delay(500);
  });
  server.on("/allOff", [](){
    server.send(200, "text/html", mainPage);
    salida_off(1);
    salida_off(2);
    salida_off(3);
    salida_off(4);
    salida_off(5);
    salida_off(6);
    salida_off(7);
    salida_off(8);
    delay(500); 
  });
  ////////////////////////////////////////////
  //setup page
  server.on("/setup", [](){
    server.send(200, "text/html", setupPage);
  });
  
  server.begin();
  //DDNS DuckDNS
  EasyDDNS.service("duckdns");    // Enter your DDNS Service Name - "duckdns" / "noip"
  EasyDDNS.client("dajich.duckdns.org","3c08f8d5-d76f-49ac-903a-67486cc1de61");    // Enter ddns Domain & Token | Example - "esp.duckdns.org","1234567"
  
  String var1 ="REEF Controller (IP: ";
  String var2 = WiFi.localIP().toString();
  String var3 =")";
  String IPstring = var1 + var2 + var3;
  char APshowIP[45] = "";
  IPstring.toCharArray(APshowIP, 45);
  
  WiFi.softAPdisconnect(true);//desconecta al usuario
  WiFi.mode(WIFI_OFF);//apaga wifi por 3 segundos
  delay(2000);//incrementar este tiempo si acaso no se desconecta del AP de configuracion
  WiFi.softAP(APshowIP, "pass-to-soft-AP");//crea mismo AP pero con seguridad para evitar reconexion abierta en vez de internet local
  WiFi.mode(WIFI_AP_STA);//inicia en modo STA y AP permanentemente
  WiFi.begin();
}
 
void loop(void){
  EasyDDNS.update(300000); // 1000 = 1 segundo
  server.handleClient();
}

void init_outs(){
    char x = EEPROM.read(0);
    digitalWrite(16, bitRead(x, 0));
    digitalWrite(5, bitRead(x, 1));
    digitalWrite(4, bitRead(x, 2));
    digitalWrite(0, bitRead(x, 3));
    digitalWrite(2, bitRead(x, 4));
    digitalWrite(14, bitRead(x, 5));
    digitalWrite(12, bitRead(x, 6));
    digitalWrite(13, bitRead(x, 7));
}

void salida_on(char pin){
    char x = EEPROM.read(0);
    switch(pin){
      case 1: digitalWrite(16, 0); bitClear(x,0); EEPROM.write(0,x); EEPROM.commit(); break;
      case 2: digitalWrite(5,  0); bitClear(x,1); EEPROM.write(0,x); EEPROM.commit(); break;
      case 3: digitalWrite(4,  0); bitClear(x,2); EEPROM.write(0,x); EEPROM.commit(); break;
      case 4: digitalWrite(0,  0); bitClear(x,3); EEPROM.write(0,x); EEPROM.commit(); break;
      case 5: digitalWrite(2,  0); bitClear(x,4); EEPROM.write(0,x); EEPROM.commit(); break;
      case 6: digitalWrite(14, 0); bitClear(x,5); EEPROM.write(0,x); EEPROM.commit(); break;
      case 7: digitalWrite(12, 0); bitClear(x,6); EEPROM.write(0,x); EEPROM.commit(); break;
      case 8: digitalWrite(13, 0); bitClear(x,7); EEPROM.write(0,x); EEPROM.commit(); break;
      default: break;
    }
}

void salida_off(char pin){
    char x = EEPROM.read(0);
    switch(pin){
      case 1: digitalWrite(16, 1); bitSet(x,0); EEPROM.write(0,x); EEPROM.commit(); break;
      case 2: digitalWrite(5,  1); bitSet(x,1); EEPROM.write(0,x); EEPROM.commit(); break;
      case 3: digitalWrite(4,  1); bitSet(x,2); EEPROM.write(0,x); EEPROM.commit(); break;
      case 4: digitalWrite(0,  1); bitSet(x,3); EEPROM.write(0,x); EEPROM.commit(); break;
      case 5: digitalWrite(2,  1); bitSet(x,4); EEPROM.write(0,x); EEPROM.commit(); break;
      case 6: digitalWrite(14, 1); bitSet(x,5); EEPROM.write(0,x); EEPROM.commit(); break;
      case 7: digitalWrite(12, 1); bitSet(x,6); EEPROM.write(0,x); EEPROM.commit(); break;
      case 8: digitalWrite(13, 1); bitSet(x,7); EEPROM.write(0,x); EEPROM.commit(); break;
      default: break;
    }
}
////////////////////SETUP DNS
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
  write_EEPROM(d,1);
  t+=";";
  write_EEPROM(t,32);
  EEPROM.commit();
  }

void handleSave() {
    if (server.hasArg("domain")&& server.hasArg("token")) 
    {//If all form fields contain data call handelSubmit()
    write_to_Memory(String(server.arg("domain")),String(server.arg("token")));
    server.send(200, "text/html", mainPage);
    }
    else 
    {//Redisplay the form
    server.send(200, "text/html", mainPage);
    }
}
