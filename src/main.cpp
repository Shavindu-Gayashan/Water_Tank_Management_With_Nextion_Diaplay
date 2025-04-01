#include <Arduino.h>
#include <nextion.h>

#define led 2

//Buttons
NexButton b0 = NexButton(0, 1, "b0");


NexTouch *nex_listen_list[] = {
  &b0,
  NULL
};


// Function declarations
void b0_pressed(void *ptr);
void b0_relesed(void *ptr);

// Function definitions
void b0_pressed(void *ptr) {
  digitalWrite(led, HIGH);  // Turn on LED
}

void b0_relesed(void *ptr) {
  digitalWrite(led, LOW);  // Turn off LED
}
void setup() {
  // Initialize Serial first
  Serial.begin(115200);
  delay(100);  // Allow time for Serial to initialize
  Serial.println("\nStarting");
  Serial.flush();  // Ensure all data is sent before proceeding
  
  // Initialize pins
  pinMode(led, OUTPUT);
  
  // Initialize Nextion
  nexInit();
  delay(1000);  // Give more time for Nextion to initialize

  // Attach button callbacks
  b0.attachPush(b0_pressed, &b0);
  b0.attachPop(b0_relesed, &b0);
  
}

void loop() {
  
  // put your main code here, to run repeatedly:
  nexLoop(nex_listen_list);
  
}
