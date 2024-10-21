#include <Arduino.h>
// C++ code
//declararea pinilor pentru butoane si led-uri
const int start_btn = 3;
const int end_btn = 2;

const int rgb_led_green = 4;
const int rgb_led_red = 6;

const int led1 = 10;
const int led2 = 9;
const int led3 = 8;
const int led4 = 7;

//starile initiale ale led-urilor
int green_led_state = HIGH;
int red_led_state = LOW;
int led1_state = LOW;
int led2_state = LOW;
int led3_state = LOW;
int led4_state = LOW;

//starile initiale ale butoanelor
int start_btn_state = HIGH;
int last_start_btn_state = HIGH;

int end_btn_state = HIGH;
int last_end_btn_state = HIGH;

// the last time the output pin was toggled
//variabile folosite la debouncing
unsigned long last_debounce_time = 0;  
const unsigned long debounce_delay = 50;

// Variabile pentru detectarea apasarii lungi a butonului END
unsigned long button_press_time = 0;
const unsigned long long_press_duration = 1000; // Apasare lunga de 1 secunda

//variabile on verifica daca incarcarea merge
//daca on == 0 atunci inseamna ca statie de incarcare e goala
//daca on == 1 inseamna ca statia incarca
int on = 0;

//variabile ce mentin cronometrul incarcarii
unsigned long load = 0;
const unsigned long led1_timer = 3000;//cat timp trebuie sa clipeasca led1
const unsigned long led2_timer = 6000;//pana cand trebuie sa clipeasca led2
const unsigned long led3_timer = 9000;//pana cand trebuie sa clipeasca led3
const unsigned long led4_timer = 12000;//pana cand trebuie sa clipeasca led4

const unsigned long total_charge_duration = 15000; //durata incarcarii in ms
const unsigned long animation_duration = 3000; //durata animatiei de incheiere
const unsigned long toggle_slow_interval = 500; //interval de toggle animatie de sfarsit

void setup()
{
  Serial.begin(115200);
  pinMode(start_btn, INPUT_PULLUP);
  pinMode(end_btn, INPUT_PULLUP);
  
  pinMode(rgb_led_green, OUTPUT);
  pinMode(rgb_led_red, OUTPUT);
  digitalWrite(rgb_led_green, green_led_state); //la inceput led-ul verde e aprins
  digitalWrite(rgb_led_red, red_led_state);  //iar cel rosu este stins
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  
}

bool charge_end(){
 	//DEBOUNCING BUTONUL DE END
    int reading2 =  digitalRead(end_btn);
    if(reading2 != last_end_btn_state){//daca starea butonului de end s-a schimbat
      last_debounce_time = millis(); // Reseteaza debounce timer-ul
    }
    
    //trec 50ms
    if ((millis() - last_debounce_time) > debounce_delay){
      if(reading2 != end_btn_state){
        end_btn_state = reading2;
        
        // Daca butonul de end a fost apasat
        if(end_btn_state == LOW){
          button_press_time = millis(); // Salveaza momentul apasarii
        }

        // Daca butonul de end a fost eliberat
        if(end_btn_state == HIGH && (millis() - button_press_time >= long_press_duration)){
          // Daca a fost o apasare lunga, opreste incarcarea
          on = 0; // Statia opreste incarcarea
        }
      }
    }
    last_end_btn_state = reading2; 
  	if(on == 0)
      return true; //functie returneaza true daca butonul end a fost apasat, pt a putea folosi rezultatul in 
  	return false; //functia charge_func. Daca butonul nu a fost apasat, se returneaza false
}


void charge_func(){
  load = millis(); //retine momentul de inceput al incarcarii
  unsigned long last_toggle = 0;//momentul in care s-a facut ultima data toggle
  const unsigned long toggle_interval = 400;  //frecventa cu care clipesc led-urile
  
  while(millis() - load <= total_charge_duration){
    
    unsigned long current_time = millis(); //masoara timpul curent pt a verifica cand sa faca toggle
    
    //in timpul incarcarii verific daca s-a apasat end
    //daca s-a apasat lung pe btn end, atunci ies din bucla de incarcare
    bool ok = charge_end();
    if(ok){
      //inainte de a iesi din bucla se realizeaza animatia
      unsigned long animation_timer = millis();
      
      //trebuie sa aprind intai toate led-urile
      led1_state = HIGH;
      led2_state = HIGH;
      led3_state = HIGH;
      led4_state = HIGH;
      
      digitalWrite(led1, led1_state);
      digitalWrite(led2, led2_state);
      digitalWrite(led3, led3_state);
      digitalWrite(led4, led4_state);
      
      while((millis() - animation_timer) <= animation_duration){
        current_time = millis();//iau timpul curent pt a verifica cand sa fac toggle
        if(current_time - last_toggle > toggle_slow_interval){  //led-urile trebuie sa clipeasca de 3 ori
         	led1_state = !led1_state;
            digitalWrite(led1, led1_state);
            led2_state = !led2_state;
            digitalWrite(led2, led2_state);
            led3_state = !led3_state;
            digitalWrite(led3, led3_state);
            led4_state = !led4_state;
            digitalWrite(led4, led4_state);
            last_toggle = current_time; 
        }
      }
      break; //se iese din while, adica dupa cele 3 clipiri, incarcarea se incheie
    }
    
    //verifica daca a trecut suficient timp de la ultimul toggle
    //verific cat timp a trecut de la apelarea functiei, i.e. inceperea incarcarii, prin millis() - load

    if((millis() - load) < led1_timer && (current_time - last_toggle) > toggle_interval){
      led1_state = !led1_state; //daca a trecut 25% din timp led1 face toggle, restul sunt stinse
      digitalWrite(led1, led1_state);
      last_toggle = current_time;
    }
    else if((millis() - load) < led2_timer && (current_time - last_toggle) > toggle_interval){//daca a trecut intre 25% si 50%
      led1_state = HIGH;                                                                    //led1 e aprins, led2 face toggle
      digitalWrite(led1, led1_state);                                                       //celelalte sunt stinse
      led2_state = !led2_state;
      digitalWrite(led2, led2_state);
      last_toggle = current_time;
    }
    else if((millis() - load) < led3_timer && (current_time - last_toggle) > toggle_interval){  //daca a trecut intre 50% si 75%
      led1_state = HIGH;                                                                    //led1 si led2 sunt aprinse
      digitalWrite(led1, led1_state);                                                       //led3 face toggle si led4 e stins
      led2_state = HIGH;
      digitalWrite(led2, led2_state);
      led3_state = !led3_state;
      digitalWrite(led3, led3_state);
      last_toggle = current_time;
    }
    else if((millis() - load) < led4_timer && (current_time - last_toggle) > toggle_interval){  //daca a trecut intre 75% si 100%
      led1_state = HIGH;                                                                        //led4 face toggle
      digitalWrite(led1, led1_state);                                                           //restul sunt aprinse
      led2_state = HIGH;
      digitalWrite(led2, led2_state);
      led3_state = HIGH;
      digitalWrite(led3, led3_state);
      led4_state = !led4_state;
      digitalWrite(led4, led4_state);
      last_toggle = current_time;
    }
    else if((millis() - load) >= led4_timer && (current_time - last_toggle) > toggle_slow_interval){//cand se termina incarcarea toate led-urile
      led1_state = !led1_state;                                                    //clipesc de 3 ori
      digitalWrite(led1, led1_state);
      led2_state = !led2_state;
      digitalWrite(led2, led2_state);
      led3_state = !led3_state;
      digitalWrite(led3, led3_state);
      led4_state = !led4_state;
      digitalWrite(led4, led4_state);
      last_toggle = current_time;
    }
  }
  on = 0;
  //delay(50);
  //la sfarsitul incarcarii
  //sting led-urile din charger
  led1_state = LOW;
  led2_state = LOW;
  led3_state = LOW;
  led4_state = LOW;
  
  digitalWrite(led1, led1_state);
  digitalWrite(led2, led2_state);
  digitalWrite(led3, led3_state);
  digitalWrite(led4, led4_state);
  
  green_led_state = !green_led_state; //led-ul rgb devine verde pt a indica ca este disponibil pt incarcare din nou
  red_led_state = !red_led_state; //culoare rosie se stinge
  
  digitalWrite(rgb_led_red, red_led_state);
  digitalWrite(rgb_led_green, green_led_state);
}

void loop()
{
  //daca statia nu incarca inca
  if(on == 0){
    //DEBOUNCING BUTONUL DE START
    int reading1 = digitalRead(start_btn);

    //daca butonul de start a fost apasat
    if(reading1 != last_start_btn_state){
      //reseteaza debounce timer-ul
      last_debounce_time = millis(); 
    }
    //trec 50ms
    if ((millis() - last_debounce_time) > debounce_delay){

      //daca starea butonului a fost schimbata cu adevarat
      if(reading1 != start_btn_state ){
          start_btn_state = reading1;
        

        //daca starea butonului e 1 logic
        if(start_btn_state == LOW){
          green_led_state = !green_led_state; //toggle led verde
          red_led_state = !red_led_state; //toggle led rosu
          on = 1;//statia a inceput incarcarea
        }
      }
    }
    digitalWrite(rgb_led_green, green_led_state);//led-ul rgb devine rosu pt a indica ca incarcarea a inceput
    digitalWrite(rgb_led_red, red_led_state);
    last_start_btn_state = reading1;
    
    //daca statia a inceput incarcarea rulez loop-ul de charge
    if(on == 1){
    	charge_func(); //apeleaza functia de incarcare
    }
    
  }
  
}