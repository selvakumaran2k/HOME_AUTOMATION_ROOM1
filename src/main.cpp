  #include <Arduino.h>
//variables
//motion sensor          111111111
  int motion = 2;
//temperature sensor     222222222
  #include "DHT.h"
  #define DHTPIN 4
  #define DHTTYPE DHT11 
  DHT dht(DHTPIN, DHTTYPE);
  
//relay
  int r1=11;
  int r2=10;
  int r3=9;
  int r4=8;
  #define TUBE r3  //2-->pinNumbers
  #define FAN1 r1  //0
  #define FAN2 r2  //1
  #define CFL r4   //3
//ir rensor              333333333
  int ir=3;
//LDR sensor             444444444
  const int LDR = A0;
  //timing
  unsigned long motionStartingTime=0UL;
  unsigned long buf;
  
  void motionON();
  void motionOFF();

  //relay on function
  bool state[] = {false, false, false, false};
  //motion sensor calculation
  bool second_flag = false;
  bool movement = false;
  void on(int pin, int statevalue)
  {
    if (state[statevalue] == false)
    {
      if (millis() - buf >= 3000)
      {
        buf = millis();
        digitalWrite(pin, LOW);
        state[statevalue] = true;
        Serial.print(" turing on relay \n");
        Serial.print(" number : -->");
        Serial.print(pin);
        Serial.print("\n");

      }
    }
}
//relay off function
void off(int pin,int statevalue)
{if(state[statevalue]==true){
  if(millis()-buf>=3000){
  buf=millis();
  digitalWrite(pin,HIGH);
  state[statevalue] = false;
  Serial.print(" turing off relay ");
    Serial.print(" number : -->");
    Serial.print(pin);
    Serial.print("\n");
  }}
}
void p(float value,String s)
{
  Serial.print(s);
  Serial.print("  ");
  Serial.print(value);
  Serial.print("\n");
}
void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
//motion sensor
  pinMode(motion,INPUT);
//tempSensor
  dht.begin();
//relay setup
  pinMode(r1,OUTPUT);
  pinMode(r2,OUTPUT);
  pinMode(r3,OUTPUT);
  pinMode(r4,OUTPUT);
//relay initial setup
  digitalWrite(r1,HIGH);
  digitalWrite(r2,HIGH);
  digitalWrite(r3,HIGH);
  digitalWrite(r4,HIGH);
  delay(1000);
  //ir setup
  pinMode(ir,INPUT_PULLUP);
  //ldr setup
  pinMode(LDR, INPUT);
}

void loop() {
  float h = dht.readHumidity();//reading humidity
  float f = dht.readTemperature(true);//reading temperature
  float hif = dht.computeHeatIndex(f, h);//calculating heatindex
  p(hif, "heat index");//temperature sensor output checking
  int temp = hif > 87.0 ?( hif>93.0 ? 2 : 1) : 0;
  movement = digitalRead(motion);
  // p(movement, "MOVEMENT");
  if ((movement && (temp == 1)) && hif != NAN)
  {
//turning on the fan
    on(FAN1, 0);
}else if((movement && temp==2) && hif!=NAN)
{
    on(FAN2, 1);
    on(FAN1, 0);
}else{
  off(FAN1, 0);
  off(FAN2, 1);
}
//ir sensor turing on dressing table light
p(digitalRead(ir), "ir sensor value");
if(digitalRead(ir) && !second_flag)
{
  Serial.println("T Ir");
  on(TUBE,2);
}else if(!second_flag)
{
  off(TUBE, 2);
}
//turnig on night light
p(analogRead(LDR), "LDR value");
p(digitalRead(motion), "Motion Sensor Value");
int LDR_data = analogRead(LDR);
bool ldr_flag = !state[2] && !state[3];
if(LDR_data >= 130.0 && LDR_data <= 180.0 && movement)
{
  if(digitalRead(ir))
  {}
  else if(ldr_flag){
    second_flag = true;
    on(TUBE, 2);
    on(CFL, 3);
  }
}else if(LDR_data < 100.0 && movement)
{
  if(digitalRead(ir)){}else if(ldr_flag){
  off(CFL, 3);
  second_flag = true;
  on(TUBE, 2);}
}else if(ldr_flag){
  if(digitalRead(ir)){}else{
  off(CFL,3);
  second_flag = false;
  off(TUBE, 2);}
}
for (int i = 0; i < 4;i++)
{
  Serial.print(state[i]);
  Serial.print(" >> ");
}
  Serial.println("\n******************************");
  delay(1200);
}
