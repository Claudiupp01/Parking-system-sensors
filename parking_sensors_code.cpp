//#include <NewPing.h>

#include <Adafruit_NeoPixel.h>

class NewPing {
  public:
    NewPing(int TRIGGER_PIN, int ECHO_PIN, int MAX_DISTANCE ){
      trigPin=TRIGGER_PIN;
      echoPin=ECHO_PIN;
      maxDistance=MAX_DISTANCE;

      pinMode(trigPin,OUTPUT);
      pinMode(echoPin,INPUT);
    }

    int ping_cm(){
      // Clears the trigPin
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      // Reads the echoPin, returns the sound wave travel time in microsecond
      long duration = pulseIn(echoPin, HIGH);
      // Calculating the distance
      int distance =  duration*0.034/2;
      // Checks out of range
      if (distance>maxDistance){
        distance=0;
      }
      return(distance);
    }

  private:
    int trigPin;
    int echoPin;
    int maxDistance;
};

// this is the bit you can modify

#define TRIGGER_PIN  10  
#define ECHO_PIN     9  
#define MAX_DISTANCE 200 

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

#define PARKING_STATE_LED 8 // in masalier sau nu
#define BUZZER_PIN 12
#define BUTTON_PIN 11
#define POTENTIOMETER_PIN A0

#define RGB_RED 4
#define RGB_BLUE 3
#define RGB_GREEN 2

#define NUM_PIXELS 10   // Number of NeoPixels in the strip
#define LED_PIN     6   // Pin where NeoPixel data line is connected

Adafruit_NeoPixel strip(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

bool buttonState = false;

float distance;
float duration;

void toggleButtonState()
{
  buttonState = !(buttonState);
}

void setup() {
  pinMode(PARKING_STATE_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  Serial.begin(9600);
}

bool on = true;

bool flag = true;

void loop() {
  
  /*
  if(buttonState == true)
  {
 	digitalWrite(PARKING_STATE_LED, HIGH);
  }
  else
  {
    //if(on == false)
    //{
    digitalWrite(PARKING_STATE_LED, LOW);
    //}
  }
  */
  
  int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  
  Serial.print(potentiometerValue);
  
  if(potentiometerValue <= 341)
  {
    digitalWrite(RGB_RED, HIGH);
     digitalWrite(RGB_BLUE, LOW);
     digitalWrite(RGB_GREEN, LOW);
    
    buttonState = false;
    flag = true;
    
    lightLedsOffUntillLedN(-1);
    strip.show();
    
    digitalWrite(PARKING_STATE_LED, LOW);
    
  }else if(potentiometerValue <= 642)
  {
    digitalWrite(RGB_RED, HIGH);
     digitalWrite(RGB_BLUE, LOW);
     digitalWrite(RGB_GREEN, HIGH);
    
    //buttonState = true;
    
    if(digitalRead(BUTTON_PIN) == HIGH)
    {
      toggleButtonState();
      
      if(buttonState == true)
      {
        on = false;
        flag = true;
        //flag = false;
      }
      else
      {
        on = true;
        flag = false;
      }
      
    }
    
    if((buttonState == true || on == true) && flag == true)
    {
      distance = distanceRead();
      updateBuzzer();
      updateNeoPixel();
      digitalWrite(PARKING_STATE_LED, HIGH);
    }
    else
    //if(flag == false)
    {
      delay(4000);
      lightLedsOffUntillLedN(-1); // inchdiem toate ledurile
      strip.show();
      digitalWrite(PARKING_STATE_LED, LOW);
    }
    /*
    if(buttonState == true)
      {
        on = true;
      }
      else
      {
        on = false;
      }
      */
  } else if(potentiometerValue <= 1023)
  {
    if(digitalRead(BUTTON_PIN) == HIGH)
    {
      toggleButtonState();
      
      if(buttonState == true)
      {
        on = false;
        flag = true;	
      }
      else
      {
        on = true;
        flag = false;
      }
      
    }
    
    
    digitalWrite(RGB_RED, LOW);
     digitalWrite(RGB_BLUE, LOW);
     digitalWrite(RGB_GREEN, HIGH);
    
    //buttonState = true;
    
    if((buttonState == true || on == true) && flag == true)
    {
      distance = distanceRead();
      updateBuzzer();
      updateNeoPixel();
      digitalWrite(PARKING_STATE_LED, HIGH);
    }
    else
    {
      delay(4000);
      lightLedsOffUntillLedN(-1); // inchdiem toate ledurile
      strip.show();
      digitalWrite(PARKING_STATE_LED, LOW);
    }
    /*
    if(buttonState == true)
      {
        on = true;
      }
      else
      {
        on = false;
      }
      */
  }
  
  //distance = distanceRead();
  //updateBuzzer();
  
  //updateNeoPixel();
  delay(10);
  
}

double distanceRead()
{
  digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2); // wait for 2 ms to avoid
                          // collision in serial monitor

    digitalWrite(
        TRIGGER_PIN, HIGH); // turn on the Trigger to generate pulse
    delayMicroseconds(
        10); // keep the trigger "ON" for 10 ms to generate
             // pulse for 10 ms.

    digitalWrite(TRIGGER_PIN,
                 LOW); // Turn off the pulse trigger to stop
                       // pulse generation

    // If pulse reached the receiver echoPin
    // become high Then pulseIn() returns the
    // time taken by the pulse to reach the
    // receiver

    duration = pulseIn(ECHO_PIN, HIGH);
    distance
        = duration * 0.0344 / 2; // Expression to calculate
                                 // distance using time

    
	return distance;
}

void updateNeoPixel()
{ 
  if(distance > 200)
  {
    lightLedsOffUntillLedN(-1);
  }else if (distance > 175) { // VERDE 1
    //strip.setPixelColor(0, 0, 255, 0);
    lightLedsOnUntillLedN(0);
    lightLedsOffUntillLedN(0); // pentru cand ne indepartam de obstacol
  } else if (distance > 150) { // VERDE 2
    //strip.setPixelColor(1, 0, 255, 0);
    lightLedsOnUntillLedN(1);
    lightLedsOffUntillLedN(1);
  } else if (distance > 135) { // GALBEN 1
    //strip.setPixelColor(2, 255, 255, 0);
    lightLedsOnUntillLedN(2);
    lightLedsOffUntillLedN(2);
  } else if (distance > 125) { // GALBEN 2
    //strip.setPixelColor(3, 255, 255, 0); 
    lightLedsOnUntillLedN(3);
    lightLedsOffUntillLedN(3);
  } else if (distance > 110) { // PORTOCALIU A 1
    //strip.setPixelColor(4, 255, 201, 14); 
    lightLedsOnUntillLedN(4);
    lightLedsOffUntillLedN(4);
  } else if (distance > 100) { // PORTOCALIU A 2
    //strip.setPixelColor(5, 255, 201, 14);
    lightLedsOnUntillLedN(5);
    lightLedsOffUntillLedN(5);
  } else if (distance > 75) { // PORTOCALIU B 1
    //strip.setPixelColor(6, 255, 127, 39); 
    lightLedsOnUntillLedN(6);
    lightLedsOffUntillLedN(6);
  } else if (distance > 50) { // PORTOCALIU B 2
    //strip.setPixelColor(7, 255, 127, 39);
    lightLedsOnUntillLedN(7);
    lightLedsOffUntillLedN(7);
  } else if (distance > 25) { // ROSU 1
    //strip.setPixelColor(8, 255, 0, 0); 
    lightLedsOnUntillLedN(8);
    lightLedsOffUntillLedN(8);
  } else if (distance > 0) { // ROSU 2
    //strip.setPixelColor(9, 255, 0, 0); 
    lightLedsOnUntillLedN(9);
    lightLedsOffUntillLedN(9);
  }
  else
  {
    lightLedsOffUntillLedN(-1);
  }
  
  strip.show();
}
  
void lightLedsOffUntillLedN(int n)
{
  int i = 0;
  for(i=9;i>n;i--)
  {
    strip.setPixelColor(i, 0, 0, 0); 
  }
}

void updateBuzzer() {
  if (distance > 150) {
    noTone(BUZZER_PIN);
  } else if (distance > 125) {
    tone(BUZZER_PIN, 1000);
    delay(1000);
    noTone(BUZZER_PIN);
    delay(1000);
  } else if (distance > 100) {
    tone(BUZZER_PIN, 1000);
    delay(800);
    noTone(BUZZER_PIN);
    delay(800);
  } else if (distance > 50){
    tone(BUZZER_PIN, 1000);
    delay(400);
    noTone(BUZZER_PIN);
    delay(400);
  }
  else
  {
    tone(BUZZER_PIN, 1000);
    delay(200);
    noTone(BUZZER_PIN);
    delay(200);
  }
}

void lightLedsOnUntillLedN(int n)
{
  if (n >= 0) 
  { // VERDE 1
    strip.setPixelColor(0, 0, 255, 0);
  }
  if (n >= 1)
  { // VERDE 2
    strip.setPixelColor(1, 0, 255, 0);
  } 
  if (n >= 2) 
  { // GALBEN 1
    strip.setPixelColor(2, 255, 255, 0);
  } 
  if (n >= 3) 
  { // GALBEN 2
    strip.setPixelColor(3, 255, 255, 0); 
  } 
  if (n >= 4) 
  { // PORTOCALIU A 1
    strip.setPixelColor(4, 255, 201, 14); 
  } 
  if (n >= 5) 
  { // PORTOCALIU A 2
    strip.setPixelColor(5, 255, 201, 14);
  } 
  if (n >= 6) 
  { // PORTOCALIU B 1
    strip.setPixelColor(6, 255, 127, 39); 
  } 
  if (n >= 7) 
  { // PORTOCALIU B 2
    strip.setPixelColor(7, 255, 127, 39);
  } 
  if (n >= 8) 
  { // ROSU 1
    strip.setPixelColor(8, 255, 0, 0); 
  } 
  if (n >= 9) 
  { // ROSU 2
    strip.setPixelColor(9, 255, 0, 0); 
  }
}


