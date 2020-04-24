#ifndef __RGB_LED_CONTROLER_LIB__
#define __RGB_LED_CONTROLER_LIB__

namespace led_controler{
  enum MODES {
      FADE,
      FLASH,
      PATROL,
      FREE
  };
  
  MODES CURRENT_MODE = FREE;
  short ANALOG_RESOLUTION = 1023;
  int R_PIN, G_PIN, B_PIN;
  short COEFF = 4;
  byte cur_rgb[3];
  float cur_hsv[3];
  
 
  float maxx(float a, float b) {
      return a > b ? a : b;
  }
  
  float minn(float a, float b) {
      return a < b ? a : b;
  }
  void rgb2hsv(byte *rgb, float *hsv) {
      float r_, g_, b_;
      r_ = rgb[0] / 255.;
      g_ = rgb[1] / 255.;
      b_ = rgb[2] / 255.;
      float cmax = maxx(r_, maxx(g_, b_));
      float cmin = minn(r_, minn(g_, b_));
      float delta = cmax - cmin;
      if (delta == 0)
          hsv[0] = 0.0;
      else if (cmax == r_)
          hsv[0] = int((g_ - b_) / delta) % 6;
      else if (cmax == g_)
          hsv[0] = (b_ - r_) / delta + 2;
      else if (cmax == b_)
          hsv[0] = (r_ - g_) / delta + 4;
      hsv[0] *= 60;
      if (cmax == 0)
          hsv[1] = 0;
      else
          hsv[1] = delta / cmax;
      hsv[2] = cmax;
  }
  
  void hsv2rgb(float *hsv, byte *rgb){
      float r, g, b, i, f, p, q, t,h ,s,v;
      h = hsv[0]; s=hsv[1]; v=hsv[2];
      h /= 360;
      i = floor(h * 6);
      f = h * 6 - i;
      p = v * (1 - s);
      q = v * (1 - f * s);
      t = v * (1 - (1 - f) * s);
      switch ((int)i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
      }
      rgb[0] = round(r * 255);
      rgb[1] = round(g * 255);
      rgb[2] = round(b * 255);
  }
  
  /*void hsv2rgb(float *hsv, byte *rgb) {
      float hue = hsv[0];
      float c = hsv[1] * hsv[2];
      float x = c * (1 - fabs((int(hue) /60) % 2 -1));
      float m = hsv[2] - c;
      float r_, g_, b_;
      r_ = g_ = b_ = 0.0;
      if (hue < 60) {
          r_ = c;
          g_ = x;
      } else if (hue < 120) {
          r_ = x;
          g_ = c;
      } else if (hue < 180) {
          b_ = x;
          g_ = c;
      } else if (hue < 240) {
          g_ = x;
          b_ = c;
      } else if (hue < 300) {
          r_ = x;
          b_ = c;
      } else if (hue < 360) {
          b_ = x;
          r_ = c;
      }
      
      Serial.print(m);Serial.print(" ");
      Serial.print(r_);Serial.print(" ");
      Serial.print(g_);Serial.print(" ");
      Serial.println(b_);
      rgb[0] = (byte)((r_ + m) * 255);
      rgb[1] = (byte)((g_ + m) * 255);
      rgb[2] = (byte)((b_ + m) * 255);
  }*/
  byte *current_rgb() {
      return cur_rgb;
  }
  float *current_hsv() {
      return cur_hsv;
  }
  
  void set_analog_resolution(short res) {
      if (res > 0) {
          ANALOG_RESOLUTION = res;
          COEFF = res / 255;
      }
  }
  
  void set_rgb_pins(int r, int g, int b) {
      R_PIN = r;
      G_PIN = g;
      B_PIN = b;
  }
  
  void apply_to_pins(byte r, byte g, byte b) {
      short color_r = r * COEFF;//ANALOG_RESOLUTION - (r * COEFF);
      short color_g = g * COEFF;//ANALOG_RESOLUTION - (g * COEFF);
      short color_b = b * COEFF;//ANALOG_RESOLUTION - (b * COEFF);
      analogWrite(R_PIN, color_r);
      analogWrite(G_PIN, color_g);
      analogWrite(B_PIN, color_b);
  }
  
  void set_color(byte r, byte g, byte b) {
      //apply_to_pins(0, 0, 0);
      //delay(3);
      apply_to_pins(r, g, b);
      //delay(1);
      cur_rgb[0] = r;
      cur_rgb[1] = g;
      cur_rgb[2] = b;
  }
  
  
  struct FADEDATA {
      short delay_time;
      short current_way;
      float step_size;
  };
  FADEDATA fade_args;
  void init_fade_mode(){
      rgb2hsv(cur_rgb, cur_hsv);
      fade_args = (FADEDATA){10, -1, .01};
  }
  void set_fade_params(short spd, float ss){
      if(spd >= 1 && spd < 1000)
          fade_args.delay_time = spd;
      /*if(cw == 1 || cw == -1)
          fade_args.current_way = cw;*/
      if(ss < 1.0 && ss > 0.0001)
          fade_args.step_size = ss;
  }
  void fade_mode_update() {
      cur_hsv[2] += fade_args.step_size * fade_args.current_way;
      if(cur_hsv[2] <= 0.0 ){
          cur_hsv[2] = 0.0;
          fade_args.current_way *= -1;
      } else if(cur_hsv[2] > 1.){
          cur_hsv[2] = 1.;
          fade_args.current_way *= -1;
      }
      hsv2rgb(cur_hsv, cur_rgb);
      set_color(cur_rgb[0], cur_rgb[1], cur_rgb[2]);
      delay(fade_args.delay_time);
  }
  
  
  struct FLASHDATA {
      int delay_dark;
      int delay_light;
      byte random_enabled;
      byte rgb[3];
      byte dorl;
      unsigned long last_change;
  };
  
  FLASHDATA flash_args;
  void init_flash_mode(){
      flash_args = (FLASHDATA){250, 500, 0, {cur_rgb[0], cur_rgb[1], cur_rgb[2]}, 1, 0};
  }
  void set_flash_params(short dd, short dl, byte re){
      if(dd >= 1 && dd < 4000000)
          flash_args.delay_dark = dd;
      if(dl >= 1 && dl < 4000000)
          flash_args.delay_light = dl;
      if(re == 1 || re == 0)
          flash_args.random_enabled = re;
  }
  
  void flash_mode_update() {
      if(flash_args.dorl){
        if((millis() - flash_args.last_change) >= flash_args.delay_light){
          set_color(0,0,0);
          flash_args.last_change = millis();
          flash_args.dorl = !flash_args.dorl;
        }
      } else {
          if((millis() - flash_args.last_change) >= flash_args.delay_dark){
             if(flash_args.random_enabled){
              byte r = random(1, 256), g, b;
              if(r > 127){
                g = random(1, 127);
                b = random(1, 127);
              } else if((g = random(1, 256)) > 127){
                b = random(1, 127);
              } else {
                b = random(127, 256);
              }
              set_color(r, g, b);
              /*
              cur_hsv[0] = random(0, 360);
              cur_hsv[1] = random(70, 100) / 100.;
              cur_hsv[2] = random(50, 100) / 100.;
              hsv2rgb(cur_hsv, cur_rgb);
              //set_color(random(1, 256),random(1, 256),random(1, 256));
              set_color(cur_rgb[0],cur_rgb[1],cur_rgb[2]);*/
            } else {
              set_color(flash_args.rgb[0],flash_args.rgb[1],flash_args.rgb[2]);
            }
            flash_args.last_change = millis();
            flash_args.dorl = !flash_args.dorl;
        }
      }
  }
  
  struct PATROLDATA {
      short interval_start;
      short interval_end;
      short delay_between;
      byte is_one_way;
      short direc;
  };
  
  PATROLDATA patrol_args;
  void init_patrol_mode(){
      patrol_args = (PATROLDATA){30, 330, 20, 0, 1};
  }
  void set_patrol_params(short is, short ie, short db, byte ow){
      if(is < ie && ie <= 360 && is >= 0){
          patrol_args.interval_start = is;
          patrol_args.interval_end = ie;
          cur_hsv[0] = is;
          patrol_args.direc = 1;
      }
      if(db >= 1 && db <= 5000)
          patrol_args.delay_between = db;
      if(ow == 1 || ow == 0)
          patrol_args.is_one_way = ow;
  }
  
  void patrol_mode_update() {
      cur_hsv[0] += patrol_args.direc;
      if(patrol_args.is_one_way){
         if(cur_hsv[0] >= patrol_args.interval_end){
          cur_hsv[0] = patrol_args.interval_start;
        }
      }else if(cur_hsv[0] <= patrol_args.interval_start || cur_hsv[0] >= patrol_args.interval_end ){
        patrol_args.direc *= -1;
      }
      hsv2rgb(cur_hsv, cur_rgb);
      set_color(cur_rgb[0], cur_rgb[1], cur_rgb[2]);
      delay(patrol_args.delay_between);
  }
  void update_color(){
    rgb2hsv(cur_rgb, cur_hsv);
    flash_args.rgb[0] = cur_rgb[0];
    flash_args.rgb[1] = cur_rgb[1];
    flash_args.rgb[2] = cur_rgb[2];
  }
 void set_mode(MODES m){
    if(m == FLASH || m == FADE) update_color();
    if(m == PATROL){
        cur_hsv[1] = cur_hsv[2] = 1.;
        cur_hsv[0] = patrol_args.interval_start;
        patrol_args.direc = 1;
    }
    CURRENT_MODE = m;  
  }
  MODES get_mode(){
    return CURRENT_MODE;
  }
String mode2str(){
  switch(CURRENT_MODE){
    case FADE: return "fade";
    case FLASH: return "flash";
    case FREE: return "free";
    case PATROL: return "patrol";
  }
}
  void random_mode_selector(){
    byte mode = random(0,3);
    switch(mode){
      case 0: 
        set_patrol_params(-1, -1, random(15, 50), -1);
        set_mode(PATROL); 
      break;
      case 1: 
        set_flash_params(random(100, 1000), random(100, 1000), random(0,2));
        set_mode(FLASH); 
      break;
      case 2: 
        set_fade_params(random(10, 20), random(10, 20) / 10.);
        set_mode(FADE);
      break;
    }
  }
  void led_update_state() {
      switch (CURRENT_MODE) {
          case FADE:
              fade_mode_update();
              break;
          case FLASH:
              flash_mode_update();
              break;
          case PATROL:
              patrol_mode_update(); 
              break;
      }
      /*Serial.print(cur_hsv[0]);Serial.print(" ");
      Serial.print(cur_hsv[1]);Serial.print(" ");
      Serial.println(cur_hsv[2]);*/
  }

}

#endif
