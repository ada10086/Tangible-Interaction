
int flexSensorPin = A0; 

void setup(){
  Serial.begin(9600);  
}

void loop(){
  int flexSensorReading = analogRead(flexSensorPin); 
  Serial.println(flexSensorReading);
  delay(50); 
}
