#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <PulseSensorPlayground.h>     

#define USE_ARDUINO_INTERRUPTS true    
#define led 12  //Might Need to change this
#define CE 9
#define CSN 10
#define PulseWire 0
#define LED13 13
#define threshold 500  



//TODO: Need to Create a reset function
// The heart module reads data from heart sensor and sends them to the bones
// Its role should be transmitter first then listener 
PulseSensorPlayground pulseSensor;  
RF24 radio(CE, CSN); // CE, CSN
const byte addresses[][6] = {"00001", "00002", "00003"}; // One heart, two bones
const int number_of_bones = 2;
boolean buttonState = 0;
bool transfered_data = false;
int pulse_check_threshold = 40;
int pulse_sum = 0;
int pulse_check_times = 0;
int current_bone_index = 1; 
int magic_number = 0;


void heart_pulse_setup(){
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);         
  pulseSensor.setThreshold(threshold);   
  if (pulseSensor.begin()) {
    Serial.println("Pulse Sensor Initiated");  
  }
}


void radio_setup(){
  pinMode(led, OUTPUT);
  radio.begin();
  radio.openWritingPipe(addresses[current_bone_index]);
  radio.openReadingPipe(1, addresses[0]);
  radio.setPALevel(RF24_PA_MIN);
}


void setup() {
  Serial.begin(9600);          
  /*heart_pulse_setup();*/
  radio_setup();
  delay(1000);

}


void get_pulse(){
  
  /*int myBPM = pulseSensor.getBeatsPerMinute();  */
  /*if (pulseSensor.sawStartOfBeat()) {             */
    /*Serial.println("♥!!!"); */
    /*Serial.print("BPM: ");                         */
    /*Serial.println(myBPM);                         */
	/*pulse_check_times += 1;*/
    /*pulse_sum += myBPM;*/
  /*}*/

  //Test
  int myBPM = 100 + random(10, 50);
  pulse_check_times += 1;
  pulse_sum += myBPM;
  delay(20);        
}


bool get_enough_pulse(){
  if (pulse_check_times == pulse_check_threshold){
    return true;
  }
  else{
    return false;
  }
}

//Send Pulse data to the current chosen bone address
void send_pulse(int pulse_to_send){
  Serial.println("--------------");
  Serial.print("Start to send the pulse ");
  Serial.println(pulse_to_send);  
  delay(5);
  radio.stopListening();
  radio.write(&pulse_to_send, sizeof(pulse_to_send));
}


void reset_pulse_params(){
  pulse_check_times = 0;
  pulse_sum = 0; 
}


//The Heart will halt, and wait for bone gets connected 
void wait_for_connection(){
  delay(5);
  Serial.println("--------");
  Serial.println("Start to wait for connection");
  radio.startListening();
  while (!radio.available()){
    Serial.println("No Feed Back Recieved Yet");  
  }
//  radio.read(&buttonState, sizeof(buttonState));
//  if (buttonState == HIGH) {
//    digitalWrite(led, HIGH);
//  }
//  else {
//    digitalWrite(led, LOW);
//  }
  radio.read(&magic_number, sizeof(magic_number));
  current_bone_index += 1;
  if (current_bone_index > number_of_bones){ 
    current_bone_index = 1; 
  }
  Serial.print("Got the connection with magic number ");
  Serial.println(magic_number);
  radio.stopListening();
  radio.openWritingPipe(addresses[current_bone_index]);
}


void loop() {
  int pulse_to_transfer = 0;
  get_pulse();
  if (get_enough_pulse()){
    pulse_to_transfer = int(pulse_sum/pulse_check_times);
    reset_pulse_params();
    if (!transfered_data){
      Serial.println("Start to send data");
      send_pulse(pulse_to_transfer);
      wait_for_connection();
      transfered_data = true;
    }
  }
}
