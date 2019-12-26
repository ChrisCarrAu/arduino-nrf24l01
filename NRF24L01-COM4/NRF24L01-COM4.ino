#include "RF24.h"//can be found through the IDE: Sketch/Include Library/Manage Libraries/ Search for RF24 and locate RF24 by TMRh20/ more info / Install

// Set up the button and LEDs
#define MOMENT_BUTTON 4
#define YELLOW_LED 2
#define RED_LED 3

enum opMode {
  TRANSMIT,
  RECIEVE
};
const opMode operationalMode = TRANSMIT;

RF24 NRF24L01 (7, 8);//create object called NRF24L01. specifying the CE and CSN pins to be used on the Arduino
byte address[] [6] = {"pipe1", "pipe2"};//set addresses of the 2 pipes for read and write
int buttonState = 0;  //used for both transmission and receive

void flashLed(int led, int count=1, int milliseconds=200);

void setup() {
  Serial.begin(9600);
  
  // Setup the Arduino pins
  pinMode(MOMENT_BUTTON, INPUT_PULLUP);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  NRF24L01.begin(); //open the pipes to read and write from board 1
  NRF24L01.openReadingPipe(1, address[0]);//open reading pipe from address pipe 1
  NRF24L01.openWritingPipe(address[1]);//open writing pipe to address pipe 2

  NRF24L01.setPALevel(RF24_PA_MIN);//set RF power output to minimum, RF24_PA_MIN (change to RF24_PA_MAX if required)
  NRF24L01.setDataRate(RF24_250KBPS);//set data rate to 250kbps

  // If the frequency of 110 below is a problem with other wi-fi for you increment by 1 until it is ok
  // Don't forget that both sets of code must have the same frequency
  NRF24L01.setChannel(111);//set frequency to channel 110.
}

void loop() {
  NRF24L01.stopListening();

  // Test for button press on THIS board
  buttonState = digitalRead(MOMENT_BUTTON);

  // Button is pulled up so test for LOW
  if (buttonState == LOW) { 
    // Send LOW state to other Arduino board

    buttonState = 200;
    if (!NRF24L01.write(&buttonState, sizeof(buttonState))) {
      // No acknowledgement of receipt by reciever
      Serial.println("Signal not recieved.");
    }
    else {
      // Flash the yellow LED to show progress
      flashLed(YELLOW_LED);
    }
  }
}

void flashLed(int led, int count, int milliseconds)
{
  for (int i = 0; i < count; i++)
  {
    digitalWrite(led, HIGH);
    delay(milliseconds);
    digitalWrite(led, LOW);
    if (i < count) { delay(milliseconds); }
  }
}
