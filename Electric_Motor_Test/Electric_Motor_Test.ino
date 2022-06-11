//The sample code for driving one way motor encoder
const byte encoder0pinA = 2;//A pin -> the interrupt pin 0
const byte encoder0pinB = 4;//B pin -> the digital pin 4
byte encoder0PinALast;
double duration;//the number of the pulses
double degree;
double large_degree;
boolean Direction;//the rotation direction


void setup()
{
 Serial.begin(57600);//Initialize the serial port
 EncoderInit();//Initialize the module
}

void loop()
{
 Serial.print("Angle:");
 Serial.println(degree);
 Serial.print("L_Angle:");
 Serial.println(large_degree);
 //duration = 0;
 delay(100);
}

void EncoderInit() 
{
 Direction = true;//default -> Forward
 pinMode(encoder0pinB,INPUT);
 attachInterrupt(0, wheelSpeed, CHANGE);
}

void wheelSpeed()
{
 int Lstate = digitalRead(encoder0pinA);
 if((encoder0PinALast == LOW) && Lstate==HIGH)
 {
 int val = digitalRead(encoder0pinB);
 if(val == LOW && Direction)
 {
 Direction = false; //Reverse
 }
 else if(val == HIGH && !Direction)
 {
 Direction = true; //Forward
 }
 }
 encoder0PinALast = Lstate;

  
 if(!Direction) {
  duration++;
 } else {
  duration--;
 }

 degree = duration/32.0 * 360.0; 
 large_degree = degree/5.2;
} 
