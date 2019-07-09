//Maze Solver Robot
//Victor Guo and Fardin Islam
//2019/01/22

#include<AFMotor.h>

//define notes for piezo music
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

//initialize both motors
AF_DCMotor leftMotor(2,MOTOR12_64KHZ); // create motor #2, 64KHz pwm

AF_DCMotor rightMotor(1,MOTOR12_64KHZ); // create motor #1, 64KHz pwm

int infrared[] = {53,51,49,47}; // pin numbers of infrared sensors
int infraredValues[] = {0,0,0,0}; // array of values storing sensor data
int infraredNumber; // for easy comparisons (converting array to a number)
int rgb[] = {16,15,14}; //pins of rgb
int button = 17; //button pin
int speakerPin = 18; //speaker pin

boolean run = true; //variable to control loop

//Startup Song
int melody[] = {
  NOTE_C3, NOTE_E3, NOTE_G3, NOTE_B3
};

// Durations for startup song
int noteDurations[] = {
  4, 4,4,8
};

//method to play finish music
void runEndMusic() {
  for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(melody[0]); thisNote++) { //plays through the melody
    //to calculate the note duration, take one second divided by the note type.
    int noteDuration = 2000 / noteDurations[thisNote];
    tone(speakerPin, melody[thisNote], noteDuration); //plays the note for the duration
    int pauseBetweenNotes = noteDuration * 1.30; //sets a pause to distinguish between the notes
    delay(pauseBetweenNotes); // Short delay to ensure smooth execution
    noTone(speakerPin); //stop the tone playing:
  };
}

//method to make car swerve left
void left_adjust(){
  rightMotor.setSpeed(255);
  leftMotor.setSpeed(0);
  rightMotor.run(FORWARD);
  leftMotor.run(RELEASE);
}

//method to make car reverse left
void left_back(){
  rightMotor.setSpeed(255);
  leftMotor.setSpeed(0);
  rightMotor.run(BACKWARD);
  leftMotor.run(RELEASE);
}

//method to make car swerve right
void right_adjust(){
  leftMotor.setSpeed(255);
  rightMotor.setSpeed(0);
  leftMotor.run(FORWARD);
  rightMotor.run(RELEASE);
}

//method to make car reverse right
void right_back(){
  leftMotor.setSpeed(255);
  rightMotor.setSpeed(0);
  leftMotor.run(BACKWARD);
  rightMotor.run(RELEASE);
}

//method to make the robot turn right
void right_turn(){
  do{
     //turn right slightly
     rightMotor.setSpeed(255);
     leftMotor.setSpeed(0);
     rightMotor.run(FORWARD);
     leftMotor.run(RELEASE);
     delay(100);

     // values of sensors
     for(int i = 0; i < 4; i++){ //iterate over the 4 sensors
        infraredValues[i] = digitalRead(infrared[i]); //read sensor data into array
     }  
     convert(infraredValues); //store the sensor data in the integer "infraredNumber"
  } 
  while(infraredNumber == 7 || infraredNumber == 15); //while the sensors continue to detect an intersection
}

//method to make the robot turn right
void left_turn(){
  //turn left by about 20 degrees
  leftMotor.setSpeed(255);
  rightMotor.setSpeed(0);
  leftMotor.run(FORWARD);
  rightMotor.run(RELEASE);
  delay(600);
  
  do{
     delay(10);//continue to swerve left
     //check positions of sensors
     for(int i = 0; i < 4; i++){ //iterate over the 4 sensors
        infraredValues[i] = digitalRead(infrared[i]); //read sensor data into array
     }  
     convert(infraredValues); //store sensor data in an integer
  } 
  while(infraredNumber == 14 || infraredNumber == 15); //while sensors continue to detect an intersection
}

//method to make a u turn
void turn_around(){
  do{
    //rotate car
     leftMotor.setSpeed(255);
     rightMotor.setSpeed(255);
     leftMotor.run(FORWARD);
     rightMotor.run(BACKWARD);

     //check positions of sensors
     for(int i = 0; i < 4; i++){ //iterate over the 4 sensors
        infraredValues[i] = digitalRead(infrared[i]); //read sensor data into array
     }  
     convert(infraredValues);
  } 
  while(infraredNumber != 6); //continue to rotate car until the car detects a black line again
}

//convert array of 1s and 0s into decimal number
void convert(int x[]){
  infraredNumber = 0; //initialize to 0
  for(int i = 0; i < 4; i++){ //iterate over every element in array
    if(x[i] == 1)
       infraredNumber += 1 << (3-i); //convert to decimal number
  }
}

//function to solve the maze
//everytime this function is called the robot makes a decision whether to go straight, right, left, or stop
void solve(){
    //check values of sensors
    for(int i = 0; i < 4; i++){ //iterate over the 4 sensors
      //note that sensors return 1 if black and 0 if white
      infraredValues[i] = digitalRead(infrared[i]); //read sensor data into array
    }
    convert(infraredValues); //stores sensor data in "infaredNumber" variable
    Serial.println(infraredNumber); //print to serial for debugging

  //solve the maze based on the following algorithm:
  //if you can turn left, then go left, if not then go straight, and if you can't go straight then turn right, otherwise turn around
  if(infraredNumber == 6){ //go straight if both middle sensors detect black
    leftMotor.setSpeed(255);
    rightMotor.setSpeed(255);
    leftMotor.run(FORWARD);
    rightMotor.run(FORWARD);
   
  }
  else if(infraredNumber == 7 || infraredNumber == 14){ //intersection detected
    if(infraredNumber == 14){ //if its a leftwards intersection
      left_turn(); //go left
    }
    else if(infraredNumber == 7){ //if its a rightwards intersection
      //before we turn right we must see if we can go straight

      //drive a little forward
      leftMotor.setSpeed(255);
      rightMotor.setSpeed(255);
      leftMotor.run(FORWARD);
      rightMotor.run(FORWARD);
      delay(300);

      //check values of sensor 
      for(int i = 0; i < 4; i++){ //iterate over the 4 sensors
         infraredValues[i] = digitalRead(infrared[i]); //read sensor data into array
      }
      convert(infraredValues); 
      
      if(infraredNumber != 6){ //if no path is detected
        //drive back to original position
        leftMotor.setSpeed(255);
        rightMotor.setSpeed(255);
        leftMotor.run(BACKWARD);
        rightMotor.run(BACKWARD);
        delay(300);
        //turn right
        right_turn();
      }
         
    }
  }
  else if(infraredNumber == 15){ //if its a two way intersection
      //first we must check if we are in the "end" condition (maze is done when detects large black square)
      
     //drive forward a bit
     leftMotor.setSpeed(255);
      rightMotor.setSpeed(255);
      leftMotor.run(FORWARD);
      rightMotor.run(FORWARD);
      delay(300);

      //check sensor values
      for(int i = 0; i < 4; i++){ //iterate over the 4 sensors
         infraredValues[i] = digitalRead(infrared[i]); //read sensor data into array
      }
      convert(infraredValues); 
      
      if(infraredNumber != 15){ //if all the sensors don't detect black
        //reverse into original position
        leftMotor.setSpeed(255);
        rightMotor.setSpeed(255);
        leftMotor.run(BACKWARD);
        rightMotor.run(BACKWARD);
        delay(300);
        left_turn(); //make a left
      }
      else{//all sensors detect black again, so maze is solved
        leftMotor.run(RELEASE);
        rightMotor.run(RELEASE);
        run = false; //exit loop
      }
  }
  else if(infraredNumber == 0){ //dead end
    turn_around(); //make u turn
  }
  else if(infraredNumber == 2 || infraredNumber == 1 || infraredNumber == 3){ //robot is swerving left
    right_adjust();
  }
  else if(infraredNumber == 4 || infraredNumber == 12 || infraredNumber == 8){ //robot is swerving right
    left_adjust(); 
  }
  delay(100); //delay for smoothness
}

//set up and initialize
void setup() {
  Serial.begin(57600);
  //initialize infrared sensors
  for(int i = 0; i < 4; i++){
    pinMode(infrared[i], INPUT);
  }
  //initialize rgb
  for(int i =0 ; i < 3; i++){
    pinMode(rgb[i], OUTPUT);
  }
  //initialize speaker
  pinMode(speakerPin,OUTPUT);
  //initialize button
  pinMode(button,INPUT_PULLUP);
}

void loop() {
  run = true; //initialize running variable
  //display red while robot waits for instructions
  digitalWrite(rgb[0],0);
  digitalWrite(rgb[1],255);
  digitalWrite(rgb[2],255);
  
  while(digitalRead(button) != LOW); //loop until button is pressed

  //display blue while robot is moving
  digitalWrite(rgb[0],255);
  digitalWrite(rgb[1],255);
  digitalWrite(rgb[2],0);
  
  while(run) //while robot is not done
    solve();

  //display green when robot done
  digitalWrite(rgb[0],255);
  digitalWrite(rgb[1],0);
  digitalWrite(rgb[2],255);
  
  runEndMusic();//play short song -- maze is finished
  delay(1000);
  
}
