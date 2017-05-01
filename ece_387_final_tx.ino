#include <RH_ASK.h> //library needed for transmission
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;
//Speed is 2000bps; RX pin = 11; TX pin = 12

const int led_pin = 13;
const int pir = 2;
int val;
static int state = 0;
long t = 0; //long to prevent overflow
long previousmillis;



void setup()
{
  pinMode(led_pin, OUTPUT);
  pinMode(pir, INPUT);
  Serial.begin(9600); 
  if (!driver.init())
    Serial.println("init failed");

  //0 means PIN 2, 1 means PIN 3
  attachInterrupt(0, pir_detect, CHANGE);
}

void loop()
{

  switch (state) {

    case 0:
      break;


    case 1:
      while (val == 1) { //while the pir detects motion
        t = millis() - previousmillis; //t = time in milli seconds
        digitalWrite(led_pin, 1); //turn on led
        Serial.println(t);
      }
      state = 2; //when motion ends, go to state 2

    case 2:
      digitalWrite(led_pin, 0); //turn off led

      //The total time pir detector detects motion determines case
      if (t > 0 && t < 5000 ) {  //t = 0s to t = 5s
        Serial.println(t);
        t = 0; //reset t
        const char *msg = "1"; //message to be sent
        driver.send((uint8_t *)msg, strlen(msg)); //send the message
        driver.waitPacketSent();
        delay(1000); //wait to make sure complete message transmitted
        Serial.println("sent 1");

      }

      else if (t >= 5000 && t < 15000) { //t = 5s to t = 15s
        Serial.println(t);
        t = 0; //reset t
        const char *msg = "2"; //message to be sent
        driver.send((uint8_t *)msg, strlen(msg));
        driver.waitPacketSent();
        delay(1000);
        Serial.println("sent 2");
      }

      else if (t >= 15000) {  //t > 15s
        Serial.println(t);
        t = 0; //reset t
        const char *msg = "3"; //message to be sent
        driver.send((uint8_t *)msg, strlen(msg));
        driver.waitPacketSent();
        delay(1000);
        Serial.println("sent 3");
      }
      break;

    default:
      Serial.println("default");
      break;
  }
}

//interupt intervenes when motion is detected
void pir_detect() {
  val = digitalRead(pir); //reads pir motion detector
  previousmillis = millis();  //resets the timer
  state = val;
}
