#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//Premade deej code
const int NUM_SLIDERS = 6;
const int analogInputs[NUM_SLIDERS] = {9, 8, 7, 6, 5, 4};

int analogSliderValues[NUM_SLIDERS];

//my led strip is on pin 10 and there are 36 leds in total
#define LED_PIN   10

#define LED_COUNT 36

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//This array is used to light up the leds in the proper order as they are daisy chained in a back and forth pattern
int ledOrder[36] = {5,4,3,2,1,0,6,7,8,9,10,11,17,16,15,14,13,12,18,19,20,21,22,23,29,28,27,26,25,24,30,31,32,33,34,35};

void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
  pixels.begin();
  Serial.begin(9600);
}

void loop() {
  //premade deej code
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  // printSliderValues(); // For debug
  delay(10);
  //clear the previous pixels
  pixels.clear();
  //This for loop goes through ever slider
  for(int i = 0; i < NUM_SLIDERS; i++){
    //uses mappedVal int to check the number of leds for the slider that are meant to be turned on
    for(int n = 0; n < mappedVal(i)+1; n ++){
      //n+(6*1) adds 6 for every slider and then n is for every led in that col
      //then i have my own colors. Not sure why the colors are over 4 but it works
      ledColor(n+(6*i),32/4, 149/4, 227/4);
    }
  }
  pixels.show();
}

//Premade deej function
void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    //I used a map function as I wanted to reverse the direction of the slider
     analogSliderValues[i] = map(analogRead(analogInputs[i]),0,1023,1023,0);
  }
}

//Premade deej function
void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  Serial.println(builtString);
}

//Premade deej function
void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}

int ledColor(int led,int r, int g, int b){
  //Int to change the color of an led
  //Could be a bit extra but I like it
  pixels.setPixelColor(ledOrder[led],pixels.Color(r,g,b));
}

int mappedVal(int numSlide){
  //This int returns the number of leds a silder is meant to have on
  int x; 
  x = analogSliderValues[numSlide]/170;
  //This if and else if is meant to stop any odd values from being returned
  //there cant be -1 leds or more then 5
  if(x>5){
    x=5;
  }else if(x<0){
    x=0;
  }
  return x;
}