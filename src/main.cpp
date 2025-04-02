#include <Arduino.h>
#include <nextion.h>
#include <EEPROM.h>

#define led 2

#define EEPROM_OFF_LEVEL_ADDR 0
#define EEPROM_ON_LEVEL_ADDR 4
#define EEPROM_FULL_HEIGHT_ADDR 8
#define EEPROM_EMPTY_HEIGHT_ADDR 12

uint32_t OffLevel; // Variable to store the value of OffLevel
uint32_t OnLevel;  // Variable to store the value of OnLevel
uint32_t FullHeight; // Variable to store the value of FullHeight
uint32_t EmptyHeight; // Variable to store the value of EmptyHeight
uint32_t AutoOnState; // Variable to store the value of AutoOnStateB
uint32_t AutoOffState; // Variable to store the value of AutoOffStateB
uint32_t ManualOnState; // Variable to store the value of ManualOnStateB
uint32_t ManualOffState; // Variable to store the value of ManualOffStateB

bool ledState = false;  // Variable to track LED state

//Buttons
NexButton bSave = NexButton(1, 5, "bSave");
NexButton bStartStop = NexButton(0, 7, "bStartStop");
NexButton bSettings = NexButton(0, 12, "bSettings");

//Texts
NexText  tOffLevel = NexText(1, 15, "tOffLevel");
NexText  tOnLevel = NexText(1, 17, "tOnLevel");
NexText  tFullHeight = NexText(1, 20, "tFullHeight");
NexText  tEmptyHeight = NexText(1, 23, "tEmptyHeight");

//Variables
NexVariable vOffLevel = NexVariable(1, 27, "vOffLevel");
NexVariable vOnLevel = NexVariable(1, 28, "vOnLevel");
NexVariable vFullHeight = NexVariable(1, 29, "vFullHeight");
NexVariable vEmptyHeight = NexVariable(1, 30, "vEmptyHeight");
NexVariable vAutoOnState = NexVariable(1, 31, "vAutoOnStateB");
NexVariable vAutoOffState = NexVariable(1, 32, "vAutoOffStateB");
NexVariable vManualOnState = NexVariable(1, 33, "vManuOnStateB");
NexVariable vManualOffState = NexVariable(1, 34, "vManuOffStateB");


NexTouch *nex_listen_list[] = {
  &bSave,
  &bStartStop,
  &bSettings,
  NULL
};


// Function declarations
void bSave_pressed(void *ptr);
void saveToEEPROM(uint32_t value, int address);
uint32_t readFromEEPROM(int address);
void readEEPROM(void *ptr);
void bSettings_pressed(void *ptr);

void setup() {
  // Initialize Serial first
  Serial.begin(9600);
  delay(100);  // Allow time for Serial to initialize
  Serial.println("\nStarting");
  Serial.flush();  // Ensure all data is sent before proceeding
  
  // Initialize pins
  pinMode(led, OUTPUT);

  EEPROM.begin(16); // Initialize EEPROM with a size of 16 bytes
  
  // Initialize Nextion
  nexInit();
  delay(1000);  // Give more time for Nextion to initialize

  // Attach button callbacks
  bSave.attachPush(bSave_pressed, &bSave);
  bStartStop.attachPush(readEEPROM, &bStartStop);
  bSettings.attachPush(bSettings_pressed, &bSettings);

  //Read Stored values from EEPROM
  OffLevel = readFromEEPROM(EEPROM_OFF_LEVEL_ADDR);
  OnLevel = readFromEEPROM(EEPROM_ON_LEVEL_ADDR);
  FullHeight = readFromEEPROM(EEPROM_FULL_HEIGHT_ADDR);
  EmptyHeight = readFromEEPROM(EEPROM_EMPTY_HEIGHT_ADDR);


  //Print the loaded values
  Serial.print("Loaded OffLevel :"); Serial.println(OffLevel);
  Serial.print("Loaded OnLevel :"); Serial.println(OnLevel);
  Serial.print("Loaded FullHeight :"); Serial.println(FullHeight);
  Serial.print("Loaded EmptyHeight :"); Serial.println(EmptyHeight);
  
}

void loop() {
  
  // put your main code here, to run repeatedly:
  nexLoop(nex_listen_list);
  
}

// Function definitions
void bSave_pressed(void *ptr) {
  Serial.println("Save Button Pressed");

  // Retry Logic for getting values
  auto getValueWithRetry = [](NexVariable &variable, uint32_t &value, const char *name) {
    const int maxRetries = 3; // Maximum number of retries
    for (int attempt = 0; attempt < maxRetries; ++attempt) {
      if (variable.getValue(&value)) {
        Serial.print(name);
        Serial.print(" is : ");
        Serial.println(value);
        return true; // Successfully retrieved the value
      } else {
        Serial.print("Failed to get value from ");
        Serial.println(name);
        delay(500); // Wait before retrying
      }
    }
    return false; // Failed after max retries
  };

  // Attempt to retrieve each value with retries
  if (getValueWithRetry(vOffLevel, OffLevel, "OffLevel")) {
    saveToEEPROM(OffLevel, EEPROM_OFF_LEVEL_ADDR);
  }
  if (getValueWithRetry(vOnLevel, OnLevel, "OnLevel")) {
    saveToEEPROM(OnLevel, EEPROM_ON_LEVEL_ADDR);
  }
  if (getValueWithRetry(vFullHeight, FullHeight, "FullHeight")) {
    saveToEEPROM(FullHeight, EEPROM_FULL_HEIGHT_ADDR);
  }
  if (getValueWithRetry(vEmptyHeight, EmptyHeight, "EmptyHeight")) {
    saveToEEPROM(EmptyHeight, EEPROM_EMPTY_HEIGHT_ADDR);
  }
}

//Save value to EEPROM
void saveToEEPROM(uint32_t value, int address) {
  EEPROM.put(address, value); // Store the value in EEPROM at the specified address
  EEPROM.commit(); // Commit the changes to EEPROM
  Serial.print("Saved value ");
  Serial.print(value);
  Serial.print(" to EEPROM address ");
  Serial.println(address);
}

//Read value from EEPROM
uint32_t readFromEEPROM(int address) {
  uint32_t value;
  EEPROM.get(address, value); // Retrieve the value from EEPROM at the specified address
  return value; // Return the retrieved value
}

//read saved values from EEPROM ans print (For testing purposes)
void readEEPROM(void *ptr) {
  Serial.print("OffLevel: ");
  Serial.println(EEPROM.read(EEPROM_OFF_LEVEL_ADDR));
  Serial.print("OnLevel: ");
  Serial.println(EEPROM.read(EEPROM_ON_LEVEL_ADDR));
  Serial.print("FullHeight: ");
  Serial.println(EEPROM.read(EEPROM_FULL_HEIGHT_ADDR));
  Serial.print("EmptyHeight: ");
  Serial.println(EEPROM.read(EEPROM_EMPTY_HEIGHT_ADDR));
}


 void bSettings_pressed(void  *ptr){
  Serial.println("Settings Button Pressed");
  vOffLevel.setValue(OffLevel); // Set the value of OffLevel in the Nextion variable
  vOnLevel.setValue(OnLevel); // Set the value of OnLevel in the Nextion variable
  vFullHeight.setValue(FullHeight); // Set the value of FullHeight in the Nextion variable
  vEmptyHeight.setValue(EmptyHeight); // Set the value of EmptyHeight in the Nextion variable
  
  tOffLevel.setText(String(OffLevel).c_str()); // Set the text of the OffLevel text field
  tOnLevel.setText(String(OnLevel).c_str()); // Set the text of the OnLevel text field
  tFullHeight.setText(String(FullHeight).c_str()); // Set the text of the FullHeight text field
  tEmptyHeight.setText(String(EmptyHeight).c_str()); // Set the text of the EmptyHeight text field

 }
