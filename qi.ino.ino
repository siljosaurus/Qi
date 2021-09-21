void setup() {
  // put your setup code here, to run once:
  Serial.begin();

}

 

int neoPin = 2; //input på digital
int maxPixels = 6;
int num_leds_to_show = 0;
int battery = antPixels;

 

void loop() {
  //sjekke om noen tar på metal nylon fabric sheet
  //sjekke om noen lyser på designerte plasser
  //sjekke om noen lader
  //sjekke om noen skrur på en lyspære
  //justere batteriet ut fra input
  //vise batterinivå

 

  if (lader){
  batterilevel -1
  }

 

}

 

void Styr_Batteriet(){
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

 

void Gi_varme(){
  //funksjon for å gi varme ved registrering, hvordan peltier skal oppføre seg ved interaksjon
  < 5 sek: level 1 (kaldest, romtemp)
  < 10 && > 5 sek: level 2 (litt varmt)
  < 15 && > 10 sek: level 3 (varmest)
}

 

boolean Registrer_touch(){
  //if true kall på Gi_varme()
  return true  
}

 

void Lyssensor(){
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
