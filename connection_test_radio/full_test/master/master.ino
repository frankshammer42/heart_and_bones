//Master to store all the heartbeats for the bones   
#include <VirtualWire.h>
#include <PJON.h>

#define BPS 8000 // transmission rate (bits per second)
#define MASTER_ID 4 // ID of the master 
#define MESSAGE_LENGTH 3 // number of bytes a data packet contains
#define MILLIS_IDLE_BETWEEN_TRANSMISSION 9 // min. time the transmitter waits before sending the next message

#define RX_PIN 12 // wireless module receiver data pin

uint8_t inputBuffer[MESSAGE_LENGTH]; // full data packet

byte rawData[VW_MAX_MESSAGE_LEN]; // a buffer to store the incoming data (can contain parts of a data packet)
byte dataLength = VW_MAX_MESSAGE_LEN; // the size of the data
int bones_heart_beats[3];

unsigned long lastDataReceived = 0; // time (millis) of last data reception
byte bytesReceived = 0; // number of bytes of current packet already received
int current_bone_index = -1;
int previous_heart_beats = 0;
bool got_all_bone_data = false;

// PJON Part 
float startTime;
float timeA;
float timeB;
PJON<SoftwareBitBang> bus(MASTER_ID); //  PJON ID 4 - consistent with radio ID

void write_serial(int value) {
  for (int i=0; i<20; i++){
    Serial.write(value);   
    delay(1);
  }
}

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  if (payload[0] == 'A') {
    if (millis() % 2000 >= 0 && millis() % 2000 < 1000) {
      write_serial(0);
      write_serial(bones_heart_beats[0]);
    }
    else {
	  /*digitalWrite(3, LOW);*/
    }
  }
  //  delay(100);
  if (payload[0] == 'B') {
    if (millis() % 2000 >= 0 && millis() % 2000 < 1000) {
      write_serial(1);
      write_serial(bones_heart_beats[1]);
    }
    //    delay(100);
    else {
	  /*digitalWrite(2, LOW);*/
    }
  }

  if (payload[0] == 'C') {
    if (millis() % 2000 >= 0 && millis() % 2000 < 1000) {
      write_serial(2);
      write_serial(bones_heart_beats[2]);
    }
    //    delay(100);
    else {
	  /*digitalWrite(2, LOW);*/
    }
  }
}

//Main Part with radio
void setup() {
  startTime = millis();
  Serial.begin(9600);
  vw_set_rx_pin(RX_PIN); // pin
  vw_setup(BPS); // transmission rate
  vw_rx_start(); // start receiver
  bus.strategy.set_pin(10);
  bus.begin();
  bus.set_receiver(receiver_function);
}

void loop() {
  bus.receive(1000);
  if (vw_get_message(rawData, &dataLength)) { // data incoming
    lastDataReceived = millis();
    // valid number of bytes received
    if (bytesReceived + dataLength > MESSAGE_LENGTH) {
      flushInputBuffer();
    }
    else {
      // append new raw data to input buffer
      for (int i = 0; i < dataLength; i++) {
        inputBuffer[bytesReceived + i] = rawData[i];
      }
      bytesReceived += dataLength;
      // check whether a full packet has been received already
      if (bytesReceived == MESSAGE_LENGTH) {
        // read recipient information (first byte)
        if (inputBuffer[0] == MASTER_ID) {
          // packet is for this receiver
          // read packet content

          int val;
          byte* valPtr = (byte*)&val;
          *(valPtr++) = inputBuffer[1];
          *(valPtr++) = inputBuffer[2];
		  if (val != previous_heart_beats){
	        current_bone_index++;
            bones_heart_beats[current_bone_index] = val;
            previous_heart_beats = val;
		  }
        }
        flushInputBuffer(); // clear input buffer
      }
    }
  }
  if (bytesReceived && lastDataReceived + MILLIS_IDLE_BETWEEN_TRANSMISSION / 2 < millis()) {
    // a broken packet has been received 
    flushInputBuffer();
  }

  //If we have enough bone data print
  /*if (current_bone_index >= 2){*/
    /*got_all_bone_data = true;*/
	/*for (int i; i<3; i++){*/
      /*Serial.print("Bone ");*/
      /*Serial.print(i);*/
      /*Serial.print(" has ");*/
      /*Serial.println(bones_heart_beats[i]);*/
      /*Serial.println("----------");*/
    /*}*/
  /*}*/
}

void flushInputBuffer() {
  for (byte i = 0; i < MESSAGE_LENGTH; i++) {
    inputBuffer[i] = 0;  
  }
  bytesReceived = 0;
}
