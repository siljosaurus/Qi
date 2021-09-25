#include <Adafruit_NeoPixel.h>

#include <Adafruit_NeoPixel.h>

#include <CapacitiveSensor.h>

int batteri = 0; // dette er en skala hvor 0 er shutdown, og 9 er fullt.

// pin 2 og 4 er for touch sensor
int slange_pin = 5;
int varmePin = 6;
// pin 8 og 7 er for touch sensor 2
int ladeLED = 9;
int varmePin2 = 10;
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

unsigned long varmeTid = 200; // lavere verdi blir til varmere peltier
int sensitivity = 200;

Adafruit_NeoPixel slange = Adafruit_NeoPixel(leds_i_slange, slange_pin,NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel slange2 = Adafruit_NeoPixel(leds_i_slange, slange_pin,NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel solcelle_slange = Adafruit_NeoPixel(leds_i_solcelleslange, solcelleLED,NEO_GRB + NEO_KHZ800);
CapacitiveSensor touch = CapacitiveSensor(4,2); // 1M resistor between pins 4 & 2, pin 2 is sensor pin
CapacitiveSensor touch2 = CapacitiveSensor(8,7); // 1M resistor between pins 8 & 7, pin 7 is sensor pin

void setup() {
  Serial.begin(9600);
  slange.begin();
  slange2.begin();
  solcelle_slange.begin();
  touch.set_CS_AutocaL_Millis(0xFFFFFFFF);
  pinMode(varmePin, OUTPUT);
  pinMode(lader, OUTPUT);
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

  slange.clear();
  slange2.clear();
   pulseWhite(10);
   gradientRed(10);
   lad();
   delay(5000);
   stopp_Lading();

   delay(1000);
  //Solcelle();
  // Styr_batteriet();

  //sjekke om noen tar på metal nylon fabric sheet
  //sjekke om noen lyser på designerte plasser
  //sjekke om noen lader
  //sjekke om noen skrur på en lyspære
  //justere batteriet ut fra input
  //vise batterinivå
}



void Styr_batteriet(){
  //for (int i= 0; i<leds_i_slange; i++) {
    //slange.setPixelColor(i, 0, 0, 255);
    //slange.show();}

  /*if (batterilevel == 0){
    //ingen lys
  }
  else if (batterilevel > 0 && < 5){
    //medium lys / halvparten
  }
  else if (batterilevel > 5 && < 10){
    //sterkt lys / alle lys
  }*/
}


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
    if (batteri == 0) { //kollaps
      batteri -= 1;
      }
      else {
        Batteri_shutdown();
        }
     delay(1000); //1 sek, halvparten av vanlig shutdown
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
  // rød og blinker, lite brightness
  }

void Batteri_shutdown(){
  // batteri = 0
  for(int i=0; i<leds_i_slange; i++) {
    slange.setPixelColor(i, slange.Color(0, 0, 0));
    slange.show();
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
  pressure_reading = analogRead(pressurePin);
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
    slange2.setBrightness(255);
    slange2.show();
    delay(wait);
  }
}

void gradientRed(int wait) { // GRB configuration

    for(int i=0; i<60; i++) {
      slange.setPixelColor(i, slange.Color(255, 0, 0));
      slange.setBrightness(255);
      delay(70);
      slange.show();
      delay(wait);
    }

    for(int i=0; i<60; i++) {
      slange.setPixelColor(i, slange.Color(0, 255, 0));
      slange.setBrightness(255);
      delay(70);
      slange.show();
      delay(wait);
    }

    for(int i=0; i<60; i++) {
      slange.setPixelColor(i, slange.Color(0, 0, 255));
      slange.setBrightness(255);
      delay(70);
      slange.show();
      delay(wait);
    }
}
