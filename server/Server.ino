#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <MirfSpiDriver.h>
#include <nRF24L01.h>
#include "LTPConstants.h"


const int SERVER_ID = 0;
int _atarget = 0;
int tstPtr = 0;

void setup() {
  Serial.begin(9600);
  Mirf.cePin = 9; 
  Mirf.csnPin = 10; 
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init(); 

  Mirf.channel = 26; 
  Mirf.payload = sizeof(int) * LTP_FRAME_SIZE; 
  Mirf.config(); 

  Mirf.setRADDR((byte *) "server"+SERVER_ID); 
  
  Serial.println("[+] Server up and running");
  Serial.print("[i] Server ID : ");
  Serial.println(SERVER_ID);
  
}

void loop() {
  
  int frame[LTP_FRAME_SIZE];
  
  if(Mirf.dataReady()){
    Mirf.getData((byte *) &frame); 
    Serial.println("[+] DATA");
    decodeFrame(frame);
  }

  if(tstPtr == 1){
    sendCalibrate(_atarget);
    tstPtr = 0;
  }
  else if(tstPtr == 2){
    sendRAZ(_atarget);
    tstPtr++;
  }
  else if(tstPtr == 3){
    sendAdd(_atarget);
    tstPtr++;
  }
      
}

void decodeFrame(int frm[] ){
  Serial.println("[+] Decoding Frame");
  if(frm[2] == LTP_CMD_ADVERTISE){
    _atarget = frm[0];
    Mirf.setTADDR((byte *) "sensor"+_atarget); 
    //tstPtr = 1;
    Serial.print("[+] Advertise received from ");
    Serial.println(_atarget);
  }
  else if(frm[2] == LTP_CMD_DETECTION){
    Serial.print("[+] Detection received from sensor#");
    Serial.println(_atarget);
    uint32_t _time = frm[4] << 8 | frm[5];

    Serial.print("[+] Data (ms): ");
    Serial.println(_time);
  }
   
}

void sendCalibrate(int target){
  int frame[LTP_FRAME_SIZE];

  frame[0] = SERVER_ID;
  frame[1] = target;
  frame[2] = LTP_CMD_CALIBRATE;
  frame[3] = LTP_DATA_NOP;
  frame[4] = LTP_DATA_NOP;
  frame[5] = LTP_DATA_NOP;
  
  Mirf.send((byte *) &frame);
  while(Mirf.isSending());
  
  Serial.println("[+] Calibrate Request sent");
}

void sendRAZ(int target){
  int frame[LTP_FRAME_SIZE];

  frame[0] = SERVER_ID;
  frame[1] = target;
  frame[2] = LTP_CMD_RAZ_FREQ;
  frame[3] = LTP_DATA_NOP;
  frame[4] = LTP_DATA_NOP;
  frame[5] = LTP_DATA_NOP;

  Mirf.send((byte *) &frame);
  while(Mirf.isSending());
  
  Serial.println("[+] RAZ Request sent");
}


void sendAdd(int target){
  int frame[LTP_FRAME_SIZE];

  frame[0] = SERVER_ID;
  frame[1] = target;
  frame[2] = LTP_CMD_ADD_FREQ;
  frame[3] = LTP_DATA_NOP;
  frame[4] = LTP_DATA_NOP;
  frame[5] = LTP_DATA_NOP;

  Mirf.send((byte *) &frame);
  while(Mirf.isSending());
  
  Serial.println("[+] ADD Request sent");
}

