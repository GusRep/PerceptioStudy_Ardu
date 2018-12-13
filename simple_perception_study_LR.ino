#include <Servo.h>
// File to test the cues of Left and Right analog cues

Servo indexServo;
Servo thumbServo;

int centerIndex = 90;
int centerThumb = 90;
int integerValue;
int state;
int slow = 80;    // milliseconds/degree
int fast = 20;     // milliseconds/degree

int angles[6] = {4, 7, 10, 13, 16, 19}; //{4, 6, 8, 10, 12, 14};
int NUM_ANGLES = 6;
int angleVal;
int actionVal;
bool skip_blank = false;
bool adjusting = true;

// Motor angles for the indexed cue
//     cue: {0,1,2,3,4,5, 6,7,8,9,10,11 ...   ,35,36 ...   ,71}
// Index : L motor (user viewpoint)
int L[72] = {5,5,5,5,5,5, 4,4,4,4,4,4, 3,3,3,3,3,3, 2,2,2,2,2,2, 1,1,1,1,1,1, 0,0,0,0,0,0, -5,-5,-5,-5,-5,-5, -4,-4,-4,-4,-4,-4, -3,-3,-3,-3,-3,-3, -2,-2,-2,-2,-2,-2, -1,-1,-1,-1,-1,-1, 0,0,0,0,0,0};
// Thumb : R motor (user viewpoint)
int R[72] = {5,4,3,2,1,0, 5,4,3,2,1,0, 5,4,3,2,1,0, 5,4,3,2,1,0, 5,4,3,2,1,0, 5,4,3,2,1,0, -5,-4,-3,-2,-1, 0, -5,-4,-3,-2,-1, 0, -5,-4,-3,-2,-1, 0, -5,-4,-3,-2,-1, 0, -5,-4,-3,-2,-1, 0, -5,-4,-3,-2,-1,0};


  void servosTrajectory(int indexStart, int indexEnd, int thumbStart, int thumbEnd, int timeStep) {
    int currentPosIndex = indexStart;
    int currentPosThumb = thumbStart;
    int incrementIndex;
    int incrementThumb;

    if (indexStart < indexEnd) {
      incrementIndex = 1;
    }
    else if (indexStart > indexEnd) {
      incrementIndex = -1;
    }
    else {
      incrementIndex = 0;
    }

    if (thumbStart < thumbEnd) {
      incrementThumb = 1;
    }
    else if (thumbStart > thumbEnd) {
      incrementThumb = -1;
    }
    else {
      incrementThumb = 0;
    }
 
    indexServo.write(currentPosIndex);
    thumbServo.write(currentPosThumb);
    while (currentPosIndex != indexEnd) {
      currentPosIndex += incrementIndex;
      currentPosThumb += incrementThumb;
      indexServo.write(currentPosIndex);
      thumbServo.write(currentPosThumb);
      delay(timeStep);

    }
        // Forcing the initial position
        //indexServo.write(indexEnd);
        //thumbServo.write(thumbEnd);
  }



void setup() {
  indexServo.attach(9);
  thumbServo.attach(6);
  indexServo.write(centerIndex);
  thumbServo.write(centerThumb);
  Serial.begin(9600);
  integerValue = 0;
  state = 0;
  Serial.println("Connected.");
}


void loop() {
  if (Serial.available() > 0) {
    //integerValue = Serial.parseInt();
    integerValue = Serial.read();
    //Serial.println(integerValue);
    
    if( integerValue != 0){

      if (adjusting) {
        switch (integerValue) {
          case 6: // increase centerIndex
            centerIndex -= 2;
            indexServo.write(centerIndex);
            Serial.println("1");

            break;
          case 7: // decrease centerIndex
            centerIndex += 2;
            indexServo.write(centerIndex);
            break;
          case 8: // increase centerThumb
            centerThumb -= 2;
            thumbServo.write(centerThumb);
            break;
          case 9: // decrease centerThumb
            centerThumb += 2;
            thumbServo.write(centerThumb);
            break;
          case 1: // proceed
            adjusting = false;
            break;
        }
        
      }
      else {
        integerValue -= 1;    // adjust for 1 indexing
        
        // START OLD FASHON
        /*
        angleVal = angles[integerValue % NUM_ANGLES];
        actionVal = integerValue / NUM_ANGLES;

        switch (actionVal) {

          case 0: // forward
            servosTrajectory(centerIndex, centerIndex - angleVal, centerThumb, centerThumb + angleVal, fast);
            delay(1000);
            servosTrajectory(centerIndex - angleVal, centerIndex, centerThumb + angleVal, centerThumb, slow);
            delay(250);
            break;
          case 1: // back
            servosTrajectory(centerIndex, centerIndex + angleVal, centerThumb, centerThumb - angleVal, fast);
            delay(1000);
            servosTrajectory(centerIndex + angleVal, centerIndex, centerThumb - angleVal, centerThumb, slow);
            delay(250);
            break;
          case 2: // left twist
            servosTrajectory(centerIndex, centerIndex - angleVal, centerThumb, centerThumb - angleVal, fast);
            delay(1000);
            servosTrajectory(centerIndex - angleVal, centerIndex, centerThumb - angleVal, centerThumb, slow);
            delay(250);
            break;
          case 3: // right twist
            servosTrajectory(centerIndex, centerIndex + angleVal, centerThumb, centerThumb + angleVal, fast);
            delay(1000);
            servosTrajectory(centerIndex + angleVal, centerIndex, centerThumb + angleVal, centerThumb, slow);
            delay(250);
            break;

        }
        */
        // END OLD FASHON
        // Using the cues vectors
        // Index : L motor (user viewpoint)
        // Thumb : R motor (user viewpoint)
        servosTrajectory(centerIndex, centerIndex + L[integerValue], centerThumb, centerThumb + R[integerValue], fast);
        delay(1200);
        servosTrajectory(centerIndex + L[integerValue], centerIndex, centerThumb + R[integerValue], centerThumb, slow);
        delay(600);
        // Forcing the initial position
        //indexServo.write(centerIndex);
        //thumbServo.write(centerThumb);
        //delay(200);    
      }
    }
  }
}
