int relay1Pin = 12;//define a pin for relay
int relay2Pin = 14;//define a pin for relay

int relay1State = 1;//initial state . 1 ON, 0 OFF
int relay2State = 1;//initial state . 1 ON, 0 OFF

String button1Title1 ="LAMPE ON";
String button1Title2 ="LAMPE OFF";

String button2Title1 ="VENTILATION ON";
String button2Title2 ="VENTILATION OFF";



#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

//votre ssid et mot de passe du wifi
const char *ssid = "*****";
const char *password = "******";

WebServer server(80);

void handleRoot() {
  
 String HTML = "<!DOCTYPE html> <html>\n";
 HTML +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
 HTML += "<h1>SEN POULAILLLER</h1>";
 HTML += "<h1>gestion des equipements </h1>";
 HTML += "<style>@import url( https://fonts.googleapis.com/css?family=Open+Sans);";
 HTML += "body { font-family: 'Open Sans', 'sans-serif'; background-image: url(http://benague.ca/files/pw_pattern.png);}";
 HTML += "h1 { color: #55acee; text-align: center;}";
HTML += ".link {  text-decoration: none; color: #55acee; border-bottom: 2px dotted #55acee; transition: .3s; -webkit-transition: .3s; -moz-transition: .3s; -o-transition: .3s; cursor: url(http://cur.cursors-4u.net/symbols/sym-1/sym46.cur)., auto;}";
HTML += ".link:hover {  color: #2ecc71;  border-bottom: 2px dotted #2ecc71;}";
HTML += "#buttons {  padding-top: 50px;  text-align: center;}";
HTML += ".btn {  border-radius: 5px;  padding: 15px 25px;  font-size: 22px;  text-decoration: none;  margin: 20px;   color: #fff; position: relative;  display: inline-block;}";
HTML += ".btn:active {  transform: translate(0px, 5px);  -webkit-transform: translate(0px, 5px); box-shadow: 0px 1px 0px 0px;}";
HTML += ".blue {  background-color: #55acee;  box-shadow: 0px 5px 0px 0px #3C93D5;}";
HTML += ".blue:hover {background-color: #6FC6FF;}";
HTML += ".green {  background-color: #2ecc71;  box-shadow: 0px 5px 0px 0px #15B358;}";
HTML += ".green:hover {  background-color: #48E68B;}";
HTML += ".red {  background-color: #e74c3c;  box-shadow: 0px 5px 0px 0px #CE3323;}";
HTML += ".red:hover {  background-color: #FF6656;}";

HTML += ".purple {  background-color: #9b59b6;  box-shadow: 0px 5px 0px 0px #82409D;}";
HTML += ".purple:hover {  background-color: #B573D0;}";
HTML += ".orange {  background-color: #e67e22;  box-shadow: 0px 5px 0px 0px #CD6509;}";

HTML += ".orange:hover {  background-color: #FF983C;}";
HTML += ".yellow {  background-color: #f1c40f;  box-shadow: 0px 5px 0px 0px #D8AB00;}";
HTML += ".yellow:hover {  background-color: #FFDE29;}";

HTML += "p {  text-align: center;  color: #55acee;  padding-top: 20px;}";
HTML += "</style>\n";
HTML += "<div id=\"buttons\">";

if(relay1State){
    HTML +="<a href=\"control?r1=off\" class=\"btn green\">ON</a>";
    HTML +=button1Title1; //Light ON title
  }else{
    HTML += "<a href=\"control?r1=on\" class=\"btn red\">OFF</a>";  
       HTML +=button1Title2;//Light OFF title 
  }
if(relay2State){
    HTML +="<a href=\"control?r2=off\" class=\"btn green\">ON</a>";
    HTML +=button2Title1; //Light ON title
  }else{
    HTML += "<a href=\"control?r2=on\" class=\"btn red\">OFF</a>";  
       HTML +=button2Title2;//Light OFF title 
  }
  
HTML += "</div>";
 HTML +="</body> </html>\n";
 
  server.send(200, "text/html", HTML);  
}//handleRoot()

void handleNotFound() {
  //Robojax.com ESP32 Relay Control
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
 //Robojax.com ESP32 Relay Control
}//end of handleNotFound()

void setup(void) {
  //Robojax.com ESP32 Relay Control 
  pinMode(relay1Pin, OUTPUT);// define a pin as output for relay
  digitalWrite(relay1Pin, relay1State);//initial state either ON or OFF

  pinMode(relay2Pin, OUTPUT);// define a pin as output for relay
  digitalWrite(relay2Pin, relay2State);//initial state either ON or OFF  
  
  Serial.begin(115200);//initialize the serial monitor

  //Relay control ON OFF 

    
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: http://");
  Serial.println(WiFi.localIP());

//multicast DNS //Robojax.com ESP32 Relay Control 
  if (MDNS.begin("robojaxESP32")) {
    Serial.println("MDNS responder started");
    Serial.println("access via http://robojaxESP32");
  }

  server.on("/", handleRoot);
  server.on("/control", HTTP_GET, relayControl);         
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

}//end of setup

void loop(void) {
  //Robojax ESP32 Relay Control
  server.handleClient();

  if(relay1State ==1)
  {
    digitalWrite(relay1Pin, LOW);
  }else
  {
    digitalWrite(relay1Pin, HIGH);
  }
  
  if(relay2State ==1)
  {
    digitalWrite(relay2Pin, LOW);
  }else{
    digitalWrite(relay2Pin, HIGH);    
  }

   delay(100);
   //Serial.print("Relay1:");Serial.print (relay1State);
   //Serial.print(" Relay2:");Serial.println(relay2State);   
 //ESP32 Relay Control
}//end of loop

/*
 * relayControl()
 * updates the value of "relayState" varible to 1 or zero 
 * returns nothing
 * written by Ahmad Shamshiri
 * on Wednesday Feb 22, 2020 at 16:20 in Ajax, Ontario, Canada
 * www.robojax.com
 */
void relayControl() {
  if(server.arg("r1") == "on")
  {
    relay1State = 1;// set state of relay1 to ON
  }else if(server.arg("r1") == "off"){
    relay1State = 0;  // set state of relay1 to OFF
  }
  
  if(server.arg("r2") == "on") {
    relay2State =1; // set state of relay2 to ON  
  }else if(server.arg("r2") == "off"){
    relay2State =0;  // set state of relay2 to OFF
  }

  handleRoot();
}//relayControl() end
