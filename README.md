# RGBLedStripServer
RGB şerit led sürmek için yazılmış nodemcu(esp8266) uyumlu arduino idesinde yazılmıs kod.

# Kullanım
ip nodemcu nun ip sini temsil etmektedir. \
http://ip/get_color \
{R: value, G: value, B: value} (ledlerin o anki renk değerlerini geri döndürür.) \
http://ip/set_color?r=value&b=value&g=value \
led lerin rengini ayarlamak için kullanılır. value 0 - 255 arasında olabilir. Üzerinde veya asagısında bir deger verildiginde modu alınır. 

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



