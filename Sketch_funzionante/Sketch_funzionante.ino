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
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);

  maxthermo.begin();
  maxthermo.setThermocoupleType(MAX31856_TCTYPE_T);

  Serial.print("Thermocouple type: ");
  switch (maxthermo.getThermocoupleType() ) {
    case MAX31856_TCTYPE_B: Serial.println("B Type"); break;
    case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
    case MAX31856_TCTYPE_J: Serial.println("J Type"); break;
    case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
    case MAX31856_TCTYPE_N: Serial.println("N Type"); break;
    case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
    case MAX31856_TCTYPE_S: Serial.println("S Type"); break;
    case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
    case MAX31856_VMODE_G8: Serial.println("Voltage x12 Gain mode"); break;
    case MAX31856_VMODE_G32: Serial.println("Voltage x3 Gain mode"); break;
    default: Serial.println("Unknown"); break;
  }

}

void loop() {
  
  digitalWrite(read_pin, HIGH);
  
      for (int i = 0; i < 16; i++) {
       if (i==0||i==15){
        if (i==0){
          my_mux.channel(i);
          delay(100);
     if(T1>30){     
    digitalWrite(in1,LOW);
     }
     else {
      digitalWrite(in1,HIGH);
     }
     if(T2>30){     
    digitalWrite(in2,LOW);
     }
     else {
      digitalWrite(in2,HIGH);
     }
    

  delay(100);


          T1 = maxthermo.readThermocoupleTemperature();
          //Serial.print("  "); 
          Serial.print(T1);  
        }
        if (i==15){
          my_mux.channel(i);
          delay(200);
          T2 = maxthermo.readThermocoupleTemperature();
        Serial.print(" , "); 
        Serial.println(T2); 
        
        }
    
        }
      }


  // Check and print any faults
  uint8_t fault = maxthermo.readFault();

 
  if (fault) {
    if (fault & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault");
    if (fault & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
    if (fault & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
    if (fault & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
    if (fault & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
    if (fault & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
    if (fault & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
    if (fault & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
  }
  delay(200);   
}
