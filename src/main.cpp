#include <Arduino.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>
#endif

#include <ESPAsyncWebServer.h>     //Local WebServer used to serve the configuration portal
#include <ESPAsyncWiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

#include "NTP.h"
#include "html.h"

WiFiUDP wifiUdp;
NTP ntp(wifiUdp);
char serInString[100];  // array that will hold the different bytes  100=100characters;
int  serInIndx  = 0;    // index of serInString[] in which to insert the next incoming byte
int  serOutIndx = 0;    // index of the outgoing serInString[] array;
AsyncWebServer server(80);
DNSServer dns;

int exg=0;
int clockMode=0;
int angle=0;
int smt=0;
unsigned long last_exg=0; 
int ctr=0;
//read a string from the serial and store it in an array
//this func uses globally set variable so it's not so reusable
//I need to find the right syntax to be able to pass to the function 2 parameters:
// the stringArray and (eventually) the index count
void readSerialString () {
    int sb;   
    if(Serial.available()) { 
       //Serial.print("reading Serial String: ");     //optional confirmation
       while (Serial.available()){ 
          sb = Serial.read();             
          serInString[serInIndx] = sb;
          serInIndx++;ctr++;
            delay(100);  
            if (serInString[0]=='E' &&  serInString[1]=='X' &&  serInString[2]=='G'  ) { 
                    serInIndx  = 0;  
                    exg=1;
            }
          //serialWrite(sb);                        //optional confirmation
       }
       //Serial.println();
    }  
}
void serialFlush(){
  int sb;   
  while(Serial.available() > 0) {   //while there are characters in the serial buffer, because Serial.available is >0
    sb = Serial.read();         // get one character
  }
  sb=sb;
} 
//print the string all in one time
//this func as well uses global variables
void serialResponse() {
  uint32_t ip;
   if( serInIndx > 0) {
      serInString[serInIndx]=0;
      if (strcmp(serInString,"T")==0) {
          ntp.update();
          Serial.write(ntp.hours());
          Serial.write(ntp.minutes());
          Serial.write(ntp.seconds());
      } else if (strcmp(serInString,"D")==0) {
          ntp.update();
          Serial.write((int8_t)(ntp.year()-2000));
          Serial.write(ntp.month());
          Serial.write(ntp.day());
      }
      else if ((strcmp(serInString,"IP")==0)) { 
          ip=WiFi.localIP();
          Serial.write((byte *) &ip, 4);
      } else if (exg==1) {                        // EXG (Byte)ClockMode (Byte)Angle/2 
          last_exg=  millis()/1000;               // RES(Bytes)Hour Minutes Seconds Year (0-99) Month Day IP-Part1 IP-Part22 IP-Part3 IP Part4
          exg=0; 
          clockMode=serInString[0];
          angle=serInString[1];
          angle=angle*2;
          smt=1;
          ntp.update();
          Serial.write('R');
          Serial.write('E');
          Serial.write('S');

          Serial.write(ntp.hours());
          Serial.write(ntp.minutes());
          Serial.write(ntp.seconds());      
          Serial.write((int8_t)(ntp.year()-2000));
          Serial.write(ntp.month());
          Serial.write(ntp.day());              
          ip=WiFi.localIP();
          Serial.write((byte *) &ip, 4);                
      } else if (strcmp(serInString,"getTime")==0) {
          ntp.update();
          Serial.println(ntp.formattedTime("%Y-%m-%d %H:%M:%S"));
      } else if  (strcmp(serInString,"Wifi")==0) {
        WiFi.printDiag(Serial);  
      } else {
        Serial.print("you said: ");     
       Serial.println(serInString);        
      }

      serOutIndx = 0;
      serInIndx  = 0;
   }

}
/**
 *   index page: show important infomation and buttons
 * */
void handleRoot(AsyncWebServerRequest *request) {
  unsigned long time;
  String s = HTML_TOP;  
  s+=ntp.formattedTime("%Y-%m-%d %H:%M:%S");
  s+="<br>";
  if (!smt) s+="Motor connection: none";
  else {
      time=millis()/1000;
      if (time-last_exg>660) s+="Error: Motor conection lost! "; // no reaction for last 11 minutes - error
      else {
        s+="Motor connected! Mode: ";
        if (clockMode==0) s+="Seconds";
        else if (clockMode==1) s+="Minutes";
        else if (clockMode==2) s+="Hour+Minutes";   
        else  s+="unknown";   
        s+="<br>";
      }

  }

  s+="<br>AP: "+WiFi.SSID()+ ", Strength:"+ WiFi.RSSI();
  s+="<br>";
  s+=HTML_BOTTOM;

  request->send(200, "text/html", s); //Send web page
}
void handleUp(AsyncWebServerRequest *request) {
  Serial.write('G');
  Serial.write('O');
  Serial.write('U');
  handleRoot(request);
}


void handleRight(AsyncWebServerRequest *request) {
  Serial.write('G');
  Serial.write('O');
  Serial.write('R');
  handleRoot(request);
}


void handleDown(AsyncWebServerRequest *request) {
  Serial.write('G');
  Serial.write('O');
  Serial.write('D');
  handleRoot(request);
}

void handleLeft(AsyncWebServerRequest *request) {
  Serial.write('G');
  Serial.write('O');
  Serial.write('L');
  handleRoot(request);
}

void handleStart(AsyncWebServerRequest *request) {
  Serial.write('S');
  Serial.write('T');
  Serial.write('A');
  handleRoot(request);
}
void handleModeSecond(AsyncWebServerRequest *request) {
  Serial.write('M');
  Serial.write('O');
  Serial.write('S');
  handleRoot(request);
}
void handleModeMinute(AsyncWebServerRequest *request) {
  Serial.write('M');
  Serial.write('O');
  Serial.write('M');
  handleRoot(request);
}
void handleModeHour(AsyncWebServerRequest *request) {
  Serial.write('M');
  Serial.write('O');
  Serial.write('H');
  handleRoot(request);
}

/*void handleCheck() {
 String s = "<a href='/'>Back</a><br> <br>";
s+=ntp.formattedTime("%Y-%m-%d %H:%M:%S");
 request.send(200, "text/html", s); //Send web page
}*/
void configModeCallback (AsyncWiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setup() 
{


//    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

    // put your setup code here, to run once:
    Serial.begin(38400); // 115200 38400
    Serial.println("Tower Clock Wifi");
  
 //   WiFi.mode(WIFI_STA); // it is a good practice to make sure your code sets wifi mode how you want it.

    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    AsyncWiFiManager wm(&server,&dns);
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
    wm.setAPCallback(configModeCallback);
    //reset settings - wipe credentials for testing
    //wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("Tower Clock Wifi",""); // non password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
        ntp.ruleDST("CEST", Last, Sun, Mar, 2, 120); // last sunday in march 2:00, timetone +120min (+1 GMT + 1h summertime offset)
        ntp.ruleSTD("CET", Last, Sun, Oct, 3, 60); // last sunday in october 3:00, timezone +60min (+1 GMT)
        ntp.begin();
        Serial.println("start NTP");
        serialFlush();
        server.on("/", handleRoot);      //Which routine to handle at root location        
        server.on("/up.html", handleUp);      
        server.on("/right.html", handleRight);      
        server.on("/down.html", handleDown);      
        server.on("/left.html", handleLeft);      
        server.on("/start.html", handleStart);      

        server.on("/set_mode_second.html", handleModeSecond);      
        server.on("/set_mode_minute
        .html", handleModeMinute);      
        server.on("/set_mode_hour.html", handleModeHour);      


    //    server.on("/check.html", handleCheck);      //Which routine to handle at check location        
        server.begin();                  //Start server
        Serial.println("HTTP server started");        
    }

}

void loop() {

  readSerialString();
  serialResponse();
 // server.handleClient();      

}