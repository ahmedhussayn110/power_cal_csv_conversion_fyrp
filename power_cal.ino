#include <Filters.h>
#include <DHT.h>;
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino



float testFrequency = 50;                     // test signal frequency (Hz)
float windowLength = 20.0/testFrequency;     // how long to average the signal, for statistist
int sensorValue = 0;
float intercept = -0.1129; // to be adjusted based on calibration testing
float slope = 0.0405; // to be adjusted based on calibration testing
float current_amps=0; // estimated actual current in amps
float voltage_v=240;
double power_kwh=0;
float power_other_W= 24.3; // fan= 24W + sensors with controllers= 0.3W for 15 pin in use + 
float power_of_heater_W=0;
float power_of_system=0;
unsigned long printPeriod = 1000; // in milliseconds
// Track time in milliseconds since last reading 
unsigned long previousMillis = 0;

// Colour sensor pins
const int s0 = 8;  
const int s1 = 9;  
const int s2 = 12;  
const int s3 = 11;  
const int out1 = 10;   
// LED pins connected to Arduino
int redLed = 7; 
int greenLed = 3;  
int blueLed = 4;
// Variables  
int red = 0;  
int green = 0;  
int blue = 0;  





void color()  
{    
  digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);  
  //count OUT, pRed, RED  
  red = pulseIn(out1, digitalRead(out1) == HIGH ? LOW : HIGH);  
  digitalWrite(s3, HIGH);  
  //count OUT, pBLUE, BLUE  
  blue = pulseIn(out1, digitalRead(out1) == HIGH ? LOW : HIGH);  
  digitalWrite(s2, HIGH);  
  //count OUT, pGreen, GREEN  
  green = pulseIn(out1, digitalRead(out1) == HIGH ? LOW : HIGH);  
}





void setup() {
  Serial.begin( 9600 );    // start the serial port
  pinMode(s0, OUTPUT);  
  pinMode(s1, OUTPUT);  
  pinMode(s2, OUTPUT);  
  pinMode(s3, OUTPUT);  
  pinMode(out1, INPUT);  
  pinMode(redLed, OUTPUT);  
  //pinMode(greenLed, OUTPUT);  
  pinMode(blueLed, OUTPUT);  
  digitalWrite(s0, HIGH);  
  digitalWrite(s1, HIGH); 
Serial.println("CLEARDATA"); //clears up any data left from previous projects
//Inserting 'LABEL' So Excel knows the next things will be the names of the columns 
Serial.println("LABEL,Prsent Time,Duration,Humididty,Temperature,Irms_A,Vrms_V,Power_W,Total_Power_W,..."); 
Serial.println("RESETTIMER"); //resets timer to 0 for excel

}


void loop() {
  RunningStatistics inputStats;                 // create statistics to look at the raw test signal
  inputStats.setWindowSecs( windowLength );
   
  while( true ) {   
    sensorValue = analogRead(A1);  // read the analog in value:
    inputStats.input(sensorValue);  // log to Stats function
        
    if((unsigned long)(millis() - previousMillis) >= printPeriod) {
      previousMillis = millis();   // update time
      

      current_amps = intercept + slope * inputStats.sigma();

     if(current_amps <= 0.08)
     current_amps =0;
      power_of_heater_W = current_amps * voltage_v;
      power_of_system = power_other_W + power_of_heater_W;
      power_kwh = (power_of_system)  + power_kwh;  // total power consumed
   //   color();
    

   float hum = dht.readHumidity();
   float temp = dht.readTemperature();

  
   Serial.print("DATA,TIME,TIMER,");
   Serial.print(hum);
   Serial.print(",");
   Serial.print(temp);
   Serial.print(",");
   Serial.print(current_amps);
   Serial.print(",");
   Serial.print(voltage_v);
   Serial.print(",");
   Serial.print(power_of_system);
   Serial.print(",");
   Serial.println(power_kwh);
   Serial.println("...");
   
    }
  
  }
}



