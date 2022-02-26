//code wriiten (or at least patched together), by HulloIsMe
//special thanks to omriharel on GitHub, IcyRespawn on Discord, and Orotavia on both discord and GitHub!
//I really appreciate the help, and the code that you lent me!
#include <FastLED.h>
#include "PinChangeInterrupt.h"   //NicoHood's library

#define NUM_LEDS 5   //number of led present in your strip
#define DATA_PIN 6   //digital pin for leds
CRGB leds[NUM_LEDS];

#define outputA 3   //CLK
#define outputB 4   //DT

int counter = 0;   //counter variable for the rotary encoder

//all you have to change too ad more virtual sliders is this value
//although you may have to add the led indicators for new sliders, as currently it is only set up for 6
const int NUM_SLIDERS = 6; 
int virtualSliderValues[NUM_SLIDERS];

void setup() { 
 FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
 pinMode(outputA, INPUT_PULLUP);
 pinMode(outputB, INPUT_PULLUP);
 attachPCINT(digitalPinToPCINT(outputA), EN0_A_ISR, RISING);   //encoder is set to rising to remove the double increment issue
 attachPCINT(digitalPinToPCINT(outputB), EN0_B_ISR, RISING);
 Serial.begin (9600);
} 

 void loop() { 
  updateSliderValues();
  sendSliderValues();
  indicatorColor();
  FastLED.show();
 }

  //reads rotary encoder inputs - courtesy of @Orotavia
void EN0_A_ISR(){
  if(digitalRead(outputA) != digitalRead(outputB)){
    counter += 1;
    Serial.println(counter);
  }
  counter = constrain(counter, 0, 5);
}

void EN0_B_ISR(){
  if(digitalRead(outputA) != digitalRead(outputB)){
    counter -= 1;
    Serial.println(counter);
  }
  counter = constrain(counter, 0, 5);
}

//updates the slider value when the rotary encoder is equal to the channel number
void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++)
    if (i == counter) {
       virtualSliderValues[i] = analogRead(A0);
    }
}

//compile string and send to deej.exe
void sendSliderValues() {
  String builtString = String(""); // start from an empty string

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)virtualSliderValues[i]); // for each pot, add its last read value

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|"); // if it's not the last pot, also add a pipe after ("|")
    }
  }
  Serial.println(builtString); // send the complete build string over serial (println adds a newline sequence for us)
}

//function for LED indicator
void indicatorColor () {
  if (counter == 0) {
    leds[0] = CRGB::White;
  } else if (counter == 1) {
    leds[0] = CRGB::Red;
  } else if (counter == 2) {
    leds[0] = CRGB::Green;
  } else if (counter == 3) {
    leds[0] = CRGB::Cyan;
  } else if (counter == 4) {
    leds[0] = CRGB::Purple;
  } else if (counter == 5) {
    leds[0] = CRGB(255, 0, 116);
  }
}
