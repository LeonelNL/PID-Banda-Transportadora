
#include <Wire.h>
#include <Servo.h>


const int trigPin1 = 7;   //Pin digital 2 para el Trigger del sensor
const int echoPin1 = 6;   //Pin digital 3 para el Echo del sensor
const int Buffersize = 3;
char buf[Buffersize];
int recive;
int pos = 35;
int d; //distancia en centimetros
int stp;
Servo myservo;  // create servo object to control a servo, later attatched to D9

int Read = 0;
float distance = 0.0;
float elapsedTime, time, timePrev;        //Variables for time control
float distance_previous_error, distance_error;
int period = 50;  //Refresh rate period of the loop is 50ms

float kp=1.4; //Mine was 8
float ki=0.05; //Mine was 0.2
float kd=300.15; //Mine was 3100
float distance_setpoint;           //Should be the distance from sensor to the middle of the bar in mm
float PID_p, PID_i, PID_d, PID_total, D_prec;


void setup() {
  Serial.begin(9600);  
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(60); //Put the servco at angle 125, so the balance is in the middle

  pinMode(trigPin1, OUTPUT); //pin como salida
  pinMode(echoPin1, INPUT);  //pin como entrada
  digitalWrite(trigPin1, LOW);//Inicializamos el pin con 0
  time = millis();
}

void loop() {
  if (millis() > time+period)
  {
    time = millis();    
    distance = measure_1();
    //distance_setpoint = stpLV();
   if (Serial.available()) {

    String dat = Serial.readStringUntil('&');
    int data = dat.toInt();
    distance_setpoint = data-10;
   } 
    String flag = "%";
    String tolab;
    if (distance < 10) {
      tolab = flag + "0" + String(distance);
    } else {
      tolab = flag + String(distance)+ " " + "$" +  distance_setpoint;
    }
    Serial.println( tolab);  

     
    distance_error = distance_setpoint - distance;   
    PID_p = kp * distance_error;
    float dist_diference = distance_error - distance_previous_error;     
    PID_d = (distance_error - distance_previous_error)*(kd/period);
    PID_d = 0.56*PID_d + 0.44*D_prec; 
    PID_i = PID_i + (ki * distance_error);
    PID_total = PID_p + PID_i + PID_d;  
    PID_total = map(PID_total, -40, 40, 140, 60);
  
    if(PID_total < 60){PID_total = 60;}
    if(PID_total > 140) {PID_total = 140; } 
  
    myservo.write(PID_total); 
    //Serial.println(PID_total); 
    //Serial.println(distance_setpoint); 
    distance_previous_error = distance_error;
    D_prec = PID_d;
    
  }
}  

int measure_1 (void) {

long durata=0;
float distanza=0; 

digitalWrite(trigPin1, LOW); 
delayMicroseconds(10); 

digitalWrite(trigPin1, HIGH);
delayMicroseconds(10);
 
digitalWrite(trigPin1, LOW);

durata = pulseIn(echoPin1, HIGH);
distanza = ((float)durata/58.2)+0.;

delay(10);

if (distanza > 32) distanza=33;
else if (distanza < 2) distanza=1;

//return 0.01*(distanza-1.5+0.5);   // meters   
return distanza;

}

int stpLV(void){

  if (Serial.available()) {

   char datos_recibidos = Serial.read();
   String grado;
   grado += datos_recibidos;
   if (datos_recibidos == '\n'){
    int stp = grado.toInt();
    grado == "";
   }
  }
return stp;
}
