/*
 * PID USING DTH11 and ESP8266
 * ----------------------------
 * This a sketch to control a heater using PID with aggressive and conservative tunings.
 * 
*/



//needed for library
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <DHT.h>
#include <PID_v1.h>

int hu;
int tp;
int sp;
int mpd;
int op;

#define DHTPIN 4
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

#define RelayPin 5
float temp, humi;
  

//Define the aggressive and conservative Tuning Parameters
double aggKp = 4, aggKi = 10, aggKd = 2;
double consKp = 2, consKi = 5, consKd = 1;
//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);


void setup()
{


  Serial.begin(9600);
  pinMode(RelayPin, OUTPUT);

  dht.begin();

  //this is the temperature we want to reach
  Setpoint = 26;

  //tell the PID to range between 0 and 255
  myPID.SetOutputLimits(0, 255);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop()
{

  humi = dht.readHumidity();
  temp = dht.readTemperature();


  sp = (Setpoint * 100) / 100; //changes humi to a 2 digit intiger
  hu = (humi * 10) / 10; //changes humi to a 2 digit intiger
  tp = (temp * 10) / 10; //changes temp to a 2 digit integer
  Input = tp;

  double gap = abs(Setpoint - Input); //distance away from setpoint
  if (gap < 10)
  { //we're close to setpoint, use conservative tuning parameters
    myPID.SetTunings(consKp, consKi, consKd);
  }
  else
  {
    //we're far from setpoint, use aggressive tuning parameters
    myPID.SetTunings(aggKp, aggKi, aggKd);
  }

  myPID.Compute();



  //Serial.print("humidity:");
  //Serial.print(hu);
  //Serial.print("\t");
  Serial.print("temp:");
  Serial.print(tp);
  Serial.print("\t");
  Serial.print("relay pin:");
  Serial.print(Output);
  Serial.print("\n");

  //Serial.print("kp: "); Serial.print(myPID.GetKp()); Serial.print(" ");
  //Serial.print("ki: "); Serial.print(myPID.GetKi()); Serial.print(" ");
  //Serial.print("kd: "); Serial.print(myPID.GetKd()); Serial.println();



  delay(10);
}
