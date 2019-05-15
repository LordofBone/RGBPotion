//Thanks to Dejan Nedelkovski, www.HowToMechatronics.com

//define outputs
#define outputA 2
#define outputB 3
 
#include <Adafruit_NeoPixel.h>

//define pin and leds for the neopixel stick
#define PIN 4
#define N_LEDS 8
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

//set button pin
int buttonPin = 7;

//set led pins
int redLED = 13;
int greenLED = 12;
int blueLED = 11;

//set states for use with encoder
int aState;
int aLastState;

//int for mode 
int mode = 0;

//colour values
int rColour = 0;
int gColour = 0;
int bColour = 0;
 
void setup() { 
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  
  pinMode (redLED,OUTPUT);
  pinMode (greenLED,OUTPUT);
  pinMode (blueLED,OUTPUT);
   
  strip.begin();
  
  Serial.begin (9600);
  // Reads the initial state of the outputA
  aLastState = digitalRead(outputA);   
}

//function for button push detection
boolean isButtonPushDown(void){
  if(!digitalRead(buttonPin)){
    delay(5);
    if(!digitalRead(buttonPin))
      return true;
  }
  return false;
}

void loop() {

  //setup leds to show what mode is being adjusted
  if (mode == 0) {
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, LOW);
    digitalWrite(blueLED, LOW);
  }
  if (mode == 1) {
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    digitalWrite(blueLED, LOW);
    
  }
  if (mode == 2) {
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
    digitalWrite(blueLED, LOW);
  }
  if (mode == 3) {
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, LOW);
    digitalWrite(blueLED, HIGH);
  }

  //when button is pushed change mode  
  if(isButtonPushDown()) {
    if (mode < 3) {
      mode++;
  }
    else {
      mode = 0;
    }
    delay(500);
  }
  
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (aState != aLastState){     
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(outputB) != aState) {

      //depending on mode change r/g/b colour - if the number goes over 255 reset to 0 and vice-versa
      if (mode == 1) {
        if (rColour < 255) {
          ++rColour;
        }
        else {
          rColour = 0;
        }
      }
      if (mode == 2) {
        if (gColour < 255) {
          ++gColour;
        }
        else {
          gColour = 0;
        }
      }
      if (mode == 3) {
        if (bColour < 255) {
          ++bColour;
        }
        else {
          bColour = 0;
        }
      }
    
    } else {
      
      if (mode == 1) {
        if (rColour > 0) {
          --rColour;
        }
        else {
          rColour = 255;
        }
      }
      if (mode == 2) {
        if (gColour > 0) {
          --gColour;
        }
        else {
          gColour = 255;
        }
      }
      if (mode == 3) {
        if (bColour > 0) {
          --bColour;
        }
        else {
          bColour = 255;
        }
      }
    
    }

    //print out led values
    Serial.print("Red: ");
    Serial.println(rColour);
    Serial.print("Green: ");
    Serial.println(gColour);
    Serial.print("Blue: ");
    Serial.println(bColour);

    //this loop applies the led values to the neopixels
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, rColour, gColour, bColour);
    }

    //display the neopixels
    strip.show();
    delay(10);
  
  } 
  aLastState = aState; // Updates the previous state of the outputA with the current state
}
