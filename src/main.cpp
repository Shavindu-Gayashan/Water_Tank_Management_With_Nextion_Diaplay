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
  if (vOffLevel.getValue(&OffLevel)) {
    Serial.print("OffLevel is : ");
    Serial.println(OffLevel);
  } else {
    Serial.println("Failed to get value from vOffLevel");
  }
  
  if (vOnLevel.getValue(&OnLevel)) {
    Serial.print("OnLevel is : ");
    Serial.println(OnLevel);
  } else {
    Serial.println("Failed to get value from vOnLevel");
  }
  
  if (vFullHeight.getValue(&FullHeight)) {
    Serial.print("FullHeight is : ");
    Serial.println(FullHeight);
  } else {
    Serial.println("Failed to get value from vFullHeight");
  }

  if (vEmptyHeight.getValue(&EmptyHeight)) {
    Serial.print("EmptyHeight is : ");
    Serial.println(EmptyHeight);
  } else {
    Serial.println("Failed to get value from vEmptyHeight");
  }

  // if (vAutoOnState.getValue(&AutoOnState)) {
  //   Serial.print("AutoOnState is : ");
  //   Serial.println(AutoOnState);
  // } else {
  //   Serial.println("Failed to get value from vAutoOnStateB");
  // }

  // if (vAutoOffState.getValue(&AutoOffState)) {
  //   Serial.print("AutoOffState is : ");
  //   Serial.println(AutoOffState);
  // } else {
  //   Serial.println("Failed to get value from vAutoOffStateB");
  // }

  // if (vManualOnState.getValue(&ManualOnState)) {
  //   Serial.print("ManualOnState is : ");
  //   Serial.println(ManualOnState);
  // } else {
  //   Serial.println("Failed to get value from vManuOnStateB");
  // }

  // if (vManualOffState.getValue(&ManualOffState)) {
  //   Serial.print("ManualOffState is : ");
  //   Serial.println(ManualOffState);
  // } else {
  //   Serial.println("Failed to get value from vManuOffStateB");
  // }
}
