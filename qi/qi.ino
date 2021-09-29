#include <Adafruit_NeoPixel.h>

#include <Adafruit_NeoPixel.h>

#include <CapacitiveSensor.h>

int batteri = 5; // dette er en skala hvor 0 er shutdown, og 9 er fullt.

// pin 2 og 4 er for touch sensor
int slange_pin = 5;
int peltier_kald = 10;
int peltier_varm = 6;

// pin 8 og 7 er for touch sensor 2
int ladeLED = 9;
int solcelleLED = 11;
int lader = 12;

int photocellPin = A0;
int photocellPin2 = A1;
int photocellPin3 = A2;
int pressurePin = A3;

int leds_i_slange = 60;
int leds_i_solcelleslange = 60;

int photocellReading;
int aktive_leds_i_solcelleslange = 0;
int LEDbrightness;

unsigned long prev = 0;

int pressure_reading; //variable for storing our reading
int pressure = 65; //verdi for press

unsigned long varmeTid = 200; // lavere verdi blir til varmere peltier
int sensitivity = 200;


Adafruit_NeoPixel batteri_slange = Adafruit_NeoPixel(leds_i_slange, slange_pin,NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel batteri_regnbue_slange = Adafruit_NeoPixel(leds_i_slange, slange_pin,NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel solcelle_slange = Adafruit_NeoPixel(leds_i_solcelleslange, solcelleLED,NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel lade_slange = Adafruit_NeoPixel(leds_i_slange, ladeLED,NEO_GRB + NEO_KHZ800);
CapacitiveSensor touch = CapacitiveSensor(4,2); // 1M resistor between pins 4 & 2, pin 2 is sensor pin
CapacitiveSensor touch2 = CapacitiveSensor(8,7); // 1M resistor between pins 8 & 7, pin 7 is sensor pin

// TODO: hva slags Type er CapacitiveSensor??? 

void setup() {
  Serial.begin(9600);
  
  batteri_slange.clear();
  batteri_regnbue_slange.clear();
  solcelle_slange.clear();
  lade_slange.clear();
  
  batteri_slange.begin();
  batteri_regnbue_slange.begin();
  solcelle_slange.begin();
  lade_slange.begin();
  
  touch.set_CS_AutocaL_Millis(0xFFFFFFFF);
  touch2.set_CS_AutocaL_Millis(0xFFFFFFFF);
  pinMode(peltier_kald, OUTPUT);
  pinMode(peltier_kald, OUTPUT);
  pinMode(lader, OUTPUT);

  gradientGreen(10);
}


void loop(void) {
  Styr_batteriet();
   
  // Kjenn_kulde(peltier_kald);
  // Kjenn_varme(peltier_varm);

 
 
 /*
  Solcelle(photocellPin);
  Solcelle(photocellPin2);
  Solcelle(photocellPin3);

  if (Registrer_touch()) {
    Gi_varme(peltier_kald);
    Serial.println("gir varme");
    } else {
      Stopp_varme(peltier_kald);
      Serial.println("stopper varme");
      }

    */
     
   if (Registrer_mobil()) {
    lad();
    Serial.println("lader");
    } else {
      stopp_Lading();
      Serial.println("lader ikke");
      } 
      
     

   delay(1000);

}

void Styr_batteriet() {
      Batteri_fullt(); // grønn
      Batteri_halvfullt(); // blå
      Batteri_lavt(); // 
      Batteri_shutdown();
  }


void Batteriet_lades_opp() {
  batteri_slange.clear();
  batteri_regnbue_slange.clear();

  // fylles opp 
   meteorRain(0xff,0xff,0xff,10, 64, true, 50);

  batteri_slange.clear();
  batteri_regnbue_slange.clear();
   
  // lade opp
  // theaterChase(238,130,238,150);
   
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
   batteri_slange.clear();
   batteri_regnbue_slange.clear();
 
   // lade ned
   Ladeeffekt(238,130,238, 50); // The first 3 parametres roughly defines the color, the last parameter indicates delay in the loop.
  
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
  
   batteri_slange.clear();
   batteri_regnbue_slange.clear();

   // kollaps 
   
    //skjer kun ved mobillading
    if (batteri == 0) { //kollaps
         Strobe(0xff, 0x77, 0x00, 10, 100, 1000);

      }
      else {
        batteri -= 1;
        }
     delay(8000); // 10 sek, halvparten av vanlig shutdown
  }

void Batteri_fullt(){

  if (batteri == 9) {
      batteri_slange.clear();
      batteri_regnbue_slange.clear();
      gradientGreen(10);
    }
    
  // batteri = 9
  // masse farger og pulserende, maks brightness
  }

void Batteri_halvfullt(){

  if (batteri > 4 and batteri < 8) {
     batteri_slange.clear();
     batteri_regnbue_slange.clear();
     gradientBlue(10);
    }
  
  
  // batteri = mellom 4 og 8
  // litt farger og litt pulserende, halvveis brightness
   
  }

void Batteri_lavt(){
  
  if (batteri < 3 and batteri > 1) { 
    
    batteri_slange.clear();
    batteri_regnbue_slange.clear();
    
    gradientRed(10);
      }
   
      else {
        batteri -= 1;
        }  
  
 
  // batteri = mellom 3 og 1
  // rød og blinker, lite brightness

  }

void Batteri_shutdown(){
  // batteri = 0

  batteri_slange.clear();
  batteri_regnbue_slange.clear();
  
  for(int i=0; i<leds_i_slange; i++) {
    batteri_slange.setPixelColor(i, batteri_slange.Color(0, 0, 0));
    batteri_slange.show();
    }
  }

boolean Registrer_touch(){
  long total = touch2.capacitiveSensor(30);
  Serial.println(total);

  if (total > sensitivity) {
    return true;
    } else {
      return false;
      }
}


void Kjenn_varme(int pin){
  Batteriet_lades_ned();

   // varmeTid parameteret kan justeres. Lavere verdi gir et varmere peltier.
  if (millis() % 2000 > varmeTid) {
    digitalWrite(pin, HIGH);
    } else {
      digitalWrite(pin, LOW);
      }
}


void Kjenn_kulde(int pin) {
  Batteriet_lades_opp();
  
  // varmeTid parameteret kan justeres. Lavere verdi gir et varmere peltier.
  if (millis() % 2000 > varmeTid) {
    digitalWrite(pin, HIGH);
    } else {
      digitalWrite(pin, LOW);
      }
}

void Stopp_varme(int pin) {
  digitalWrite(pin, LOW);
}


boolean Registrer_mobil(){
  pressure_reading = analogRead(pressurePin);
  Serial.println(pressure_reading);

  if (pressure_reading > pressure) {
    //Batteriet_kollapser();
    return true;
    } else {
      return false;
      }
}

void lad() {
  // koble inn ledlyslenke, og telle ned til kollaps
  
  digitalWrite(lader, HIGH);
  Batteriet_kollapser();
}


void stopp_Lading() {
  digitalWrite(lader, LOW);
}


void Solcelle(int pin){
  int photocellReading = analogRead(pin);
  Serial.print(pin);
  Serial.print("  Analog reading = ");
  Serial.println(photocellReading);

  //photocellReading = 1023 - photocellReading;

 // for (int i= 0; i<leds_i_slange; i++) {
    // solcelle_slange.setPixelColor(i, 0, 0, 255);
    //solcelle_slange.show();}

  //LEDbrightness = map(photocellReading, 0, 210, 0, 255);
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
      
      Batteriet_lades_opp();
    }

    for (int i=0; i < leds_i_solcelleslange; i++){
      if (i <= num_leds_to_show){
        solcelle_slange.setPixelColor(i, solcelle_slange.Color(0, 150, 0));
      }
       else{
          solcelle_slange.setPixelColor(i, solcelle_slange.Color(0, 0, 0));
        }
  }
     solcelle_slange.show();
}


void pulseWhite(uint8_t wait) { // GRBW configuration
  /*
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    batteri_regnbue_slange.fill(batteri_regnbue_slange.Color(255,255, 255, batteri_regnbue_slange.gamma8(j))); // 100, 100, 100 for mer rolige farger
    batteri_regnbue_slange.setBrightness(255);
    batteri_regnbue_slange.show();
    delay(wait);
  }
  */

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    batteri_regnbue_slange.fill(batteri_regnbue_slange.Color(0, 255, 255, batteri_regnbue_slange.gamma8(j))); // 0, 100, 100 for lolipop farger
    batteri_regnbue_slange.setBrightness(255);
    batteri_regnbue_slange.show();
    delay(wait);
  }
}

void gradientRed(int wait) { // GRB configuration
    for(int i=0; i<60; i++) { 
      batteri_slange.setPixelColor(i, batteri_regnbue_slange.Color(255, 0, 0));
      delay(70);
      batteri_slange.show();
      delay(wait);
    }
}

void gradientGreen(int wait) { // GRB configuration
    for(int i=0; i<60; i++) { 
      batteri_slange.setPixelColor(i, batteri_regnbue_slange.Color(0, 255, 0));
      delay(70);
      batteri_slange.show();
      delay(wait);
    }
}

void gradientBlue(int wait) { // GRB configuration
    for(int i=0; i<60; i++) { 
      batteri_slange.setPixelColor(i, batteri_regnbue_slange.Color(0, 0, 255));
      delay(70);
      batteri_slange.show();
      delay(wait);
    }
}

void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    batteri_regnbue_slange.fill(batteri_regnbue_slange.Color(255, 255, 255, batteri_regnbue_slange.gamma8(j)));
    batteri_regnbue_slange.show();
    delay(FlashDelay);
    batteri_regnbue_slange.fill(batteri_regnbue_slange.Color(0, 0, 0, batteri_regnbue_slange.gamma8(j)));
    batteri_regnbue_slange.show();
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
        batteri_slange.setPixelColor(i-j, red, green, blue);
      }
    }
   
    batteri_slange.show();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = batteri_slange.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    batteri_slange.setPixelColor(ledNo, r,g,b);
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
        batteri_slange.setPixelColor(i+q, red, green, blue); //turn every third pixel on
      }
      batteri_slange.show();
      delay(SpeedDelay);
     
      for (int i=0; i < 60; i=i+3) {
        batteri_slange.setPixelColor(i+q, 0,0,0); //turn every third pixel off
      }
    }
  }
}


 void Ladeeffekt(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
 
  for(int j=0; j<60*2; j++) {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<60; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        batteri_slange.setPixelColor(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
      batteri_slange.setBrightness(255);
      batteri_slange.show();
      delay(WaveDelay);
  }
}
