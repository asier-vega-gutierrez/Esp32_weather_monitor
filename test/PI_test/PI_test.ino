int anteriorA = HIGH;
long int t1 = 0;
long int t2 = 0;


void setup() {
  Serial.begin(9600);
  pinMode(18, INPUT);
}

void loop() {

  t1 = millis();
  t2 = t1;
  int cont = 0;
  while ((t2-t1) <= 5000){
    int actualA = digitalRead(18);
    if ((anteriorA == 1) && (actualA == 0)){
      cont = cont + 1;
    }
    t2 = millis();
    anteriorA = actualA;
  }
  float res = (float(cont)/5)*60;
  Serial.print("RPM:");
  Serial.println(res);
  cont = 0;
}