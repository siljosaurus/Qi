#include <Adafruit_NeoPixel.h>

#include <Adafruit_NeoPixel.h>

#include <CapacitiveSensor.h>

int batteri = 0; // dette er en skala hvor 0 er shutdown, og 9 er fullt.

int slange_pin = 5;
int leds_i_slange = 59;

int photocellPin = A1;
int photocellReading;
int leds_i_solcelleslange = 60;
int aktive_leds_i_solcelleslange = 0;
int LEDpin = 11;
int LEDbrightness;

unsigned long prev = 0;

int pressure_pin = A2;
int pressure_reading; //variable for storing our reading

int varmePin = 10;
unsigned long varmeTid = 200; // lavere verdi blir til varmere peltier
int sensitivity = 200;

int lader = 6;

Adafruit_NeoPixel slange = Adafruit_NeoPixel(leds_i_slange, slange_pin,NEO_GRB + NEO_KHZ800);// ensfargete
Adafruit_NeoPixel slange2 = Adafruit_NeoPixel(leds_i_slange, slange_pin,NEO_GRBW + NEO_KHZ800); // regnbueting

Adafruit_NeoPixel solcelle_slange = Adafruit_NeoPixel(leds_i_solcelleslange, LEDpin,NEO_GRB + NEO_KHZ800);
CapacitiveSensor touch = CapacitiveSensor(4,2); // 1M resistor between pins 4 & 2, pin 2 is sensor pin


void setup() {
  slange.clear();
  slange2.clear();
  Serial.begin(9600);
  slange.begin();
  slange2.begin();
  solcelle_slange.begin();
  //touch.set_CS_AutocaL_Millis(0xFFFFFFFF);
  //pinMode(varmePin, OUTPUT);
  //pinMode(lader, OUTPUT);
}


void loop(void) {
  /*
  if (Registrer_touch()) {
    Gi_varme();
    Serial.println("gir varme");
    } else {
      Stopp_varme();
      Serial.println("stopper varme");
      }
   
   if (Registrer_mobil()) {
    lad();
    Serial.println("lader");
    } else {
      stopp_Lading();
      Serial.println("lader ikke");
      }
      */

    /*
    solcelle_slange.clear();
    Solcelle();
    solcelle_slange.clear();
    */

    
  slange2.clear();
  slange.clear();

  // lade ned
  Ladeeffekt(238,130,238, 50); // The first 3 parametres roughly defines the color, the last parameter indicates delay in the loop.
  
  slange.clear();
  slange2.clear();

  // lade opp
  theaterChase(238,130,238,150);

  slange.clear();
  slange2.clear();

  // fylles opp 
  meteorRain(0xff,0xff,0xff,10, 64, true, 50);
  
  slange.clear();
  slange2.clear();

  // kollaps kanskje?
  Strobe(0xff, 0x77, 0x00, 10, 100, 1000);
  
  slange.clear();
  slange2.clear();
}





void Styr_batteriet(){}


void Batteriet_lades_opp() {
  if (batteri < 9) {
    batteri += 1;
    }
    else {
      Batteri_fullt();
      }
    delay(4000); // 4 sek for å lade opp
    // batteriet pulserer litt for å indikere at det lades opp
  }
  
void Batteriet_lades_ned() {
  if (batteri > 0) {
    batteri -= 1;
    }
    else {
      Batteri_shutdown();
      }
    delay(2000); // 2 sek for å lade ned
    // batteriet blir svakere, ser ut som det draines
  }
  
void Batteriet_kollapser() {
    //skjer kun ved mobillading
    //if (batteri == 0) {kollaps}
    //else {Batteriet_lades_ned();}
    // delay(1000) 1 sek, halvparten av vanlig shutdown
  }

void Batteri_fullt(){
  // batteri = 9
  // masse farger og pulserende, maks brightness
  }
  
void Batteri_halvfullt(){
  // batteri = mellom 4 og 8
  // litt farger og litt pulserende, halvveis brightness
  }
  
void Batteri_lavt(){
  // batteri = mellom 3 og 1
  // rød og blinker, lite brigtness
  }
  
void Batteri_shutdown(){
  // batteri = 0
  // ingen lys
  }

boolean Registrer_touch(){
  long total = touch.capacitiveSensor(30);
  Serial.println(total);
  
  if (total > sensitivity) {
    return true;
    } else {
      return false;
      }
}

void Gi_varme(){
  //Batteriet_lades_opp();
  if (millis() % 2000 > varmeTid) {
    digitalWrite(varmePin, HIGH);
    } else {
      digitalWrite(varmePin, LOW);
      }
  
  //funksjon for å gi varme ved registrering, hvordan peltier skal oppføre seg ved interaksjon
  //< 5 sek: level 1 (kaldest, romtemp)
  //< 10 && > 5 sek: level 2 (litt varmt)
  //< 15 && > 10 sek: level 3 (varmest)
}

void Stopp_varme() {
  digitalWrite(varmePin, LOW);
}


boolean Registrer_mobil(){
  pressure_reading = analogRead(pressure_pin);
  Serial.println(pressure_reading);
  
  if (pressure_reading > 0) {
    //Batteriet_kollapser();
    return true;
    } else {
      return false;
      }
}

void lad() {
  digitalWrite(lader, HIGH);
}


void stopp_Lading() {
  digitalWrite(lader, LOW);
}

 
void Solcelle(){
  int photocellReading = analogRead(photocellPin);
  Serial.print("Analog reading = ");
  Serial.println(photocellReading);

  
  //photocellReading = 1023 - photocellReading;
  //for (int i= 0; i<leds_i_slange; i++) {
     //solcelle_slange.setPixelColor(i, 0, 0, 255);
    //solcelle_slange.show();}
    
  LEDbrightness = map(photocellReading, 0, 210, 0, 255);
  //Serial.println(LEDbrightness);
  
    int num_leds_to_show = map(photocellReading, 60, 700, 0, leds_i_solcelleslange);
    //peak = alt lyser
    Serial.println(num_leds_to_show);
    int diff = num_leds_to_show - aktive_leds_i_solcelleslange;

    if (diff > 1) {
      unsigned long now = millis();
      if (now - prev > 100) { //har det gått mer enn et sekund siden forrige økning?:
         diff = 1; // 
         prev = now;
        } else {
          diff = 0;
          }
    }

    num_leds_to_show = aktive_leds_i_solcelleslange + diff;
    aktive_leds_i_solcelleslange = num_leds_to_show;
    
    if(num_leds_to_show > 40){
      // batteri_ladesoppeller ned
    }

    for (int i=0; i < leds_i_solcelleslange; i++){
      if (i <= num_leds_to_show){
        solcelle_slange.setPixelColor(i, solcelle_slange.Color(0, 150, 0));
      }
       else{
          solcelle_slange.setPixelColor(i, solcelle_slange.Color(0, 0, 0));
        }
     solcelle_slange.show();
  }
}
 

void Tenn_Lysbulb(){
 /* if buttonPressed:
    send strøm */
}

void pulseWhite(uint8_t wait) { // GRBW configuration
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    slange2.fill(slange2.Color(255,255, 255, slange2.gamma8(j))); // 100, 100, 100 for mer rolige farger
    slange2.setBrightness(255);
    slange2.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    slange2.fill(slange2.Color(0, 255, 255, slange2.gamma8(j))); // 0, 100, 100 for lolipop farger
    slange2.show();
    delay(wait);
  }
}

void gradientRed(int wait) { // GRB configuration
  
    for(int i=0; i<60; i++) { 
      slange.setPixelColor(i, slange.Color(0, 255, 0));
      delay(70);
      slange.show();   
      delay(wait);
    }
 
}

void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    slange2.fill(slange2.Color(255, 255, 255, slange2.gamma8(j)));
    slange2.show();
    delay(FlashDelay);
    slange2.fill(slange2.Color(0, 0, 0, slange2.gamma8(j)));
    slange2.show();
    delay(FlashDelay);
  }
 
 delay(EndPause);
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
   int NUM_LEDS = 60;
  // slange needs to be clear

  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        slange.setPixelColor(i-j, red, green, blue);
      }
    }
   
    slange.show();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = slange.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    slange.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}

void theaterChase(byte red, byte green, byte blue, int SpeedDelay) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < 60; i=i+3) {
        slange.setPixelColor(i+q, red, green, blue); //turn every third pixel on
      }
      slange.show();
      delay(SpeedDelay);
     
      for (int i=0; i < 60; i=i+3) {
        slange.setPixelColor(i+q, 0,0,0); //turn every third pixel off
      }
    }
  }
}


 void Ladeeffekt(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
 
  for(int j=0; j<60*2; j++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<60; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        slange.setPixelColor(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
      slange.setBrightness(255);
      slange.show();
      delay(WaveDelay);
  }
}
