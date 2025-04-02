#include <Arduino.h>
#include <nextion.h>

#define led 2

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

//Texts
//NexText t0 = NexText(0, 2, "t0");

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
  NULL
};


// Function declarations
void bSave_pressed(void *ptr);

void setup() {
  // Initialize Serial first
  Serial.begin(9600);
  delay(100);  // Allow time for Serial to initialize
  Serial.println("\nStarting");
  Serial.flush();  // Ensure all data is sent before proceeding
  
  // Initialize pins
  pinMode(led, OUTPUT);
  
  // Initialize Nextion
  nexInit();
  delay(1000);  // Give more time for Nextion to initialize

  // Attach button callbacks
  bSave.attachPush(bSave_pressed, &bSave);
  
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
  getValueWithRetry(vOffLevel, OffLevel, "OffLevel");
  getValueWithRetry(vOnLevel, OnLevel, "OnLevel");
  getValueWithRetry(vFullHeight, FullHeight, "FullHeight");
  getValueWithRetry(vEmptyHeight, EmptyHeight, "EmptyHeight");
}
