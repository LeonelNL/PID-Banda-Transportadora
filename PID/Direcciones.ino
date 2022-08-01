void Left(){
  analogWrite(ENA, Velocity);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void Right(){
  analogWrite(ENA, Velocity);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void Stop(){
  analogWrite(ENA, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
