#include <ShiftPWM.h>

bool buttonPushed = false;   // variabele die cheacked of er een knop word ingdrukt
bool plaatStatus[8];         // of een individule plaat aan of uit is waarin false uit is
bool buttonsUp[8];           //een array met alle knoppen die een plaat zijn tempratuur omhoog zetten
bool buttonsDown[8];         //een array met alle knoppen die een plaat zijn tempratuur omhoog zetten
int  plaatHitte[8];          // duh
int  ledFelheid[8];          // same
const byte LED = 9;          // ledje dat de warmtestand weergeeft
const byte Button = 2;       // knopje dat de warmstestand verhoogd


byte ButtonState;
byte lastState = LOW;
byte count = 0;


const int ShiftPWM_latchPin = 8;
const bool ShiftPWM_invertOutputs = false;
const bool ShiftPWM_balanceLoad = false;

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

byte data;
byte dataArray[10];

void setup() {
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  ShiftPWM.SetAmountOfRegisters(1);
  ShiftPWM.Start(60,255); // freq, maxBrightness
  VulPlaatStatus();
}

void SendFelheid(){
for(int i = 0; i<8; i++){
  ShiftPWM.SetOne(i, ledFelheid[i]);
  }
}

bool ButtonChecker(){
ButtonState = digitalRead(Button);

  if (ButtonState && ButtonState != lastState) {
  lastState = ButtonState;
    return true;
  }else{
    return false;
  }
}


void VulPlaatStatus(){
  for(int i = 0; i  < 8 ; i++){
    plaatStatus[i] = false;
  }
}

void CheckHeat(){
  // dit cheacked voor knopjes en de tempratuur
  for(int i = 0; i < 8; i++){
      if(plaatStatus[i] == false && buttonPushed == true){ // als de plaat uit is en er word een knop ingrukt
        if(buttonsUp[i] == true){
          plaatHitte[i] = 0;
          plaatStatus[i] = true;
        }
      if(plaatStatus == true && buttonPushed == true){  // als de plaat aanstaat en er word een knop ingedrukt
        if(buttonsUp[i] == true){                             // om de tempratuur omhoog te doen
          plaatHitte[i]++;
        }

        if(buttonsDown[i] == true && plaatHitte[i]>0){  // om de tempratuur omlaag te doen
          plaatHitte[i]--;
        }else if(buttonsDown[i] == true && plaatHitte[i]<=0){ // om de plaat uit te zetten wanneer de hitte 0 is
          plaatStatus[i] = false;
          plaatHitte[i] = 0;
        }
      }
    }
  }
}

void CheckFelheid(){
  for(int i = 0; i < 8; i++){
    ledFelheid[i] = plaatHitte[i] * 25;
  }
}

void loop() {
  CheckHeat();
  CheckFelheid();
  SendFelheid();
}
