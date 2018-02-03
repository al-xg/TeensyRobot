//Teensy libaries
#include <PulsePosition.h>

//Custom Robot Libaries
#include "weaponObjectMachine.h" 

extern char* __brkval;

unsigned int freeRam() {
    char tos;

    return &tos - __brkval;
}

const int DebugLevel=0;
#ifdef WillsBoard
const uint32_t PPMPin = 9;         //only certain pins work, eg, 9 and 10 on teensy 3.1 and teensy 3.5
#else
const uint32_t PPMPin = 10;         //only certain pins work, eg, 9 and 10 on teensy 3.1 and teensy 3.5
#endif
const uint32_t boardLEDPin = 13;
//const uint32_t FETLEDPin = 9;

unsigned int flashingTimer1=0;
unsigned int flashingTimer2=0;
unsigned int timeoutTimer=0;

StateMachineClass theWeapon(Serial,Serial1);
PulsePositionInput RadioIn;

void setup() {
  Serial.print("freeRam: "); Serial.println(freeRam());
	RadioIn.begin(PPMPin);
	//Serial1.begin(115200);
	Serial1.begin(57600);
	//Serial1.begin(9600);
	
	theWeapon.EnableStateMachine();
	theWeapon.externalRequest(0);
	
	pinMode(boardLEDPin,OUTPUT);
	//pinMode(FETLEDPin,OUTPUT);
	//PinMode(ValvePins[valveII],OUTPUT);
	
}

int ValidPPMdata=-1;
int valRadio1,valRadio2,valRadio3,valRadio4;
int switch1,switch2,switch3,switch4,switch5;

int pot1;

void oneSwitchFromChan(int chanel,int &switchA){

	chanel-=1000;
	if (chanel<325){
		switchA=0;
	}else if (chanel<700) {
		switchA=1;
	} else {
		switchA=2;
	}

}

void twoSwitchesFromChan(int chanel,int &switchA,int &switchB) {
	

	chanel-=1000;
	if (chanel<325){
		switchA=0;
	}else if (chanel<700) {
		switchA=1;
	} else {
		switchA=2;
	}
	
	chanel-=switchA*350;
		
	if (chanel<100){
		switchB=0;
	}else if (chanel<250) {
		switchB=1;
	}else{
		switchB=2;
	}	


}

unsigned int flashstate1=0;
unsigned int flashstate2=0;

void loop() {
  
	//theWeapon.foo();
	
	theWeapon.tickFunction();
	
	int tmptime=millis();
	
	
	if (flashingTimer1+2000 < tmptime){
		if (flashstate1 == 1){
			flashstate1=0;
		}else{
			flashstate1=1;
		}
		digitalWrite(boardLEDPin,flashstate1);
		Serial.print("freeRam: "); Serial.println(freeRam());
		flashingTimer1=millis();
	}
	
	
/*	if (flashingTimer2+2000 < tmptime){
		if (flashstate2 == 1){
			flashstate2=0;
		}else{
			flashstate2=1;
		}
		digitalWrite(FETLEDPin,flashstate2);
		flashingTimer2=millis();
	}*/
	
	
	int num = RadioIn.available();
	//if (DebugLevel>20){Serial.print("radio avail");Serial.println(num);}
	if (num == 8) {
		
		timeoutTimer=millis();
		
		valRadio1 = RadioIn.read(5);
		valRadio2 = RadioIn.read(6);
		valRadio3 = RadioIn.read(7);
		valRadio4 = RadioIn.read(8);
		// We should validate the read values here. If outside of 950-2050us, then raise error.
		
		ValidPPMdata=1;
		//if (DebugLevel>10){Serial.println(valArm);}
				

		if (DebugLevel>10){Serial.print(valRadio1);Serial.print(" ");Serial.print(valRadio2);Serial.print(" ");Serial.print(valRadio3);Serial.print(" ");Serial.println(valRadio4);}
	
		oneSwitchFromChan(valRadio4,switch5);
		twoSwitchesFromChan(valRadio2,switch1,switch2);
		twoSwitchesFromChan(valRadio3,switch3,switch4);
		
		if (DebugLevel>10){Serial.print(switch1);Serial.print(" ");Serial.print(switch2);Serial.print(" ");Serial.print(switch3);Serial.print(" ");Serial.print(switch4);Serial.print(" ");Serial.println(switch5);}
		
		//should we have something to only let the data through if we have two the same in a small time frame?
		// this would add loads of delay in to firing etc tho..
		// the switch values are described in the StateMachineClass::updateSwitches code
		theWeapon.updateSwitches(switch1,switch2,switch3,switch4,switch5);
		
		
		if (DebugLevel>10){Serial.print(pot1);Serial.print(" ");Serial.print(switch1);Serial.print(" ");Serial.print(switch2);Serial.print(" ");Serial.print(switch3);Serial.print(" ");Serial.println(switch4);}
	}
	
	
	if (timeoutTimer+2000 < tmptime){
		theWeapon.externalRequest(0);
		Serial.println("PPM signal not recived for too long so going to safe!!");
		#ifdef WillsBoard
			Serial.println("Wills Board check PPM pin");
		#endif
	
	}
	
	
  	
}

