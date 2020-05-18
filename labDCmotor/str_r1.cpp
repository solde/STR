#include "core/Arduino.h"
#include "libraries/SparkFun_LSM6DS3_Breakout/src/SparkFunLSM6DS3.h"
#include "libraries/Wire/src/Wire.h"
#include "libraries/SPI/src/SPI.h"
#include "libraries/Ethernet/src/Ethernet.h"
#include "libraries/Ethernet/src/EthernetUdp.h"
#include "libraries/Grove-Ranging-sensor-VL53L0X/src/Seeed_vl53l0x.h"

#include "FreeRTOS/Arduino_FreeRTOS.h"
#include "FreeRTOS/timers.h"

//pin mapping
#define pinEncoderA 2//attach yellow wire from the motor to pin 2 of arduino
#define pinEncoderB 3//attach purple wire from the motor to pin 19 of arduino
#define pinPWM 11
#define pinDir 12
#define pinDirInv 10

// function prototypes
// two tasks for IMU (Inertial Measurement Unit) & TimeOfFlight (distance) sensors
void TaskU( void *pvParameters );
void taskEncA( void *pvParameters );
void taskEncB( void *pvParameters );
void taskPrintParam( void *pvParameters );
// one shot timer to stop the RT kernel
void OneShotTimerCallback( TimerHandle_t xTimer );

/*LSM6DS3 myIMU; //Default constructor is I2C, addr 0x6B
Seeed_vl53l0x VL53L0X; //TimeOfFlight distance sensor
*/

//function prototypes
void ISREncoderA(void);
void ISREncoderB(void);

//Global variables
float u=0;
signed int ISRCounter=0;//used inside the ISR

float Kp=1.3, Ki=0.1, Kd=0.03, previousError = 0, errorIntegral=0;
int angle = 0, previousAngle = 0;
bool dir;

//Task handlers
TaskHandle_t TaskUHandle;
TaskHandle_t taskEncAHandle;
TaskHandle_t taskEncBHandle;
TaskHandle_t taskPrintParamHandle;

//timer handlers
TimerHandle_t xPeriodicTimer, xOneShotTimer;
BaseType_t xPeriodicTimerStarted, xOneShotStarted;

float str_getTime(void)
{
  float t=(float)(0.5e-3*((float)OCR1A*xTaskGetTickCount()+TCNT1));//Sent time in milliseconds!!!
  return t;
}

static void ExternalInterruptEncoderAHandler(){
	taskYieldRequired = xTaskResumeFromISR(taskEncAHandle);
}

static void ExternalInterruptEncoderBHandler(){
	taskYieldRequired = xTaskResumeFromISR(taskEncBHandle);
}

void setup() {
	
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_BUILTIN,OUTPUT);
    pinMode(pinEncoderA, INPUT_PULLUP);
    pinMode(pinEncoderB, INPUT_PULLUP);
    pinMode(pinDir, OUTPUT);
    pinMode(pinDirInv, OUTPUT);
    pinMode(pinPWM, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(pinEncoderA), ExternalInterruptEncoderAHandler, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pinEncoderB), ExternalInterruptEncoderBHandler, CHANGE);
    Serial.begin(115200);

	xOneShotTimer = xTimerCreate("OneShotTimer", 100  , pdFALSE, 0, OneShotTimerCallback );
	xOneShotStarted = xTimerStart( xOneShotTimer, 0 );

	xTaskCreate(
		TaskU
		,  "Task U"   // A name just for humans
		,  192  // This stack size can be checked & adjusted by reading the Stack Highwater
		,  NULL
		,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
		,  &TaskUHandle );
	xTaskCreate(
		taskEncB
		,  "Tasc Encoder B"   // A name just for humans
		,  512  // This stack size can be checked & adjusted by reading the Stack Highwater
		,  NULL
		,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
		,  &taskEncBHandle );
	xTaskCreate(
		taskEncA
		,  "Task Encoder A"   // A name just for humans
		,  512  // This stack size can be checked & adjusted by reading the Stack Highwater
		,  NULL
		,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
		,  &taskEncAHandle );
	xTaskCreate(
		taskPrintParam
		,  "Task Print Parameters"   // A name just for humans
		,  512  // This stack size can be checked & adjusted by reading the Stack Highwater
		,  NULL
		,  0  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
		,  &taskPrintParamHandle );
}

void taskPrintParam(void *pvParameters){
	Serial.print(angle);
  	Serial.print(",");
  	Serial.println(dest);
}

void taskEncA(void *pvParameters){
  // Si un flanc ascendent d'a es troba amb B a 1, girem en sentit horari
      // Si un flanc ascendent d'a es troba amb B a 1, girem en sentit horari
 	(void) pvParameters;

	(void) pvParameters;
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	for (;;) // A Task shall never return or exit.
	{
		previousAngle = angle;
		if(digitalRead(pinEncoderA) == digitalRead(pinEncoderB)) {
			dir = true;
			angle+=1;
		}
		else {
			dir = false;
			angle -=1;
		}
		vTaskSuspend(taskEncBHandle);
	}
}

void taskEncB(void *pvParameters){
    // Si un flanc ascendent d'a es troba amb B a 1, girem en sentit horari
 	(void) pvParameters;

	(void) pvParameters;
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	for (;;) // A Task shall never return or exit.
		{
		previousAngle = angle;  
		if(digitalRead(pinEncoderA) != digitalRead(pinEncoderB)) {
			dir = true;
			angle+=1;
		}
		else {
			dir = false;
			angle -=1;
		}
		vTaskSuspend(taskEncBHandle);
	}
}

void TaskU(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  (void) pvParameters;
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  for (;;) // A Task shall never return or exit.
  {
	digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) ^ 1);
    isr_timer1_count++;
    if(isr_timer1_count%100 == 0){
      dest *= -1;
      //errorIntegral = 0;
    }
  	
    float error = dest - angle;
    errorIntegral = errorIntegral + error;
    float errorDerivative = error-previousError;
    
    previousError = error;

    u = Kp*error + Ki*errorIntegral + Kd*errorDerivative;
    
  	//No para el motor (peta el sim)
	if ( (u>0) && (u<10) )u=15;
  	if ( (u>-10) && (u<0) )u=-15;

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
    vTaskDelay(10 / portTICK_PERIOD_MS ); // wait for 50 milliseconds
  }
}

void OneShotTimerCallback( TimerHandle_t xTimer )
{
  TickType_t xTimeNow;
  xTimeNow = xTaskGetTickCount();

  //oneshottimer_count++;

    //stop the RTOS kernel and send data
	//stop the kernel...
	//  vTaskSuspend(Task1Handle);
	//  vTaskSuspend(Task2Handle);
	//  vTaskSuspend(Task3Handle);
	//  vTaskSuspend(Task4Handle);
	vTaskSuspendAll();

	//...and sent data to the host PC
	unsigned int i;
	for (i = 0; i < BUFF_SIZE; i++)
	{
		Serial.print((float)t[i]);
		Serial.write((uint8_t)circ_buffer1[i]);
		Serial.write((uint8_t)circ_buffer2[i]);
		Serial.print((float)debug_data1[i]);
		Serial.println();
	}
}

void loop()
{
    //Nothing to do in background
}

