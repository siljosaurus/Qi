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

int sensitivity = 200;


Adafruit_NeoPixel batteri_slange = Adafruit_NeoPixel(leds_i_slange, slange_pin,NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel batteri_regnbue_slange = Adafruit_NeoPixel(leds_i_slange, slange_pin,NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel solcelle_slange = Adafruit_NeoPixel(leds_i_solcelleslange, solcelleLED,NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel lade_slange = Adafruit_NeoPixel(leds_i_slange, ladeLED,NEO_GRB + NEO_KHZ800);
CapacitiveSensor touch = CapacitiveSensor(4,2); // 1M resistor between pins 4 & 2, pin 2 is sensor pin
CapacitiveSensor touch2 = CapacitiveSensor(8,7); // 1M resistor between pins 8 & 7, pin 7 is sensor pin





// TODO: hva slags Type er CapacitiveSensor??? 

void setup() {
  Serial.println("Running void setup: ");
  Serial.begin(9600);
  
  Serial.println("Cleared LED-snakes.");

  batteri_slange.begin();
  batteri_regnbue_slange.begin();
  solcelle_slange.begin();
  lade_slange.begin();
  Serial.println("Began LED-snakes.");
  
  touch.set_CS_AutocaL_Millis(0xFFFFFFFF);
  touch2.set_CS_AutocaL_Millis(0xFFFFFFFF);
  pinMode(peltier_kald, OUTPUT);
  pinMode(peltier_varm, OUTPUT);
  pinMode(lader, OUTPUT);

  // gradientGreen(10);

  batteri_slange.clear();
  batteri_regnbue_slange.clear();
  solcelle_slange.clear();
  lade_slange.clear();
  
  batteri_slange.show();
  batteri_regnbue_slange.show();
  solcelle_slange.show();
  lade_slange.show();
    
}


void loop(void) {

 
   
   //pulseWhite(10);
 
  
   //Styr_batteriet();

  //Test();
  Solcelle_lader_batteri(1000);

  Solcelle(photocellPin);
  SolcelleLiten(photocellPin2);
  SolcelleLiten(photocellPin3);

    if (Registrer_mobil()) {
    lad();
    Serial.println("lader");
    } else {
      stopp_Lading();
      Serial.println("lader ikke");
      }

    if (Registrer_touch()) {
    Kjenn_varme(peltier_varm);
    Serial.println("gir varme");
    } else {
      Stopp_varme(peltier_varm);
      Serial.println("stopper varme");
      }

    if (Registrer_touch2()) {
    Kjenn_kulde(peltier_kald);
    Serial.println("gir varme");
    } else {
      Stopp_varme(peltier_kald);
      Serial.println("stopper varme");
      }
      
  
}

void Styr_booleans() {
  
  
  int photocellReading = analogRead(photocellPin);
  

 }

void Styr_batteriet() {
  
      Batteri_fullt(); // Batteriet gløder blått
      Batteri_halvfullt(); // Batteriet gløder grønt
      Batteri_lavt(); // Batteriet gløder rødt
      Batteri_shutdown(); // Rød/lilla strobeeffekt + lyd
      
  }


void Test() {
    
    Solcelle(photocellPin);
    Solcelle(photocellPin2);
    Solcelle(photocellPin3);
/*
    if (Registrer_touch()) {
    Kjenn_varme(peltier_varm);
    Serial.println("gir varme");
    } else {
      Stopp_varme(peltier_varm);
      Serial.println("stopper varme");
      }

    
     
   if (Registrer_mobil()) {
    lad();
    Serial.println("lader");
    } else {
      stopp_Lading();
      Serial.println("lader ikke");
      } */
}


unsigned long forrige_oppladning = 0;
void Batteriet_lades_opp() {

  unsigned long now = millis();
  int diff = now - forrige_oppladning;

  meteorRain(0xff,0xff,0xff,10, 64, true, 50);

  if (diff > 4000) {
    forrige_oppladning = now;
   
  if (batteri < 9) {
    batteri += 1;
    }
    else {
      Batteri_fullt();
      }
    }
}


unsigned long forrige_nedladning = 0;

void Batteriet_lades_ned() {
  
  unsigned long now = millis();
  
     cold(10);

 
  if (forrige_nedladning - now > 2000){

     if (batteri == 0) {
      Batteri_shutdown();
      }
      // lade ned
      //Ladeeffekt(238,130,238, 50); // The first 3 parametres roughly defines the color, the last parameter indicates delay in the loop.
     
      if (batteri > 0) {
        batteri -= 1;
      }
    }
  }

unsigned long forrige_kollaps = 0;

void Batteriet_kollapser() { 
  unsigned long now = millis();

  if (forrige_kollaps - now > 8000) {

    if (batteri == 0) { //kollaps
            Batteri_shutdown();

        //negativeMeteorRain(255,0,0,10, 64, true, 50);
        //gradientRed(10);
      }
      else {
        batteri -= 1;
        meteorRain(255,0,0,10, 64, true, 50);
        }
    }  
}


void Batteri_fullt(){

  if (batteri == 9) {
      batteri_slange.clear();
      batteri_regnbue_slange.clear();
      //gradientGreen(10);
   }
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
    batteri_slange.setPixelColor(59, batteri_slange.Color(0, 0, 255));
    batteri_slange.show();

    
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
  //Strobe(100, 0, 0, 10, 100, 1000);
  
  for(int i=0; i<leds_i_slange; i++) {
    batteri_slange.setPixelColor(i, batteri_slange.Color(0, 0, 0));
    batteri_slange.show();
    }
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

boolean Registrer_touch2(){
  long total = touch2.capacitiveSensor(30);
  Serial.println(total);

  if (total > sensitivity) {
    return true;
    } else {
      return false;
      }
}

int varme = 500;
void Kjenn_varme(int pin){
  Batteriet_lades_ned();

   // varmeTid parameteret kan justeres. Lavere verdi gir et varmere peltier.
  if (millis() % 2000 > varme) {
    digitalWrite(pin, HIGH);
    } else {
      digitalWrite(pin, LOW);
      }
}

int kulde = 200;
void Kjenn_kulde(int pin) {
  Batteriet_lades_opp();
  
  // varmeTid parameteret kan justeres. Lavere verdi gir et varmere peltier.
  if (millis() % 2000 > kulde) {
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
    return true;
    } else {
      return false;
      }
}

void lad() {
  // koble inn ledlyslenke, og telle ned til kollaps
  
  digitalWrite(lader, HIGH);
  Batteriet_kollapser();
  mobil_Lader(50);
}


void stopp_Lading() {
  digitalWrite(lader, LOW);
    
  for(int i=0; i<leds_i_slange; i++) {
    lade_slange.setPixelColor(i, lade_slange.Color(0, 0, 0));
    lade_slange.show();
    }
}


unsigned long forrige_grryellow = 0;
int yellow_i = 59;

void Solcelle_lader_batteri(int wait) {
  unsigned long now = millis();

  if ((now - forrige_grryellow) > wait) {
    forrige_grryellow = now;
      if (yellow_i < 60) {
        Serial.println(yellow_i);
        solcelle_slange.setPixelColor(yellow_i, batteri_slange.Color(255, 255, 51));
        solcelle_slange.show();
        yellow_i = yellow_i - 1;  
        }
      else {
        yellow_i = 0;
        // batteri ++;
        solcelle_slange.clear();
        solcelle_slange.show();
        }
    }
}

unsigned long forrige = 0;
int i = 0;

void mobil_Lader(int wait) {
  unsigned long now = millis();

  if ((now - forrige) > wait) {
    forrige = now;
      if (i < 60) {
        Serial.println(i);
        lade_slange.setPixelColor(i, lade_slange.Color(255, 0, 0));
        lade_slange.show();
        i = i + 1;  
        }
      else {
        i = 0;
        lade_slange.clear();
        lade_slange.show();
        }
    }
}

void SolcelleLiten(int pin) {
  int photocellReading = analogRead(pin);
  Serial.print(pin);
  Serial.print("  Analog reading = ");
  Serial.println(photocellReading);
  if (photocellReading > 60) {
      Batteriet_lades_opp();
      }
  }


void Solcelle(int pin){
  int photocellReading = analogRead(pin);
  Serial.print(pin);
  Serial.print("  Analog reading = ");
  Serial.println(photocellReading);
  
    //int num_leds_to_show = map(photocellReading, 60, 700, 0, leds_i_solcelleslange);
    if (photocellReading > 60) {
      Batteriet_lades_opp();
      Solcelle_lader_batteri(1000);
      }
    //peak = alt lyser
    /*
    int diff = num_leds_to_show - aktive_leds_i_solcelleslange;

    if (diff > 1) {
      unsigned long now = millis();
      if (now - prev > 100) { //har det gått mer enn et sekund siden forrige økning?:
         diff = 1;
         prev = now;
        } else {
          diff = 0;
          }
    }

    num_leds_to_show = aktive_leds_i_solcelleslange + diff;
    aktive_leds_i_solcelleslange = num_leds_to_show;

    if(num_leds_to_show > 60){
      //Batteriet_lades_opp();
    }

    for (int i=0; i < leds_i_solcelleslange; i++){
      if (i <= num_leds_to_show){
        solcelle_slange.setPixelColor(i, solcelle_slange.Color(0, 150, 0));
      }
       else{
          solcelle_slange.setPixelColor(i, solcelle_slange.Color(0, 0, 0));
        }
  }*/
     solcelle_slange.show();
}



void pulseWhite(uint8_t wait) { // GRBW configuration
  
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    batteri_regnbue_slange.fill(batteri_regnbue_slange.Color(255,255, 255, batteri_regnbue_slange.gamma8(j))); // 100, 100, 100 for mer rolige farger
    batteri_regnbue_slange.setBrightness(255);
    batteri_regnbue_slange.show();
    delay(wait);
  }
  
/*
  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    batteri_regnbue_slange.fill(batteri_regnbue_slange.Color(0, 255, 255, batteri_regnbue_slange.gamma8(j))); // 0, 100, 100 for lolipop farger
    batteri_regnbue_slange.setBrightness(255);
    batteri_regnbue_slange.show();
    delay(wait);
  }
  */
}

void cold(int wait) { // GRB configuration
  

    for(int i=0; i<60; i++) { 
      batteri_slange.setPixelColor(i, batteri_slange.Color(0, 0, 10));
      batteri_slange.show();   // Send the updated pixel colors to the hardware.
}}

unsigned long forrige_grrblue = 0;
int blue_i = 0;

void gradientBlue(int wait) { // GRB configuration

  unsigned long now = millis();
  
  if ((now - forrige_grrblue) > wait) {
    forrige_grrblue = now;
      if (blue_i < 60) {
        Serial.println(blue_i);
        batteri_slange.setPixelColor(blue_i, batteri_slange.Color(0, 0, 255));
        batteri_slange.show();
        blue_i = blue_i +1;  
        }
      else {
        blue_i = 0;
        // batteri ++;
        batteri_slange.clear();
        batteri_slange.show();
        }
    }
}




unsigned long forrige_grred = 0;
int red_i = 60;

void gradientRed(int wait) { // GRB configuration

  unsigned long now = millis();
  
  if ((now - forrige_grred) > wait) {
    forrige_grrblue = now;
      if (red_i < 60) {
        batteri_slange.setPixelColor(red_i, batteri_slange.Color(255, 0, 0));
        batteri_slange.show();
        red_i = red_i -1;  
        }
      else {
        red_i = 0;
        // batteri ++;
        batteri_slange.clear();
        batteri_slange.show();
        }
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


unsigned long forrige_meteorRain = 0;
int meteor_i = 60;

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {
    
   int NUM_LEDS = 60;
   unsigned long now = millis();
   
    if ((forrige_meteorRain - now) > SpeedDelay) {
      // slange needs to be clear
      if (meteor_i < NUM_LEDS) {
         
          // fade brightness all LEDs one step
        for(int j=0; j<NUM_LEDS; j++) {
          if( (!meteorRandomDecay) || (random(10)>5)) {
            fadeToBlack(j, meteorTrailDecay );        
          }
        }
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( meteor_i-j <NUM_LEDS) && (meteor_i-j>=0) ) {
        batteri_slange.setPixelColor(meteor_i-j, red, green, blue);
      }
    }
   
    batteri_slange.show();
    meteor_i ++;
    
    } else {
      meteor_i = 0;
      }
    }
}


unsigned long forrige_negative_meteorRain = 0;
int neg_meteor_i = 59;

void negativeMeteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {
    
   int NUM_LEDS = 60;
   unsigned long now = millis();
   
    if ((forrige_negative_meteorRain - now) > SpeedDelay) {
      // slange needs to be clear
      if (neg_meteor_i < NUM_LEDS) {
         
          // fade brightness all LEDs one step
        for(int j=0; j<NUM_LEDS; j++) {
          if( (!meteorRandomDecay) || (random(10)>5)) {
            fadeToBlack(j, meteorTrailDecay );        
          }
        }
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( neg_meteor_i-j <NUM_LEDS) && (neg_meteor_i-j>=0) ) {
        batteri_slange.setPixelColor(neg_meteor_i-j, red, green, blue);
      }
    }
   
    batteri_slange.show();
    neg_meteor_i --;
    
    } else {
      neg_meteor_i = 0;
      }
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
