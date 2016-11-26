#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <MirfSpiDriver.h>
#include <nRF24L01.h>
#include "LTPConstants.h"

const int SENSOR_ID = 1;
const int SERVER_ID = 0;
int ptr = 0;

void setup() {
  Serial.begin(9600);
  Mirf.cePin = 9; 
  Mirf.csnPin = 10; 
  Mirf.spi = &MirfHardwareSpi; 
  Mirf.init(); 

  Mirf.channel = 26; 
  Mirf.payload = sizeof(int) * LTP_FRAME_SIZE; 
  Mirf.config(); 

  Mirf.setRADDR((byte *) "sensor"+SENSOR_ID); 
  Mirf.setTADDR((byte *) "server"+SERVER_ID); 
  
  Serial.println("[+] Sensor up and running");
  Serial.print("[i] Sensor ID : ");
  Serial.println(SENSOR_ID);
  advertise();
  
}

void loop() {
  
  int frame[LTP_FRAME_SIZE];

  if(Mirf.dataReady()){
    Mirf.getData((byte *) &frame); 
    decodeFrame(frame);
  }
  

  //TODO : 
  // Traitement detection RSSI VTX

  //Si un VTX est detecté avec un RSSI valide
  delay(5000);
  sendDetectionFrame(0x12,24440);


  
}

void advertise(){
  int frame[LTP_FRAME_SIZE];

  frame[0] = SENSOR_ID;
  frame[1] = SERVER_ID;
  frame[2] = LTP_CMD_ADVERTISE;
  frame[3] = LTP_DATA_NOP;
  frame[4] = LTP_DATA_NOP;
  frame[5] = LTP_DATA_NOP;
  
  Mirf.send((byte *) &frame);
  while(Mirf.isSending());
  
  Serial.println("[i] Advertise done");
}

void sendDetectionFrame(int encodedFrequency, int timeInMs){
  int frame[LTP_FRAME_SIZE];
  
  frame[0] = SENSOR_ID;
  frame[1] = SERVER_ID;
  frame[2] = LTP_CMD_DETECTION;
  frame[3] = encodedFrequency;
  frame[4] = highByte(timeInMs);
  frame[5] = lowByte(timeInMs);
  
  Mirf.send((byte *) &frame);
  while(Mirf.isSending());
  Serial.println("[i] Detection done");

}


void decodeFrame(int frm[] ){
  if(frm[2] == LTP_CMD_CALIBRATE){
    Serial.println("[+] Calibration request received");
    //TODO
  }
  else if(frm[2] == LTP_CMD_RAZ_FREQ){
    Serial.println("[+] Freq. reset request received");
    //TODO
  }
  else if(frm[2] == LTP_CMD_ADD_FREQ){
    Serial.println("[+] Add freq. request received");
    //TODO
  }
  
  
}

