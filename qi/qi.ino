#include <Adafruit_NeoPixel.h>

#include <Adafruit_NeoPixel.h>

#include <CapacitiveSensor.h>

int batteri = 0; // dette er en skala hvor 0 er shutdown, og 9 er fullt.

//int slange_pin = A0;

int leds_i_solcelleslange = 60;
int aktive_leds_i_solcelleslange = 0;

int pressure_pin = A1;
int pressure_reading; //variable for storing our reading

int photocellPin = A0;
int photocellReading;
int LEDpin = 11;
int LEDbrightness;
long total = 0;

int varmePin = 10;
//Adafruit_NeoPixel slange = Adafruit_NeoPixel(leds_i_slange, slange_pin,NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel solcelle_slange = Adafruit_NeoPixel(leds_i_solcelleslange, LEDpin,NEO_GRB + NEO_KHZ800);
CapacitiveSensor touch = CapacitiveSensor(4,2); // 1M resistor between pins 4 & 2, pin 2 is sensor pin


void setup() {
  Serial.begin(9600);
  //slange.begin();
  solcelle_slange.begin();
  touch.set_CS_AutocaL_Millis(0xFFFFFFFF);
  pinMode(varmePin, OUTPUT);
}


void loop(void) {
  total = 0;
  if (Registrer_touch()) {
    Gi_varme();
    Serial.println("gir varme");
    } else {
      Stopp_varme();
      Serial.println("stopper varme");
      }
   delay(1000);
  //Solcelle();
  // Styr_batteriet();
  
  //sjekke om noen tar på metal nylon fabric sheet
  //sjekke om noen lyser på designerte plasser
  //sjekke om noen lader
  //sjekke om noen skrur på en lyspære
  //justere batteriet ut fra input
  //vise batterinivå

  //if (lader){
  //batterilevel -1
  //}
  //delay(100);
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
    delay(2000); // 2 sek for å lade opp
  }
  
void Batteriet_lades_ned() {
  if (batteri > 0) {
    batteri -= 1;
    }
    else {
      Batteri_shutdown();
      }
    delay(1000); // 1 sek for å lade ned
  }
  
void Batteriet_kollapser() {
    //skjer kun ved mobillading
    //if (batteri == 0) {}
    //else {Batteriet_lades_ned();}
  }

void Batteri_fullt(){
  // batteri = 9
  }
  
void Batteri_halvfullt(){
  // batteri = mellom 4 og 8
  }
  
void Batteri_lavt(){
  // batteri = mellom 3 og 1
  }
  
void Batteri_shutdown(){
  // batteri = 0
  }

unsigned long varmeTid = 0;

void Gi_varme(){
  //Batteriet_lades_opp();
  if (millis() % 2000 > 1000) {
    Serial.println("aktiv");
    digitalWrite(varmePin, HIGH);
    } else {
      Serial.println("inactive");
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

 

boolean Registrer_touch(){
  total = touch.capacitiveSensor(30);
  Serial.println(total);
  
  if (total > 200) {
    return true;
    } else {
      return false;
      }
}

 
unsigned long prev = 0;
 
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
        Serial.print("!");
        solcelle_slange.setPixelColor(i, solcelle_slange.Color(0, 150, 0));
        /*if (i <= 40){
          pixels[i] = (0, 100, 50);
        }
        else{
          pixels[i] = (255, 255, 0);
        }*/
      }
       else{
        Serial.print(".");
          solcelle_slange.setPixelColor(i, solcelle_slange.Color(0, 0, 0));
        }
    
     solcelle_slange.show();
    
  }
  
}

 

boolean Lad_mobil(){
  if (pressure_reading > 0) {
    Batteriet_kollapser();
    return true;
    }
  //lader en mobil i et gitt tidsrom, hvis lenger kutt strømmen
}

 

void Tenn_Lysbulb(){
 /* if buttonPressed:
    send strøm */
}
