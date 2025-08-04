#include <SPI.h>
#include <ezButton.h>
#include <string>
#include "RF24.h"

#define LED_PIN 1
#define BUTTON_PIN 3

//Using esp8266 module
RF24 radio(2, 4);  //GPIO2, GPIO4
byte i = 45;
ezButton button(BUTTON_PIN);

void setup() {
  Serial.begin(9600);
  Serial.println("Setup Starting!");

  //Main board setup
  button.setDebounceTime(100);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  SPI.begin();

  //Radio setup
  radio.powerUp();
  delay(5);
  radio.startConstCarrier(RF24_PA_MAX, i);

  delay(900);
  if (radio.begin()) {
    delay(200);
    radio.setAutoAck(false);
    radio.stopListening();
    radio.setRetries(0, 0);
    radio.setPayloadSize(5);
    radio.setAddressWidth(3);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);
    radio.setCRCLength(RF24_CRC_DISABLED);
    radio.printPrettyDetails();
    radio.startConstCarrier(RF24_PA_MAX, i);
  }
  Serial.println("Setup Complete!");
}

//State swtich function
uint8_t state = 0;
void loop() {
  button.loop();
  if (button.isPressed()) {
    state = (state + 1) % 2;
  }
  switch (state) {
    case 0:
      digitalWrite(LED_PIN, LOW);
      break;
    case 1:
      fullAttack();
      digitalWrite(LED_PIN, HIGH);
      break;
  }
}

//Channel switch function
void fullAttack() {
  for (size_t i = 0; i < 80; i++) {
    radio.setChannel(i);
  }
}