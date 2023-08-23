#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Preferences.h>
#include <ArduinoOTA.h>

#include <ESP8266WebServer.h>
#include <variables.cpp>
#include <ArduinoJson.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266TimeAlarms.h>

#include <index_html.cpp>

#define BRIGHTNESS_MIN 43

const int ledPin = 2;
const int swPin = 0;

// Replace with your network credentials
const char *ssid     = "";
const char *password = "";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

ESP8266WebServer server(80);

Preferences prefs;
structVars flashVars;


void hardReset(structVars *x){
    x->mondayTime[0]=0;x->mondayTime[1]=0;x->mondayTime[2]=0;
    x->tuesdayTime[0]=0;x->tuesdayTime[1]=0;x->tuesdayTime[2]=0;
    x->wednesdayTime[0]=0;x->wednesdayTime[1]=0;x->wednesdayTime[2]=0;
    x->thursdayTime[0]=0;x->thursdayTime[1]=0;x->thursdayTime[2]=0;
    x->fridayTime[0]=0;x->fridayTime[1]=0;x->fridayTime[2]=0;
    x->saturdayTime[0]=0;x->saturdayTime[1]=0;x->saturdayTime[2]=0;
    x->sundayTime[0]=0;x->sundayTime[1]=0;x->sundayTime[2]=0;

    x->brightness = 100;
    x->decayTime = 180;
    x->rgb[0]=100;x->rgb[1]=100;x->rgb[2]=100;
    x->disableAll = false;
    x->lampOn = false;
};

void updateRtc(){
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  timeClient.update();
  time_t rtc = timeClient.getEpochTime();
  timeval tv = { rtc, 0 };
  settimeofday(&tv, nullptr);
}

void toggleDisableAllAlarms(bool en);
void updateAlarm(AlarmId *alarm, timeDayOfWeek_t dayOfWeek, int hh, int mm, int enable, OnTick_t callback);

void handleLamp(int brightness);
void handleLampDecay();

void triggerAlarm(void);

void Repeats2();
void digitalClockDisplay();


struct Alarms {
  AlarmId monday;
  AlarmId tuesday;
  AlarmId wednesday;
  AlarmId thursday;
  AlarmId friday;
  AlarmId saturday;
  AlarmId sunday;
} alarms;
AlarmId timerDecay;
AlarmId timerWifi;
int currBrightness = BRIGHTNESS_MIN*100; // 0~25500

void initAlarms(Alarms* als){
  // updateAlarm(&als->monday,dowMonday,flashVars.mondayTime[0],flashVars.mondayTime[1],flashVars.mondayTime[2], triggerAlarm);
  // als->monday = flashVars.mondayTime
  alarms.monday = Alarm.alarmRepeat(dowMonday,flashVars.mondayTime[0],flashVars.mondayTime[1],flashVars.mondayTime[2], triggerAlarm);
  alarms.tuesday = Alarm.alarmRepeat(dowTuesday,flashVars.tuesdayTime[0],flashVars.tuesdayTime[1],flashVars.tuesdayTime[2], triggerAlarm);
  alarms.wednesday = Alarm.alarmRepeat(dowWednesday,flashVars.wednesdayTime[0],flashVars.wednesdayTime[1],flashVars.wednesdayTime[2], triggerAlarm);
  alarms.thursday = Alarm.alarmRepeat(dowThursday,flashVars.thursdayTime[0],flashVars.thursdayTime[1],flashVars.thursdayTime[2], triggerAlarm);
  alarms.friday = Alarm.alarmRepeat(dowFriday,flashVars.fridayTime[0],flashVars.fridayTime[1],flashVars.fridayTime[2], triggerAlarm);
  alarms.saturday = Alarm.alarmRepeat(dowSaturday,flashVars.saturdayTime[0],flashVars.saturdayTime[1],flashVars.saturdayTime[2], triggerAlarm);
  alarms.sunday = Alarm.alarmRepeat(dowSunday,flashVars.sundayTime[0],flashVars.sundayTime[1],flashVars.sundayTime[2], triggerAlarm);
  toggleDisableAllAlarms(flashVars.disableAll);
}

void toggleDisableAllAlarms(bool en){
  if(!en){
    Alarm.enable(alarms.monday);
    Alarm.enable(alarms.tuesday);
    Alarm.enable(alarms.wednesday);
    Alarm.enable(alarms.thursday);
    Alarm.enable(alarms.friday);
    Alarm.enable(alarms.saturday);
    Alarm.enable(alarms.sunday);
  } else{
  Alarm.disable(alarms.monday);
  Alarm.disable(alarms.tuesday);
  Alarm.disable(alarms.wednesday);
  Alarm.disable(alarms.thursday);
  Alarm.disable(alarms.friday);
  Alarm.disable(alarms.saturday);
  Alarm.disable(alarms.sunday);
  }
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  // Preferences
  prefs.begin("my-app");
  prefs.getBytes("flashVars",&flashVars,sizeof(flashVars));    // init flash stored config variables.

  // WiFi
  // WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  IPAddress local_IP(192, 168, 0, 15);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress dns1(8, 8, 8, 8);
  WiFi.config(local_IP, gateway, subnet, dns1, dns1);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  handleLamp(BRIGHTNESS_MIN);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi Connected:");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  timeClient.setTimeOffset(-3600*3);
  updateRtc();
  initAlarms(&alarms);
  Alarm.timerRepeat(3600, updateRtc);           // timer for every 4h
  // Alarm.timerRepeat(5, Repeats2);      // timer for every 2 seconds
  
  server.on("/",  [](){
    Serial.println("GET /");
    // server.send(200, "text/plain", "Hola mundo!");
    server.send(200, "text/html", index_html);
  });
  server.on("/alarm",  [](){
    Serial.println("GET /alarm");
    triggerAlarm();
    server.send(300, "text/plain", "trigger alarm");
  });
  
  server.on("/config/set", HTTP_POST, [](){
    Serial.println("POST /config/set");

    StaticJsonDocument<2048> doc;
    deserializeJson(doc,server.arg("plain"));

    Serial.println("received JSON:");
    serializeJson(doc, Serial);

    
    if(doc.containsKey("mondayTime")){
      flashVars.mondayTime[0] = (int)(doc["mondayTime"][0]);flashVars.mondayTime[1] = (int)(doc["mondayTime"][1]);flashVars.mondayTime[2] = (int)(doc["mondayTime"][2]);
      updateAlarm(&alarms.monday,dowMonday,flashVars.mondayTime[0],flashVars.mondayTime[1],flashVars.mondayTime[2], triggerAlarm);
    } if (doc.containsKey("tuesdayTime")){
      flashVars.tuesdayTime[0] = (int)(doc["tuesdayTime"][0]);flashVars.tuesdayTime[1] = (int)(doc["tuesdayTime"][1]);flashVars.tuesdayTime[2] = (int)(doc["tuesdayTime"][2]);
      updateAlarm(&alarms.tuesday,dowTuesday,flashVars.tuesdayTime[0],flashVars.tuesdayTime[1],flashVars.tuesdayTime[2], triggerAlarm);
    } if (doc.containsKey("wednesdayTime")){
      flashVars.wednesdayTime[0] = (int)(doc["wednesdayTime"][0]);flashVars.wednesdayTime[1] = (int)(doc["wednesdayTime"][1]);flashVars.wednesdayTime[2] = (int)(doc["wednesdayTime"][2]);
      updateAlarm(&alarms.wednesday,dowWednesday,flashVars.wednesdayTime[0],flashVars.wednesdayTime[1],flashVars.wednesdayTime[2], triggerAlarm);
    } if (doc.containsKey("thursdayTime")){
      flashVars.thursdayTime[0] = (int)(doc["thursdayTime"][0]);flashVars.thursdayTime[1] = (int)(doc["thursdayTime"][1]);flashVars.thursdayTime[2] = (int)(doc["thursdayTime"][2]);
      updateAlarm(&alarms.thursday,dowThursday,flashVars.thursdayTime[0],flashVars.thursdayTime[1],flashVars.thursdayTime[2], triggerAlarm);
    } if (doc.containsKey("fridayTime")){
      flashVars.fridayTime[0] = (int)(doc["fridayTime"][0]);flashVars.fridayTime[1] = (int)(doc["fridayTime"][1]);flashVars.fridayTime[2] = (int)(doc["fridayTime"][2]);
      updateAlarm(&alarms.friday,dowFriday,flashVars.fridayTime[0],flashVars.fridayTime[1],flashVars.fridayTime[2], triggerAlarm);
    } if (doc.containsKey("saturdayTime")){
      flashVars.saturdayTime[0] = (int)(doc["saturdayTime"][0]);flashVars.saturdayTime[1] = (int)(doc["saturdayTime"][1]);flashVars.saturdayTime[2] = (int)(doc["saturdayTime"][2]);
      updateAlarm(&alarms.saturday,dowSaturday,flashVars.saturdayTime[0],flashVars.saturdayTime[1],flashVars.saturdayTime[2], triggerAlarm);
    } if (doc.containsKey("sundayTime")){
      flashVars.sundayTime[0] = (int)(doc["sundayTime"][0]);flashVars.sundayTime[1] = (int)(doc["sundayTime"][1]);flashVars.sundayTime[2] = (int)(doc["sundayTime"][2]);
      updateAlarm(&alarms.sunday,dowSunday,flashVars.sundayTime[0],flashVars.sundayTime[1],flashVars.sundayTime[2], triggerAlarm);
    } if (doc.containsKey("brightness")){
      flashVars.brightness = (int)( doc["brightness"] );
      // analogWrite(ledPin, (255*flashVars.brightness)/(100));
      handleLamp(flashVars.brightness);
    } if (doc.containsKey("decayTime")){
      flashVars.decayTime = (int)(doc["decayTime"]);
    } if (doc.containsKey("rgb")){
      flashVars.rgb[0] = doc["rgb"][0];flashVars.rgb[1] = doc["rgb"][1];flashVars.rgb[2] = doc["rgb"][2];
    } if (doc.containsKey("disableAll")){
      flashVars.disableAll = doc["disableAll"] ? true:false ;
      toggleDisableAllAlarms(flashVars.disableAll);

    } if (doc.containsKey("lampOn")){
      flashVars.lampOn = doc["lampOn"] ? true:false;
      Alarm.disable(timerDecay);
      handleLamp(flashVars.lampOn ? 100:0);
    }

    prefs.putBytes("flashVars",&flashVars,sizeof(flashVars));

    server.send(200, "application/json", server.arg("plain"));
  });
  server.on("/getvars",  [](){
    Serial.println("GET /getVars");
    digitalClockDisplay();
    
    StaticJsonDocument<2048> doc;
    doc["mondayTime"][0] = flashVars.mondayTime[0];doc["mondayTime"][1] = flashVars.mondayTime[1];doc["mondayTime"][2] = flashVars.mondayTime[2];
    doc["tuesdayTime"][0] = flashVars.tuesdayTime[0];doc["tuesdayTime"][1] = flashVars.tuesdayTime[1];doc["tuesdayTime"][2] = flashVars.tuesdayTime[2];
    doc["wednesdayTime"][0] = flashVars.wednesdayTime[0];doc["wednesdayTime"][1] = flashVars.wednesdayTime[1];doc["wednesdayTime"][2] = flashVars.wednesdayTime[2];
    doc["thursdayTime"][0] = flashVars.thursdayTime[0];doc["thursdayTime"][1] = flashVars.thursdayTime[1];doc["thursdayTime"][2] = flashVars.thursdayTime[2];
    doc["fridayTime"][0] = flashVars.fridayTime[0];doc["fridayTime"][1] = flashVars.fridayTime[1];doc["fridayTime"][2] = flashVars.fridayTime[2];
    doc["saturdayTime"][0] = flashVars.saturdayTime[0];doc["saturdayTime"][1] = flashVars.saturdayTime[1];doc["saturdayTime"][2] = flashVars.saturdayTime[2];
    doc["sundayTime"][0] = flashVars.sundayTime[0];doc["sundayTime"][1] = flashVars.sundayTime[1];doc["sundayTime"][2] = flashVars.sundayTime[2];
    doc["brightness"] = flashVars.brightness;
    doc["decayTime"] = flashVars.decayTime;
    doc["rgb"][0] = flashVars.rgb[0];doc["rgb"][1] = flashVars.rgb[1];doc["rgb"][2] = flashVars.rgb[2];
    doc["disableAll"] = flashVars.disableAll;
    doc["lampOn"] = flashVars.lampOn;
    time_t tnow = time(nullptr);
    doc["time"] = ctime(&tnow);

    String x;
    serializeJson(doc,x);
    server.send(200, "application/json", x);
  });
  server.on("/reset",  [](){
    Serial.println("GET /reset");
    hardReset(&flashVars);
    prefs.putBytes("flashVars",&flashVars,sizeof(flashVars));
    server.send(200, "text/plain", "Config reset OK");
  });
  server.onNotFound([](){
    Serial.println("Handle not found!");
    server.send(404, "text/plain", "Not found");
  });

  server.begin();
  Serial.println("HTTP server started");
  handleLamp(0);
  ArduinoOTA.begin();
  
}




void Repeats2() {
  Serial.println("5 second timer");
  digitalClockDisplay();
}

void loop() {
  server.handleClient();
  Alarm.delay(10);
  ESP.wdtFeed(); // service the WDT here
  if(!digitalRead(swPin)){
      Alarm.disable(timerDecay);
      handleLamp(0);  
      // Alarm.free(timerWifi);
      // timerWifi = Alarm.timerOnce(600);
  }
  ArduinoOTA.handle();
}

void updateAlarm(AlarmId *alarm, timeDayOfWeek_t dayOfWeek, int hh, int mm, int enable, OnTick_t callback){
  // bool en;
  Alarm.free(*alarm);
      *alarm = Alarm.alarmRepeat(dayOfWeek,
        hh,
        mm,
        0,
        callback);
  // en = enable ? true:false;
  if(enable) Alarm.enable(*alarm);else Alarm.disable(*alarm);

  // Serial.println("New alarm:");
  // Serial.print("day: ");Serial.println(dayOfWeek);
  // Serial.print("hh: ");Serial.println(hh);
  // Serial.print("mm: ");Serial.println(mm);
  // Serial.print("enable: ");Serial.println(enable);
  // Serial.println("Alarm count:");
  // Serial.println(Alarm.count());
  // Serial.println("---------------");
}

void  triggerAlarm(){
  Serial.println("\n\n**** ALARM ****\n\n");
  timerDecay = Alarm.timerRepeat(1,handleLampDecay);
}

void handleLampDecay(){
  int m = (flashVars.brightness*100) / flashVars.decayTime;
  
  currBrightness += m;
  if( currBrightness >= flashVars.brightness*100){
    currBrightness = flashVars.brightness * 100;
    Alarm.disable(timerDecay);
    handleLamp(currBrightness/100);
    currBrightness = BRIGHTNESS_MIN*100;
  } else{
    handleLamp(currBrightness/100);
  }
}

void handleLamp(int brightness){  // 0~100
  // brightness = brightness/2 + BRIGHTNESS_MIN;
  brightness = 100-brightness;
  // analogWrite(ledPin, ((255*brightness)/200)+BRIGHTNESS_MIN);
  analogWrite(ledPin, (uint8)((uint16)(255*brightness)/100));
  Serial.print("Lamp brightness:");
  Serial.println((255*brightness)/100);
  Serial.print("currBrightness:");

  Serial.println(currBrightness);
  if(brightness == 0)currBrightness = BRIGHTNESS_MIN*100;
}

void digitalClockDisplay() {

  Serial.print("------------- time;\n");
  // time_t epochTime = timeClient.getEpochTime();
  // Serial.print("Epoch Time: ");
  // Serial.println(epochTime);
  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);  
  // time_t tnow = time(nullptr);
  // // Serial.println(system_get_time());
  // Serial.print("time(): ");
  // Serial.println(tnow);
}