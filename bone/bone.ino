#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#define connection_point 4
RF24 radio(9, 10); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
boolean bone_connected = 0;
boolean got_pulse = false;
int pulse = 0;

//Test parameter
int check_times = 0; //mock parameter for checking connection 


void setup() {
  pinMode(connection_point, INPUT);
  radio.begin();
  radio.openWritingPipe(addresses[0]); 
  radio.openReadingPipe(1, addresses[1]); 
  radio.setPALevel(RF24_PA_MIN);
}


void loop() {
  delay(5);
  radio.startListening();
  if ( radio.available()) {
    while (radio.available()) {
      pulse = 0;
      radio.read(&pulse, sizeof(pulse));
 	  got_pulse = true;
      /* 
      In here after we get the pulse we change the light in the bone
      */ 
      Serial.print("Get Pulse ----- ");
      Serial.println(pulse);
    }
   }
   if (got_pulse){
      check_times += 1;
     if (check_times > 100){
       bone_connected = true;
     }
     //Uncomment this in production 
     /*bone_connected = digitalRead(connection_point);*/
     if (bone_connected){
       Serial.println("Bone Connected Transfer State Back Now");
	   radio.stopListening();
	   radio.write(&bone_connected, sizeof(bone_connected));
     }
   }
}
