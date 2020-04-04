#include "ESP8266WiFi.h" 
#include <string.h>

#define R_pin D1
#define G_pin D2
#define B_pin D3

char buff[25];
char req[512];
byte cur_r, cur_g, cur_b;
const char* ssid = "wifi_adiniz";
const char* password = "wifi_sifreniz";
WiFiServer server(80);

String successResponse(String content){
  return "HTTP/1.1 200 OK\nContent-Type: application/json\n\n"+content+"\n\n";
}

String failResponse(){
  return "HTTP/1.1 400 Bad Request\nContent-Type: application/json\n\n\n";
}

void setColor(byte r, byte g, byte b){
  short color_r = 1023 - (r * 4) % 1024;
  short color_g = 1023 - (g * 4) % 1024;
  short color_b = 1023 - (b * 4) % 1024;
  
  analogWrite(R_pin, color_r);
  analogWrite(G_pin, color_g);
  analogWrite(B_pin, color_b);
  
  cur_r = r;
  cur_g = g;
  cur_b = b;
}

String getColor(){
  sprintf(buff, "{R:%u,G:%u,B:%u}", cur_r, cur_g, cur_b);
  String resp = buff;
  return successResponse(resp);
}

void setup() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  for(size_t i = 0; i <= 255; i+= 15){
    setColor((byte)i,(byte)i,(byte)i);
    delay(255 - i);
  }
  server.begin();

}

void loop() {
  WiFiClient client = server.available();
  if(client){
    uint32_t now = millis();
    while(!client.available() && millis() - now < 2000){
      delay(5);
    }
    if(client.available()){
      String request = client.readStringUntil('\r');
      client.flush();
      if(request.indexOf("set_color") != -1){
        request.toCharArray(req, 512);
        char * str = &req[request.indexOf("set_color")+10];
        char * tok;
        byte state = -1, r, g, b;
        r = g = b = 0;
        tok = strtok(str, "&= ");
        do{
          if(strlen(tok) > 4 && tok[0] == 'H' && tok[1] == 'T' && tok[2] == 'T' && tok[3] == 'P') break;
          switch(state){
            case 1: r = (byte)atoi(tok); break;
            case 2: g = (byte)atoi(tok); break;
            case 3: b = (byte)atoi(tok); break;
             
          }
          if(strlen(tok) == 1)
            switch(tok[0]){
              case 'R': case 'r': state = 1; break;
              case 'G': case 'g': state = 2; break;
              case 'B': case 'b': state = 3; break;
              default: state = -1;
            }
          else
            state = -1;
        }while((tok = strtok(NULL, "&= ")) != NULL);
        setColor(r, g, b);
        client.println(getColor());
      }else if(request.indexOf("get_color") != -1){
        client.println(getColor());
      } else {
        client.println(failResponse());
      }
    }
  }  
}
