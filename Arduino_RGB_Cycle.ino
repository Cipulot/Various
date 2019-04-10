//Arduino LED controller
//Cycle through the color spectrum
//Last updated: 01/04/2018

#define Red_Pin 9
#define Green_Pin 10
#define Blue_Pin 11
int pot;
int R, G, B, VRed, VGreen, VBlue;

void setup() {
  Serial.begin(9600);
}

void loop() {
  pot = analogRead(A5);
  Serial.println(pot);

  if (pot == 0) {
    //white lights
    R = 255;
    G = 255;
    B = 255;
    analogWrite(Red_Pin, R);
    analogWrite(Green_Pin, G);
    analogWrite(Blue_Pin, B);
  }
  else {
    if ((pot > 1) && (pot <= 170.5)) { //0° to 60°
      R = 1023;
      G = pot * 6;
      B = 0;
    }
    if ((pot > 170.5) && (pot <= 341)) { //60° to 120°
      R = 1023 - (pot - 170.5) * 6;
      G = 1023;
      B = 0;
    }
    if ((pot > 341) && (pot <= 511.5)) { //120° to 180°
      R = 0;
      G = 1023;
      B = (pot - 341) * 6;
    }
    if ((pot > 511.5) && (pot <= 682)) { //180° to 240°
      R = 0;
      G = 1023 - (pot - 511.5) * 6;
      B = 1023;
    }
    if ((pot > 682) && (pot <= 852.5)) { //240° to 300°
      R = (pot - 682) * 6;
      G = 0;
      B = 1023;
    }
    if ((pot > 852.5) && (pot <= 1023)) { //300° to 360°
      R = 1023;
      G = 0;
      B = 1023 - (pot - 852.5) * 6;
    }
    VRed = map(R, 0, 1023, 0, 255);
    VGreen = map(G, 0, 1023, 0, 255);
    VBlue = map(B, 0, 1023, 0, 255);
    analogWrite(Red_Pin, VRed);
    analogWrite(Green_Pin, VGreen);
    analogWrite(Blue_Pin, VBlue);
  }
  delay(1);
}
