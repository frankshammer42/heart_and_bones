#include <PJON.h>
PJON<SoftwareBitBang> bus(4); //  PJON ID 4 - consistent with radio ID

float startTime;
float timeA;
float timeB;
int heart_beats[3] = {79, 85, 90};

void write_serial(int value) {
  for (int i=0; i<20; i++){
    Serial.write(value);   
    delay(1);
  }
}

void setup() {
  startTime = millis();
  Serial.begin(9600);
  bus.strategy.set_pin(10);
  bus.begin();
  bus.set_receiver(receiver_function);
};


void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  if (payload[0] == 'A') {
    if (millis() % 2000 >= 0 && millis() % 2000 < 1000) {
      write_serial(0);
      write_serial(heart_beats[0]);
	  /*Serial.println("Got Data from Board 1");*/
	  /*digitalWrite(3, HIGH);*/
    }
    //    delay(200);
    else {
	  /*digitalWrite(3, LOW);*/
    }
  }
  //  delay(100);
  if (payload[0] == 'B') {
    if (millis() % 2000 >= 0 && millis() % 2000 < 1000) {
      write_serial(1);
      write_serial(heart_beats[1]);
	  /*Serial.println("Got Data from Board 2");*/
	  /*digitalWrite(2, HIGH);*/
    }
    //    delay(100);
    else {
	  /*digitalWrite(2, LOW);*/
    }
  }

}

void loop() {
  bus.receive(1000);
};
