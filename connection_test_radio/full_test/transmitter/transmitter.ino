
#include <VirtualWire.h>
#define USE_ARDUINO_INTERRUPTS false     
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
bool transfer_data = false;
bool record_heartbeat = false;
int threshold = 300; //To Avoid Noise When Idle
int pulse_check_threshold = 8;
int pulse_check_times = 0;
int pulse_sum = 0;
int pulse_to_transfer = 0; 
byte samplesUntilReport; 
const byte SAMPLES_PER_SERIAL_SAMPLE = 10;
int current_index = 1; //start from zero
int button_pin = 7;
int send_times = 0;
int heart_pulse_accumulator = 0;
int zero_times = 0;
int zero_check_threshold = 5;

bool printed_prompt = false;

//int heartbeat = [];

void heart_pulse_setup(){
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);         
  pulseSensor.setThreshold(threshold);   
  pulseSensor.setSerial(Serial);
  samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;
  if (pulseSensor.begin()) {
	/*Serial.println("Pulse Sensor Initiated");  */
  }
}

bool get_enough_pulse(){
  if (pulse_check_times == pulse_check_threshold){
    return true;
  }
  else{
    return false;
  }
}

void reset_pulse_params(){
  samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE; //Reset the sample to avoid wrong data
  pulse_check_times = 0;
  pulse_sum = 0; 
}

void send_pulse(int receiverId, int value) {
  byte* valPtr = (byte*)&value;
  outputBuffer[0] = (uint8_t)receiverId;
  outputBuffer[1] = (uint8_t) *(valPtr++);
  outputBuffer[2] = (uint8_t) *(valPtr++);
  vw_send(outputBuffer, MESSAGE_LENGTH);
  vw_wait_tx(); // wait until the whole message is gone
  delay(MILLIS_IDLE_BETWEEN_TRANSMISSION); // wait some more time
}

void print_BPM(int myBPM){
  Serial.print("♥!!!"); 
  Serial.print(" Get beats ----------");	
  Serial.println(myBPM);
}

//use loop to ensure data is safely sent
void write_to_serial(byte value){
  if (value == 1){
    for (int i=0; i<5; i++){
	  Serial.write(value);
	}
  }
  else{
    Serial.write(value);
  }
  /*for (int i=0; i<10; i++){*/
  /*}*/
}


void setup() {
  Serial.begin(115200);
  pinMode(button_pin, INPUT);
  heart_pulse_setup(); 
  vw_set_tx_pin(DATA_PIN); // transmitter module data pin
  vw_setup(BPS); // transmission rate
}

void loop() {
  /*if (record_heartbeat){*/
	/*byte wt = 250;*/
	/*write_to_serial(250);*/
  /*}*/

  boolean button_pressed = digitalRead(button_pin);
  if (button_pressed){
	/*Serial.println("♥ Start to read heart beat --------");*/
	/*byte wt = 250;*/
	/*write_to_serial(250);*/
	printed_prompt = true;
    record_heartbeat = true;
  }

  if (pulseSensor.sawNewSample() && record_heartbeat) {
    if (--samplesUntilReport == (byte) 0) {
      samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;
	  /*pulseSensor.outputSample(); // out put samples to check*/
	  if (pulseSensor.sawStartOfBeat() || heart_pulse_accumulator > 600) {
		int myBPM = pulseSensor.getBeatsPerMinute();
		if (myBPM == 0){
          zero_times++;
		}
        if (zero_times > zero_check_threshold){
          if (pulse_check_times >= 1){
	  		record_heartbeat = false;
		    transfer_data = true;
			/*Serial.print("Pulse Check Times is ");*/
			/*Serial.println(pulse_check_times);*/
			/*Serial.print("Pulse Sum is ");*/
			/*Serial.println(pulse_sum);*/
	      }
          else{
            zero_times = 0;
          }
	    }
		/*Serial.write(111);*/
		bool bpm_corrected = false;
		if (myBPM < 100 && myBPM > 45){
			pulse_check_times += 1;
			/*Serial.println(pulse_check_times);*/
			pulse_sum += myBPM;
			/*print_BPM(myBPM);*/ //IO will effect the accuracy
	    }
		else if (myBPM < 45 && myBPM > 0){
			pulse_check_times += 1;
			/*Serial.println(pulse_check_times);*/
			myBPM = 2*myBPM;
			pulse_sum += myBPM;
			bpm_corrected = true;
 		}
		else if (myBPM > 100){
			if (!bpm_corrected){
				pulse_check_times += 1;
				/*Serial.println(pulse_check_times);*/
				myBPM = myBPM/2;
				pulse_sum += myBPM;
			}
		}
		/*Serial.print("Got BPM ");*/
		/*Serial.println(myBPM);*/
		write_to_serial(myBPM);
		heart_pulse_accumulator = 0;
	  }
    }
	heart_pulse_accumulator++;
    if (get_enough_pulse()){
	  heart_pulse_accumulator = 0;
	  /*Serial.print("Sum is "); */
	  /*Serial.println(pulse_sum);*/
	  record_heartbeat = false;
      transfer_data = true;
    }
	
  }
  
  if (!record_heartbeat && transfer_data){
	 /*byte to_send = 1;*/
	 /*write_to_serial(to_send);*/
     pulse_to_transfer = int(pulse_sum/pulse_check_times);
	 byte to_send = 1;
	 write_to_serial(to_send);
	 /*Serial.print("♥!!! "); */
	 /*Serial.print("Start to send data to ");*/
	 /*Serial.println(current_index);*/
	 /*Serial.print("The pulse is "); */
	 /*Serial.println(pulse_to_transfer);*/
     send_pulse(current_index , pulse_to_transfer);
     send_pulse(MASTER_ID, pulse_to_transfer);
	 send_times++;
	 //heartbeat[current_index] = pulse_to_transfer;
  }
  if (send_times > 100){
	/*Serial.println("Send more than 100 times now wait");*/
    reset_pulse_params();
    transfer_data = false; 
    current_index++;
    if (current_index > 3){
      current_index = 1;
    }
    send_times = 0;
	printed_prompt = false;
	byte to_send = 0;
	write_to_serial(to_send);
    delay(3000);
  }
}
