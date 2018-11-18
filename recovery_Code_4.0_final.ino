 #include<Wire.h>
 #include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;
  
const int MPU=0x68;  // I2C address of the MPU-6050
float AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
const int numReadings = 5;
const int numRead = 2;

int readings[numReadings];
int readings1[numRead];     
int readIndex = 0;  
int readIndex1 = 0;  
int total = 0;
int total1 = 0;                  
float average = 0.00;
float average1 = 0.00;
             
int LED=5;

void setup(){ 
   if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }  
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set t/o zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(115200);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;}
    for (int thisReading1 = 0; thisReading1 < numRead; thisReading1++) {
    readings1[thisReading1] = 0;}
    
      Serial.println("Program started");
      

}
float velocity(float,float,float);
void loop()
{
   double y = bmp.readAltitude(); 
   //Serial.println(y);
     total1 = total1 - readings1[readIndex1];
  readings1[readIndex1] = y;
  total1 = total1 + readings1[readIndex1];
  readIndex1 = readIndex1 + 1;

  if (readIndex1 >= numRead) {
    readIndex1 = 0;
  average1 = total1 / numRead;
  double z = average1 - 170.00;
 // Serial.println(z);
    if(z>1)
    {
          Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);  
  AcX=Wire.read()<<8|Wire.read();  
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  float a = AcX/16384;
  float b = AcY/16384;
  float c = AcZ/16384;
  constrain(a,0,1000);
  constrain(b,0,1000);
  constrain(c,0,1000);
  //Serial.print("Accelerometer: ");
  //Serial.print("X = "); Serial.print(a);
  //Serial.print(" | Y = "); Serial.print(b);
  //Serial.print(" | Z = "); Serial.println(c);
  
  
 float x=velocity(AcX,AcY,AcZ);
 x = x-16;
 x = constrain(x,0,1000);
  total = total - readings[readIndex];
  readings[readIndex] = x;
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
 Serial.println(average);
  if(average==0)
 {
  digitalWrite(LED,HIGH);
  delay(1000);
  digitalWrite(LED,LOW);
  delay(1000);
 // Serial.println("ejection initiated");
 }
 delay(1);
  }
    
}
}
}
 
  
    
float velocity(float m,float n,float o)
{
float Vx,Vy,Vz,v;float t=0.001;
Vx=(m*t);
Vy=(n*t);
Vz=(o*t);
v+=sqrt((Vx*Vx)+(Vy*Vy)+(Vz*Vz));
return v;
}
