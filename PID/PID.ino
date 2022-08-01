int IN1 = 4;        ////IN1 = 1, ENA = 1, IN2 = 0 Motor Izquierda
int IN2 = 2;        //IN1 = 0, ENA = 1, IN2 = 1 Motor Izquierda
int ENA = 3;        //ENA = 0 Motro apagado
float SIR;
int Distance;
int DistanceSt = 0;
int adc_raw = 0;
String SetPoint;
int SetPointINT;
int Velocity = 250;
#define alpha 0.05

void setup() 
{
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
}

void loop() 
{
  adc_raw = analogRead(A0);
  SIR = (alpha * adc_raw) + ((1-alpha) * SIR);
  Distance = pow(10, log10(SIR/1821.2)/-0.65);  
  
  if(Distance != 0)
  {
    DistanceSt = Distance;
  }
  
  Serial.println(DistanceSt);
  
  if(Serial.available())
  {
     SetPoint = Serial.parseInt();
     SetPointINT = SetPoint.toInt();
          
     if(SetPointINT < 4)
     {
        Stop();
     }
     else
     {
        Movimientos();
     }
  }
    delay(10);
}
