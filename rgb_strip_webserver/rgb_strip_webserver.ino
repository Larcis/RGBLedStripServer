//#define DEBUG_ESP_HTTP_SERVER
#include "ESP8266WiFi.h" 
#include <ESP8266WebServer.h>
#include "led_controler.h"
#include <string.h>

using namespace led_controler;

const char* ssid = "wifi_adiniz";
const char* password = "wifi_sifreniz";
ESP8266WebServer server(80);

char buff[25];
String getColor(){
  byte * cur_rgb = current_rgb();
  sprintf(buff, "{\"r\":%u,\"g\":%u,\"b\":%u}", cur_rgb[0], cur_rgb[1], cur_rgb[2]);
  String resp = buff;
  return resp;
}

void set_color_handler(){
  if(get_mode() == FREE){
    byte r, g, b;
    r = g = b = 0;
    if(server.hasArg("R")) r = server.arg("R").toInt();
    if(server.hasArg("r")) r = server.arg("r").toInt();
    if(server.hasArg("G")) g = server.arg("G").toInt();
    if(server.hasArg("g")) g = server.arg("g").toInt();
    if(server.hasArg("B")) b = server.arg("B").toInt();
    if(server.hasArg("b")) b = server.arg("b").toInt();
    set_color(r, g, b);
    server.send(200);
  } else {
    server.send(400);
  }
}
void get_color_handler(){
  server.send(200, "application/json", getColor());
}

void set_mode_handler(){
  String md = "FREE";
  if(server.hasArg("mode")) md = server.arg("mode");
  md.toUpperCase();
  
  if(md.equals("FADE")) set_mode(FADE);
  else if(md.equals("FLASH")) set_mode(FLASH);
  else if(md.equals("PATROL")) set_mode(PATROL);
  else set_mode(FREE);
  server.send(200, "text/plain", "OK");
}

void get_mode_handler(){
  server.send(200, "text/plain", mode2str());
}

void fade_param_handler(){
  short db = -1;
  float ss = -1;
  if(server.hasArg("delay")) db = server.arg("delay").toInt();
  if(server.hasArg("step_size")) ss = server.arg("step_size").toFloat();
  set_fade_params(db, ss);
  server.send(200, "text/plain", "OK");
}

void flash_param_handler(){
  short delay_dark = -1;
  short delay_light = -1;
  byte random_enabled = 3; 
  if(server.hasArg("delay_dark")) delay_dark = server.arg("delay_dark").toInt();
  if(server.hasArg("delay_light")) delay_light = server.arg("delay_light").toInt();
  if(server.hasArg("random_enabled")) random_enabled = server.arg("random_enabled").toInt();
  set_flash_params(delay_dark, delay_light, random_enabled);
  server.send(200, "text/plain", "OK");
}

void patrol_param_handler(){
  short is = -1;
  short ie = -1;
  short db = -1;
  byte ow = 3;
  if(server.hasArg("interval_start")) is = server.arg("interval_start").toInt();
  if(server.hasArg("interval_end")) ie = server.arg("interval_end").toInt();
  if(server.hasArg("delay")) db = server.arg("delay").toInt();
  if(server.hasArg("one_way")) ow = server.arg("one_way").toInt();
  set_patrol_params(is, ie, db , ow);
  server.send(200, "text/plain", "OK");
}
void random_mode_handler(){
  random_mode_selector();
  server.send(200, "text/plain", mode2str());
}
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println(FADE);
  server.on("/set_color", set_color_handler);
  server.on("/get_color", get_color_handler);
  server.on("/set_mode", set_mode_handler);
  server.on("/get_mode", get_mode_handler);
  server.on("/change_fade_params", fade_param_handler);
  server.on("/change_flash_params", flash_param_handler);
  server.on("/change_patrol_params", patrol_param_handler); 
  server.on("/random_mode", random_mode_handler); 
  
  set_rgb_pins(D1, D2, D3);
  init_fade_mode();
  init_flash_mode();
  init_patrol_mode();
  set_mode(PATROL);
  server.begin();
}

void loop() {
  led_update_state();
  server.handleClient(); 
}
