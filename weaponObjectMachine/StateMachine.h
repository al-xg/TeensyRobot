#include "Arduino.h"
#include <usb_serial.h>
#include <HardwareSerial.h>

class StateMachineClass
{public:

  int bar;
  
  float Sensor1;
  float Sensor2;
  void updateSwitches(int switch1, int switch2, int switch3, int switch4, int switch5); 
 int defaultStateTimeOutFunction(int input);
  StateMachineClass( usb_serial_class &print,HardwareSerial &print2 );
  //StateMachineClass(  );
  
void tickFunction();
void debugFunction();
void QuickdebugFunction();
void debugFunction(HardwareSerial MySerial );
void EnableStateMachine();
void externalRequest(int NewState);
  private:
    usb_serial_class* printer;  
    HardwareSerial* printerHW; 
  int happyMachine=-1;
  int DebugLevel=22;
  int debugHW=1;
  void setMachineState(int NewState);
  int currentState=0;
  int TransitionState=0;
  unsigned long TransitionStartMicros=0;
  int TelemetryMode=1;
  unsigned long telemMillis=0;
  unsigned long telemMillisRefreshRate=1000;
};
