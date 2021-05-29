#include <RHRouter.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>
#include <TimeLib.h>

String node_id = "02"; // Set for Rosedale
int node_id_mesh = 2; // Set for Rosedale
#define RH_HAVE_SERIAL

#ifdef ARDUINO_AVR_UNO
String board = "ARDUINO_AVR_UNO";
#define RFM95_CS 10
#define RFM95_RST 5
#define RFM95_INT  2
#elif ARDUINO_AVR_MEGA2560
String board = "ARDUINO_AVR_MEGA2560";
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#elif __AVR_ATmega1284P__
String board = "__AVR_ATmega1284P__";
#define RFM95_CS 4
#define RFM95_RST 1
#define RFM95_INT 2
#else
String board = "XCIS Pro or Pro mini";
#define RFM95_CS 4
#define RFM95_RST 5
#define RFM95_INT  2
#endif

RH_RF95 rf95(RFM95_CS, RFM95_INT);
RHMesh *manager;

// Analog PIN for Battery input
#define BATTERY_VOLTAGE A0
// Analog PIN for Reset switch
#define RESET_SW A3
// Analog PIN for Sensor On Indicator
#define SENSOR_ON A1
// Analog PIN for LORA Active
//#define LORA A2
#define LORA 0
// Digital PIN for Pulse Inpit
#define PULSE 10

int pulseCount = 0;
int accumulatedPulses = 0;
bool pulseFlag = false;
String pulseFlag_s = "0";
long accumulatedDataToken;
int inputState = 0;
int lastInputState = 0;
unsigned long delayStart = 0; // the time the delay started
bool delayRunning = 0; // true if still waiting for delay to finish
unsigned long delayStartPulse = 0;
unsigned long delayRainCheck = 0;
int defaultSwitchPress = 0;

void setup()
{
   randomSeed(analogRead(0)); // Seed the random number generator
   
   pinMode(RESET_SW,INPUT); // A3 Reset Switch
   //pinMode(PULSE,INPUT); // Pulse input
  
   pinMode(SENSOR_ON,OUTPUT);// A1, PC11 output LOW = ON SENSOR
   digitalWrite(SENSOR_ON,1);
   
   pinMode(LORA,OUTPUT); // A2, PCI2 output LOW = ON LORA
   digitalWrite(LORA,0);
    
   pinMode(RFM95_RST, OUTPUT); // LORA RESET
   digitalWrite(RFM95_RST, 1);
    // manual reset
    digitalWrite(RFM95_RST, 0);
    delay(10);
    digitalWrite(RFM95_RST, 1);
    delay(10);

    manager = new RHMesh(rf95, node_id_mesh); // Node ID - NEED TO SET THIS
    Serial.begin(115200);
  
    //manager->init();
    if (!manager->init()) {
      Serial.println(F("Radio init failed"));
    } else {
    Serial.println("Radion init success");
    }
    Serial.print("Device version:");
    Serial.println(rf95.getDeviceVersion());
    Serial.print("Registers:");
    Serial.println(rf95.printRegisters());
    
    delayStart = millis();   // start delay
    delayStartPulse = millis();   // start delay
    delayRainCheck = millis();
    delayRunning = true; // not finished yet
  
    Serial.print(F("Ready"));
    Serial.print(" Board:");
    Serial.println(board);
    digitalWrite(SENSOR_ON,0);
    digitalWrite(LORA,0);
}

void loop()
{
  /* 
   inputState = digitalRead(PULSE);
   if (inputState != lastInputState)
   {
     if (inputState == 1)
     {
       Serial.println("Got pulse");
       countPulses();
     }
   }
   lastInputState = inputState;
   */
   
  // check if delay has timed out after 10sec == DELAY_TIME
  if (delayRunning && ((millis() - delayStartPulse) >= 900000))//unsigned long DELAY_TIME = 3600 s
  {
    delayStartPulse += 900000; // 3600secs
    storePulseCount();
  }

  // check if delay has timed out after 10sec == DELAY_TIME
  if (delayRunning && ((millis() - delayRainCheck) >= 60000))//unsigned long DELAY_TIME = 06 s
  {
    delayRainCheck += 60000; // 60 secs
    checkRaining();
  }
  
  // parse for a packet, and call onReceive with the result:
  if (onReceive())
  {
    String message = "Got Message!";   // send a message
  }

}
