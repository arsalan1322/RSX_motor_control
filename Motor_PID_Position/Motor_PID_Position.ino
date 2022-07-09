// atomic.h. Instead, interrupts() and noInterrupts() 
// are used. Please use this code if your 
// platform does not support ATOMIC_BLOCK.

#define ENCA 2 
#define ENCB 4 
#define PWM  9
#define DIR  7

volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
long prevT = 0;
float eprev = 0;
float eintegral = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
  
  pinMode(PWM,OUTPUT);
  pinMode(DIR,OUTPUT);
  
  Serial.println("target pos");
}

void loop() {
  //180 -> 4.20
  //360 -> 4.20
  //720 -> 2.16
  //1080 -> 2.16
  //1440 -> 2.17
  //1800 -> 2.16756 
  // set target position
  int target = 1800/2.16756;
  //int target = 250*sin(prevT/1e6);
  
  // PID constants
  float kp = 10;
  float kd = 0.025;
  float ki = 0;

  // time difference
  long currT = micros();
  float deltaT = ((float) (currT - prevT))/(1e6);
  prevT = currT;

  // Read the position
  int pos = 0; 
  noInterrupts(); // disable interrupts temporarily while reading
  pos = posi;
  interrupts(); // turn interrupts back on
  
  // error
  int e = pos - target;

  // derivative
  float dedt = (e-eprev)/(deltaT);

  // integral
  eintegral = eintegral + e*deltaT;

  // control signal
  float u = kp*e + kd*dedt + ki*eintegral;

  // motor power
  float pwr = fabs(u);
  if( pwr > 255 ){
    pwr = 255;
  }

  // motor direction
  int dir = 1;
  if(u<0){
    dir = -1;
  }

  // signal the motor
  setMotor(dir,pwr,PWM,DIR);


  // store previous error
  eprev = e;

  Serial.print(target);
  Serial.print(" ");
  Serial.print(pos);
  Serial.println();
}

void setMotor(int dir, int pwmVal, int pwm, int dirVal){
  analogWrite(pwm,pwmVal);
  if(dir == 1){
    digitalWrite(dirVal,LOW);
  }
  else if(dir == -1){
    digitalWrite(dirVal,HIGH);
  }
}

void readEncoder(){
  int b = digitalRead(ENCB);
  if(b > 0){
    posi++;
  }
  else{
    posi--;
  }
}
