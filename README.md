
# RGBLedStripServer v2
### Endpoints: 
## :81/set_color  
##### kullanım: /set_color?r=255&g=255&b=255
##### r: kırmızı
##### g : yeşil
##### b : mavi - [0,255] aralığında hepsi
#### Not: set_color endpointine 81. porttan erişmeniz gerekiyor.
## /get_color 
## /set_mode 
##### mode : flash, fade, patrol veya free olabilir
## /get_mode 

## /change_fade_params
##### delay : azaltmalar ve arttırmalar arasındaki bekleme süresi
##### step_size : hsv degeri üzerinden her adımda v değerindeki azaltmanın ya da arttırmanın ne kadar olacagı

## /change_flash_params 
##### delay_dark : ledler kapalı olarak beklenecek sure 
##### delay_light : ledler acık olarak beklenecek sure
##### random_enabled : tek renk mı yanıp sonsun yoksa her yanısta rastgele bır renk mı secılsın 

## /change_patrol_params 
##### interval_start : hue degerı uzerınden gezılecek aralıgın baslangıc noktası
##### interval_end : bıtıs noktası 
##### delay : hue arttırılmadan veya azaltılmadan once beklenecek sure
##### one_way : renk degısımlerı sadece ileri gidip basamı donsun yoksa ileri geri ileri geri seklınde mi olsun -> (interval [0,360] arasında olmalıdır)
## /random_mode



# ~~RGBLedStripServer v1~~ --deprecated
(v2 serverın modu free yapılarak v1 yerine kullanılmaya devam edılebılır)
RGB şerit led sürmek için yazılmış nodemcu(esp8266) uyumlu arduino idesinde yazılmıs kod.

# Kullanım

ip nodemcu nun ip sini temsil etmektedir. \
http://ip/get_color \
{R: value, G: value, B: value} (ledlerin o anki renk değerlerini geri döndürür.) \
http://ip/set_color?r=value&b=value&g=value \
led lerin rengini ayarlamak için kullanılır. value 0 - 255 arasında olabilir. Üzerinde veya asagısında bir deger verildiginde modu alınır. 
### Not: kodu atmadan önce wifi_adiniz ve wifi_sifreniz kısımlarını doldurun.

# Bağlantılar
## Nodemcu icin: 
R : D1 pinine baglı \
G : D2 pinine baglı \
B : D3 pininine baglı \
Vin : l298n in 5v cıkısına baglı \ 
G : güç kaynagının grounduna baglı 
## l298n icin 
nodemcu ya baglanan RGB pinleri sırasıyla \
in1, in2, in3 pinlerine baglı ve out1, out2, out3 sırasıyla let şeridin RGB groundlarına baglı. \
Vin ve GND direk guc kaynagına baglı
## led serit icin 
+12V direk guc kaynagına baglı \
RGB ground kabloları da l298n den cıkan RGB kablolarına baglı.

# Faydalı Linkler
https://www.waveformlighting.com/home-residential/how-to-choose-a-power-supply-for-your-led-strip-projectv \
https://www.electronicwings.com/nodemcu/nodemcu-pwm-with-arduino-ide \
https://randomnerdtutorials.com/esp8266-web-server/ \
https://learn.adafruit.com/rgb-led-strips/usage
