
#include <VirtualWire.h>
#define USE_ARDUINO_INTERRUPTS false    // Set-up low-level interrupts for most
#include <PulseSensorPlayground.h>
#define BPS 8000 // transmission rate (bits per second)

// IDs of the receiver modules [1;255]
#define RECEIVER1_ID 1
#define RECEIVER2_ID 2
#define RECEIVER3_ID 3
#define MASTER_ID 4


#define MESSAGE_LENGTH 3 // packet size in bytes

#define MILLIS_IDLE_BETWEEN_TRANSMISSION 9 // recommended approx. MESSAGE_LENGTH * 8 / BPS * 1,000 * 3
#define DATA_PIN 12
#define LED13 13
#define PulseWire 0


PulseSensorPlayground pulseSensor;  
uint8_t outputBuffer[MESSAGE_LENGTH]; // output buffer
bool transfered_data = false;
bool record_heartbeat = true;
int threshold = 550;
int pulse_check_threshold = 1;
int pulse_check_times = 0;
int pulse_sum = 0;
int pulse_to_transfer = 0; byte samplesUntilReport; const byte SAMPLES_PER_SERIAL_SAMPLE = 10;
int current_index = 1;




void heart_pulse_setup(){
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);         
  pulseSensor.setThreshold(threshold);   
  pulseSensor.setSerial(Serial);
  samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;
  if (pulseSensor.begin()) {
    Serial.println("Pulse Sensor Initiated");  
  }
}

/*void get_pulse(){*/
  /*int myBPM = pulseSensor.getBeatsPerMinute();  */
  /*Serial.println(myBPM);*/
  /*if (pulseSensor.sawStartOfBeat()) {             */
	  /*Serial.println("♥!!!"); */
	  /*Serial.print("BPM: ");                         */
	  /*Serial.println(myBPM);                         */
	  /*pulse_check_times += 1;*/
	  /*pulse_sum += myBPM;*/
  /*}*/

  /*//Test*/
  /*delay(20);        */
/*}*/


bool get_enough_pulse(){
  if (pulse_check_times == pulse_check_threshold){
    return true;
  }
  else{
    return false;
  }
}

void reset_pulse_params(){
  pulse_check_times = 0;
  pulse_sum = 0; 
}

void send_pulse(int receiverId, int value) {
  byte* valPtr = (byte*)&value;
  
  // fill output buffer with data
  outputBuffer[0] = (uint8_t)receiverId;
  outputBuffer[1] = (uint8_t) *(valPtr++);
  outputBuffer[2] = (uint8_t) *(valPtr++);
  
  // transmitt data
  vw_send(outputBuffer, MESSAGE_LENGTH);
  vw_wait_tx(); // wait until the whole message is gone
  delay(MILLIS_IDLE_BETWEEN_TRANSMISSION); // wait some more time
}

void setup() {
  Serial.begin(115200);
  heart_pulse_setup(); 
  vw_set_tx_pin(DATA_PIN); // transmitter module data pin
  vw_setup(BPS); // transmission rate
}

void loop() {
  /*int pulse_to_transfer = 0;*/
  /*get_pulse();*/
  if (pulseSensor.sawNewSample() && record_heartbeat) {
    if (--samplesUntilReport == (byte) 0) {
      samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;
      /*pulseSensor.outputSample();*/ // out put samples to check
      if (pulseSensor.sawStartOfBeat()) {
		int myBPM = pulseSensor.getBeatsPerMinute();
		if (myBPM < 100 && myBPM > 60){
			Serial.print("♥!!!"); 
			Serial.print(" Get beats ----------");	
			Serial.println(myBPM);
			pulse_check_times += 1;
			pulse_sum += myBPM;
	    }
      }
    }
    if (get_enough_pulse()){
      pulse_to_transfer = int(pulse_sum/pulse_check_times);
	  Serial.print("Sum is "); 
	  Serial.println(pulse_sum);
      reset_pulse_params();
	  record_heartbeat = false;
    }
  }
  
  if (!record_heartbeat){
	 Serial.print("♥!!! "); 
	 Serial.print("Start to send data");
	 Serial.println(pulse_to_transfer);
     send_pulse(current_index , pulse_to_transfer);
	 send_pulse(MASTER_ID, pulse_to_transfer);
  }
  
  /*if (get_enough_pulse()){*/
    /*int pulse_to_transfer = int(pulse_sum/pulse_check_times);*/
    /*reset_pulse_params();*/
    /*if (!transfered_data){*/
      /*Serial.println("Start to send data");*/
      /*send_pulse(RECEIVER1_ID , pulse_to_transfer);*/
    /*}*/
  /*}*/
}
