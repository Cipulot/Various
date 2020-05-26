//Parking structure
//PIC16F877A(16bit)
//Total number of spots = 512
//8 Sectors ---> each sector = 64 slots
//Need to divide the number in nibble for the 7-seg display
//MUX is used to multiplex the output line:
//PORTB= "DECINE""UNITA"(n) latch enable sequence
//for traffic light a NOT gate is used due to the fact that only red/green will be displayed
//if (total slots < 10)
//momentum switch (with rise/fall detection) for each sector to detect the enter or the exit of the car.(8IN  8OUT)
//selector used to "ask" every switch it's state.(both in and out). The output is only one

void main() {
  //******************* INIT *******************
  int TSPOTS;
  unsigned char ENTER[8], EXIT[8], UNIT, TENS, HUNDREDS, ACTIVE, LIGHTS = 0, SFULL = 64;
  unsigned char USED[8] = {10, 12, 20, 30, 40, 15, 16, 8}, USEDBCD[8], i;
  TRISA = 0B00010000; //selector line and input data
  TRISB = 0; //7-seg display data bus
  TRISC = 0; //latch enable
  TRISD = 0; //traffic lights
  TRISE = 0; //output for general status led
  do { //used to do a loop for the functions
    //******************* DISPLAY 7-seg *******************
    ACTIVE = 1; // LE selector value
    for (i = 0; i < 8; i++) {
      UNIT = USED[i] % 10; //return the UNIT of the spots
      TENS = floor(USED[i] / 10); //return the TENS of the spots
      TENS = TENS << 4; //shift the TENS value to the MSB part of the byte
      USEDBCD[i] = TENS | UNIT; //merge the UNIT and TENS to the same byte
      PORTB = USEDBCD[i]; //put the merged byte to the output port of the BCD
      PORTC = ACTIVE; //write of the LE value to allow the value to be write
      PORTC = 0B00000000; //disable the LE to allow persistence on the display
      ACTIVE = ACTIVE << 1;
    }
    //******************* ENTER & EXIT *******************
    for (i = 0; i < 8; i++) {
      PORTA = i; //take IN0
      if (PORTA.F4) { //detect rise edge for the ENTER button
        ENTER[i] = 1; //SET enter flag
      }
      if ((!PORTA.F4) && (ENTER[i] == 1)) { //detect fall edge for the ENTER button
        USED[i] = USED[i] + 1; //add 1 to the total nember of car in the 1st sector(mAX64)
        ENTER[i] = 0; //reset ENTER flag
      }
    }

    for (i = 8; i < 16; i++) {
      PORTA = i; //take OUT0
      if (PORTA.F4) { //detect rise edge for the EXIT button
        EXIT[i - 8] = 1; //set EXIT flag
      }
      if ((!PORTA.F4) && (EXIT[i - 8] == 1)) { //detect fall edge for the EXIT button
        USED[i - 8] = USED[i - 8] - 1; //subtract 1 to the total nember of car in the 1st sector(max64)
        EXIT[i - 8] = 0; //reset EXIT flag
      }
    }
    //******************* LIGHTS *******************
    ACTIVE = 1;
    for (i = 0; i < 8; i++) {
      if (USED[i] >= SFULL) {
        LIGHTS = LIGHTS | ACTIVE;
      }
      else {
        LIGHTS = LIGHTS & (~ACTIVE);//~ what does that mean?
      }
      ACTIVE = ACTIVE << 1; //shift register
    }
    PORTD = LIGHTS;

    //******************* FULL SPOTS *******************
    TSPOTS = 0; //initialize total spots to 0 
    for (i = 0; i < 8; i++) { //cycle through all 8 Sectors
      TSPOTS = TSPOTS + USED[i]; //collect used spots
    }
    if (TSPOTS > 502) { //if collected used spots are more than ~98% turn on an indicator
      PORTE = 1;
    }
    else {
      PORTE = 0;
    }
  }
  while (1);
}