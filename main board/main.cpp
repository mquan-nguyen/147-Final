#include <Arduino.h>
#include "DHT20.h"
#include <DHT.h>
#include <DHT_U.h>
#include "SparkFunLSM6DSO.h"
#include <WiFi.h>
#include <HttpClient.h>

// This example downloads the URL "http://arduino.cc/"

char ssid[] = "UCInet Mobile Access";    // your network SSID (name) 
char pass[] = ""; // your network password (use for WPA, or use as key for WEP)

// Name of the server we want to connect to
const char kHostname[] =  "54.183.249.22";
int port = 5000;
// Path to download (this is the bit after the hostname in the URL
// that you want to download

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

LSM6DSO myIMU; // gyro / accel obj

DHT20 DHT(&Wire); //temp / humidity obj
float measuredTemp;
float measuredHumid;

float rme(float x, float y, float z){
  return sqrtf((x*x + y*y + z*z));
}

bool dhtStatus(){
  Serial.print("DHT20, \t");
  int status = DHT.read();
  switch (status)
  {
  case DHT20_OK:
    Serial.print("OK,\t");
    return true;
  case DHT20_ERROR_CHECKSUM:
    Serial.print("Checksum error,\t");
    return false;
  case DHT20_ERROR_CONNECT:
    Serial.print("Connect error,\t");
    return false;
  case DHT20_MISSING_BYTES:
    Serial.print("Missing bytes,\t");
    return false;
  default:
    Serial.print("Unknown error,\t");
    return false;
  }
  return true;
}

bool getHumidTemp(){
  measuredHumid = DHT.getHumidity();
  measuredTemp = DHT.getTemperature();
  
  Serial.print(measuredHumid, 1);
  Serial.print(",\t");
  Serial.println(measuredTemp, 1);

  if (measuredHumid > 60 || 40 > measuredHumid){
    return false;
  }
  if (measuredTemp > 30 || 20 > measuredTemp){
    return false;
  }

  return true;
}

float x_accel_offset = 0.010;
float y_accel_offset = 0.090;

float x_gyro_offset = 0.5;
float y_gyro_offset = 0.7;
float z_gyro_offset = -0.25;


float x_accel_read = 0;
float y_accel_read = 0;
float z_accel_read = 0;

float x_gyro_read = 0;
float y_gyro_read = 0;
float z_gyro_read = 0;

bool getAccelGyro(){
  x_accel_read = myIMU.readFloatAccelX() - x_accel_offset;
  y_accel_read = myIMU.readFloatAccelY() - y_accel_offset;
  z_accel_read = myIMU.readFloatAccelZ(); //No offset need to add offset

  Serial.print("\nAccelerometer:\n");
  Serial.print(" X = ");
  Serial.println(x_accel_read, 3);
  Serial.print(" Y = ");
  Serial.println(y_accel_read, 3);
  Serial.print(" Z = ");
  Serial.println(z_accel_read, 3);

  x_gyro_read = myIMU.readFloatGyroX() - x_gyro_offset;
  y_gyro_read = myIMU.readFloatGyroY() - y_gyro_offset;
  z_gyro_read = myIMU.readFloatGyroZ() - z_gyro_offset;

  //sub offsets from full read to zero
  Serial.print("\nGyroscope:\n");
  //0.5 max x-offset
  Serial.print(" X = ");
  Serial.println(x_gyro_read, 3);
  //0.7 max y-offset
  Serial.print(" Y = ");
  Serial.println(y_gyro_read, 3);
  //-0.25 max z-offset
  Serial.print(" Z = ");
  Serial.println(z_gyro_read, 3);

  double acc = rme(x_accel_read, y_accel_read, z_accel_read); 
  Serial.print("Acceleration: ");
  Serial.println(acc); 

  double gy = rme(x_gyro_read, y_gyro_read, z_gyro_read);
  Serial.print("Gyro: ");
  Serial.println(gy);
  if(acc > 1.05 && gy > 10) //Acceleration magnitude more then (+1 G) A FALL DETECTED BASED ON ACCEL
  {
    return false;
  }

  return true;
}

WiFiClient wifi;
HttpClient http = HttpClient(wifi, kHostname, port);

void sendToCloud(String data_type, String msg){

  String contentType = "application/json";

  if (data_type.compareTo("weather") == 0) {

    http.post("/data/weather", contentType, msg);
  } else if (data_type.compareTo("gyro") == 0) {
    http.post("/data/gyro", contentType, msg );
  } else {
    return;
  }

  int statusCode = http.responseStatusCode();
  String response = http.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
    
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // We start by connecting to a WiFi network
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());

  Wire.begin(21,22);
  // DHT20 - temp / humidity setup
  DHT.begin();

  // gyro / accel setup
  if (myIMU.begin()) {
    Serial.println("Gyro / accel setup correctly.");
  } else {
    Serial.println("Could not connect to IMU");
  }
  if (myIMU.initialize(BASIC_SETTINGS)) {
    Serial.println("Loaded gyro settings");
  }
}



void loop() {
// put your main code here, to run repeatedly:

  //Temp and Humid Sensor
  
  char buffer[1000];

  if (millis() - DHT.lastRead() >= 1000)
  {
    uint32_t start = micros();
    uint32_t stop = micros();
    if(!dhtStatus()){
      return;
    }


    if(!getHumidTemp()){
      //Sends notification
      sprintf(buffer, "{\"temp\":\"%f\",\"humidity\":\"%f\",\"send_notification\": true}", measuredTemp, measuredHumid);
      sendToCloud("weather", String(buffer));
    }
    else{
      sprintf(buffer, "{\"temp\":\"%f\",\"humidity\":\"%f\",\"send_notification\":false}", measuredTemp, measuredHumid);
      sendToCloud("weather", String(buffer));      
    }

    Serial.print(stop - start); //Confirm read times
    Serial.print("\n");
  }

  //Gyroscope and Accel Sensor
  
  if(!getAccelGyro()){
    //Send notification
    sprintf(buffer, "{\"gyroX\":\"%f\",\"gyroY\":\"%f\",\"gyroZ\":\"%f\",\"accelX\":\"%f\",\"accelY\":\"%f\",\"accelZ\":\"%f\",\"gyroRMS\":\"%f\",\"accelRMS\":\"%f\",\"send_notification\":true}", x_gyro_read, y_gyro_read, z_gyro_read, x_accel_read, y_accel_read, z_accel_read, rme(x_gyro_read,y_gyro_read,z_gyro_read), rme(x_accel_read,y_accel_read,z_accel_read));
    sendToCloud("gyro", String(buffer));
  }
  else{
    sprintf(buffer, "{\"gyroX\":\"%f\",\"gyroY\":\"%f\",\"gyroZ\":\"%f\",\"accelX\":\"%f\",\"accelY\":\"%f\",\"accelZ\":\"%f\",\"gyroRMS\":\"%f\",\"accelRMS\":\"%f\",\"send_notification\": false}", x_gyro_read, y_gyro_read, z_gyro_read, x_accel_read, y_accel_read, z_accel_read, rme(x_gyro_read,y_gyro_read,z_gyro_read), rme(x_accel_read,y_accel_read,z_accel_read));
    sendToCloud("gyro", String(buffer));
  }
  //Camera Sensor

}

