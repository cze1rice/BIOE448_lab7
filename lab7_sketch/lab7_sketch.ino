#include "thingProperties.h"


int sensor_pin = 0;
int pulse_signal = 0;
int pulse_period = 0;
int counter = 0;
//float BPM = 0;

bool ignore = false;

// initialize flags
bool any_peak_detected = false;
bool first_peak_detected = false;
unsigned long first_pulse_time = 0;
unsigned long second_pulse_time = 0;

int upper_threshold = 900;
int lower_threshold = 200;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  delay(1500);
  initProperties();

  //Connect to cloud and get info/errors
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  //Wait for cloud connection
  while (ArduinoCloud.connected() != 1) {
    ArduinoCloud.update();
    delay(500);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  pulse_signal = analogRead(sensor_pin);
  //Serial.println(pulse_signal);
  delay(100);
  //Serial.println("first peak ");
  //Serial.println(first_peak_detected);
  //Serial.println("any peak ");
  //Serial.println(any_peak_detected);
  if (pulse_signal > upper_threshold && any_peak_detected == false) {
    any_peak_detected = true;
    if (first_peak_detected == false) {
      first_pulse_time = millis();
      first_peak_detected = true;
    }
    else {
      second_pulse_time = millis();
      pulse_period = second_pulse_time - first_pulse_time;
      BPM = 60000/pulse_period;
      Serial.println(BPM);
      first_peak_detected = false;
    }
  }
  if (pulse_signal < lower_threshold) {
    any_peak_detected = false;
  }

  //sending to cloud
  counter++;
  if (counter > 50) {
    ArduinoCloud.update();
    Serial.println(BPM);
    counter = 0;
  }
}