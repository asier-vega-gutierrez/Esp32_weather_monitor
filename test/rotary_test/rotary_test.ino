int pinCLK = 16; // Connected to CLK 
int pinDT = 17; // Connected to DT

int encoderPosCount = 0;
int pinCLKLast;
int CLKVal;
void setup() 
{
pinMode (pinCLK,INPUT);
pinMode (pinDT,INPUT);
/* Read Pin A
Whatever state it's in will reflect the last position
*/
pinCLKLast = digitalRead(pinCLK); 
Serial.begin (9600);
}
void loop() {
  CLKVal = digitalRead(pinCLK);
  if (CLKVal != pinCLKLast){ // Means the knob is rotating
    if (digitalRead(pinDT) != CLKVal){ // Means pin A Changed first - We're 
      encoderPosCount ++;
    } 
else { // Otherwise B changed first and we're 
  encoderPosCount--;
}

Serial.print("Encoder Position: ");
Serial.println(encoderPosCount);
}
pinCLKLast = CLKVal;
}