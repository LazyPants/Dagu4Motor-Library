/*
Created by: William Garrido - FriedCircuits.us
Exmaple of Dagu 4 Channel Motor Controller with 4 motors on the Rover 5
Encoder support is provided by Teensy Encoder Library.

Serial processing code is borrowed from
https://github.com/hbrobotics/ros_arduino_bridge

*/

#include "Dagu4Motor.h"

//Serial input
// Variable to hold an input character
char chr;

// Variable to hold the current single-character command
char cmd;

//Character arrays to hold the first argument
char argv1[16];

// A pair of varibles to help parse serial commands (thanks Fergs)
int arg = 0;
int index_ = 0;

// The arguments converted to integers
long arg1;

bool boolMove = false;

unsigned time = millis();

//Motors
const int pwm_a = 3;  //PWM for CH1 LF
const int pwm_b = 5;  //PWM for CH4 RR  - Revserved Encoder pins to make postive
const int pwm_c = 6;  //PWM for CH2 LR
const int pwm_d = 9;  //PWM for CH3 RF
const int dir_a = 2;  //DIR for CH1
const int dir_b = 4;  //DIR for CH4
const int dir_c = 7;  //DIR for CH2
const int dir_d = 8;  //DIR for CH3

//Current Sensors
const int CURRENTA = A2; 
const int CURRENTB = A3; 
const int CURRENTC = A4; 
const int CURRENTD = A5; 
const int CURRENT_LIMIT = (1024 / 5) * 2.6;  // amps

//Encoder Interrupts
//Needs Teensy Encoder library
const int encA1 = 2;
const int encA2 = 46;
const int encB1 = 3;
const int encB2 = 47;
const int encC1 = 18;
const int encC2 = 48;
const int encD1 = 19;
const int encD2 = 49;



Dagu4Motor motor1(pwm_a, dir_a, CURRENTA);  // CH1
Dagu4Motor motor4(pwm_b, dir_b, CURRENTB);  // CH4
Dagu4Motor motor2(pwm_c, dir_c, CURRENTC);  // CH2
Dagu4Motor motor3(pwm_d, dir_d, CURRENTD);  // CH3

void mReverse(int speed)
{

	motor1.setMotorDirection(true);
	motor2.setMotorDirection(true);
	motor3.setMotorDirection(false);
	motor4.setMotorDirection(false);
	motor1.setSpeed(speed);
        motor2.setSpeed(speed);
        motor3.setSpeed(speed);
        motor4.setSpeed(speed);
        boolMove = true;

}


void mForward(int speed)
{

	motor1.setMotorDirection(false);
  motor2.setMotorDirection(false);
  motor3.setMotorDirection(true);
  motor4.setMotorDirection(true);
	motor1.setSpeed(speed);
        motor2.setSpeed(speed);
        motor3.setSpeed(speed);
        motor4.setSpeed(speed);
        boolMove = true;    

}

void mRight(int speed)
{

	motor1.setMotorDirection(true);
	motor2.setMotorDirection(false);
	motor3.setMotorDirection(false);
	motor4.setMotorDirection(true);
	motor1.setSpeed(speed);
        motor2.setSpeed(speed);
        motor3.setSpeed(speed);
        motor4.setSpeed(speed);
        boolMove = true;


}

void mLeft(int speed)
{


	motor1.setMotorDirection(false);
	motor2.setMotorDirection(true);
	motor3.setMotorDirection(true);
	motor4.setMotorDirection(false);
	motor1.setSpeed(speed);
        motor2.setSpeed(speed);
        motor3.setSpeed(speed);
        motor4.setSpeed(speed);
        boolMove = true;

}



void mStop()
{

	motor1.stopMotors();
	motor2.stopMotors();
	motor3.stopMotors();
	motor4.stopMotors();
        boolMove = false;
}

void runCommand(){
  
     arg1 = atoi(argv1);
   
    switch (cmd){
    
      case 'f':
            Serial.print("Forward: ");  
            Serial.println(arg1);   
            mForward(arg1);
      break;
      
      case 'l':
          Serial.print("Left: "); 
          Serial.println(arg1);  
          mLeft(arg1); 
      break;
      
      case 'r':
         Serial.print("Right: ");
         Serial.println(arg1);
         mRight(arg1);     
      break;
      
      case 'b':
        Serial.print("Reverse: "); 
        Serial.println(arg1); 
        mReverse(arg1);      
      break;
      
      case 's':
        mStop();
        Serial.println("Stopped");
      break;
  }
  
  cmd = NULL;
  memset(argv1, 0, sizeof(argv1));
  arg = 0;
  arg1 = 0;
  index_ = 0;



}

void setup(){

  Serial.begin(115200);
  Serial.println("Rover 5 Example Sketch");

  motor1.begin();
  motor2.begin();
  motor3.begin();
  motor4.begin();

}



void loop(){
  
  

  while (Serial.available() > 0) {
    
    // Read the next character
    chr = Serial.read();

    // Terminate a command with a CR
    if (chr == 13) {
      if (arg == 1) argv1[index_] = NULL;
      runCommand();
    }
    // Use spaces to delimit parts of the command
    else if (chr == ' ') {
      // Step through the arguments
      if (arg == 0) arg = 1;
      else if (arg == 1)  {
        argv1[index_] = NULL;
        arg = 2;
        index_ = 0;
      }
      continue;
    }
    else {
      if (arg == 0) {
        // The first arg is the single-letter command
        cmd = chr;
      }
      else if (arg == 1) {
        // Subsequent arguments can be more than one character
        argv1[index_] = chr;
        index_++;
      }
      
    }
  }
   
 
  if (boolMove){
  
    if(millis() > (time+1000)){
    
      Serial.print("M1-Current: ");
      Serial.println(motor1.getCurrent());
      
      Serial.print("M2-Current: ");
      Serial.println(motor2.getCurrent());
      
      Serial.print("M3-Current: ");
      Serial.println(motor3.getCurrent());
      
      Serial.print("M4-Current: ");
      Serial.println(motor4.getCurrent());
      
      time=millis();
    
    }
  
  }
  

}


