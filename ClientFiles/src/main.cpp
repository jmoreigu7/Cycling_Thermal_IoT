#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_TCS34725.h>
#include <Adafruit_MAX31856.h>
#include <mySD.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <TimeLib.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
// Include the UI lib
#include "OLEDDisplayUi.h"

File root;  // Object for read SSID from SDCard
File pass;  // Object for read PASS from SDCard
String buffer;  // Contains the SSID
String baffer;  // Contains thek
int int_executed;
String s_errors_response = "Error:";
String s_status_commands;
String s_data_commands;

char char_ssid[20]; // Contains the SSID
char char_pass[20]; // Contains the PASS

/* Definitions */
#define RELAY_DC3_FAN                   25
#define RELAY_AC4_FAN2                  26
#define RELAY_AC5_MUFLA                 27
#define RELAY_DC1_GENERAL               32
#define RELAY_DC2_AIR                   33
#define MOTOR_STEP                      00
#define MOTOR_DIR                       04
#define SPI_CLK_TC                      14
#define SPI_MISO_TC                     12
#define SPI_MOSI_TC                     13
#define SPI_MAX1                        17
#define SPI_MAX2                        16
#define SPI_MAX3                        02
#define SPI_MAX4                        15
#define SPI_CS_SD                       05
#define SPI_CLK_SD                      18
#define SPI_MOSI_SD                     23
#define SPI_MISO_SD                     19
#define I2C_SCL                         22
#define I2C_SDA                         21
#define OLED_ADDR                       0x3c
#define ON                              0
#define OFF                             1
#define MOTOR_UP                        0x1
#define MOTOR_DOWN                      0x0

const char* host = "tesmaterials.com";    // hostname and php file that we use in interface
const char* insert = "insert.php";
const char* select_comlink = "select_comlink.php";
const char* parameters = "parameters.php";
const char* update = "update.php";
const char* credential = "credential.php";

char buf_f1[20], buf_f2[20], buf_f3[20], buf_f4[20];  // use that buffers to save floats in charactesrs with "gcvt" function

// to clean up those variables, because many of them are useless
int int_id_comm, int_commands, int_data_comm, int_execute, int_response, int_user, int_pass = 0;
int int_id_param, int_status_parameter, int_total_cycles, int_performed_cycles, int_t_max_sample, int_t_min_sample, int_t_max_hf, int_t_min_hf = 0;
int int_t4_max, int_t4_min, int_tub_position, int_rgb_color, int_furnance_power, int_fan_status, int_air_pump, int_motor_status, int_general_status = 0;
int int_line_check = 0;
int int_up, int_down = 0;
// table commands
char id[20], commands[20], data_comm[20], execute[20], response[20];
// table parameters
char id_param[20], status_param[20], total_cycles[20], performed_cycles[20], t_max_cycles[20], t_min_cycles[20], t_max_hf[20], t_min_hf[20], t_max_cu[20];
char t4_max[20], t4_min[20], tub_position[20], tub_time_limit[20], sampling_time[20], moving_speed[20], rgb_max_white[20], rgb_min_white[20], rgb_max_black[20], rgb_min_black[20];
char rgb_max_color[20], rgb_min_color[20], furnance_power[20], fan_status[20], air_pump[20], motor_small_step[20];

typedef enum{
    S_CYCLE                 = 1,
    S_WARMING               = 2,
    S_COOLING               = 3,
    S_END_OF_CYCLE          = 4,
    S_PAUSA                 = 5,
    S_CONTINUAR             = 6,
}states_t;

/* RGB & MAX*/
float f_colorTemp, f_lux = 0;
float f_red, f_green, f_blue = 0;
float f_temp1, f_temp2, f_temp3, f_temp4 = 0;
int int_cycle = 0;
// OLED
SSD1306Wire display(OLED_ADDR, I2C_SDA, I2C_SCL);

/* Wifi Client class to create TCP connections */
WiFiClient clientImportParameters, clientInsertParameters;               // cliente de lectura/escritura para la db.parametros antes del buclue
WiFiClient clientReadTemperature, clientInsertTemperature, clientUpdateTemperature;             // cliente de lectura/escritura para la db.temperaturas antes del buclue

WiFiClient clientImportParametersAlways, clientInsertParametersAlways, clientUpdateParametersAlways;        // cliente de lectura/escritura para la db.parametros buclue
WiFiClient clientReadTemperatureAlways, clientInsertTemperatureAlways, clientUpdateTemperatureAlways;     // cliente de lectura/escritura para la db.temperaturas buclue

WiFiClient clientImportParametersCycle, clientInsertParametersCycle, clientUpdateParametersCycle;         // cliente de lectura/escritura para la db.parametros dentro del estado cicla (Cycle)
WiFiClient clientReadTemperatureCycle, clientInsertTemperatureCycle, clientUpdateTemperatureCycle;      // cliente de lectura/escritura para la db.temperaturas dentro del estado cicla (Cycle)

WiFiClient clientImportParametersWarming, clientInsertParametersWarming, clientUpdateParametersWarming;     // cliente de lectura/escritura para la db.parametros dentro del estado calentando(Warming)
WiFiClient clientReadTemperatureWarming, clientInsertTemperatureWarming, clientUpdateTemperatureWarming;   // cliente de lectura/escritura para la db.temperaturas dentro del estado calentando(Warming)

WiFiClient clientImportParametersCooling, clientInsertParametersCooling, clientUpdateParametersCooling;       // cliente de lectura/escritura para la db.parametros dentro del estado enfriando(Cooling)
WiFiClient clientReadTemperatureCooling, clientInsertTemperatureCooling, clientUpdateTemperatureCooling;    // cliente de lectura/escritura para la db.temperaturas dentro del estado enfriando(Cooling)
/* Port 80 - HTTP/TCP */
const int httpPort = 80;

// SPI: CS(1,2,3,4), MOSI, MISO, CLK
Adafruit_MAX31856 maxthermo1  = Adafruit_MAX31856(SPI_MAX1, SPI_MOSI_TC, SPI_MISO_TC, SPI_CLK_TC);
Adafruit_MAX31856 maxthermo2  = Adafruit_MAX31856(SPI_MAX2, SPI_MOSI_TC, SPI_MISO_TC, SPI_CLK_TC);
Adafruit_MAX31856 maxthermo3  = Adafruit_MAX31856(SPI_MAX3, SPI_MOSI_TC, SPI_MISO_TC, SPI_CLK_TC);
Adafruit_MAX31856 maxthermo4  = Adafruit_MAX31856(SPI_MAX4, SPI_MOSI_TC, SPI_MISO_TC, SPI_CLK_TC);
Adafruit_TCS34725 tcs         = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

/* Functions */
void step(int steps, int servodelay, boolean direccion){
  digitalWrite(MOTOR_DIR,direccion);
  for(int i=0;i<steps;i++){
    digitalWrite(MOTOR_STEP, HIGH);
    delayMicroseconds(servodelay);
    digitalWrite(MOTOR_STEP, LOW);
    delayMicroseconds(servodelay);
  }
}

void motor_stepper(int steps, int servodelay, boolean direccion){
  pinMode(MOTOR_STEP,OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);
  step(steps, servodelay, direccion);
}

void general(int on_off){
  if(on_off == ON){
    pinMode(RELAY_DC1_GENERAL, OUTPUT);
    digitalWrite(RELAY_DC1_GENERAL, on_off);
    int_general_status = 1;
  }else{
    pinMode(RELAY_DC1_GENERAL, OUTPUT);
    digitalWrite(RELAY_DC1_GENERAL, on_off);
    int_general_status = 1;
  }
}

void air(int on_off){
  if(on_off == ON){
    pinMode(RELAY_DC2_AIR, OUTPUT);
    digitalWrite(RELAY_DC2_AIR, on_off);
    int_air_pump = 1;
  }else{
    pinMode(RELAY_DC2_AIR, OUTPUT);
    digitalWrite(RELAY_DC2_AIR, on_off);
    int_air_pump = 0;
  }
}

void fan1(int on_off){
  if(on_off == ON){
    pinMode(RELAY_DC3_FAN, OUTPUT);
    digitalWrite(RELAY_DC3_FAN, on_off);
    int_fan_status = 1;
  }else{
    pinMode(RELAY_DC3_FAN, OUTPUT);
    digitalWrite(RELAY_DC3_FAN, on_off);
    int_fan_status = 1;
  }
}

void fan2(int on_off){
  if(on_off == ON){
    pinMode(RELAY_AC4_FAN2, OUTPUT);
    digitalWrite(RELAY_AC4_FAN2, on_off);
    int_motor_status = 1;
  }else{
    pinMode(RELAY_AC4_FAN2, OUTPUT);
    digitalWrite(RELAY_AC4_FAN2, on_off);
    int_motor_status = 0;
  }
}

void mufla(int on_off){
  if(on_off == ON){
    pinMode(RELAY_AC5_MUFLA, OUTPUT);
    digitalWrite(RELAY_AC5_MUFLA, on_off);
    int_furnance_power = 1;
  }else{
    pinMode(RELAY_AC5_MUFLA, OUTPUT);
    digitalWrite(RELAY_AC5_MUFLA, on_off);
    int_furnance_power = 0;
  }
}

void printDirectory(File dir, int numTabs){
  while(true) {
     File entry =  dir.openNextFile();
     if(! entry){
       break;
     }
     for(uint8_t i=0; i<numTabs; i++){
       Serial.print('\t');   // we'll have a nice indentation
     }
     // Print the name
     Serial.print(entry.name());
     /* Recurse for directories, otherwise print the file size */
     if(entry.isDirectory()){
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     }else{
       /* files have sizes, directories do not */
       Serial.print("\t\t");
       Serial.println(entry.size());
     }
     entry.close();
   }
}

// Check and print any faults
void faults(){
  uint8_t fault1 = maxthermo1.readFault();  uint8_t fault2 = maxthermo2.readFault(); uint8_t fault3 = maxthermo3.readFault(); uint8_t fault4 = maxthermo4.readFault();
  if (fault1) {
    if (fault1 & MAX31856_FAULT_CJRANGE) Serial.println("[INFO] #1 Cold Junction Range Fault");  if (fault1 & MAX31856_FAULT_TCRANGE) Serial.println("[INFO] #1 Thermocouple Range Fault");
    if (fault1 & MAX31856_FAULT_CJHIGH)  Serial.println("[INFO] #1 Cold Junction High Fault");   if (fault1 & MAX31856_FAULT_CJLOW)   Serial.println("[INFO] #1 Cold Junction Low Fault");
    if (fault1 & MAX31856_FAULT_TCHIGH)  Serial.println("[INFO] #1 Thermocouple High Fault");    if (fault1 & MAX31856_FAULT_TCLOW)   Serial.println("[INFO] #1 Thermocouple Low Fault");
    if (fault1 & MAX31856_FAULT_OVUV)    Serial.println("[INFO] #1 Over/Under Voltage Fault");   if (fault1 & MAX31856_FAULT_OPEN)    Serial.println("[INFO] #1 Thermocouple Open Fault");
    // while(1); //halt!
  }
  if (fault2) {
    if (fault2 & MAX31856_FAULT_CJRANGE) Serial.println("[INFO] #2 Cold Junction Range Fault");  if (fault2 & MAX31856_FAULT_TCRANGE) Serial.println("[INFO] #2 Thermocouple Range Fault");
    if (fault2 & MAX31856_FAULT_CJHIGH)  Serial.println("[INFO] #2 Cold Junction High Fault");   if (fault2 & MAX31856_FAULT_CJLOW)   Serial.println("[INFO] #2 Cold Junction Low Fault");
    if (fault2 & MAX31856_FAULT_TCHIGH)  Serial.println("[INFO] #2 Thermocouple High Fault");    if (fault2 & MAX31856_FAULT_TCLOW)   Serial.println("[INFO] #2 Thermocouple Low Fault");
    if (fault2 & MAX31856_FAULT_OVUV)    Serial.println("[INFO] #2 Over/Under Voltage Fault");   if (fault2 & MAX31856_FAULT_OPEN)    Serial.println("[INFO] #2 Thermocouple Open Fault");
    // while(1); // halt!
  }
  if (fault3) {
    if (fault3 & MAX31856_FAULT_CJRANGE) Serial.println("[INFO] #3 Cold Junction Range Fault");  if (fault3 & MAX31856_FAULT_TCRANGE) Serial.println("[INFO] #3 Thermocouple Range Fault");
    if (fault3 & MAX31856_FAULT_CJHIGH)  Serial.println("[INFO] #3 Cold Junction High Fault");   if (fault3 & MAX31856_FAULT_CJLOW)   Serial.println("[INFO] #3 Cold Junction Low Fault");
    if (fault3 & MAX31856_FAULT_TCHIGH)  Serial.println("[INFO] #3 Thermocouple High Fault");    if (fault3 & MAX31856_FAULT_TCLOW)   Serial.println("[INFO] #3 Thermocouple Low Fault");
    if (fault3 & MAX31856_FAULT_OVUV)    Serial.println("[INFO] #3 Over/Under Voltage Fault");   if (fault3 & MAX31856_FAULT_OPEN)    Serial.println("[INFO] #3 Thermocouple Open Fault");
    // while(1); // halt!
  }
  if (fault4) {
    if (fault4 & MAX31856_FAULT_CJRANGE) Serial.println("[INFO] #4 Cold Junction Range Fault");  if (fault4 & MAX31856_FAULT_TCRANGE) Serial.println("[INFO] #4 Thermocouple Range Fault");
    if (fault4 & MAX31856_FAULT_CJHIGH)  Serial.println("[INFO] #4 Cold Junction High Fault");   if (fault4 & MAX31856_FAULT_CJLOW)   Serial.println("[INFO] #4 Cold Junction Low Fault");
    if (fault4 & MAX31856_FAULT_TCHIGH)  Serial.println("[INFO] #4 Thermocouple High Fault");    if (fault4 & MAX31856_FAULT_TCLOW)   Serial.println("[INFO] #4 Thermocouple Low Fault");
    if (fault4 & MAX31856_FAULT_OVUV)    Serial.println("[INFO] #4 Over/Under Voltage Fault");   if (fault4 & MAX31856_FAULT_OPEN)    Serial.println("[INFO] #4 Thermocouple Open Fault"); // circuito abierto
    // while(1); // halt!
  }
}

void max_begin(){
  maxthermo1.begin(); maxthermo2.begin(); maxthermo3.begin(); maxthermo4.begin();
  maxthermo1.setThermocoupleType(MAX31856_TCTYPE_K);
  Serial.print("[SUCCESS] Thermocouple #1 type: ");
  switch (maxthermo1.getThermocoupleType()){
    case MAX31856_TCTYPE_B: Serial.println("B Type"); break;  case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
    case MAX31856_TCTYPE_J: Serial.println("J Type"); break;  case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
    case MAX31856_TCTYPE_N: Serial.println("N Type"); break;  case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
    case MAX31856_TCTYPE_S: Serial.println("S Type"); break;  case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
    case MAX31856_VMODE_G8: Serial.println("Voltage x8 Gain mode"); break;  case MAX31856_VMODE_G32: Serial.println("Voltage x8 Gain mode"); break;
    default: Serial.println("Unknown"); break;
  }
  Serial.print("[SUCCESS] Thermocouple #2 type: ");
  switch (maxthermo2.getThermocoupleType()){
    case MAX31856_TCTYPE_B: Serial.println("B Type"); break;  case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
    case MAX31856_TCTYPE_J: Serial.println("J Type"); break;  case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
    case MAX31856_TCTYPE_N: Serial.println("N Type"); break;  case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
    case MAX31856_TCTYPE_S: Serial.println("S Type"); break;  case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
    case MAX31856_VMODE_G8: Serial.println("Voltage x8 Gain mode"); break;  case MAX31856_VMODE_G32: Serial.println("Voltage x8 Gain mode"); break;
    default: Serial.println("Unknown"); break;
  }
  Serial.print("[SUCCESS] Thermocouple #3 type: ");
  switch (maxthermo3.getThermocoupleType()){
    case MAX31856_TCTYPE_B: Serial.println("B Type"); break;  case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
    case MAX31856_TCTYPE_J: Serial.println("J Type"); break;  case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
    case MAX31856_TCTYPE_N: Serial.println("N Type"); break;  case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
    case MAX31856_TCTYPE_S: Serial.println("S Type"); break;  case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
    case MAX31856_VMODE_G8: Serial.println("Voltage x8 Gain mode"); break;  case MAX31856_VMODE_G32: Serial.println("Voltage x8 Gain mode"); break;
    default: Serial.println("Unknown"); break;
  }
  Serial.print("[SUCCESS] Thermocouple #4 type: ");
  switch (maxthermo4.getThermocoupleType()){
    case MAX31856_TCTYPE_B: Serial.println("B Type"); break;  case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
    case MAX31856_TCTYPE_J: Serial.println("J Type"); break;  case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
    case MAX31856_TCTYPE_N: Serial.println("N Type"); break;  case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
    case MAX31856_TCTYPE_S: Serial.println("S Type"); break;  case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
    case MAX31856_VMODE_G8: Serial.println("Voltage x8 Gain mode"); break;  case MAX31856_VMODE_G32: Serial.println("Voltage x8 Gain mode"); break;
    default: Serial.println("Unknown"); break;
  }
}

void motor_follow_rgb(){ // mejor utilizar el while!!!!
    if (int(f_colorTemp) > 5000 && int(f_colorTemp) < 10000 ){
      while(int(f_colorTemp) > 5000){
        motor_stepper(1400, 400, false);
        tcs.getRGB(&f_red, &f_green, &f_blue);
        f_colorTemp = tcs.calculateColorTemperature(f_red, f_green, f_blue);
      }
      int_down = 1; int_up = 0;
    } else {
      int_up = 1; int_down = 0;
    }
    if (int(f_colorTemp) > 13000){
      while(int(f_colorTemp) > 5000){
        motor_stepper(1400, 400, true);
        tcs.getRGB(&f_red, &f_green, &f_blue);
        f_colorTemp = tcs.calculateColorTemperature(f_red, f_green, f_blue);
      }
      int_up = 1; int_down = 0;
    } else {
      int_down = 1; int_up = 0;
    }
}

void read_thermocouples(){
  f_temp1 = maxthermo1.readThermocoupleTemperature(); sprintf(buf_f1, "%f", f_temp1);
  f_temp2 = maxthermo2.readThermocoupleTemperature(); sprintf(buf_f2, "%f", f_temp2);
  f_temp3 = maxthermo3.readThermocoupleTemperature(); sprintf(buf_f3, "%f", f_temp3);
  f_temp4 = maxthermo4.readThermocoupleTemperature(); sprintf(buf_f4, "%f", f_temp4);
}

void read_rgb(){
  tcs.getRGB(&f_red, &f_green, &f_blue);
  f_colorTemp = tcs.calculateColorTemperature(f_red, f_green, f_blue);
}

void display_oled(String a1){
  display.clear();   // clear the display
  display.drawString(0, 0,  a1);
  display.drawString(0, 16, "T1:"); display.drawString(20, 16, String(f_temp1));
  display.drawString(0, 32, "T2:"); display.drawString(20, 32, String(f_temp2));
  display.drawString(60, 16, "T3:"); display.drawString(80, 16, String(f_temp3));
  display.drawString(60, 32, "T4:"); display.drawString(80, 32, String(f_temp4));
  display.drawString(20, 48, "COLOR:"); display.drawString(60, 48,  String(f_colorTemp));
  display.display();   // write the buffer to the display
}
/* 1. request_clientImportParameters */
void request_clientImportParameters(){
  if (!clientImportParameters.connect(host, httpPort)){ Serial.println("[ERROR][clientImportParameters] connection to database"); ;return; }
  String s_ImportParameters = "/ciclado/"; s_ImportParameters += select_comlink;
  s_ImportParameters += "?usr=tesmater_cicla01&pwd=Diopma2203*"; s_ImportParameters += "&select=parameters";
  s_ImportParameters += "&query=SELECT%20*%20FROM%20tesmater_ciclado01.parameters";
  Serial.print("[clientImportParameters] Requesting: ");Serial.println(s_ImportParameters);
  clientImportParameters.print(String("GET ") + s_ImportParameters + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
  unsigned long timeoutImportParameters = millis();
  while (clientImportParameters.available() == 0) {
    if (millis() - timeoutImportParameters > 5000) {
      Serial.println("[ERROR][clientImportParameters] Timeout !");
      clientImportParameters.stop();
      return;
    }
  }
  while(clientImportParameters.available()) {
    int_id_param = clientImportParameters.readStringUntil(';').toInt();
    int_status_parameter = clientImportParameters.readStringUntil(';').toInt();
    int_total_cycles = clientImportParameters.readStringUntil(';').toInt();
    int_performed_cycles = clientImportParameters.readStringUntil(';').toInt();
    int_t_max_sample = clientImportParameters.readStringUntil(';').toInt();
    int_t_min_sample = clientImportParameters.readStringUntil(';').toInt();
    int_t_max_hf = clientImportParameters.readStringUntil(';').toInt();
    int_t_min_hf = clientImportParameters.readStringUntil(';').toInt();
    int_t4_max = clientImportParameters.readStringUntil(';').toInt();
    int_t4_min = clientImportParameters.readStringUntil(';').toInt();
    int_tub_position = clientImportParameters.readStringUntil(';').toInt();
    int_rgb_color = clientImportParameters.readStringUntil(';').toInt();
    int_furnance_power = clientImportParameters.readStringUntil(';').toInt();
    int_fan_status = clientImportParameters.readStringUntil(';').toInt();
    int_air_pump = clientImportParameters.readStringUntil(';').toInt();
    int_motor_status = clientImportParameters.readStringUntil(';').toInt();
    int_general_status = clientImportParameters.readStringUntil('!').toInt();
  }
}
/* 2. request_clientImportParametersAlways */
void request_clientImportParametersAlways(){
  if (!clientImportParametersAlways.connect(host, httpPort)){ Serial.println("[ERROR][clientImportParametersAlways] connection to database"); return;}
  String s_ImportParametersAlways = "/ciclado/"; s_ImportParametersAlways += select_comlink;
  s_ImportParametersAlways += "?usr=tesmater_cicla01&pwd=Diopma2203*"; s_ImportParametersAlways += "&select=parameters";
  s_ImportParametersAlways += "&query=SELECT%20*%20FROM%20tesmater_ciclado01.parameters"; 
  Serial.print("[INFO][clientImportParametersAlways] Requesting: ");Serial.println(s_ImportParametersAlways);
  clientImportParametersAlways.print(String("GET ") + s_ImportParametersAlways + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
  unsigned long timeoutImportParamsAlways = millis();
  while (clientImportParametersAlways.available() == 0) {
    if (millis() - timeoutImportParamsAlways > 5000) {
      Serial.println("[ERROR][clientImportParametersAlways] Timeout !");
      clientImportParametersAlways.stop();
      return;
    }
  }
  while(clientImportParametersAlways.available()) {
    clientImportParametersAlways.readStringUntil('&'); // (&) inicio
    int_id_param = clientImportParametersAlways.readStringUntil(';').toInt();
    int_status_parameter = clientImportParametersAlways.readStringUntil(';').toInt();
    int_total_cycles = clientImportParametersAlways.readStringUntil(';').toInt();
    int_performed_cycles = clientImportParametersAlways.readStringUntil(';').toInt();
    int_t_max_sample = clientImportParametersAlways.readStringUntil(';').toInt();
    int_t_min_sample = clientImportParametersAlways.readStringUntil(';').toInt();
    int_t_max_hf = clientImportParametersAlways.readStringUntil(';').toInt();
    int_t_min_hf = clientImportParametersAlways.readStringUntil(';').toInt();
    int_t4_max = clientImportParametersAlways.readStringUntil(';').toInt();
    int_t4_min = clientImportParametersAlways.readStringUntil(';').toInt();
    int_tub_position = clientImportParametersAlways.readStringUntil(';').toInt();
    int_rgb_color = clientImportParametersAlways.readStringUntil(';').toInt();
    int_furnance_power = clientImportParametersAlways.readStringUntil(';').toInt();
    int_fan_status = clientImportParametersAlways.readStringUntil(';').toInt();
    int_air_pump = clientImportParametersAlways.readStringUntil(';').toInt();
    int_motor_status = clientImportParametersAlways.readStringUntil(';').toInt();
    int_general_status = clientImportParametersAlways.readStringUntil('!').toInt(); // (!) fin
  }
}
/* 3. request_clientInsertTemperatureAlways */
void request_clientInsertTemperatureAlways(){
  if (!clientInsertTemperatureAlways.connect(host, httpPort)){ Serial.println("[ERROR][clientInsertTemperatureAlways] connection to database"); return; }
  String s_insert_TempAlways = "/ciclado/"; s_insert_TempAlways += insert; s_insert_TempAlways +="?option=temperature";
  s_insert_TempAlways += "&T1="; s_insert_TempAlways += buf_f1;
  s_insert_TempAlways += "&T2="; s_insert_TempAlways += buf_f2;
  s_insert_TempAlways += "&T3="; s_insert_TempAlways += buf_f3;
  s_insert_TempAlways += "&T4="; s_insert_TempAlways += buf_f4;
  s_insert_TempAlways += "&Estado="; s_insert_TempAlways += int_status_parameter;
  Serial.print("[INFO][clientInsertTemperatureAlways] Requesting: "); Serial.println(s_insert_TempAlways);
  clientInsertTemperatureAlways.print(String("GET ") + s_insert_TempAlways + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
  unsigned long timeoutInsertTempAlways = millis();
  while (clientInsertTemperatureAlways.available() == 0) {
      if (millis() - timeoutInsertTempAlways > 5000) {
          Serial.println("[ERROR][clientInsertTemperatureAlways] Timeout !");
          clientInsertTemperatureAlways.stop();
          return;
      }
  }
  while(clientInsertTemperatureAlways.available()) {
    clientInsertTemperatureAlways.readStringUntil('&');
    int_line_check = clientInsertTemperatureAlways.readStringUntil('!').toInt();
    switch (int_line_check){
      case 200: Serial.println("[SUCCESS][clientInsertTemperatureAlways]  Insert OK"); break;
      case 500: Serial.println("[ERROR][clientInsertTemperatureAlways] No se ha realizado el insert de las temperaturas"); break;
      default: Serial.println("[WARNING][clientInsertTemperatureAlways] Check your code"); Serial.println(int_line_check); break;
    }
  }
}
/* 4. request_clientInsertTemperatureCycle */
void request_clientInsertTemperatureCycle (){
  if (!clientInsertTemperatureCycle.connect(host, httpPort)){ Serial.println("[ERROR][CYCLE] connection to database"); s_errors_response += "connect to db.temperature, "; return; }
  String s_insert_TempCycle = "/ciclado/"; s_insert_TempCycle += insert; s_insert_TempCycle +="?option=temperature";
  s_insert_TempCycle += "&T1="; s_insert_TempCycle += buf_f1;
  s_insert_TempCycle += "&T2="; s_insert_TempCycle += buf_f2;
  s_insert_TempCycle += "&T3="; s_insert_TempCycle += buf_f3;
  s_insert_TempCycle += "&T4="; s_insert_TempCycle += buf_f4;
  s_insert_TempCycle += "&Estado="; s_insert_TempCycle += "cicla";
  Serial.print("[INFO][clientInsertTemperatureCycle] Requesting: "); Serial.println(s_insert_TempCycle);
  clientInsertTemperatureCycle.print(String("GET ") + s_insert_TempCycle + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
  unsigned long timeoutInsertTempCycle = millis();
  while (clientInsertTemperatureCycle.available() == 0) {
      if (millis() - timeoutInsertTempCycle > 5000) {
          Serial.println("[ERROR][clientInsertTemperatureCycle] Timeout !");
          s_errors_response += "timeout insert temperature, ";
          clientInsertTemperatureCycle.stop();
          return;
      }
  }
  while(clientInsertTemperatureCycle.available()) {
    clientInsertTemperatureCycle.readStringUntil('&');
    int_line_check = clientInsertTemperatureCycle.readStringUntil('!').toInt();
    switch (int_line_check){
      case 200: Serial.println("[SUCCESS][clientInsertTemperatureCycle] Insert OK"); break;
      case 500: Serial.println("[ERROR][clientInsertTemperatureCycle] No se ha realizado el insert de las temperaturas"); s_errors_response += "insert temp(500), "; break;
      default: Serial.println("[WARNING][clientInsertTemperatureCycle] Check your code"); Serial.println(int_line_check); s_errors_response += "insert temp, "     ; break;
    }
  }
}
/* 5. request_clientUpdateParametersCycle*/
void request_clientUpdateParametersCycle(){
  if (!clientUpdateParametersCycle.connect(host, httpPort)){ Serial.println("[ERROR][clientUpdateParametersCycle] connection to database"); s_errors_response += "connect to db.parameters, "; return; }
  String s_urlUpdateParam1 = "/ciclado/"; s_urlUpdateParam1 += update; s_urlUpdateParam1 +="?option=updateParam";
  s_urlUpdateParam1 += "&status="; s_urlUpdateParam1 += int_status_parameter;
  s_urlUpdateParam1 += "&tub_position="; s_urlUpdateParam1 += int(f_colorTemp);
  s_urlUpdateParam1 += "&rgbcolor="; s_urlUpdateParam1 += int(f_colorTemp);
  s_urlUpdateParam1 += "&furnancepow="; s_urlUpdateParam1 += int_furnance_power;
  s_urlUpdateParam1 += "&fan="; s_urlUpdateParam1 += int_fan_status;
  s_urlUpdateParam1 += "&airpump="; s_urlUpdateParam1 += int_air_pump;
  s_urlUpdateParam1 += "&motorstatus="; s_urlUpdateParam1 += int_motor_status;
  s_urlUpdateParam1 += "&generalstatus="; s_urlUpdateParam1 += int_general_status;
  Serial.print("[INFO][clientUpdateParametersCycle] Requesting: "); Serial.println(s_urlUpdateParam1);
  clientUpdateParametersCycle.print(String("GET ") + s_urlUpdateParam1 + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
  unsigned long timeoutUpdateParamtersCycle = millis();
  while (clientUpdateParametersCycle.available() == 0) {
    if (millis() - timeoutUpdateParamtersCycle > 5000) {
      Serial.println("[ERROR][clientUpdateParametersCycle] Timeout !");
      s_errors_response += "timeout insert parameters, ";
      clientUpdateParametersCycle.stop();
      return;
    }
  }
  while(clientUpdateParametersCycle.available()) {
    clientUpdateParametersCycle.readStringUntil('&');
      int_line_check = clientUpdateParametersCycle.readStringUntil('!').toInt();
      switch (int_line_check){
        case 200: Serial.println("[SUCCESS][clientUpdateParametersCycle] Insert OK"); break;
        case 500: Serial.println("[ERROR][clientUpdateParametersCycle] No se ha realizado el insert de las temperaturas"); s_errors_response += "insert param(500), "; break;
        default: Serial.println("[WARNING][clientUpdateParametersCycle] Check your code"); Serial.println(int_line_check); s_errors_response += "insert param, "     ; break;
      }
  }
}
/* 6. request_clientImportParametersCycle */
void request_clientImportParametersCycle(){
  if (!clientImportParametersCycle.connect(host, httpPort)){ Serial.println("[ERROR][clientImportParametersCycle] connection to database"); return;}
  String s_ReadParametersCycle = "/ciclado/"; s_ReadParametersCycle += select_comlink;
  s_ReadParametersCycle += "?usr=tesmater_cicla01&pwd=Diopma2203*"; s_ReadParametersCycle += "&select=parameters";
  s_ReadParametersCycle += "&query=SELECT%20*%20FROM%20tesmater_ciclado01.parameters";
  Serial.print("[INFO][clientImportParametersCycle] Requesting: "); Serial.println(s_ReadParametersCycle);
  clientImportParametersCycle.print(String("GET ") + s_ReadParametersCycle + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
  unsigned long timeoutReadParamsCycle = millis();
  while (clientImportParametersCycle.available() == 0) {
    if (millis() - timeoutReadParamsCycle > 5000) {
      Serial.println("[ERROR][clientImportParametersCycle] Timeout !");
      clientImportParametersCycle.stop();
      return;
    }
  }
  while(clientImportParametersCycle.available()) {
    clientImportParametersCycle.readStringUntil('&'); // (&) inicio
    int_id_param = clientImportParametersCycle.readStringUntil(';').toInt();
    int_status_parameter = clientImportParametersCycle.readStringUntil(';').toInt();
    int_total_cycles = clientImportParametersCycle.readStringUntil(';').toInt();
    int_performed_cycles = clientImportParametersCycle.readStringUntil(';').toInt();
    int_t_max_sample = clientImportParametersCycle.readStringUntil(';').toInt();
    int_t_min_sample = clientImportParametersCycle.readStringUntil(';').toInt();
    int_t_max_hf = clientImportParametersCycle.readStringUntil(';').toInt();
    int_t_min_hf = clientImportParametersCycle.readStringUntil(';').toInt();
    int_t4_max = clientImportParametersCycle.readStringUntil(';').toInt();
    int_t4_min = clientImportParametersCycle.readStringUntil(';').toInt();
    int_tub_position = clientImportParametersCycle.readStringUntil(';').toInt();
    int_rgb_color = clientImportParametersCycle.readStringUntil(';').toInt();
    int_furnance_power = clientImportParametersCycle.readStringUntil(';').toInt();
    int_fan_status = clientImportParametersCycle.readStringUntil(';').toInt();
    int_air_pump = clientImportParametersCycle.readStringUntil(';').toInt();
    int_motor_status = clientImportParametersCycle.readStringUntil(';').toInt();
    int_general_status = clientImportParametersCycle.readStringUntil('!').toInt(); // (!) fin
  }
}
/* 7. request_clientInsertTemperatureWarming */
void request_clientInsertTemperatureWarming(){
  if (!clientInsertTemperatureWarming.connect(host, httpPort)){ Serial.println("[ERROR][clientInsertTemperatureWarming] connection to database"); s_errors_response += "connect to db.temperature, "; return; }
  String s_insert_tempWarming = "/ciclado/"; s_insert_tempWarming += insert; s_insert_tempWarming +="?option=temperature";
  s_insert_tempWarming += "&T1="; s_insert_tempWarming += buf_f1;
  s_insert_tempWarming += "&T2="; s_insert_tempWarming += buf_f2;
  s_insert_tempWarming += "&T3="; s_insert_tempWarming += buf_f3;
  s_insert_tempWarming += "&T4="; s_insert_tempWarming += buf_f4;
  s_insert_tempWarming += "&Estado="; s_insert_tempWarming += int_status_parameter;
  Serial.print("[INFO][clientInsertTemperatureWarming] Requesting: "); Serial.println(s_insert_tempWarming);
  clientInsertTemperatureWarming.print(String("GET ") + s_insert_tempWarming + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
  unsigned long timeoutInsertTempWarming = millis();
  while (clientInsertTemperatureWarming.available() == 0) {
      if (millis() - timeoutInsertTempWarming > 5000) {
          Serial.println("[ERROR][clientInsertTemperatureWarming] Timeout !");
          s_errors_response += "timeout insert temperature, ";
          clientInsertTemperatureWarming.stop();
          return;
      }
  }
  while(clientInsertTemperatureWarming.available()) {
    clientInsertTemperatureWarming.readStringUntil('&');
    int_line_check = clientInsertTemperatureWarming.readStringUntil('!').toInt();
    switch (int_line_check){
      case 200: Serial.println("[SUCCESS][clientInsertTemperatureWarming] Insert OK"); break;
      case 500: Serial.println("[ERROR][clientInsertTemperatureWarming] No se ha realizado el insert de las temperaturas"); s_errors_response += "insert temp(500), "; break;
      default: Serial.println("[WARNING][clientInsertTemperatureWarming] Check your code"); Serial.println(int_line_check); s_errors_response += "insert temp, "     ; break;
    }
  }
}
/* 8. request_clientUpdateParametersWarming */
void request_clientUpdateParametersWarming(){
  if (!clientUpdateParametersWarming.connect(host, httpPort)){ Serial.println("[ERROR][clientUpdateParametersWarming] connection to database"); return; }
  String s_urlUpdateParam2 = "/ciclado/"; s_urlUpdateParam2 += update; s_urlUpdateParam2 +="?option=updateParam";
  s_urlUpdateParam2 += "&status="; s_urlUpdateParam2 += int_status_parameter;
  s_urlUpdateParam2 += "&tub_position="; s_urlUpdateParam2 += int(f_colorTemp);
  s_urlUpdateParam2 += "&rgbcolor="; s_urlUpdateParam2 += int(f_colorTemp);
  s_urlUpdateParam2 += "&furnancepow="; s_urlUpdateParam2 += int_furnance_power;
  s_urlUpdateParam2 += "&fan="; s_urlUpdateParam2 += int_fan_status;
  s_urlUpdateParam2 += "&airpump="; s_urlUpdateParam2 += int_air_pump;
  s_urlUpdateParam2 += "&motorstatus="; s_urlUpdateParam2 += int_motor_status;
  s_urlUpdateParam2 += "&generalstatus="; s_urlUpdateParam2 += int_general_status;
  Serial.print("[clientUpdateParametersWarming] Requesting: ");Serial.println(s_urlUpdateParam2);
  clientUpdateParametersWarming.print(String("GET ") + s_urlUpdateParam2 + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
  unsigned long timeoutUpdateParamtersWarminng = millis();
  while (clientUpdateParametersWarming.available() == 0) {
    if (millis() - timeoutUpdateParamtersWarminng > 5000) {
      Serial.println("[ERROR][clientUpdateParametersWarming] Timeout !");
      s_errors_response += "timeout insert parameters, ";
      clientUpdateParametersWarming.stop();
      return;
    }
  }
  while(clientUpdateParametersWarming.available()) {
    clientUpdateParametersWarming.readStringUntil('&');
      int_line_check = clientUpdateParametersWarming.readStringUntil('!').toInt();
      switch (int_line_check){
        case 200: Serial.println("[SUCCESS][clientUpdateParametersWarming] Insert OK"); break;
        case 500: Serial.println("[ERROR][clientUpdateParametersWarming] No se ha realizado el insert de las temperaturas"); s_errors_response += "insert param(500), "; break;
        default: Serial.println("[WARNING][clientUpdateParametersWarming] Check your code"); Serial.println(int_line_check); s_errors_response += "insert param, "     ; break;
      }
  }
}
/* 9. request_clientImportParametersWarming */
void request_clientImportParametersWarming(){
  if (!clientImportParametersWarming.connect(host, httpPort)){ Serial.println("[ERROR][clientImportParametersWarming] connection to database db.parameters failed"); return;}
  String s_ReadParametersWarming = "/ciclado/"; s_ReadParametersWarming += select_comlink;
  s_ReadParametersWarming += "?usr=tesmater_cicla01&pwd=Diopma2203*"; s_ReadParametersWarming += "&select=parameters";
  s_ReadParametersWarming += "&query=SELECT%20*%20FROM%20tesmater_ciclado01.parameters";
  Serial.print("[clientImportParametersWarming] Requesting: ");Serial.println(s_ReadParametersWarming);
  clientImportParametersWarming.print(String("GET ") + s_ReadParametersWarming + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
  unsigned long timeoutReadParamsWarming = millis();
  while (clientImportParametersWarming.available() == 0) {
    if (millis() - timeoutReadParamsWarming > 5000) {
      Serial.println("[ERROR][clientImportParametersWarming] Timeout !");
      clientImportParametersWarming.stop();
      return;
    }
  }
  while(clientImportParametersWarming.available()) {
    clientImportParametersWarming.readStringUntil('&'); // (&) inicio
    int_id_param = clientImportParametersWarming.readStringUntil(';').toInt();
    int_status_parameter = clientImportParametersWarming.readStringUntil(';').toInt();
    int_total_cycles = clientImportParametersWarming.readStringUntil(';').toInt();
    int_performed_cycles = clientImportParametersWarming.readStringUntil(';').toInt();
    int_t_max_sample = clientImportParametersWarming.readStringUntil(';').toInt();
    int_t_min_sample = clientImportParametersWarming.readStringUntil(';').toInt();
    int_t_max_hf = clientImportParametersWarming.readStringUntil(';').toInt();
    int_t_min_hf = clientImportParametersWarming.readStringUntil(';').toInt();
    int_t4_max = clientImportParametersWarming.readStringUntil(';').toInt();
    int_t4_min = clientImportParametersWarming.readStringUntil(';').toInt();
    int_tub_position = clientImportParametersWarming.readStringUntil(';').toInt();
    int_rgb_color = clientImportParametersWarming.readStringUntil(';').toInt();
    int_furnance_power = clientImportParametersWarming.readStringUntil(';').toInt();
    int_fan_status = clientImportParametersWarming.readStringUntil(';').toInt();
    int_air_pump = clientImportParametersWarming.readStringUntil(';').toInt();
    int_motor_status = clientImportParametersWarming.readStringUntil(';').toInt();
    int_general_status = clientImportParametersWarming.readStringUntil('!').toInt(); // (!) fin
  }
}

void request_clientInsertTemperatureCooling () {
  if (!clientInsertTemperatureCooling.connect(host, httpPort)){ Serial.println("[ERROR][clientInsertTemperatureCooling] connection to database"); s_errors_response += "connect to db.temperature, "; return; }
  String s_insert_tempCooling = "/ciclado/"; s_insert_tempCooling += insert; s_insert_tempCooling +="?option=temperature";
  s_insert_tempCooling += "&T1="; s_insert_tempCooling += buf_f1;
  s_insert_tempCooling += "&T2="; s_insert_tempCooling += buf_f2;
  s_insert_tempCooling += "&T3="; s_insert_tempCooling += buf_f3;
  s_insert_tempCooling += "&T4="; s_insert_tempCooling += buf_f4;
  s_insert_tempCooling += "&Estado="; s_insert_tempCooling += int_status_parameter;
  Serial.print("[clientInsertTemperatureCooling] Requesting: ");Serial.println(s_insert_tempCooling);
  clientInsertTemperatureCooling.print(String("GET ") + s_insert_tempCooling + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
  unsigned long timeoutInsertTempCooling = millis();
  while (clientInsertTemperatureCooling.available() == 0) {
      if (millis() - timeoutInsertTempCooling > 5000) {
          Serial.println("[ERROR][clientInsertTemperatureCooling] Timeout !");
          s_errors_response += "timeout insert temperature, ";
          clientInsertTemperatureCooling.stop();
          return;
      }
  }
  while(clientInsertTemperatureCooling.available()) {
    clientInsertTemperatureCooling.readStringUntil('&');
    int_line_check = clientInsertTemperatureCooling.readStringUntil('!').toInt();
    switch (int_line_check){
      case 200: Serial.println("[SUCCESS][clientInsertTemperatureCooling] Insert OK"); break;
      case 500: Serial.println("[ERROR][clientInsertTemperatureCooling] No se ha realizado el insert de las temperaturas"); s_errors_response += "insert temp(500), "; break;
      default: Serial.println("[WARNING][clientInsertTemperatureCooling] Check your code"); Serial.println(int_line_check); s_errors_response += "insert temp, "     ; break;
    }
  }
}
/* 10.request_clientUpdateParametersCooling */
void request_clientUpdateParametersCooling() {
  if (!clientUpdateParametersCooling.connect(host, httpPort)){ Serial.println("[ERROR][clientUpdateParametersCooling] connection to database"); s_errors_response += "connect to db.parameters, "; return; }
  String s_urlUpdateParam3 = "/ciclado/"; s_urlUpdateParam3 += update; s_urlUpdateParam3 +="?option=updateParam";
  s_urlUpdateParam3 += "&status="; s_urlUpdateParam3 += int_status_parameter;
  s_urlUpdateParam3 += "&tub_position="; s_urlUpdateParam3 += int(f_colorTemp);
  s_urlUpdateParam3 += "&rgbcolor="; s_urlUpdateParam3 += int(f_colorTemp);
  s_urlUpdateParam3 += "&furnancepow="; s_urlUpdateParam3 += int_furnance_power;
  s_urlUpdateParam3 += "&fan="; s_urlUpdateParam3 += int_fan_status;
  s_urlUpdateParam3 += "&airpump="; s_urlUpdateParam3 += int_air_pump;
  s_urlUpdateParam3 += "&motorstatus="; s_urlUpdateParam3 += int_motor_status;
  s_urlUpdateParam3 += "&generalstatus="; s_urlUpdateParam3 += int_general_status;
  Serial.print("[clientUpdateParametersCooling] Requesting: ");Serial.println(s_urlUpdateParam3);
  clientUpdateParametersCooling.print(String("GET ") + s_urlUpdateParam3 + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
  unsigned long timeoutUpdateParamterscCooling = millis();
  while (clientUpdateParametersCooling.available() == 0) {
    if (millis() - timeoutUpdateParamterscCooling > 5000) {
      Serial.println("[ERROR][clientUpdateParametersCooling] Timeout !");
      s_errors_response += "timeout insert parameters, ";
      clientUpdateParametersCooling.stop();
      return;
    }
  }
  while(clientUpdateParametersCooling.available()) {
    clientUpdateParametersCooling.readStringUntil('&');
      int_line_check = clientUpdateParametersCooling.readStringUntil('!').toInt();
      switch (int_line_check){
        case 200: Serial.println("[SUCCESS][clientUpdateParametersCooling] Insert OK"); break;
        case 500: Serial.println("[ERROR][clientUpdateParametersCooling] No se ha realizado el insert de las temperaturas"); s_errors_response += "insert param(500), "; break;
        default: Serial.println("[WARNING][clientUpdateParametersCooling] Check your code"); Serial.println(int_line_check); s_errors_response += "insert param, "     ; break;
      }
  }
}
/* 11. request_clientImportParametersCooling */
void request_clientImportParametersCooling(){
  if (!clientImportParametersCooling.connect(host, httpPort)){ Serial.println("[ERROR][clientImportParametersCooling] connection to database"); return;}
  String s_ReadParameters = "/ciclado/"; s_ReadParameters += select_comlink;
  s_ReadParameters += "?usr=tesmater_cicla01&pwd=Diopma2203*"; s_ReadParameters += "&select=parameters";
  s_ReadParameters += "&query=SELECT%20*%20FROM%20tesmater_ciclado01.parameters";
  Serial.print("[clientImportParametersCooling] Requesting: ");Serial.println(s_ReadParameters);
  clientImportParametersCooling.print(String("GET ") + s_ReadParameters + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
  unsigned long timeoutReadParamsCooling = millis();
  while (clientImportParametersCooling.available() == 0) {
    if (millis() - timeoutReadParamsCooling > 5000) {
      Serial.println("[ERROR][clientImportParametersCooling] Timeout !");
      clientImportParametersCooling.stop();
      return;
    }
  }
  while(clientImportParametersCooling.available()) {
    clientImportParametersCooling.readStringUntil('&'); // (&) inicio
    int_id_param = clientImportParametersCooling.readStringUntil(';').toInt();
    int_status_parameter = clientImportParametersCooling.readStringUntil(';').toInt();
    int_total_cycles = clientImportParametersCooling.readStringUntil(';').toInt();
    int_performed_cycles = clientImportParametersCooling.readStringUntil(';').toInt();
    int_t_max_sample = clientImportParametersCooling.readStringUntil(';').toInt();
    int_t_min_sample = clientImportParametersCooling.readStringUntil(';').toInt();
    int_t_max_hf = clientImportParametersCooling.readStringUntil(';').toInt();
    int_t_min_hf = clientImportParametersCooling.readStringUntil(';').toInt();
    int_t4_max = clientImportParametersCooling.readStringUntil(';').toInt();
    int_t4_min = clientImportParametersCooling.readStringUntil(';').toInt();
    int_tub_position = clientImportParametersCooling.readStringUntil(';').toInt();
    int_rgb_color = clientImportParametersCooling.readStringUntil(';').toInt();
    int_furnance_power = clientImportParametersCooling.readStringUntil(';').toInt();
    int_fan_status = clientImportParametersCooling.readStringUntil(';').toInt();
    int_air_pump = clientImportParametersCooling.readStringUntil(';').toInt();
    int_motor_status = clientImportParametersCooling.readStringUntil(';').toInt();
    int_general_status = clientImportParametersCooling.readStringUntil('!').toInt(); // (!) fin
  }
}

void setup(){
  Serial.begin(115200); // BaudRate
  Serial.println("[INFO] Welcome to UB Thermal Stress Program");
  Serial.println("[INFO] Start Checking devices");
  /* LCD */
  display.init();
  /* RGB Sensor-TCS34725 */
  Serial.println("[INFO] Check RGB Sensor...");
  if(tcs.begin()){
    Serial.println("[SUCCESS] RGB Sensor-TCS34725 detected");
  }else{
    Serial.println("[ERROR] No found RGB Sensor-TCS34725");
    // while(1); // halt! muere!
  }
  /* microSD - HC4050M*/
  Serial.println("[INFO] Check SD card...");
  if (!SD.begin(SPI_CS_SD, SPI_MOSI_SD, SPI_MISO_SD, SPI_CLK_SD)) {
    Serial.println("[ERROR] SD initialization failed!");
    return;
  }else{
    Serial.println("[SUCCESS] SD detected");
  }
  /* Thermocouple Amplifier MAX31865 */
  Serial.println("[INFO] Check MAX31856's...");
  max_begin();
  faults();
  /* open files for writing&reading */
  root = SD.open("SSID", FILE_WRITE);
  pass = SD.open("PSWD", FILE_WRITE);
  /* Read credentials */
  root = SD.open("SSID");
  if (root) {
    while (root.available()) {
      root.readStringUntil('&');
      buffer = root.readStringUntil(';');
      root.read();
    }
    root.close(); // podemos borrar aqui los archivos
  } else {
    Serial.println("[ERROR] can't read SSID");
  }
  pass = SD.open("PSWD");
  if (pass) {
    while (pass.available()) {
      pass.readStringUntil('&');
      baffer = pass.readStringUntil(';');
      pass.readStringUntil('\n');
    }
    pass.close();
  } else {
    Serial.println("[ERROR] can't read PASS");
  }
  // copying the contents of the string to char array
  strcpy(char_ssid, buffer.c_str());
  strcpy(char_pass, baffer.c_str());
  // FALTA UNA COMPROBACION DEL MOTOR
  // Se podria hacer que si RGB = X en t=0
  // mover el motor hacia otra franja y ver que RGB != X
  motor_stepper(500, 400, true);  // true: right
  motor_stepper(500, 400, false); // false: left

  Serial.println("[INFO] Check WIFI connection...");
  Serial.print("[INFO] Connecting to Network:"); Serial.print(char_ssid);
  WiFi.begin(char_ssid, char_pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("[SUCCESS] Connected to the WIFI network!");
  Serial.print("[INFO] IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("[INFO] Finish the setup, all sensor are running");
  int_cycle = 0;
}
// The commented out code in loop is example of getRawData with clear value.
// Processing example colorview.pde can work with this kind of data too, but It requires manual conversion to
// [0-255] RGB value. You can still uncomments parts of colorview.pde and play with clear value.
void loop(){
  /* Read thermocouples */
  read_thermocouples();
  /* Read RGB sensor */
  read_rgb();
  /* Display */
  display_oled("WELCOME! (0)");
  /* 1. clientUpdateParameters - Request parameters (import parameters from db) - BEGIN*/
  request_clientImportParameters();

  /* EL USUARIO HA MANDADO UNA ORDEN??? */
  while(int_status_parameter!=0){
    /* Read thermocouples */
    read_thermocouples();
    /* Read RGB sensor */
    read_rgb();
    // display_oled("WARMING FURNANCE");
    /* 2. clientImportParametersAlways - Request parameters (import parameters from db) - ALWAYS */
    // delay(200);
    // request_clientImportParametersAlways();
    if (!clientImportParametersAlways.connect(host, httpPort)){ Serial.println("[ERROR][clientImportParametersAlways] connection to database"); return;}
    String s_ImportParametersAlways = "/ciclado/"; s_ImportParametersAlways += select_comlink;
    s_ImportParametersAlways += "?usr=tesmater_cicla01&pwd=Diopma2203*"; s_ImportParametersAlways += "&select=parameters";
    s_ImportParametersAlways += "&query=SELECT%20*%20FROM%20tesmater_ciclado01.parameters"; 
    Serial.print("[INFO][clientImportParametersAlways] Requesting: ");Serial.println(s_ImportParametersAlways);
    clientImportParametersAlways.print(String("GET ") + s_ImportParametersAlways + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "Connection: close\r\n\r\n");
    unsigned long timeoutImportParamsAlways = millis();
    while (clientImportParametersAlways.available() == 0) {
      if (millis() - timeoutImportParamsAlways > 5000) {
        Serial.println("[ERROR][clientImportParametersAlways] Timeout !");
        clientImportParametersAlways.stop();
        return;
      }
    }
    while(clientImportParametersAlways.available()) {
      clientImportParametersAlways.readStringUntil('&'); // (&) inicio
      int_id_param = clientImportParametersAlways.readStringUntil(';').toInt();
      int_status_parameter = clientImportParametersAlways.readStringUntil(';').toInt();
      int_total_cycles = clientImportParametersAlways.readStringUntil(';').toInt();
      int_performed_cycles = clientImportParametersAlways.readStringUntil(';').toInt();
      int_t_max_sample = clientImportParametersAlways.readStringUntil(';').toInt();
      int_t_min_sample = clientImportParametersAlways.readStringUntil(';').toInt();
      int_t_max_hf = clientImportParametersAlways.readStringUntil(';').toInt();
      int_t_min_hf = clientImportParametersAlways.readStringUntil(';').toInt();
      int_t4_max = clientImportParametersAlways.readStringUntil(';').toInt();
      int_t4_min = clientImportParametersAlways.readStringUntil(';').toInt();
      int_tub_position = clientImportParametersAlways.readStringUntil(';').toInt();
      int_rgb_color = clientImportParametersAlways.readStringUntil(';').toInt();
      int_furnance_power = clientImportParametersAlways.readStringUntil(';').toInt();
      int_fan_status = clientImportParametersAlways.readStringUntil(';').toInt();
      int_air_pump = clientImportParametersAlways.readStringUntil(';').toInt();
      int_motor_status = clientImportParametersAlways.readStringUntil(';').toInt();
      int_general_status = clientImportParametersAlways.readStringUntil('!').toInt(); // (!) fin
    }
    /* 3. clientInsertTemperatureAlways - Request temperature (insert into db.temperature) - ALWAYS */
    // delay(200);
    // request_clientInsertTemperatureAlways();
    if (!clientInsertTemperatureAlways.connect(host, httpPort)){ Serial.println("[ERROR][clientInsertTemperatureAlways] connection to database"); return; }
    String s_insert_TempAlways = "/ciclado/"; s_insert_TempAlways += insert; s_insert_TempAlways +="?option=temperature";
    s_insert_TempAlways += "&T1="; s_insert_TempAlways += buf_f1;
    s_insert_TempAlways += "&T2="; s_insert_TempAlways += buf_f2;
    s_insert_TempAlways += "&T3="; s_insert_TempAlways += buf_f3;
    s_insert_TempAlways += "&T4="; s_insert_TempAlways += buf_f4;
    s_insert_TempAlways += "&Estado="; s_insert_TempAlways += int_status_parameter;
    Serial.print("[INFO][clientInsertTemperatureAlways] Requesting: "); Serial.println(s_insert_TempAlways);
    clientInsertTemperatureAlways.print(String("GET ") + s_insert_TempAlways + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "Connection: close\r\n\r\n");
    unsigned long timeoutInsertTempAlways = millis();
    while (clientInsertTemperatureAlways.available() == 0) {
        if (millis() - timeoutInsertTempAlways > 5000) {
            Serial.println("[ERROR][clientInsertTemperatureAlways] Timeout !");
            clientInsertTemperatureAlways.stop();
            return;
        }
    }
    while(clientInsertTemperatureAlways.available()) {
      clientInsertTemperatureAlways.readStringUntil('&');
      int_line_check = clientInsertTemperatureAlways.readStringUntil('!').toInt();
      switch (int_line_check){
        case 200: Serial.println("[SUCCESS][clientInsertTemperatureAlways]  Insert OK"); break;
        case 500: Serial.println("[ERROR][clientInsertTemperatureAlways] No se ha realizado el insert de las temperaturas"); break;
        default: Serial.println("[WARNING][clientInsertTemperatureAlways] Check your code"); Serial.println(int_line_check); break;
      }
    }
    // delay(200);
    /* Follow the black color () */
    motor_follow_rgb();
    /* CONDITION */
    if(int(f_temp4) > int_t_min_hf){ // Wait warming hot furnance
      /* Read RGB sensor */
      display_oled("HOT FURNANCE TMIN!");
      switch (int_status_parameter){
        /*
        S_CYCLE(1)
        1. proceso 1 - Mufla on - control de todos los reles
        2. proceso 2 - START MAXs
        3. proceso 3 - START lcd, rgb
        4. proceso 4 - Busqueda del color negro con el motor
        5. proceso 5 - Insert en la tabla temperatura
        6. proceso 6 - CONDITION to warming
        7. proceso 7 - Actualizar tabla commando
        8. proceso 8 - Actualizar tabla parameters
        9. Salir del estado y volver a leer command/orden. --> int_id_comm = 0
        */
        case S_CYCLE:{
          /* proceso 1 - Mufla on - control de todos los reles */
          mufla(ON); fan1(OFF); fan2(OFF); air(OFF); general(OFF);
          read_thermocouples();
          /* proceso 2 - END */
          /* proceso 3 - START lcd, rgb */
          read_rgb();
          display_oled("STATUS (1): WARM HF");
          /* proceso 3 - END */
          /* proceso 4 - Busqueda del color negro con el motor */
          motor_follow_rgb();
          /* proceso 4 - END */
          /* proceso 5 - insert en la tabla temperatura */
          /* 4. clientInsertTemperatureCycle - Request temperature (insert into db.temperature) - CYCLE */
          // request_clientInsertTemperatureCycle();
          /* proceso 5 - END*/
          /* proceso 6 - CONDITION to warming */
          int_status_parameter = 1;
          if (int(f_temp4) >= int_t_max_hf){ // esta condicion requiere que la mufla llegue a valores muy altos
            // pasamos a calentar
            read_thermocouples();
            read_rgb();
            display_oled("S -->> WARM SAMPLE");
            /* CONDITION 1. mover la muestra hacia la zona de calentamiento (naranja) */
            motor_follow_rgb();
            int_status_parameter = 2; // estado(2) warming
            int_status_parameter = 2;
            delay(50);
          }
          /* END-CONDICIONANTE */
          /* proceso 8 - actualizar tabla parameters */
          read_rgb();
          request_clientUpdateParametersCycle(); // escribo en la db
          // if (!clientUpdateParametersCycle.connect(host, httpPort)){ Serial.println("[ERROR][clientUpdateParametersCycle] connection to database"); s_errors_response += "connect to db.parameters, "; return; }
          // String s_urlUpdateParam1 = "/ciclado/"; s_urlUpdateParam1 += update; s_urlUpdateParam1 +="?option=updateParam";
          // s_urlUpdateParam1 += "&status="; s_urlUpdateParam1 += int_status_parameter;
          // s_urlUpdateParam1 += "&tub_position="; s_urlUpdateParam1 += int(f_colorTemp);
          // s_urlUpdateParam1 += "&rgbcolor="; s_urlUpdateParam1 += int(f_colorTemp);
          // s_urlUpdateParam1 += "&furnancepow="; s_urlUpdateParam1 += int_furnance_power;
          // s_urlUpdateParam1 += "&fan="; s_urlUpdateParam1 += int_fan_status;
          // s_urlUpdateParam1 += "&airpump="; s_urlUpdateParam1 += int_air_pump;
          // s_urlUpdateParam1 += "&motorstatus="; s_urlUpdateParam1 += int_motor_status;
          // s_urlUpdateParam1 += "&generalstatus="; s_urlUpdateParam1 += int_general_status;
          // Serial.print("[INFO][clientUpdateParametersCycle] Requesting: "); Serial.println(s_urlUpdateParam1);
          // clientUpdateParametersCycle.print(String("GET ") + s_urlUpdateParam1 + " HTTP/1.1\r\n" +
          //               "Host: " + host + "\r\n" +
          //               "Connection: close\r\n\r\n");
          // unsigned long timeoutUpdateParamtersCycle = millis();
          // while (clientUpdateParametersCycle.available() == 0) {
          //   if (millis() - timeoutUpdateParamtersCycle > 5000) {
          //     Serial.println("[ERROR][clientUpdateParametersCycle] Timeout !");
          //     s_errors_response += "timeout insert parameters, ";
          //     clientUpdateParametersCycle.stop();
          //     return;
          //   }
          // }
          // while(clientUpdateParametersCycle.available()) {
          //   clientUpdateParametersCycle.readStringUntil('&');
          //     int_line_check = clientUpdateParametersCycle.readStringUntil('!').toInt();
          //     switch (int_line_check){
          //       case 200: Serial.println("[SUCCESS][clientUpdateParametersCycle] Insert OK"); break;
          //       case 500: Serial.println("[ERROR][clientUpdateParametersCycle] No se ha realizado el insert de las temperaturas"); s_errors_response += "insert param(500), "; break;
          //       default: Serial.println("[WARNING][clientUpdateParametersCycle] Check your code"); Serial.println(int_line_check); s_errors_response += "insert param, "     ; break;
          //     }
          // }
          // request_clientImportParametersCycle(); // leo de la db
          /* proceso 8 - END */
        } break;
          /*
          S_WARMING
          1. proceso 1 - Leer temp / rgb color y pasarlo por lcd
          2. proceso 2 - Baja la muestra
          3. proceso 3 - CONDITION - f(muestra) > f_max_cycles
          4. proceso 4 - Actualizar tabla parameters
          Select execute,response from commands where id = 'int_id_comm'-1, si execute/response = 1, continuamos. De lo contrario printar ERROR.
          LCD, mensaje de inicio más ayuda con los parametros que hay que introducir
          Actualizar "commands" con execute = 1, y response = "un texto de ayuda de porque no se ha ejecutado"
          */
        case S_WARMING:{
          /* proceso 1 - leer temp / rgb color y pasarlo por lcd */
          read_thermocouples();
          read_rgb();
          display_oled("STATUS (2): WARMING)");
          /* 2. proceso 2 - baja la muestra - busca el color naranja*/
          motor_follow_rgb();
          /* 2.1 proceso 2.1 */
          /* 2. clientInsertTemperatureWarming - Request temperature (insert into db.temperature) - WARMING */
          // request_clientInsertTemperatureWarming();
          /* proceso 3 - condicion f(muestra) > f_max_cycles */
          int_status_parameter = 2;
          delay(50);
          if (int(f_temp4) >= int_t_max_sample){ // esta condicion requiere que la mufla llegue a valores muy altos
            int_cycle += 1;
            // pasamos a enfriar
            display_oled("S --> COOLING SAMPLE");
            // 1. mover la muestra hacia la zona de calentamiento (naranja) -->
            motor_follow_rgb();
            int_status_parameter = 3; // estado(3) cooling
            int_status_parameter = 3;
            delay(50);
          }
          /* proceso 4 - Actualizar tabla parameters */
          read_rgb();
          // delay(50);
          request_clientUpdateParametersWarming();
          // if (!clientUpdateParametersWarming.connect(host, httpPort)){ Serial.println("[ERROR][clientUpdateParametersWarming] connection to database"); return; }
          // String s_urlUpdateParam2 = "/ciclado/"; s_urlUpdateParam2 += update; s_urlUpdateParam2 +="?option=updateParam";
          // s_urlUpdateParam2 += "&status="; s_urlUpdateParam2 += int_status_parameter;
          // s_urlUpdateParam2 += "&tub_position="; s_urlUpdateParam2 += int(f_colorTemp);
          // s_urlUpdateParam2 += "&rgbcolor="; s_urlUpdateParam2 += int(f_colorTemp);
          // s_urlUpdateParam2 += "&furnancepow="; s_urlUpdateParam2 += int_furnance_power;
          // s_urlUpdateParam2 += "&fan="; s_urlUpdateParam2 += int_fan_status;
          // s_urlUpdateParam2 += "&airpump="; s_urlUpdateParam2 += int_air_pump;
          // s_urlUpdateParam2 += "&motorstatus="; s_urlUpdateParam2 += int_motor_status;
          // s_urlUpdateParam2 += "&generalstatus="; s_urlUpdateParam2 += int_general_status;
          // Serial.print("[clientUpdateParametersWarming] Requesting: ");Serial.println(s_urlUpdateParam2);
          // clientUpdateParametersWarming.print(String("GET ") + s_urlUpdateParam2 + " HTTP/1.1\r\n" +
          //               "Host: " + host + "\r\n" +
          //               "Connection: close\r\n\r\n");
          // unsigned long timeoutUpdateParamtersWarminng = millis();
          // while (clientUpdateParametersWarming.available() == 0) {
          //   if (millis() - timeoutUpdateParamtersWarminng > 5000) {
          //     Serial.println("[ERROR][clientUpdateParametersWarming] Timeout !");
          //     s_errors_response += "timeout insert parameters, ";
          //     clientUpdateParametersWarming.stop();
          //     return;
          //   }
          // }
          // while(clientUpdateParametersWarming.available()) {
          //   clientUpdateParametersWarming.readStringUntil('&');
          //     int_line_check = clientUpdateParametersWarming.readStringUntil('!').toInt();
          //     switch (int_line_check){
          //       case 200: Serial.println("[SUCCESS][clientUpdateParametersWarming] Insert OK"); break;
          //       case 500: Serial.println("[ERROR][clientUpdateParametersWarming] No se ha realizado el insert de las temperaturas"); s_errors_response += "insert param(500), "; break;
          //       default: Serial.println("[WARNING][clientUpdateParametersWarming] Check your code"); Serial.println(int_line_check); s_errors_response += "insert param, "     ; break;
          //     }
          // }
          // delay(50);
        } break;
          /*
          S_COOLING
          1. proceso 1 -
          2. proceso 2 -
          3. proceso 3 -
          4. proceso 4 -
          5. proceso 5 -
          6. proceso 6 -
          parametersin (el php ya se encarga de guardar los 4 datos en la BD o lo realiza el micro=
          Cargamos los datos en nuestras variables: int_total_cycles, int_performed_cycles, int_t_max_sample, int_t_min_sample
          Aplicar el algoritmo de control. (aplicarlo durante el numero de ciclos que haya metido el usuario)
            Enviar datos en todo momento a la BD cada 10 segundos
            Si la muestra supera la int_t_max_sample
              Activamos el ventilador de la mufla fria
              Activamos la bomba de aire
              Subimos la muestra hasta la zona negra (zona de enfriamiento)
              LCD, enfriando la muestra
              Actualizar "parameters": RGB, RELES, CYCLES...
            Si la muestra es inferior a int_t_min_sample
              Bajamos la muestra hasta la zona blanca (zona de calentamiento)
              Desactivamos el ventilador de la mufla fria
              Desactivamos la bomba de aire
              LCD, calentando la muestra
              Actualizar "parameters": RGB, RELES, CYCLES...
            Al acabar el numero de ciclos
              Revisar que el RGB este en la franja negra (zona de enfriamiento)
              Apagar apagar la mufla
              Dejar el ventilador encendido durante 1 minuto
              Actualizar "parameters": RGB, RELES, CYCLES...
              Actualizar "commands" con execute = 1, y response = "un texto de ayuda de porque no se ha ejecutado"
          */
        case S_COOLING:{
          read_thermocouples();
          read_rgb();
          display_oled("STATUS (3): COOLING");
          /* CONDITION */
          int_status_parameter = 3;
          if( int(f_temp4) >= int_t_min_sample ){
            int_status_parameter = 2;
            int_cycle += 1;
          }
           /* . clientInsertTemperatureCooling - Request temperature (insert into db.temperature) - WARMING */
          // request_clientInsertTemperatureCooling();
          read_rgb();
          // delay(50);
          // request_clientUpdateParametersCooling();
          if (!clientUpdateParametersCooling.connect(host, httpPort)){ Serial.println("[ERROR][clientUpdateParametersCooling] connection to database"); s_errors_response += "connect to db.parameters, "; return; }
          String s_urlUpdateParam3 = "/ciclado/"; s_urlUpdateParam3 += update; s_urlUpdateParam3 +="?option=updateParam";
          s_urlUpdateParam3 += "&status="; s_urlUpdateParam3 += int_status_parameter;
          s_urlUpdateParam3 += "&tub_position="; s_urlUpdateParam3 += int(f_colorTemp);
          s_urlUpdateParam3 += "&rgbcolor="; s_urlUpdateParam3 += int(f_colorTemp);
          s_urlUpdateParam3 += "&furnancepow="; s_urlUpdateParam3 += int_furnance_power;
          s_urlUpdateParam3 += "&fan="; s_urlUpdateParam3 += int_fan_status;
          s_urlUpdateParam3 += "&airpump="; s_urlUpdateParam3 += int_air_pump;
          s_urlUpdateParam3 += "&motorstatus="; s_urlUpdateParam3 += int_motor_status;
          s_urlUpdateParam3 += "&generalstatus="; s_urlUpdateParam3 += int_general_status;
          Serial.print("[clientUpdateParametersCooling] Requesting: ");Serial.println(s_urlUpdateParam3);
          clientUpdateParametersCooling.print(String("GET ") + s_urlUpdateParam3 + " HTTP/1.1\r\n" +
                        "Host: " + host + "\r\n" +
                        "Connection: close\r\n\r\n");
          unsigned long timeoutUpdateParamterscCooling = millis();
          while (clientUpdateParametersCooling.available() == 0) {
            if (millis() - timeoutUpdateParamterscCooling > 5000) {
              Serial.println("[ERROR][clientUpdateParametersCooling] Timeout !");
              s_errors_response += "timeout insert parameters, ";
              clientUpdateParametersCooling.stop();
              return;
            }
          }
          while(clientUpdateParametersCooling.available()) {
            clientUpdateParametersCooling.readStringUntil('&');
              int_line_check = clientUpdateParametersCooling.readStringUntil('!').toInt();
              switch (int_line_check){
                case 200: Serial.println("[SUCCESS][clientUpdateParametersCooling] Insert OK"); break;
                case 500: Serial.println("[ERROR][clientUpdateParametersCooling] No se ha realizado el insert de las temperaturas"); s_errors_response += "insert param(500), "; break;
                default: Serial.println("[WARNING][clientUpdateParametersCooling] Check your code"); Serial.println(int_line_check); s_errors_response += "insert param, "     ; break;
              }
          }
          // delay(50);
        } break;
        /*
        S_END_OF_CYCLE
        */
        case S_END_OF_CYCLE: {
          printf("FINISH \n!"); // pensemos que tenemos que meter nosotros los parametros
          int_status_parameter = 0;
          delay(500);
        } break;
        /*
        S_PAUSA
        */
        case S_PAUSA: {
          printf("Loading parameters into DB ... \n!"); // pensemos que tenemos que meter nosotros los parametros
          int_id_comm = 4;
        } break;
        /*
        S_CONTINUAR
        */
        case S_CONTINUAR: {
          printf("Loading parameters into DB ... \n!"); // pensemos que tenemos que meter nosotros los parametros
          int_id_comm = 4;
        } break;
        default: {
          int_id_comm = 0;
        } break;
        }
      }
    }


  Serial.println();
  Serial.println("[CLOSE PROGRAM] FINISH");
  delay(100);
}