#define PIN_LED 7
unsigned int count, toggle;

void setup(){
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);
  delay(500);
}

void loop(){
  digitalWrite(PIN_LED, LOW); 
  delay(1000);

  for(int i=0; i<=5; i++){
  digitalWrite(PIN_LED, HIGH);
  delay(100);
  digitalWrite(PIN_LED, LOW);
  delay(100);
  }

  digitalWrite(PIN_LED, HIGH);
  while(1){}
}
