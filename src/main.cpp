#include <Arduino.h>
#include <nextion.h>
#include <EEPROM.h>

#define led 2

#define EEPROM_OFF_LEVEL_ADDR 0
#define EEPROM_ON_LEVEL_ADDR 4
#define EEPROM_FULL_HEIGHT_ADDR 8
#define EEPROM_EMPTY_HEIGHT_ADDR 12
#define EEPROM_AUTO_ON_STATE_ADDR 16
#define EEPROM_AUTO_OFF_STATE_ADDR 20
#define EEPROM_MANUAL_ON_STATE_ADDR 24
#define EEPROM_MANUAL_OFF_STATE_ADDR 28

#define TRIGGER_PIN 25 
#define ECHO_PIN 26      
#define SoundSpeed 0.034 // Speed of sound in cm/us
#define NUM_SAMPLES 5    // Number of samples to average
#define READING_INTERVAL 1000  // Read water level every 1000ms (1 second)

constexpr float MIN_VALID_DISTANCE = 2.0;    // cm
constexpr float MAX_VALID_DISTANCE = 400.0;  // cm
constexpr int MIN_LEVEL = 0;
constexpr int MAX_LEVEL = 100;

int currentPage = 0;
int previousWaterLevel; // Variable to store the previous water level
int waterLevel;       // Variable to store the current water level

uint32_t OffLevel;    // Variable to store the value of OffLevel
uint32_t OnLevel;     // Variable to store the value of OnLevel
uint32_t FullHeight;  // Variable to store the value of FullHeight
uint32_t EmptyHeight; // Variable to store the value of EmptyHeight

uint32_t AutoOnState;    // Variable to store the value of AutoOnStateB
uint32_t AutoOffState;   // Variable to store the value of AutoOffStateB
uint32_t ManualOnState;  // Variable to store the value of ManualOnStateB
uint32_t ManualOffState; // Variable to store the value of ManualOffStateB

bool mode = false; // Variable to track the mode (true for manual, false for automatic)

bool ledState = false; // Variable to track LED state

unsigned long lastReadTime = 0;  // To store the last reading time

// Buttons
NexButton bSave = NexButton(1, 5, "bSave");
NexButton bStartStop = NexButton(0, 7, "bStartStop");
NexButton bSettings = NexButton(0, 12, "bSettings");
NexButton bBack = NexButton(1, 4, "bBack");
NexButton bAutoOnSettings = NexButton(1, 2, "bAuto");
NexButton bAutoOffSettings = NexButton(1, 3, "bManual");


// Texts
NexText tOffLevel = NexText(1, 15, "tOffLevel");
NexText tOnLevel = NexText(1, 17, "tOnLevel");
NexText tFullHeight = NexText(1, 20, "tFullHeight");
NexText tEmptyHeight = NexText(1, 23, "tEmptyHeight");
NexText tWaterLevel = NexText(0, 13, "tWaterLevel");

// Variables
NexVariable vOffLevel = NexVariable(1, 27, "vOffLevel");
NexVariable vOnLevel = NexVariable(1, 28, "vOnLevel");
NexVariable vFullHeight = NexVariable(1, 29, "vFullHeight");
NexVariable vEmptyHeight = NexVariable(1, 30, "vEmptyHeight");

NexVariable vAutoOnState = NexVariable(1, 31, "vAutoOnStateB");
NexVariable vAutoOffState = NexVariable(1, 32, "vAutoOffStateB");
NexVariable vManualOnState = NexVariable(1, 33, "vManuOnStateB");
NexVariable vManualOffState = NexVariable(1, 34, "vManuOffStateB");

// DualStateButtons
NexDSButton btAutoOn = NexDSButton(1, 25, "btAutoOn");
NexDSButton btAutoOff = NexDSButton(1, 26, "btAutoOff");

//pictures
NexPicture pError = NexPicture(0, 14, "pError");

NexTouch *nex_listen_list[] = {
    &bSave,
    &bStartStop,
    &bSettings,
    &bBack,
    &bAutoOnSettings,
    &bAutoOffSettings,
    NULL};

// Function declarations
void bSave_pressed(void *ptr);
void saveToEEPROM(uint32_t value, int address);
uint32_t readFromEEPROM(int address);
void readEEPROM(void *ptr);
void bSettings_pressed(void *ptr);
void bBack_pressed(void *ptr);
float getDistence();
int calcuateWaterLevel(float distance);
int getWaterLevel();

void setup()
{
  // Initialize Serial first
  Serial.begin(9600);
  delay(100); // Allow time for Serial to initialize
  Serial.println("\nStarting");
  Serial.flush(); // Ensure all data is sent before proceeding

  // Initialize pins
  pinMode(led, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT); // Set the trigger pin as output
  pinMode(ECHO_PIN, INPUT);     // Set the echo pin as input

  EEPROM.begin(32); // Initialize EEPROM with a size of 16 bytes

  // Initialize Nextion
  nexInit();
  delay(1000); // Give more time for Nextion to initialize

  // Attach button callbacks
  bSave.attachPush(bSave_pressed, &bSave);
  bStartStop.attachPush(readEEPROM, &bStartStop);
  bSettings.attachPush(bSettings_pressed, &bSettings);
  bBack.attachPush(bBack_pressed, &bBack);

  // Read Stored values from EEPROM
  OffLevel = readFromEEPROM(EEPROM_OFF_LEVEL_ADDR);
  OnLevel = readFromEEPROM(EEPROM_ON_LEVEL_ADDR);
  FullHeight = readFromEEPROM(EEPROM_FULL_HEIGHT_ADDR);
  EmptyHeight = readFromEEPROM(EEPROM_EMPTY_HEIGHT_ADDR);
  AutoOnState = readFromEEPROM(EEPROM_AUTO_ON_STATE_ADDR);
  AutoOffState = readFromEEPROM(EEPROM_AUTO_OFF_STATE_ADDR);
  ManualOnState = readFromEEPROM(EEPROM_MANUAL_ON_STATE_ADDR);
  ManualOffState = readFromEEPROM(EEPROM_MANUAL_OFF_STATE_ADDR);

  // Print the loaded values
  Serial.print("Loaded OffLevel :");
  Serial.println(OffLevel);
  Serial.print("Loaded OnLevel :");
  Serial.println(OnLevel);
  Serial.print("Loaded FullHeight :");
  Serial.println(FullHeight);
  Serial.print("Loaded EmptyHeight :");
  Serial.println(EmptyHeight);
  Serial.print("Loaded AutoOnState :");
  Serial.println(AutoOnState);
  Serial.print("Loaded AutoOffState :");
  Serial.println(AutoOffState);
  Serial.print("Loaded ManualOnState :");
  Serial.println(ManualOnState);
  Serial.print("Loaded ManualOffState :");
  Serial.println(ManualOffState);
}

void loop()
{
  // put your main code here, to run repeatedly:
  nexLoop(nex_listen_list);
  
  unsigned long currentTime = millis();
  
  if (currentPage == 0 && (currentTime - lastReadTime >= READING_INTERVAL))
  {
    waterLevel = getWaterLevel(); // Get the distance from the ultrasonic sensor
    Serial.print("Water Level : ");
    Serial.println(waterLevel); // Print the distance for debugging
    lastReadTime = currentTime;  // Update the last read time
  }
}

// Function definitions
void bSave_pressed(void *ptr)
{
  Serial.println("Save Button Pressed");

  // Retry Logic for getting values
  auto getVeriableValueWithRetry = [](NexVariable &variable, uint32_t &value, const char *name)
  {
    const int maxRetries = 3; // Maximum number of retries
    for (int attempt = 0; attempt < maxRetries; ++attempt)
    {
      if (variable.getValue(&value))
      {
        Serial.print(name);
        Serial.print(" is : ");
        Serial.println(value);
        return true; // Successfully retrieved the value
      }
      else
      {
        Serial.print("Failed to get value from ");
        Serial.println(name);
        delay(500); // Wait before retrying
      }
    }
    return false; // Failed after max retries
  };

  // Retry Logic for getting values from DualStateButtons
  auto getDualStateButtonValueWithRetry = [](NexDSButton &button, uint32_t &value, const char *name)
  {
    const int maxRetries = 3; // Maximum number of retries
    for (int attempt = 0; attempt < maxRetries; ++attempt)
    {
      if (button.getValue(&value))
      {
        Serial.print(name);
        Serial.print(" is : ");
        Serial.println(value);
        return true; // Successfully retrieved the value
      }
      else
      {
        Serial.print("Failed to get value from ");
        Serial.println(name);
        delay(500); // Wait before retrying
      }
    }
    return false; // Failed after max retries
  };

  // Attempt to retrieve each value with retries
  if (getVeriableValueWithRetry(vOffLevel, OffLevel, "OffLevel"))
  {
    saveToEEPROM(OffLevel, EEPROM_OFF_LEVEL_ADDR);
  }
  if (getVeriableValueWithRetry(vOnLevel, OnLevel, "OnLevel"))
  {
    saveToEEPROM(OnLevel, EEPROM_ON_LEVEL_ADDR);
  }
  if (getVeriableValueWithRetry(vFullHeight, FullHeight, "FullHeight"))
  {
    saveToEEPROM(FullHeight, EEPROM_FULL_HEIGHT_ADDR);
  }
  if (getVeriableValueWithRetry(vEmptyHeight, EmptyHeight, "EmptyHeight"))
  {
    saveToEEPROM(EmptyHeight, EEPROM_EMPTY_HEIGHT_ADDR);
  }
  // Get the button color
  uint32_t buttonColor;
  Serial.println("Getting button color...");
  if (bAutoOnSettings.Get_font_color_pco(&buttonColor))
  {
    Serial.print("Button font color is: ");
    Serial.println(buttonColor); // Print the button color for debugging
    if (buttonColor == 24521)
    { // Green color
      if (getDualStateButtonValueWithRetry(btAutoOn, AutoOnState, "AutoOnState"))
      {
        saveToEEPROM(AutoOnState, EEPROM_AUTO_ON_STATE_ADDR);
      }
      if (getDualStateButtonValueWithRetry(btAutoOff, AutoOffState, "AutoOffState"))
      {
        saveToEEPROM(AutoOffState, EEPROM_AUTO_OFF_STATE_ADDR);
      }
    }
    else if (buttonColor == 65535)
    { // White color
      if (getDualStateButtonValueWithRetry(btAutoOn, ManualOnState, "ManualOnState"))
      {
        saveToEEPROM(ManualOnState, EEPROM_MANUAL_ON_STATE_ADDR);
      }
      if (getDualStateButtonValueWithRetry(btAutoOff, ManualOffState, "ManualOffState"))
      {
        saveToEEPROM(ManualOffState, EEPROM_MANUAL_OFF_STATE_ADDR);
      }
    }
  }
}

// Save value to EEPROM
void saveToEEPROM(uint32_t value, int address)
{
  EEPROM.put(address, value); // Store the value in EEPROM at the specified address
  EEPROM.commit();            // Commit the changes to EEPROM
  Serial.print("Saved value ");
  Serial.print(value);
  Serial.print(" to EEPROM address ");
  Serial.println(address);
}

// Read value from EEPROM
uint32_t readFromEEPROM(int address)
{
  uint32_t value;
  EEPROM.get(address, value); // Retrieve the value from EEPROM at the specified address
  return value;               // Return the retrieved value
}

// read saved values from EEPROM ans print (For testing purposes)
void readEEPROM(void *ptr)
{
  Serial.print("OffLevel: ");
  Serial.println(EEPROM.read(EEPROM_OFF_LEVEL_ADDR));
  Serial.print("OnLevel: ");
  Serial.println(EEPROM.read(EEPROM_ON_LEVEL_ADDR));
  Serial.print("FullHeight: ");
  Serial.println(EEPROM.read(EEPROM_FULL_HEIGHT_ADDR));
  Serial.print("EmptyHeight: ");
  Serial.println(EEPROM.read(EEPROM_EMPTY_HEIGHT_ADDR));

  Serial.print("AutoOnState: ");
  Serial.println(EEPROM.read(EEPROM_AUTO_ON_STATE_ADDR));
  Serial.print("AutoOffState: ");
  Serial.println(EEPROM.read(EEPROM_AUTO_OFF_STATE_ADDR));
  Serial.print("ManualOnState: ");
  Serial.println(EEPROM.read(EEPROM_MANUAL_ON_STATE_ADDR));
  Serial.print("ManualOffState: ");
  Serial.println(EEPROM.read(EEPROM_MANUAL_OFF_STATE_ADDR));
  Serial.println("EEPROM values read successfully.");
}

void bSettings_pressed(void *ptr)
{
  Serial.println("Settings Button Pressed");
  currentPage = 1;
  vOffLevel.setValue(OffLevel);       // Set the value of OffLevel in the Nextion variable
  vOnLevel.setValue(OnLevel);         // Set the value of OnLevel in the Nextion variable
  vFullHeight.setValue(FullHeight);   // Set the value of FullHeight in the Nextion variable
  vEmptyHeight.setValue(EmptyHeight); // Set the value of EmptyHeight in the Nextion variable

  tOffLevel.setText(String(OffLevel).c_str());       // Set the text of the OffLevel text field
  tOnLevel.setText(String(OnLevel).c_str());         // Set the text of the OnLevel text field
  tFullHeight.setText(String(FullHeight).c_str());   // Set the text of the FullHeight text field
  tEmptyHeight.setText(String(EmptyHeight).c_str()); // Set the text of the EmptyHeight text field

  vAutoOffState.setValue(AutoOffState);     // Set the value of AutoOffState in the Nextion variable
  vAutoOnState.setValue(AutoOnState);       // Set the value of AutoOnState in the Nextion variable
  vManualOnState.setValue(ManualOnState);   // Set the value of ManualOnState in the Nextion variable
  vManualOffState.setValue(ManualOffState); // Set the value of ManualOffState in the Nextion variable

  // Noob code to set veriable values and button stattes. i will fix this Later

  String cmd = "vAutoOnState.val=" + String(AutoOnState); // Create the command string to set the value of AutoOnState in the Nextion variable
  sendCommand(cmd.c_str());                               // Send the command to the Nextion display
  cmd = "vAutoOffState.val=" + String(AutoOffState);      // Create the command string to set the value of AutoOffState in the Nextion variable
  sendCommand(cmd.c_str());                               // Send the command to the Nextion display
  cmd = "vManuOnState.val=" + String(ManualOnState);      // Create the command string to set the value of ManualOnState in the Nextion variable
  sendCommand(cmd.c_str());                               // Send the command to the Nextion display
  cmd = "vManuOffState.val=" + String(ManualOffState);    // Create the command string to set the value of ManualOffState in the Nextion variable
  sendCommand(cmd.c_str());                               // Send the command to the Nextion display
  cmd = "btAutoOn.val=" + String(AutoOnState);            // Create the command string to set the value of AutoOnState in the Nextion dual state button
  sendCommand(cmd.c_str());                               // Send the command to the Nextion display
  cmd = "btAutoOff.val=" + String(AutoOffState);          // Create the command string to set the value of AutoOffState in the Nextion dual state button
  sendCommand(cmd.c_str());                               // Send the command to the Nextion display
}

void bBack_pressed(void *ptr)
{
  Serial.println("Back Button Pressed");
  currentPage = 0;
}

float getDistence()
{
  float samples[NUM_SAMPLES];
  float totalDistance = 0;
  
  // Take multiple samples
  for(int i = 0; i < NUM_SAMPLES; i++) {
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    samples[i] = (duration * SoundSpeed) / 2;
    totalDistance += samples[i];
    delay(10); // Short delay between samples
  }
  
  // Return average distance
  return totalDistance / NUM_SAMPLES;
}

int calcuateWaterLevel(float distance) {
    // Validate input parameters
    if (distance < MIN_VALID_DISTANCE || distance > MAX_VALID_DISTANCE) {
        return -1; // Invalid parameters
    }

    // Calculate the water level percentage
    int level = static_cast<int>(
        ((EmptyHeight - distance) * MAX_LEVEL) / (EmptyHeight - FullHeight)
    );
    
    return constrain(level, MIN_LEVEL, MAX_LEVEL);
}

int getWaterLevel() {
    static int lastValidLevels[3] = {0, 0, 0};  // Rolling buffer for last 3 valid readings
    static int validReadingCount = 0;
    
    float distance = getDistence();
    
    // Validate distance reading
    if (distance <= MIN_VALID_DISTANCE || distance > MAX_VALID_DISTANCE) {
        pError.setPic(12);  // Show error picture
        tWaterLevel.setText("Error");
        return lastValidLevels[0];  // Return most recent valid reading
    }
    
    int currentLevel = calcuateWaterLevel(distance);
    if (currentLevel < 0) {  // Invalid calculation
        pError.setPic(12);
        tWaterLevel.setText("Error");
        return lastValidLevels[0];
    }
    
    // Shift previous readings and add new one
    for (int i = 2; i > 0; i--) {
        lastValidLevels[i] = lastValidLevels[i-1];
    }
    lastValidLevels[0] = currentLevel;
    
    if (validReadingCount < 3) validReadingCount++;
    
    // Calculate smoothed reading
    int smoothedLevel = 0;
    for (int i = 0; i < validReadingCount; i++) {
        smoothedLevel += lastValidLevels[i];
    }
    smoothedLevel /= validReadingCount;
    
    // Update display
    pError.setPic(15);  // Hide error picture
    char buffer[10];
    sprintf(buffer, "%d", smoothedLevel);
    tWaterLevel.setText(buffer);
    
    return smoothedLevel;
}

