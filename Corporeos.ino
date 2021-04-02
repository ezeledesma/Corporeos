#include <FastLED.h>
#include <SoftwareSerial.h>
#include "Arduino.h"

#define LED_PIN     5
#define NUM_LEDS    183
#define LED_TYPE    WS2812B
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];

uint8_t BRIGHTNESS = 255;

SoftwareSerial mySerial(8, 7); // Rx, Tx

String code;
char buff = ' ';

void update_rainbow_line(uint8_t line);

unsigned long previousMillis = 0;
unsigned long interval = 20;                 // Ticks en ms

void setup() {
  delay(1000);  // Por las dudas
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  mySerial.begin(19200);
  // Para DEBUG
   Serial.begin(9600);
   Serial.println("READY.....");
}

#define REPOSO  0
#define PULSO   1
#define WAVE    2
#define RAINBOW 3
#define STARS   4
#define OFF     5
uint8_t estado = REPOSO;
uint8_t i = 0;

void update_bt();
void update_estado();
void rainbow_all();
void breathe_all ();

void loop() {
  update_bt();
  update_estado();
  timer();
}

void timer() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    timer_handler();
  }
}

uint8_t color[3] = {0, 0, 0};

void timer_handler() {
    if (estado == REPOSO) {     // Reposo, esperando comandos
    //DotMove(255, 255, 255);
    //ProbarColor(50);
    //RainBowWave();
    //p_RainBow();
    //breathe_all();
    //rainbow_all();
    //lines();
    //stars();
    //pincel();
    //letras();
    set_color();
  }
  else if (estado == RAINBOW) {
    RainBowWave();
  }
  else if (estado == WAVE) {
    //p_RainBow();
    pincel();
  }
  else if (estado == PULSO) {
    lines();
  }
  else if (estado == OFF) {
  }
  else if (estado == STARS) {
    stars();
  }
  FastLED.show();
}

void set_color() {
  uint8_t j = 0;
  while (j < NUM_LEDS) {
    leds[j].r = color[0];
    leds[j].g = color[1];
    leds[j].b = color[2];
    j++;
  }
}

void DotMove(uint8_t r, uint8_t g, uint8_t b) {
  leds[i].r = 0;
  leds[i].g = 0;
  leds[i].b = 0;
  i++;
  i %= NUM_LEDS;
  leds[i].r = r;
  leds[i].g = g;
  leds[i].b = b;
}

void ProbarColor(uint8_t dot) {
  uint8_t j = 0;
  while(j < NUM_LEDS) {
   //leds[j].r = 0;
   //leds[j].g = 0;
   //leds[j].b = 0;
   leds[j].r = analogRead(0)/4;
   leds[j].g = analogRead(1)/4;
   leds[j].b = analogRead(2)/4;
   j++;
   }
   //leds[dot].r = analogRead(0)/4;
   //leds[dot].g = analogRead(1)/4;
   //leds[dot].b = analogRead(2)/4;
}

#define RED     0
#define D_RED   1
#define GREEN   2
#define D_GREEN 3
#define BLUE    4
#define D_BLUE  5

uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;

uint8_t tira_r[NUM_LEDS];
uint8_t tira_g[NUM_LEDS];
uint8_t tira_b[NUM_LEDS];

void RainBowWave() {
  static uint8_t cargar = 0;
  uint8_t i = 0;
  uint8_t x = 0;
  if (!cargar) {
    while (i < NUM_LEDS) {
      tira_r[i] = red;
      tira_g[i] = green;
      tira_b[i] = blue;
      while (x < (1530/(NUM_LEDS/2)) ) {
        update_rainbow();
        x++;
      }
      x = 0;
      i++;
    }
    cargar = 1;
  }
  while(x < NUM_LEDS) {
    leds[x].r = tira_r[x];
    leds[x].g = tira_g[x];
    leds[x].b = tira_b[x];
    x++;
  }
  i = 0;
  uint8_t aux_r = tira_r[0];
  uint8_t aux_g = tira_g[0];
  uint8_t aux_b = tira_b[0];
  while (i < (NUM_LEDS-1)) {
    tira_r[i] = tira_r[i+1];
    tira_g[i] = tira_g[i+1];
    tira_b[i] = tira_b[i+1];
    i++;
  }
  tira_r[i] = aux_r;
  tira_g[i] = aux_g;
  tira_b[i] = aux_b;
}

void update_rainbow() {
  static uint8_t r_estado = RED;
  if (r_estado == RED) {
    if (red == 255) r_estado = D_BLUE;
    else red++;
  }
  if (r_estado == D_BLUE) {
    if (!blue) r_estado = GREEN;
    else blue--;
  }
  if (r_estado == GREEN) {
    if (green == 255) r_estado = D_RED;
    else green++;
  }
  if (r_estado == D_RED) {
    if (!red) r_estado = BLUE;
    else red--;
  }
  if (r_estado == BLUE) {
    if (blue == 255) r_estado = D_GREEN;
    else blue++;
  }
  if (r_estado == D_GREEN) {
    if (!green) r_estado = RED;
    else green--;
  }
}

uint8_t pred = 0;
uint8_t pgreen = 0;
uint8_t pblue = 255;

void p_RainBow() {
  static uint8_t p_line = 0;
  uint8_t x = 0;
  static uint8_t dir = 0;
  while (x < 1020/NUM_LEDS) {
   update_p_rainbow();
   x++;
  }
  leds[p_line].r = pred;
  leds[p_line].g = pgreen;
  leds[p_line].b = pblue;
  if (dir == 0) { 
   p_line++;
  }
  else {
   p_line--;
  }
  if (p_line == (NUM_LEDS-1)) {
     dir = 1;
  }
  else if (!p_line) {
    dir = 0;
  }
}

void update_p_rainbow() {
  static uint8_t p_estado = GREEN;
  if (p_estado == GREEN) {
    if (pgreen == 255) p_estado = RED;
    else pgreen++;
  }
  if (p_estado == RED) {
    if (pred == 255) p_estado = D_RED;
    else pred++;    
  }
  if (p_estado == D_RED) {
    if (!pred) p_estado = D_GREEN;
    else pred--;
  }
  if (p_estado == D_GREEN) {
    if (!pgreen) p_estado = GREEN;
    else pgreen--;
  }
}


void breathe_all () {
  uint8_t j = 0;
  static uint8_t blink_a = 0;
  static uint8_t state = 0;
  while (j < NUM_LEDS) {
    leds[j].r = tira_r[j];
    leds[j].g = tira_g[j];
    leds[j].b = tira_b[j];
    j++;
  }
  if (state == 0) {
    if (BRIGHTNESS == 255) state = 1;
    else BRIGHTNESS++;
  }
  if (state == 1) {
   if (!BRIGHTNESS) {
    state = 0;
   }
   else BRIGHTNESS--; 
  }
  FastLED.setBrightness(BRIGHTNESS);
}

void rainbow_all() {
  uint8_t j = 0;
  uint8_t i = 0;
  while (i < 1530/100) {
      update_rainbow();
      i++;
  }
  i = 0;
  while ((j+18) <= 35) {
    leds[j].r = red;
    leds[j].g = green;
    leds[j].b = blue;
    j++;
  }
  j = 0;
  while (j <= 17) {
    while (i < 1530/100) {
      update_rainbow();
      i++;
    }
    i = 0;
    leds[17-j].r = red;
    leds[17-j].g = green;
    leds[17-j].b = blue;
    leds[j+35].r = red;
    leds[j+35].g = green;
    leds[j+35].b = blue;
    j++;
  }
  j = 52;
  while (j <= 78) {
    while (i < 1530/100) {
      update_rainbow();
      i++;
    }
    i = 0;
    leds[j].r = red;
    leds[j].g = green;
    leds[j].b = blue;
    j++;
  }
  j += 26;
  while (j <= 111) {
    while (i < 1530/100) {
      update_rainbow();
      i++;
    }
    i = 0;
    leds[j].r = red;
    leds[j].g = green;
    leds[j].b = blue;
    j++;
  }
   j = 78;
  while (j <= 104) {
    while (i < 1530/100) {
      update_rainbow();
      i++;
    }
    i = 0;
    leds[j].r = red;
    leds[j].g = green;
    leds[j].b = blue;
    j++;
  }
}

uint8_t l_green = 0;
uint8_t l_red = 0;
uint8_t l_blue = 0;
uint8_t l_green2 = 0;
uint8_t l_red2 = 0;
uint8_t l_blue2 = 0;

void lines() {
  static uint8_t len = 10;
  static uint8_t y = len;
  static uint8_t y2 = len + NUM_LEDS/2;
  static uint8_t i2 = NUM_LEDS/2;
  if (y2 >= NUM_LEDS) {
    y2 -= NUM_LEDS;
      update_rainbow_line(2);
  }
  if (y >= NUM_LEDS) {
    y -= NUM_LEDS;
    update_rainbow_line(1);
  }
  leds[y].r = l_red;
  leds[y].g = l_green;
  leds[y].b = l_blue;
  leds[y2].r = l_red2;
  leds[y2].g = l_green2;
  leds[y2].b = l_blue2;
  leds[i].r = 0;
  leds[i].g = 0;
  leds[i].b = 0;
  leds[i2].r = 0;
  leds[i2].g = 0;
  leds[i2].b = 0;
  
  y++;
  y2++;
  i++;
  i%=NUM_LEDS;
  i2++;
  i2%=NUM_LEDS;
}

void update_rainbow_line(uint8_t line) {
  uint8_t red_buffer[] =    {0,   0,    0,    255,  255,  255,  255};
  uint8_t green_buffer[] =  {0,   255,  255,  255,    0,  0,    255};
  uint8_t blue_buffer[] =   {255, 255,  0,      0,    0,  255,  255};
  static uint8_t l = 0;
  static uint8_t l2 = 1;
  if (line == 1) {
    l_red = red_buffer[l];
    l_green = green_buffer[l];
    l_blue = blue_buffer[l];
    l++;
    l %= 7;
  }
  else if (line == 2) {
    l_red2 = red_buffer[l2];
    l_green2 = green_buffer[l2];
    l_blue2 = blue_buffer[l2];
    l2++;
    l2 %= 7;
  }
}

void stars () {
  static uint32_t star = 0;
  uint8_t j = 0;
  static uint32_t tiempo = 0;
  static uint8_t count = 0;
  if (tiempo) {
    tiempo--;
    update_rainbow();
    return;
  }
  else {
    tiempo = random();
    tiempo %=50;
  }
  star=random();
  star%=NUM_LEDS;
  leds[star].r = red;
  leds[star].g = green;
  leds[star].b = blue;
  count++;
  if (count == NUM_LEDS) {
    while (j < NUM_LEDS) {
      leds[j].r = 0;
      leds[j].g = 0;
      leds[j].b = 0;
      j++;
    }
    count = 0;
  }
}

#define DERECHA 0
#define IZQUIERDA 1

void pincel() {
  static uint8_t dir = DERECHA;
  static uint8_t pos = 0;
  static uint8_t p_r[] = {0,   255,   0,  239,  150, 255};
  static uint8_t p_g[] = {0,     0, 255,  127,    0, 255};
  static uint8_t p_b[] = {255,   0,   0,   26,  255, 0};
  static uint8_t ind = 0;
  leds[pos].r = p_r[ind];
  leds[pos].g = p_g[ind];
  leds[pos].b = p_b[ind];

  if (dir == DERECHA) {
    pos++;
  }
  else if (dir == IZQUIERDA) {
    pos--;
  }
  if (pos == (NUM_LEDS-1)) {
    dir = IZQUIERDA;
    ind++;
    ind %= 6;
  }
  else if (pos == 0) {
    dir = DERECHA;
    ind++;
    ind %= 6;
  }
}

void letras () {
  static uint8_t pos = 0;
  static uint8_t pos2 = 51;
  static uint8_t del2 = 98;
  static uint8_t last_r = 0;
  static uint8_t last_g = 0;
  static uint8_t last_b = 0;
  static uint8_t last2_r = 0;
  static uint8_t last2_g = 0;
  static uint8_t last2_b = 0;
  static uint8_t pos3 = 110;
  //----C----//
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  update_rainbow();
  leds[pos].r = red;
  leds[pos].g = green;
  leds[pos].b = blue;
  pos++;
  pos%=50;
  if (pos == 1) {
    last_r = red;
    last_g = green;
    last_b = blue;
  }
  //----C----//
  
  //----A----//
  leds[pos2].r = last_r;
  leds[pos2].g = last_g;
  leds[pos2].b = last_b;
  leds[del2].r = 0;
  leds[del2].g = 0;
  leds[del2].b = 0;
  del2 ++;
  pos2 ++;
  if (pos2 == 101) {
    pos2 = 51;
  }
  if (del2 == 101) {
    del2 = 51;
  }
  if (pos2 == 100) {
    last2_r = last_r;
    last2_g = last_g;
    last2_b = last_b;
    pos3 = 110;
  }
  //----A----//

  //----N----//
  leds[pos3].r = last2_r;
  leds[pos3].g = last2_g;
  leds[pos3].b = last2_b;
  pos3++;
  if (pos3 == (NUM_LEDS-1)) {
    pos3 = 110;
  }
  leds[pos3].r = last2_r;
  leds[pos3].g = last2_g;
  leds[pos3].b = last2_b;
  pos3++;
  if (pos3 == (NUM_LEDS-1)) {
    pos3 = 110;
  }
  //----N----//
}

#define LEIDO 0
#define SIN_LEER 1

uint8_t bt = LEIDO;
uint8_t change = 0;

void update_bt() {
   uint8_t i = 0;
   while (mySerial.available()) {
    buff = mySerial.read();
   if (buff == '#') {
      code = "";
      bt = LEIDO;
   }
   else if(buff == '$') {
      Serial.print("Code: ");
      Serial.println(code);
      bt = SIN_LEER;
      if (i == 2 && change) bt = LEIDO;
      if (change) change = 0;
      break;
   }
   else if (buff == 'c') {
      change = 1;
   }
   else {
    if (change) {
     color[i] = buff;
     i++; 
    }
    else {
      code += buff;
    }
    }
  }
}

void update_estado() {
  if (bt == LEIDO) return;
  if (code == "ir") {
    estado = RAINBOW;
  }
  else if (code == "pi") {
    estado = WAVE;
  }
  else if (code == "p-") {
    if (interval <= 45) {
      interval += 5;
    }
  }
  else if (code == "p+") {
    if (interval >= 10) {
      interval -= 5;
    }
  }
  else if (code == "li") {
    estado = PULSO;
  }
  else if (code == "of") {
    estado = OFF;
    off();
  }
  else if (code == "b+") {
    if (BRIGHTNESS <= 225) {
      BRIGHTNESS += 30;
      FastLED.setBrightness(BRIGHTNESS);
    }
  }
  else if (code == "b-") {
    if (BRIGHTNESS >= 31) {
      BRIGHTNESS -= 30;
      FastLED.setBrightness(BRIGHTNESS);
    }
  }
  else if (code == "st") {
    estado = STARS;
  }
  else if (code == "br") {
    estado = REPOSO;
  }
  bt = LEIDO;
  // Limpiar cartel?
}

void off () {
  uint8_t j = 0;
  while (j < NUM_LEDS) {
    leds[j].r = 0;
    leds[j].g = 0;
    leds[j].b = 0;
    j++;
  }
}

