#include  <Adafruit_MAX31856.h>
#include <CD74HC4067.h>
CD74HC4067 my_mux(4, 5, 6, 7);
const int read_pin = (10, 11, 12, 13);
//int pinLed = 8;
//float a; 
int i;
float T1;
float T2;

int in1 = 3;
int in2 = 2;

int out1 = 8;
int out2 = 9;

float fs = 1;  // [Hz] sampling time (( < 1.2 ))
float Ts = 1/fs;  // [s] sampling frequency

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10, 11, 12, 13);
// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10);

void setup() {
  //pinMode(pinLed,OUTPUT);
  pinMode(read_pin, OUTPUT);
  Serial.begin(115200);
  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(out1, INPUT);
  pinMode(out2, INPUT);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);

  maxthermo.begin();
  maxthermo.setThermocoupleType(MAX31856_TCTYPE_T);

//  Serial.print("Thermocouple type: ");
//  switch (maxthermo.getThermocoupleType() ) {
//    case MAX31856_TCTYPE_B: Serial.println("B Type"); break;
//    case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
//    case MAX31856_TCTYPE_J: Serial.println("J Type"); break;
//    case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
//    case MAX31856_TCTYPE_N: Serial.println("N Type"); break;
//    case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
//    case MAX31856_TCTYPE_S: Serial.println("S Type"); break;
//    case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
//    case MAX31856_VMODE_G8: Serial.println("Voltage x12 Gain mode"); break;
//    case MAX31856_VMODE_G32: Serial.println("Voltage x3 Gain mode"); break;
//    default: Serial.println("Unknown"); break;
//  }

  digitalWrite(read_pin, HIGH);  // enable read pins (thermocouples)

}

void loop() {

  long counter_start = millis(); // get current time at each loop() iteration
 
  // SENSOR ACQUISITION
  my_mux.channel(0);
  float counter_DAQ = millis();
  T1 = maxthermo.readThermocoupleTemperature();  // first temperature reading 
  my_mux.channel(15);  // second temperature reading
  T2 = maxthermo.readThermocoupleTemperature();
  
  // Serial.println("counter_DAQ = " + String(millis()-counter_DAQ) + " ms"); 

  // create the string for serial communication 
  String serial_MSG = "[" + String(T1) + ", " + String(T2) + "]";

  // serial communication 
  Serial.println(serial_MSG);

  // ACTUATION
  if (Serial.available() > 0) 
  {
    int msg_in = Serial.read();
    Serial.println("MSG_RECEIVED: " + String(msg_in));  // for debugging ...   

    switch (msg_in) 
    {
      case 49: digitalWrite(out1, LOW);  break;
      case 50: digitalWrite(out1, HIGH); break;
      case 51: digitalWrite(out2, LOW);  break;
      case 52: digitalWrite(out2, HIGH); break;
    }
  }

  bool RT_check = (millis()-counter_start) >= Ts*1000;
  
  if (RT_check) {  
      Serial.println("RT execution error (data acquisition too slow for given sampling frequency)");
      Serial.println("(DAQ time: " + String(millis()-counter_start) + " ms)");
      };

  bool timeout = false; 
  while ( !timeout ){
    if ( (millis()-counter_start) >= Ts*1000 ) {  
      timeout = true;
      };
  };
 
}
