#include <Arduino.h>
#include <nextion.h>

#define led 2

bool ledState = false;  // Variable to track LED state

//Buttons
NexButton b0 = NexButton(0, 1, "b0");
NexButton b1 = NexButton(0, 4, "b1");

//Texts
NexText t0 = NexText(0, 2, "t0");

//Variables
NexVariable va0 = NexVariable(0, 3, "va0");


NexTouch *nex_listen_list[] = {
  &b0,
  &b1,
  NULL
};


// Function declarations
void b0_pressed(void *ptr);
void b0_relesed(void *ptr);
void b1_pressed(void *ptr);

// Function definitions
void b0_pressed(void *ptr) {
  if (ledState) {
    digitalWrite(led, LOW);
    ledState = false;
    t0.setText("LED OFF");
  }else {
    digitalWrite(led, HIGH);
    ledState = true;
    t0.setText("LED ON");
  }
}

void b0_relesed(void *ptr) {
  //button relesed
}

void b1_pressed(void *ptr) {
  Serial.println("Button pressed");
  uint32_t value;
  if (va0.getValue(&value)) {
    Serial.print("Value is : ");
    Serial.println(value);
    t0.setText(String(value).c_str());  // Convert value to string
  } else {
    Serial.println("Failed to get value from va0");
  }
}

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
  b0.attachPush(b0_pressed, &b0);
  b0.attachPop(b0_relesed, &b0);
  b1.attachPush(b1_pressed, &b1);
  
}

void loop() {
  
  // put your main code here, to run repeatedly:
  nexLoop(nex_listen_list);
  
}
