#include "DHT.h"

//Common
#define PIN_DHT 5
#define DHT_TYPE DHT22
#define DEBUG_MODE 0 //1 = enabled, 0 = disabled
#define LOOP_DELAY DEBUG_MODE ? 1000 : 900000 //900000 15m or 1s on debug
#define FIRST_LOOP_DELAY 5000 //5s 
#define SERIAL_BOUND_RATE 9600
#define LIGHT_SENSOR_PIN A0
#define HUMIDITY_SENSOR_PIN A1
#define MAX_MEASUREMENTS_PER_LOOP 10

//Response states
#define RESPONSE_STATE_OK  0x0
#define RESPONSE_STATE_ERROR  0x1

#define CALIBRATION_BYTE 0x9

//Globar attributes
DHT dhtManager(PIN_DHT, DHT_TYPE);
float temperatureValue, humidityValue, lightValue;
byte temperatureStatus,humidityStatus,lightStatus;
bool firstLoop;

void writeReadable(float* temperature, float* humidity, float* light){
    Serial.print("Temperature: "); Serial.print(*temperature); 
    Serial.print(", Humidity: "); Serial.print(*humidity);
    Serial.print(", Light: "); Serial.print(*light);
    Serial.print("\n");
}

void writeAsBytes(byte* temperatureStatus, float* temperature, byte* humidityStatus, float* humidity, byte* lightStatus, float* light){
    Serial.write(CALIBRATION_BYTE); Serial.write(CALIBRATION_BYTE); Serial.write(CALIBRATION_BYTE);
    Serial.write(*temperatureStatus); Serial.write((byte *) temperature, 4);
    Serial.write(*humidityStatus); Serial.write((byte *) humidity, 4);
    Serial.write(*lightStatus); Serial.write((byte *) light, 4);
    Serial.flush();
}

void write(byte* temperatureStatus, float* temperature, byte* humidityStatus, float* humidity, byte* lightStatus, float* light){
    if(DEBUG_MODE == 1){
      writeReadable(temperature, humidity, light);
    }else{
      writeAsBytes(temperatureStatus, temperature,humidityStatus, humidity, lightStatus, light);
    }
}

void writeInitBytes(){
  for(int i=0; i<5; i++){
     Serial.write(0x0); 
  }  
  Serial.flush();
}

byte readTemperature(float * temperature){
  for(int i=0; i< MAX_MEASUREMENTS_PER_LOOP; i++){
    (* temperature) = dhtManager.readTemperature();
    
    if(!isnan( (* temperature))) {
      return RESPONSE_STATE_OK;
    }
  }

  (* temperature) = 0;  
  return RESPONSE_STATE_ERROR;
}

byte readHumidity(float * humidity){
  float reading;
      for(int i=0; i< MAX_MEASUREMENTS_PER_LOOP; i++){
        reading = analogRead(HUMIDITY_SENSOR_PIN); //Read humidity analog value
        
        if(!isnan( reading)) {
            (* humidity) = 1.0 - (reading / 1023.0); //Get percent of maximum value (1023)
            return RESPONSE_STATE_OK;
        }
      }
      
    (* humidity) = 0;
    return RESPONSE_STATE_ERROR;
}

byte readLight(float * light){
  float reading;
      for(int i=0; i< MAX_MEASUREMENTS_PER_LOOP; i++){
        reading = analogRead(LIGHT_SENSOR_PIN); //Read light level
        
        if(!isnan( reading)) {
            (* light) = reading; //Get percent of maximum value (1023)
            return RESPONSE_STATE_OK;
        }
      }
      
    (* light) = 0;
    return RESPONSE_STATE_ERROR;
}

//#### Setup
void setup() {
   firstLoop = true;
   //Setup pins
   pinMode(A0, INPUT); //light sensor
   pinMode(A1, INPUT); //humidity sensor
   
  //Serial config
  Serial.begin(SERIAL_BOUND_RATE); 
  //DHT init & start
  dhtManager.begin();
}

//#### Loop
void loop() {

  if(firstLoop){
      delay(FIRST_LOOP_DELAY);
      firstLoop = false;
      writeInitBytes();
   }
  
  //read temperature
  temperatureStatus = readTemperature(&temperatureValue);
  humidityStatus = readHumidity(&humidityValue);
  lightStatus = readLight(&lightValue);
  write(&temperatureStatus, &temperatureValue,&humidityStatus,&humidityValue,&lightStatus,&lightValue);
  delay(LOOP_DELAY);
}
