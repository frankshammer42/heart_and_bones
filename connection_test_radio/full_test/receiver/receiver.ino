#include <VirtualWire.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            7

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      10

#define BPS 8000 // transmission rate (bits per second)
#define RECEIVER_ID 3 // ID of this receiver
#define MESSAGE_LENGTH 3 // number of bytes a data packet contains
#define MILLIS_IDLE_BETWEEN_TRANSMISSION 9 // min. time the transmitter waits before sending the next message

#define RX_PIN 12 // wireless module receiver data pin

uint8_t inputBuffer[MESSAGE_LENGTH]; // full data packet

byte rawData[VW_MAX_MESSAGE_LEN]; // a buffer to store the incoming data (can contain parts of a data packet)
byte dataLength = VW_MAX_MESSAGE_LEN; // the size of the data

unsigned long lastDataReceived = 0; // time (millis) of last data reception
byte bytesReceived = 0; // number of bytes of current packet already received
int outputPin = 7;
bool got_data = false;
int my_bpm = 0;

//PJON Part Setup

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void blink(int bpm){ 
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(200,0,0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(30000/bpm); // Delay for a period of time (in milliseconds).

  }
}


void setup() {
  Serial.begin(9600);
  pinMode(outputPin, OUTPUT);
 

  pixels.begin(); // This initializes the NeoPixel library.
  vw_set_rx_pin(RX_PIN); // pin
  vw_setup(BPS); // transmission rate
  vw_rx_start(); // start receiver
}


void loop() {
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
        if (inputBuffer[0] == RECEIVER_ID) {
          // packet is for this receiver
          
          // read packet content
          int val;
          byte* valPtr = (byte*)&val;
          *(valPtr++) = inputBuffer[1];
          *(valPtr++) = inputBuffer[2];
    	  Serial.println("♥ Got Heart Beat ");
          Serial.println(val);
          my_bpm = val; 
		  got_data = true;
        }

        flushInputBuffer(); // clear input buffer
      }
    }
  }

  if (bytesReceived && lastDataReceived + MILLIS_IDLE_BETWEEN_TRANSMISSION / 2 < millis()) {
    // a broken packet has been received 
    flushInputBuffer();
  }
  
  if(got_data){
    blink(my_bpm);
  }
}

void flushInputBuffer() {
  for (byte i = 0; i < MESSAGE_LENGTH; i++) {
    inputBuffer[i] = 0;  
  }
  bytesReceived = 0;
}
