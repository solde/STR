//pin mapping
#define pinEncoderA 2//attach yellow wire from the motor to pin 2 of arduino
#define pinEncoderB 3//attach purple wire from the motor to pin 19 of arduino
#define pinPWM 11
#define pinDir 12
#define pinDirInv 10

float u=0;
signed int ISRCounter=0;//used inside the ISR

//function prototypes
void ISREncoderA(void);
void ISREncoderB(void);

int Kp;
int error;
int Ki;
int totalError;
int Kd;
int lastError;

void setsup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_BUILTIN,OUTPUT);
    pinMode(pinEncoderA, INPUT_PULLUP);
    pinMode(pinEncoderB, INPUT_PULLUP);
    pinMode(pinDir, OUTPUT);
    pinMode(pinDirInv, OUTPUT);
    pinMode(pinPWM, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(pinEncoderA), ISREncoderA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pinEncoderB), ISREncoderB, CHANGE);
    Serial.begin(115200);
    // TIMER 1 for interrupt frequency 100 Hz:
    cli(); // stop interrupts
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1 = 0; // initialize counter value to 0
    OCR1A = 19999; // = 16000000 / (8 * 100) - 1 (must be <65536)
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
    TIMSK1 |= (1 << OCIE1A);
    sei(); // allow interrupts
    unsigned long isr_timer1_count=0;
}

float Kp=40, Ki=10, Kd=0;
float setpointTemps = 50;

ISR(TIMER1_COMPA_vect){
    digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) ^ 1);
    isr_timer1_count++;

    float error = setpointTemps - isr_timer1_count*(1/100);

    float errorIntegral = errorIntegral + error;
    float errorDerivative = error-previousError;
    
    previousError = error;

    u = Kp*error + Ki* errorIntegral + Kd * errorDerivative;

    if (isr_timer1_count%200>100){
        u=10;
    }
    else{
        u=-10;
    }
    //check for motor direction
    if (u>=0){
        digitalWrite(pinDir,0);
        digitalWrite(pinDirInv,1);
    }
    else{
        digitalWrite(pinDir,1);
        digitalWrite(pinDirInv,0);
    }
    //check for saturation
    if (u>255.0){
        u=255.0;
    }
    if (u<-255.0){
        u=-255.0;
    }
    analogWrite(pinPWM,abs(u));//update output
    Serial.println(ISRCounter);
}

void ISREncoderA(void){
  // Si un flanc ascendent d'a es troba amb B a 1, girem en sentit horari
  if(digitalRead(pinEncoderA) == digitalRead(pinEncoderB)) {
    dir = true;
    angle+=1;
  }
  else {
  	dir = false;
    angle -=1;
  }
}

void ISREncoderB(void){
    // Si un flanc ascendent d'a es troba amb B a 1, girem en sentit horari
    if(digitalRead(pinEncoderA) != digitalRead(pinEncoderB)) {
        dir = true;
        angle+=1;
    }
    else {
        dir = false;
        angle -=1;
    }
}

void loop(){

}