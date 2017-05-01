#include <RH_ASK.h> //library needed for transmission
#include <SPI.h> // Not actualy used but needed to compile

RH_ASK driver;
const int yellow_led = 13; //set pins for leds
const int red_led = 12;
const int buzzerPin = 8; //set pin for buzzer
const int frequency = 2400; //set frequency of buzzers


void setup()
{
  pinMode(yellow_led, OUTPUT);
  Serial.begin(9600);
  if (!driver.init())
    Serial.println("init failed");

}

void loop()
{

 
  static int state = 0;  //begin in state 0

  uint8_t buf[1]; //received message will be one char in length
  uint8_t buflen = sizeof(buf); //buflength will be one



  switch (state)
  {
    case 0:
      buf[0] = '0'; //reset the buf to 0
      
      //if a message is recieved
      if (driver.recv(buf, &buflen))
      {
        //choose a state based on recieved message
        if (buf[0] == '1') {
          state = 1;
        }

        else if (buf[0] == '2') {
          state = 2;
        }
        
        else if (buf[0] == '3') {
          state = 3;
        }
      }
      break;

    case 1:
      Serial.println("State 1");
      digitalWrite(yellow_led, 1); //turn on yellow led
      delay(3000); //wait 3 seconds
      digitalWrite(yellow_led, 0); //turn off yellow led
      state = 0; //return to state 0
      break;

    case 2:
      Serial.println("State 2");
      digitalWrite(red_led, 1); //turn on red led
      delay(3000);
      digitalWrite(red_led, 0); //turn off red led
      state = 1; //return to state one
      break;
      
    case 3:
      Serial.println("State 3");
      digitalWrite(red_led, 1); //turn on red led
      tone(buzzerPin, frequency); //turn on buzzer
      delay(3000);
      digitalWrite(red_led, 0); //turn off red led
      noTone(buzzerPin); //turn off buzzer
      state = 2; //return to state 2
      break;
      
    default:
      state = 0;
      break;
  }

}
