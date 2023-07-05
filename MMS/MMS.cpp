/*------BASE_CONTROL-----*/

#include "Arduino.h"

// PinChangeInterrupt Lib for Arduino: 
// https://github.com/NicoHood/PinChangeInterrupt
#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>


// Left motor
#define DIR1A   2           // Direction CW
#define DIR2A   3           // Direction CWW
#define PWM1    4           // Speed (0-255)
#define TRQ1    A0          // #IF PINMODE=OUTPUT# Torque (HIGH=Enable)
#define DIAG1   A0          // #IF PINMODE=INPUT#  Diagnostic (HIGH=NO_PROBLEM)
#define CUR1    A2          // Present Current (0-1023) (0-65 mA)
//Left encoder
#define EN1A  10            // Trigger of Interrupt for left encoder
#define EN2A  11            // Second pin of left encoder
#define TIX_PER_SPIN_L 496  // Ticks per one spin


// Right motor
#define DIR1B   5           // Direction CW
#define DIR2B   6           // Direction CWW 
#define PWM2    7           // Speed (0-255)
#define TRQ2    A1          // #IF PINMODE=OUTPUT# Torque (HIGH=Enable)
#define DIAG2   A1          // ##IF PINMODE=INPUT#  Diagnostic (HIGH=NO_PROBLEM)
#define CUR2    A3          // Present Current (0-1023) (0-65 mA)
// Right encoder 
#define EN1B  13            // Srcond pin of right encoder
#define EN2B  12            // Trigger of Interrupt for right encoder
#define TIX_PER_SPIN_R 696  // Ticks per one spin


// Robot settings
#define BASE_WIDTH 0.315    // meter
#define MAX_CURRENT 10      // NOT USED NOW
#define MAX_VEL 3.5         // TODO!!!!
#define WHEEL_DIAMETER 0.2  // meter



class MMS 
{
  public:
  int CURRENT_L = 0;
  int CURRENT_R = 0;
  bool DIAG_L = true;
  bool DIAG_B = true;


  // All pinMode in one function
  // DONT WORK IN CPP FILE, ONLY IN INO FILE
  void start(){
    // Left motor
    pinMode(PWM1,OUTPUT);
    pinMode(DIR1A,OUTPUT);
    pinMode(DIR2A,OUTPUT);
    pinMode(DIAG1,INPUT);    // ATTENTION: Torque pin in Diagnostic workmode
    pinMode(CUR1,INPUT);
    // Left encoder
    pinMode(EN1A, INPUT_PULLUP);
    pinMode(EN2A, INPUT_PULLUP);

    // Right motor
    pinMode(PWM2,OUTPUT);
    pinMode(DIR1B,OUTPUT);
    pinMode(DIR2B,OUTPUT);
    pinMode(DIAG2,INPUT);   // ATTENTION: Torque pin in Diagnostic workmode
    pinMode(CUR2,INPUT);
    // Right encoder
    pinMode(EN1B, INPUT_PULLUP);
    pinMode(EN2B, INPUT_PULLUP);

    //GPIO Pins for power supply of right encoder
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
  }
 
  // Stop motors when linear and angular velocities = 0.0
  // DONT WORK IN CPP FILE, ONLY IN INO FILE
  void motor_stop(){
    analogWrite(PWM1, 0);  
    analogWrite(PWM2, 0);
    digitalWrite(DIR1A,LOW); digitalWrite(DIR2A,LOW);
    digitalWrite(DIR1B,LOW); digitalWrite(DIR2B,LOW);
    digitalWrite(DIR1A,HIGH); digitalWrite(DIR2A,HIGH);
    digitalWrite(DIR1B,HIGH); digitalWrite(DIR2B,HIGH);
  }

  // Make wheels free for spinning by hands, zero speed
  // DONT WORK IN CPP FILE, ONLY IN INO FILE
  void torque_disable(){
    analogWrite(PWM1, 0);  
    analogWrite(PWM2, 0);
    digitalWrite(DIR1A,LOW); digitalWrite(DIR2A,LOW);
    digitalWrite(DIR1B,LOW); digitalWrite(DIR2B,LOW);
  }
  
  void clear(){
  // EMPTY
  }

  void set_pwm(float SPD_L, float SPD_R){
    // Potential protection
    if((SPD_L == 0.0)&&(SPD_R == 0.0)){
      // DONT WORK IN CPP FILE, ONLY IN INO FILE
      //motor_stop();
      //torque_disable();
    }

    // Set direction of spinning for each wheels
    if(SPD_L > 0.0) {
      digitalWrite(DIR1A,HIGH); digitalWrite(DIR2A,LOW);
    }
    if(SPD_L < 0.0) {
      digitalWrite(DIR1A,LOW); digitalWrite(DIR2A,HIGH);
    }
    if(SPD_R > 0.0) {
      digitalWrite(DIR1B,HIGH); digitalWrite(DIR2B,LOW);
    }
    if(SPD_R < 0.0) {
      digitalWrite(DIR1B,LOW); digitalWrite(DIR2B,HIGH);
    }

    // Set PWM speed to each wheels
    analogWrite(PWM1, abs(SPD_L));  
    analogWrite(PWM2, abs(SPD_R));
  }

  private:
  int16_t SPD_L = 0;
  int16_t SPD_R = 0;
};

  

