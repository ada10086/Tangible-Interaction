void setup() {
  Serial.begin(9600);
}

void loop() {
  int xSensor = analogRead(A5);
  delay(5);
  int x = map(xSensor, 0, 1023, 0, 2)-1;

//  Serial.println(xSensor);
  Serial.println(x);

}
