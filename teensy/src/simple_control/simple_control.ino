#include <Dynamixel2Arduino.h>

#define DXL_SERIAL   Serial1    
#define DEBUG_SERIAL Serial     
const uint8_t DXL_DIR_PIN = 2;  // DYNAMIXEL Shield DIR PIN
const uint8_t DXL_IDL = 1;
const uint8_t DXL_IDR = 2;
const float DXL_PROTOCOL_VERSION = 1.0;

// Pin 7 & 8
// Pin 0 & 1

/*Dynamixel Library*/
int k=0; 

// Predefined states
int grip_open[2]={450,574};
int grip_close[2]={512,512};
int comodin0[2]={522,502};
int comodin1[2]={532,492};
int comodin2[2]={542,482};
int comodin3[2]={552,472};
int comodin4[2]={562,462};
int comodin5[2]={572,452};

// To store the baudrate for DYNAMIXEL
unsigned long ax_bps;

// Set Port baudrate to 9600bps.
int serial_bps = 9600;

// We initialize an object with an open serial connection and the DIR_PIN
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

/*Continous sensing*/
int actual_value[2]={512,512};

int val = 0;
bool set_up_dynamixel()
{
  /* Function to check the port baudrates, set the baudrates for communication and
   * initialize both the dynamixels 
   */
  bool result_l = false;
  bool result_r = false;
  
  ax_bps = dxl.getPortBaud();

  // Set Port baudrate. Should be 57600. Initializes Serial comms with DYNAMIXEL
  dxl.begin(ax_bps);
  DEBUG_SERIAL.print(ax_bps);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  
  // Checks the connection status of Left DYNAMIXEL.
  result_l = dxl.ping(DXL_IDL);
  if (result_l){
    // Turn off torque when configuring items in EEPROM area
    dxl.torqueOff(DXL_IDL);
    dxl.setOperatingMode(DXL_IDL, OP_POSITION);
    dxl.torqueOn(DXL_IDL);

    DEBUG_SERIAL.print("\n Connected to the a motor! Motor ID: ");
    DEBUG_SERIAL.print(DXL_IDL);
    DEBUG_SERIAL.print("\n Detected dynamixel baudrate, using common baudrate:\n");
    DEBUG_SERIAL.print(ax_bps);
  }
  else{
    DEBUG_SERIAL.print("\n Can not connect to the motor");
    DEBUG_SERIAL.print(DXL_IDL);
    DEBUG_SERIAL.print("\n"); 
  }

  // Checks the connection status of Right DYNAMIXEL.
  result_r = dxl.ping(DXL_IDR);
  if (result_r){
    // Turn off torque when configuring items in EEPROM area
    dxl.torqueOff(DXL_IDR);
    dxl.setOperatingMode(DXL_IDR, OP_POSITION);
    dxl.torqueOn(DXL_IDR);

    DEBUG_SERIAL.print("\n Connected to the a motor! Motor ID: ");
    DEBUG_SERIAL.print(DXL_IDR);
    DEBUG_SERIAL.print("\n Detected dynamixel baudrate, using common baudrate:\n");
    DEBUG_SERIAL.print(ax_bps);
  }
  else{
    DEBUG_SERIAL.print("\n Can not connect to the motor");
    DEBUG_SERIAL.print(DXL_IDR);
    DEBUG_SERIAL.print("\n"); 
  }
  // DEBUG_SERIAL.print("\n Can not connect to the motors! \n");
  // We return false if either doesn't work
  return result_l & result_r;
}


//bool set_up_dynamixel()
//{
//  bool result = false;
//  
//  ax_bps = dxl.getPortBaud();
//
//  // Set Port baudrate. Should be 57600.
//  dxl.begin(ax_bps);
//  
//  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
//  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
//  
//  // Get DYNAMIXEL information
//  result = dxl.ping(DXL_IDL);
//
//  if (result)
//  {
//    // Turn off torque when configuring items in EEPROM area
//    dxl.torqueOff(DXL_IDL);
//    dxl.setOperatingMode(DXL_IDL, OP_POSITION);
//    dxl.torqueOn(DXL_IDL);
//
//    DEBUG_SERIAL.print("\n Connected to the motors! \n");
//      
//    DEBUG_SERIAL.print("\n Detected dynamixel baudrate:\n");
//    DEBUG_SERIAL.print(ax_bps);
//
//    return result;
//  }
//
//  DEBUG_SERIAL.print("\n Can not connect to the motors! \n");
//  return result;
//}

  
void set_gripper_goal_position(int desired_action[]){
  /* Function which sets the goal position
   * Input:     2 element Integer array of the final positions of each dynamixel
   */
  for(int k=1;k<=2;k++){
    dxl.setGoalPosition(k, desired_action[k-1]);
  }
}

void display_position(){
  /* Function to print the current position of the DYNAMIXEL motors
   * Print Format - RAW - [0,1023]
   * The UNIT_DEGREE isn't working for some reason
   */
  char buf[100];
  sprintf(buf, "\nDynamixel motor 1:  Position: %f, \nDynamixel motor 2:  Position: %f", dxl.getPresentPosition(1), dxl.getPresentPosition(2)); 
  DEBUG_SERIAL.println(buf);
}

void display_all_variables(int loop_counter = 0, bool count = false){
  /* Function which displays the position, velocity, current and PWN of the two Dyamixels ID 1 & 2
   * Single line serial print functions result in faster execution times when compared to multiple print statements.
   */
  if(count){
    char buf[200];
    sprintf(buf, "\nDynamixel motor 1:  Position: %f, Velocity: %f, Current: %f, PWM: %f\nDynamixel motor 2:  Position: %f, Velocity: %f, Current: %f, PWM: %f", dxl.getPresentPosition(1), dxl.getPresentVelocity(1), dxl.getPresentCurrent(1), dxl.getPresentPWM(1), dxl.getPresentPosition(2), dxl.getPresentVelocity(2), dxl.getPresentCurrent(2), dxl.getPresentPWM(2)); 
    DEBUG_SERIAL.println(buf);  
  }
  else{
    char buf[200];
    sprintf(buf, "\n Read Instance: %d Dynamixel motor 1:  Position: %f, Velocity, Current: %f, PWM: %f: %f\nDynamixel motor 2:  Position: %f, Velocity: %f, Current: %f, PWM: %f", loop_counter, dxl.getPresentPosition(1), dxl.getPresentVelocity(1), dxl.getPresentCurrent(1), dxl.getPresentPWM(1), dxl.getPresentPosition(2), dxl.getPresentVelocity(2), dxl.getPresentCurrent(2), dxl.getPresentPWM(2)); 
    DEBUG_SERIAL.println(buf);  
  }  
}   


void display_position_and_velocity(int loop_counter = 0, bool count = false){
  // Function which displays the position and velocity of the two Dyamixels ID 1 & 2
  if(count){
    char buf[150];
    sprintf(buf, "\nDynamixel motor 1:  Position: %f, Velocity: %f\nDynamixel motor 2:  Position: %f, Velocity: %f", dxl.getPresentPosition(1), dxl.getPresentVelocity(1), dxl.getPresentPosition(2), dxl.getPresentVelocity(2)); 
    DEBUG_SERIAL.println(buf);  
  }
  else{
    char buf[150];
    sprintf(buf, "\n Read Instance: %d Dynamixel motor 1:  Position: %f, Velocity: %f\nDynamixel motor 2:  Position: %f, Velocity: %f", loop_counter, dxl.getPresentPosition(1), dxl.getPresentVelocity(1), dxl.getPresentPosition(2), dxl.getPresentVelocity(2)); 
    DEBUG_SERIAL.println(buf);  
  }
}

void move_to_goal_and_give_feedback(int desired_action[]){
  // Goal positions for both the DYNAMIXELs
  int position_tolerance = 10;
  for(k=1;k<=2;k++)
  {
    dxl.setGoalPosition(k, desired_action[k-1]);
  }
  // Hardcoded for now
  for( k=0; (abs(desired_action[0] - dxl.getPresentPosition(1)) > position_tolerance) && (abs(desired_action[1] - dxl.getPresentPosition(2)) > position_tolerance); k++ ){
    display_all_variables(k, true);
  }
}

void simple_movement(int desired_action[]){
  /* Function to move the grippers to a particular position. In case the grippers are
   * not able to move further due to some reason, the grippers are stopped to prevent
   * overheating
   * Input:     2 element Integer array of the final positions of each dynamixel
   */
  int sliding_window_size = 5;
  float sliding_window[2][5];
  int position_tolerance = 10;
  int minimum_movment = 2;
  
  // Goal positions for both the DYNAMIXELs
  for(int k=1;k<=2;k++)
  {
    dxl.setGoalPosition(k, desired_action[k-1]);
  }
  
  // Hardcoded for now
  // Continue monitoring till final position is reached
  for( k=0; (abs(desired_action[0] - dxl.getPresentPosition(1)) > position_tolerance) && (abs(desired_action[1] - dxl.getPresentPosition(2)) > position_tolerance); k++ ){
    if (k < sliding_window_size){
      sliding_window[0][k] = dxl.getPresentPosition(1);
      sliding_window[1][k] = dxl.getPresentPosition(2);
    }
    else{
      if((abs(sliding_window[0][0] - sliding_window[0][sliding_window_size-1]) < minimum_movment) && (abs(sliding_window[1][0] - sliding_window[1][sliding_window_size-1]) < minimum_movment)){
        desired_action[0] = dxl.getPresentPosition(1);
        desired_action[1] = dxl.getPresentPosition(2);
        simple_movement(desired_action);
        break;
      }
      // We left shift all the elements and update the new one
      for(int i=0; i< sliding_window_size - 1; i++){
          sliding_window[0][i] = sliding_window[0][i+1];
          sliding_window[1][i] = sliding_window[1][i+1];
      }
      sliding_window[0][sliding_window_size-1] = dxl.getPresentPosition(1);
      sliding_window[2][sliding_window_size-1] = dxl.getPresentPosition(2);
    }
  }
}

void set_velocity(int velocity){
  // Function to set the operating velocity and revert back to position mode
  // Setting to velocity mode and the operating velocity
  for(k=1; k<=2; k++){
    dxl.torqueOff(k);
    dxl.setOperatingMode(k, OP_VELOCITY);
    dxl.setGoalVelocity(k, velocity);
    dxl.torqueOn(k);
  }
  
  // Setting back to Position Mode
  for(k=1; k<=2; k++){
    dxl.torqueOff(k);
    dxl.setOperatingMode(k, OP_POSITION);
    dxl.torqueOn(k);
  } 
}

void setup() {
  DEBUG_SERIAL.begin(serial_bps);
  bool result = false;

  // Initialize/Set up the dynamixel
  while (!result)
  {
    result = set_up_dynamixel();
    delay(500);
  }
  // Set default velocity
  set_velocity(10);
}



void loop() {
move_to_goal_and_give_feedback(grip_close);
move_to_goal_and_give_feedback(grip_open);
delay(500);
}