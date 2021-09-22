#include <Adafruit_NeoPixel.h>

#include <Adafruit_NeoPixel.h>

#include <CapacitiveSensor.h>

int batteri = 0; // dette er en skala hvor 0 er shutdown, og 9 er fullt.

int slange_pin = A0;
int leds_i_slange = 5;
Adafruit_NeoPixel slange = Adafruit_NeoPixel(leds_i_slange, slange_pin,NEO_GRB + NEO_KHZ800);
CapacitiveSensor touch = CapacitiveSensor(4,2); // 1M resistor between pins 4 & 2, pin 2 is sensor pin


void setup() {
  slange.begin();
  slange.show();
  touch.set_CS_AutocaL_Millis(0xFFFFFFFF);
  Serial.begin(9600);
}


void loop() {
 
  Styr_batteriet();
  
  //sjekke om noen tar på metal nylon fabric sheet
  //sjekke om noen lyser på designerte plasser
  //sjekke om noen lader
  //sjekke om noen skrur på en lyspære
  //justere batteriet ut fra input
  //vise batterinivå

  //if (lader){
  //batterilevel -1
  //}
  delay(100);
}

 

void Styr_batteriet(){
  for (int i= 0; i<leds_i_slange; i++) {
    slange.setPixelColor(i, 0, 0, 255);
    slange.show();
    }
  
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


void Batteriet_lades_opp() {}
void Batteriet_lades_ned(){}
void Batteriet_kollapser(){}

void Batteri_fullt(){
  // batteri = 9
  }
  
void Batteri_halvfullt(){
  // batteri = mellom 4 og 8
  }
  
void Batteri_lavt(){
  // batteri = mellom 3 og 1
  }
  
void Batteri_shutdowm(){
  // batteri = 0
  }

 

void Gi_varme(){
  //funksjon for å gi varme ved registrering, hvordan peltier skal oppføre seg ved interaksjon
  //< 5 sek: level 1 (kaldest, romtemp)
  //< 10 && > 5 sek: level 2 (litt varmt)
  //< 15 && > 10 sek: level 3 (varmest)
}

 

boolean Registrer_touch(){
  long total = touch.capacitiveSensor(30);
  
  if (total > 50) {
    Gi_varme();
    return true;
    }
}

 

void Photoresistor(){
  
  photocellReading = analogRead(photocellPin);
  Serial.print("Analog reading = ");
  Serial.println(photocellReading);

  //photocellReading = 1023 - photocellReading;
  LEDbrightness = map(photocellReading, 0, 210, 0, 255);
  Serial.println(LEDbrightness);
  analogWrite(LEDpin, LEDbrightness);
  delay(100);
  
  //registrer mengde lys som blir sendt inn
 //lengde man gir lys gir batteriet mer/mindre energi
 // < 5 sek: level 1 (kaldest, romtemp)
 // < 10 && > 5 sek: level 2 (litt varmt)
 // < 15 && > 10 sek: level 3 (varmest)

 

 /*
 
  while(True){
    //peak = alt lyser
    diff = peak - num_leds_to_show

 

    if (diff > 1){
      diff = 1; // Hvorfor?
    }

 

    num_leds_to_show = num_leds_to_show + diff;

 

    if(num_leds_to_show > 40){
      num_leds_to_show = 59;
    }

 

    //light value remapped to pixel position
    if (light.value < 0){
      //fyll lyslenken
    }

 

    for (int i=0; i < maxPixels; i++){
      if (i <= num_leds_to_show){
        if (i <= 40){
          pixels[i] = (0, 100, 50);
        }
        else{
          pixels[i] = (255, 255, 0);
        }
      }
       else{
          pixels[i] = (0, 0, 0);
        }
    }
     pixels.show()
    time.sleep(0.5)
    
  }*/
  
}

 

boolean Lad_mobil(){
  //lader en mobil i et gitt tidsrom, hvis lenger kutt strømmen
}

 

void Tenn_Lysbulb(){
 /* if buttonPressed:
    send strøm */
}
