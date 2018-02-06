#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <EasyDDNS.h>
#include <EEPROM.h>

ESP8266WebServer server(80);

String mainPage = "";
String setupPage = "";
char APpass[] = "12345678";
const char* www_username = "reefacuario";//sitio web protegido por usuario y password
const char* www_password = "qwerty";
char memoria;
char flag_1 = 0;


IPAddress ip(192,168,4,1);//este nombre puede ser cualquiera no es una funcion fija ip-IP-ipe-ip1 es del AP
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

int salida_1 = 16;
int salida_2 = 5;
int salida_3 = 4;
int salida_4 = 0;
int salida_5 = 2;
int salida_6 = 14;
int salida_7 = 12;
int salida_8 = 13;

//pagina
const char HTTP_PAGE1[] PROGMEM = "<!DOCTYPE html><html lang='en'><head><title>REEF Controller</title><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><link href='https://maxcdn.bootstrapcdn.com/bootswatch/3.3.7/darkly/bootstrap.min.css' rel='stylesheet'><script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script><script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script></head><body>";
const char HTTP_DATA1[] PROGMEM = "<div class= 'well'><h1 class ='text-center'>REEF Controller</h1> <hr><h3 class ='text-left'>Salidas</h3>  <div class='btn-group-vertical'><div class='btn-group'>";

const char HTTP_BUTTON1_ON[] PROGMEM = "<a href=\'socket1Off\'><button type='button' class='btn btn-lg btn-success'>1</button>&nbsp;&nbsp;&nbsp;</a>";
const char HTTP_BUTTON1_OFF[] PROGMEM = "<a href=\'socket1On\'><button type='button' class='btn btn-lg btn-danger'>1</button>&nbsp;&nbsp;&nbsp;</a>";
const char HTTP_BUTTON2_ON[] PROGMEM = "<a href=\'socket2Off\'><button type='button' class='btn btn-lg btn-success'>2</button>&nbsp;&nbsp;&nbsp;</a>";
const char HTTP_BUTTON2_OFF[] PROGMEM = "<a href=\'socket2On\'><button type='button' class='btn btn-lg btn-danger'>2</button>&nbsp;&nbsp;&nbsp;</a>";
const char HTTP_BUTTON3_ON[] PROGMEM = "<a href=\'socket3Off\'><button type='button' class='btn btn-lg btn-success'>3</button>&nbsp;&nbsp;&nbsp;</a>";
const char HTTP_BUTTON3_OFF[] PROGMEM = "<a href=\'socket3On\'><button type='button' class='btn btn-lg btn-danger'>3</button>&nbsp;&nbsp;&nbsp;</a>";
const char HTTP_BUTTON4_ON[] PROGMEM = "<a href=\'socket4Off\'><button type='button' class='btn btn-lg btn-success'>4</button></a>";
const char HTTP_BUTTON4_OFF[] PROGMEM = "<a href=\'socket4On\'><button type='button' class='btn btn-lg btn-danger'>4</button></a>";

const char HTTP_DATA2[] PROGMEM = "</div><h6></h6><div class='btn-group'>";

const char HTTP_BUTTON5_ON[] PROGMEM = "<a href=\'socket5Off\'><button type='button' class='btn btn-lg btn-success'>5</button>&nbsp;&nbsp;&nbsp;</a>";
const char HTTP_BUTTON5_OFF[] PROGMEM = "<a href=\'socket5On\'><button type='button' class='btn btn-lg btn-danger'>5</button>&nbsp;&nbsp;&nbsp;</a>";
const char HTTP_BUTTON6_ON[] PROGMEM = "<a href=\'socket6Off\'><button type='button' class='btn btn-lg btn-success'>6</button>&nbsp;&nbsp;&nbsp;</a>";
const char HTTP_BUTTON6_OFF[] PROGMEM = "<a href=\'socket6On\'><button type='button' class='btn btn-lg btn-danger'>6</button>&nbsp;&nbsp;&nbsp;</a>";
const char HTTP_BUTTON7_ON[] PROGMEM = "<a href=\'socket7Off\'><button type='button' class='btn btn-lg btn-success'>7</button>&nbsp;&nbsp;&nbsp;</a>";
const char HTTP_BUTTON7_OFF[] PROGMEM = "<a href=\'socket7On\'><button type='button' class='btn btn-lg btn-danger'>7</button>&nbsp;&nbsp;&nbsp;</a>";
const char HTTP_BUTTON8_ON[] PROGMEM = "<a href=\'socket8Off\'><button type='button' class='btn btn-lg btn-success'>8</button></a>";
const char HTTP_BUTTON8_OFF[] PROGMEM = "<a href=\'socket8On\'><button type='button' class='btn btn-lg btn-danger'>8</button></a>";

const char HTTP_DATA3[] PROGMEM = "</div></div><hr>";

const char HTTP_BUTTON_ALL_OFF[] PROGMEM = "<a href=\'allOff\'><button type='button' class='btn btn-danger btn-lg btn-block'>Apagar todas las salidas</button></a>";
const char HTTP_SITIO[] PROGMEM = "<hr><h4 class='text-center'>www.reefacuario.com</h4><hr>";
const char HTTP_BUTTON_DNS[] PROGMEM = "<a href=\'setup\'><button type='button' class='btn btn-default btn-lg btn-block'>Configurar DNS</button></a>";
const char HTTP_END1[] PROGMEM = "</div></body></html>";

void pageHome() {
  //if(!server.authenticate(www_username, www_password))//estas 2 lineas son para proteger con password el acceso al servidor
  //    return server.requestAuthentication();
  
  if (server.hasArg("domain")&& server.hasArg("token")) 
  {//If all form fields contain data call handelSubmit()
    write_to_Memory(String(server.arg("domain")),String(server.arg("token")));
    String content = page();//get main page content
    server.send(200, "text/html", content);//write page
  }
  else 
  {//Redisplay the form
    String content = page();//get main page content
    server.send(200, "text/html", content);//write page
  }
}

String page() {
    String content = "";
    content += FPSTR(HTTP_PAGE1);
    content += FPSTR(HTTP_DATA1);
    if(bitRead(memoria,0) == 0){content += FPSTR(HTTP_BUTTON1_ON);}
    else if(bitRead(memoria,0) == 1){content += FPSTR(HTTP_BUTTON1_OFF);}
    if(bitRead(memoria,1) == 0){content += FPSTR(HTTP_BUTTON2_ON);}
    else if(bitRead(memoria,1) == 1){content += FPSTR(HTTP_BUTTON2_OFF);}
    if(bitRead(memoria,2) == 0){content += FPSTR(HTTP_BUTTON3_ON);}
    else if(bitRead(memoria,2) == 1){content += FPSTR(HTTP_BUTTON3_OFF);}
    if(bitRead(memoria,3) == 0){content += FPSTR(HTTP_BUTTON4_ON);}
    else if(bitRead(memoria,3) == 1){content += FPSTR(HTTP_BUTTON4_OFF);}
    content += FPSTR(HTTP_DATA2);
    if(bitRead(memoria,4) == 0){content += FPSTR(HTTP_BUTTON5_ON);}
    else if(bitRead(memoria,4) == 1){content += FPSTR(HTTP_BUTTON5_OFF);}
    if(bitRead(memoria,5) == 0){content += FPSTR(HTTP_BUTTON6_ON);}
    else if(bitRead(memoria,5) == 1){content += FPSTR(HTTP_BUTTON6_OFF);}
    if(bitRead(memoria,6) == 0){content += FPSTR(HTTP_BUTTON7_ON);}
    else if(bitRead(memoria,6) == 1){content += FPSTR(HTTP_BUTTON7_OFF);}
    if(bitRead(memoria,7) == 0){content += FPSTR(HTTP_BUTTON8_ON);}
    else if(bitRead(memoria,7) == 1){content += FPSTR(HTTP_BUTTON8_OFF);}
    content += FPSTR(HTTP_DATA3);
    content += FPSTR(HTTP_BUTTON_ALL_OFF);
    content += FPSTR(HTTP_SITIO);
    content += FPSTR(HTTP_BUTTON_DNS);
    content += FPSTR(HTTP_END1);
    return content;
    }


////////////////////////////////////////////////////////////S E T U P///////////////////////////////////////////////////////////////////////////////////////////////////////
void setup(void){
  EEPROM.begin(512);

  pinMode(salida_1, OUTPUT);
  pinMode(salida_2, OUTPUT);
  pinMode(salida_3, OUTPUT);
  pinMode(salida_4, OUTPUT);
  pinMode(salida_5, OUTPUT);
  pinMode(salida_6, OUTPUT);
  pinMode(salida_7, OUTPUT);
  pinMode(salida_8, OUTPUT);
  pinMode(3, INPUT_PULLUP);//entrada de boton para borrar ejecutar WiFi.disconnect(); y borrar red guardada

  if(digitalRead(3) == LOW )
  {
    WiFi.disconnect();//borra wifi guardada
    EEPROM.write(0,0); //all on
    EEPROM.commit();
    delay(5000);
    while(digitalRead(3) == LOW);
    ESP.reset();
  }
  
  memoria = EEPROM.read(0);
  init_outs();
//////////////////////////////////////////////B O R R A R    D A T O S   P R E V I O S////////////////
//WiFi.disconnect(); //will erase ssid/password segun tzapu autor de la libreria wifi manager//////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
WiFi.hostname("reef-controller");
//WiFi.softAPConfig(ip, gateway, subnet);
//WiFi.mode(WIFI_AP_STA);  
WiFiManager wifiManager;
wifiManager.autoConnect("REEF Setup (IP: 192.168.4.1)");// se desborda y no pone el nombre
show_IP();


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


  //server.on("/", pageHome);
  server.on("/", [](){
    //if(!server.authenticate(www_username, www_password))
      //return server.requestAuthentication();
    pageHome();
  });

  server.on("/socket1On", [](){
    salida_on(1);
    pageHome();
    delay(500);
  });
  server.on("/socket1Off", [](){
    salida_off(1);
    pageHome();
    delay(500);
  });
  
   server.on("/socket2On", [](){
    salida_on(2);
    pageHome();
    delay(500);
  });
  server.on("/socket2Off", [](){
    salida_off(2);
    pageHome();
    delay(500);
  });
    
    server.on("/socket3On", [](){
    salida_on(3);
    pageHome();
    delay(500);
  });
  server.on("/socket3Off", [](){
    salida_off(3);
    pageHome();
    delay(500);
  });

    server.on("/socket4On", [](){
    salida_on(4);
    pageHome();
    delay(500);
  });
  server.on("/socket4Off", [](){
    salida_off(4);
    pageHome();
    delay(500);
  });
  
    server.on("/socket5On", [](){
    salida_on(5);
    pageHome();
    delay(500);
  });
  server.on("/socket5Off", [](){
    salida_off(5);
    pageHome();
    delay(500);
  });
  
    server.on("/socket6On", [](){
    salida_on(6);
    pageHome();
    delay(500);
  });
  server.on("/socket6Off", [](){
    salida_off(6);
    pageHome();
    delay(500);
  });
  
    server.on("/socket7On", [](){
    salida_on(7);
    pageHome();
    delay(500);
  });
  server.on("/socket7Off", [](){
    salida_off(7);
    pageHome();
    delay(500);
  });
  
    server.on("/socket8On", [](){
    salida_on(8);
    pageHome();
    delay(500);
  });
  server.on("/socket8Off", [](){
    salida_off(8);
    pageHome();
    delay(500);
  });
  //////////////////////
  server.on("/allOff", [](){
    salida_off(1);
    salida_off(2);
    salida_off(3);
    salida_off(4);
    salida_off(5);
    salida_off(6);
    salida_off(7);
    salida_off(8);
    pageHome();
    delay(500);
  });
  /////////////////////
  //setup page
  server.on("/setup", [](){
    server.send(200, "text/html", setupPage);
  });
  
  server.begin();//importante!!
  //DDNS DuckDNS
  EasyDDNS.service("duckdns");    // Enter your DDNS Service Name - "duckdns" / "noip"
  EasyDDNS.client("dajich.duckdns.org","3c08f8d5-d76f-49ac-903a-67486cc1de61");    // Enter ddns Domain & Token | Example - "esp.duckdns.org","1234567"
}
///////////////////////////////////////////////////////////L O O P////////////////////////////////////////////////////////////////////////// 
void loop(void)
{
  EasyDDNS.update(300000); // 1000 = 1 segundo
  server.handleClient();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void init_outs(){
    //memoria = EEPROM.read(0);
    digitalWrite(salida_1, bitRead(memoria, 0));
    digitalWrite(salida_2, bitRead(memoria, 1));
    digitalWrite(salida_3, bitRead(memoria, 2));
    digitalWrite(salida_4, bitRead(memoria, 3));
    digitalWrite(salida_5, bitRead(memoria, 4));
    digitalWrite(salida_6, bitRead(memoria, 5));
    digitalWrite(salida_7, bitRead(memoria, 6));
    digitalWrite(salida_8, bitRead(memoria, 7));
}

void salida_on(char pin){
    //memoria = EEPROM.read(0);
    switch(pin){
      case 1: digitalWrite(salida_1, 0); bitClear(memoria,0); EEPROM.write(0,memoria); EEPROM.commit(); break;
      case 2: digitalWrite(salida_2, 0); bitClear(memoria,1); EEPROM.write(0,memoria); EEPROM.commit(); break;
      case 3: digitalWrite(salida_3, 0); bitClear(memoria,2); EEPROM.write(0,memoria); EEPROM.commit(); break;
      case 4: digitalWrite(salida_4, 0); bitClear(memoria,3); EEPROM.write(0,memoria); EEPROM.commit(); break;
      case 5: digitalWrite(salida_5, 0); bitClear(memoria,4); EEPROM.write(0,memoria); EEPROM.commit(); break;
      case 6: digitalWrite(salida_6, 0); bitClear(memoria,5); EEPROM.write(0,memoria); EEPROM.commit(); break;
      case 7: digitalWrite(salida_7, 0); bitClear(memoria,6); EEPROM.write(0,memoria); EEPROM.commit(); break;
      case 8: digitalWrite(salida_8, 0); bitClear(memoria,7); EEPROM.write(0,memoria); EEPROM.commit(); break;
      default: break;
    }
}

void salida_off(char pin){
    //memoria = EEPROM.read(0);
    switch(pin){
      case 1: digitalWrite(salida_1, 1); bitSet(memoria,0); EEPROM.write(0,memoria); EEPROM.commit(); break;
      case 2: digitalWrite(salida_2, 1); bitSet(memoria,1); EEPROM.write(0,memoria); EEPROM.commit(); break;
      case 3: digitalWrite(salida_3, 1); bitSet(memoria,2); EEPROM.write(0,memoria); EEPROM.commit(); break;
      case 4: digitalWrite(salida_4, 1); bitSet(memoria,3); EEPROM.write(0,memoria); EEPROM.commit(); break;
      case 5: digitalWrite(salida_5, 1); bitSet(memoria,4); EEPROM.write(0,memoria); EEPROM.commit(); break;
      case 6: digitalWrite(salida_6, 1); bitSet(memoria,5); EEPROM.write(0,memoria); EEPROM.commit(); break;
      case 7: digitalWrite(salida_7, 1); bitSet(memoria,6); EEPROM.write(0,memoria); EEPROM.commit(); break;
      case 8: digitalWrite(salida_8, 1); bitSet(memoria,7); EEPROM.write(0,memoria); EEPROM.commit(); break;
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

void show_IP(){
  String var1 ="REEF (IP: ";
  String var2 = WiFi.localIP().toString();
  String var3 =")";
  String IPstring = var1 + var2 + var3;
  char APshowIP[45] = "";
  IPstring.toCharArray(APshowIP, 45);
  //WiFi.softAPdisconnect(true);
  //WiFi.mode(WIFI_OFF);
  //delay(2000);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(APshowIP, APpass);//cambia el nombre al AP
}
