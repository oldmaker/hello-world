/*
 * Demo_NHD0420CW-Ax3_parallel_6800_4b.ino
 * 
 * Tutorial sketch for use of character OLED slim display family by Newhaven with Arduino Uno, without 
 * using any library.  Models: NHD0420CW-Ax3, NHD0220CW-Ax3, NHD0216CW-Ax3. Controller: US2066
 * In this example, the display is connected to Arduino via 8-bit 6800 parallel interface.
 *
 * Displays on the OLED alternately a 4-line message and a sequence of character "block".
 * This sketch assumes the use of a 4x20 display; if different, modify the values of the two variables 
 * ROW_N e COLUMN_N.
 * The sketch uses the minimum possible of Arduino's pins; if you intend to use also R/W, /CS or /RES 
 * lines, the related instructions are already present, it's sufficient to remove the comment markers.
 *
 * The circuit:
 * OLED pin 1 (Vss)    to Arduino pin ground
 * OLED pin 2 (VDD)    to Arduino pin 5V
 * OLED pin 3 (REGVDD) to Arduino pin 5V
 * OLED pin 4 (D/C)    to Arduino pin D2
 * OLED pin 5 (R/W)    to Vss ground (always write); to enable also read, connect to Arduino pin D13
 * OLED pin 6 (E)      to Arduino pin D3
 * OLED pin 7 (DB0)    to Vss ground
 * OLED pin 8 (DB1)    to Vss ground
 * OLED pin 9 (DB2)    to Vss ground
 * OLED pin 10 (DB3)   to Vss ground
 * OLED pin 11 (DB4)   to Arduino pin D8
 * OLED pin 12 (DB5)   to Arduino pin D9
 * OLED pin 13 (DB6)   to Arduino pin D10
 * OLED pin 14 (DB7)   to Arduino pin D11
 * OLED pin 15 (/CS)   to Vss ground  (or to Arduino pin D12, in case of use of more than one display)
 * OLED pin 16 (/RES)  to Arduino pin Reset or VDD 5V (or to Arduino pin D13, to control reset by sw)
 * OLED pin 17 (BS0)   to VDD 5V
 * OLED pin 18 (BS1)   to Vss ground
 * OLED pin 19 (BS2)   to VDD 5V
 * OLED pin 20 (Vss)   to Vss ground
 *
 * Original example created by Newhaven Display International Inc.
 * Modified and adapted to Arduino Uno 16 Mar 2015 by Pasquale D'Antini
 * Modified 19 May 2015 by Pasquale D'Antini
 *
 * This example code is in the public domain.
 */

const byte ROW_N = 4;                 // Number of display rows
const byte COLUMN_N = 20;             // Number of display columns

const byte DC = 2;                    // Arduino's pin assigned to the D/C line
//const byte RW = 13;                 // Arduino's pin assigned to the R/W line (optional, can be always low)
const byte E = 3;                     // Arduino's pin assigned to the E line
//const byte CS = 12;                 // Arduino's pin assigned to the /CS line (optional, can be always low)
//const byte RES = 13;                // Arduino's pin assigned to the Reset line (optional, can be always high)
const byte DATA_PINS[4] = {8, 9, 10, 11};                  // Arduino's pins assigned to the data bus

const byte TEXT[4][21] = {"1-Newhaven Display--", 
                          "2-------Test--------", 
                          "3-16/20-Characters--", 
                          "4!@#$%^&*()_+{}[]<>?"};         // Strings to be displayed

byte new_line[4] = {0x80, 0xA0, 0xC0, 0xE0};               // DDRAM address for each line of the display
byte rows = 0x08;                     // Display mode: 1/3 lines or 2/4 lines; default 2/4 (0x08)
// _______________________________________________________________________________________

void command(byte c)                  // SUBROUTINE: PREPARES THE TRANSMISSION OF A COMMAND
{
   digitalWrite(DC, LOW);             // Sets LOW the D/C line of the display -> command
//   digitalWrite(RW, LOW);           // Sets LOW the R/W line of the display (optional, can be always low)
   send4bit(c >> 4);                  // Sends the higher 4 bits on the data bus
   enableCycle();                     // Calls the enable signal cycle subroutine
   send4bit(c);                       // Sends the lower 4 bits on the data bus
   enableCycle();                     // Calls the enable signal cycle subroutine
}
// _______________________________________________________________________________________

void data(byte d)                     // SUBROUTINE: PREPARES THE TRANSMISSION OF A BYTE OF DATA
{
   digitalWrite(DC, HIGH);            // Sets HIGH the D/C line of the display -> data
//   digitalWrite(RW, LOW);           // Sets LOW the R/W line of the display (optional, can be always low)
   send4bit(d >> 4);                  // Sends the higher 4 bits on the data bus
   enableCycle();                     // Calls the enable signal cycle subroutine
   send4bit(d);                       // Sends the lower 4 bits on the data bus
   enableCycle();                     // Calls the enable signal cycle subroutine
}
// _______________________________________________________________________________________

void enableCycle(void)                // SUBROUTINE: EXECUTE THE ENABLE SIGNAL CYCLE (DATA LATCH)
{
   delayMicroseconds(1);              // Waits 1 us (required for timing purpose)
//   digitalWrite(CS, LOW);           // Sets LOW the /CS line of the display (optional, can be always low)
   digitalWrite(E, HIGH);             // Sets HIGH the E line of the display
   delayMicroseconds(1);              // Waits 1 us (required for timing purpose)
   digitalWrite(E, LOW);              // Sets LOW the E line of the display
//   digitalWrite(CS, HIGH);          // Sets HIGH the /CS line of the display (optional, can be always low)
   delayMicroseconds(1);              // Waits 1 us (required for timing purpose)
}
// _______________________________________________________________________________________

void send4bit(byte value)             // SUBROUTINE: SENDS HALF BYTE ON THE DATA BUS
{
   for (byte i = 0; i < 4; i++)                          // One bit at a time,
   {
      digitalWrite(DATA_PINS[i], (value >> i) & 0x01);   //  sets the four lines of the data bus, 
   }                                                     //  to send the half character to the display
}
// _______________________________________________________________________________________

void output(void)                     // SUBROUTINE: DISPLAYS THE FOUR STRINGS, THEN THE SAME IN REVERSE ORDER
{
   byte r = 0;                        // Row index
   byte c = 0;                        // Column index

   command(0x01);                     // Clears display (and cursor home)
   delay(2);                          // After a clear display, a minimum pause of 1-2 ms is required
   
   for (r=0; r<ROW_N; r++)            // One row at a time,
   {
      command(new_line[r]);           //  moves the cursor to the first column of that line
      for (c=0; c<COLUMN_N; c++)      // One character at a time, 
      {
         data(TEXT[r][c]);            //  displays the correspondig string
      }
   }

   delay(2000);                       // Waits, only for visual effect purpose
   
   for (r=0; r<ROW_N; r++)            // One row at a time,
   {
      command(new_line[r]);           //  moves the cursor to the first column of that line
      for (c=0; c<COLUMN_N; c++)      // One character at a time, 
      {
         data(TEXT[3-r][c]);          //  displays the correspondig string (in reverse order)
      }
   }
}
// _______________________________________________________________________________________

void blocks(void)                     // SUBROUTINE: FILLS THE ENTIRE DISPLAY WITH THE CHARACTER "BLOCK"
{
   byte r = 0;                        // Row index
   byte c = 0;                        // Column index

   command(0x01);                     // Clear display (and cursor home)
   delay(2);                          // After a clear display, a minimum pause of 1-2 ms is required

   for (r=0; r<ROW_N; r++)            // One row at a time,
   {
      command(new_line[r]);           //  moves the cursor to the first column of that line
      for (c=0; c<COLUMN_N; c++)      // One character at a time, 
      {
         data(0xDB);                  //  displays the character 0xDB (block)
         delay(50);                   // Waits, only for visual effect purpose
      }
      delay(500);                     // Waits, only for visual effect purpose
   }
}
// _______________________________________________________________________________________

void setup(void)                      // INITIAL SETUP
{
   pinMode(4, OUTPUT);
   digitalWrite(4, LOW);
   pinMode(5, OUTPUT);
   digitalWrite(5, LOW);
   pinMode(6, OUTPUT);
   digitalWrite(6, LOW);
   pinMode(7, OUTPUT);
   digitalWrite(7, LOW);
   pinMode(DC, OUTPUT);               // Initializes Arduino pin for the D/C line
   digitalWrite(DC, LOW);             // Sets LOW the D/C line of the display
//   pinMode(RW, OUTPUT);             // Initializes Arduino pin for the R/W line (optional)
//   digitalWrite(RW, LOW);           // Sets LOW the R/W line of the display (optional, can be always low)
   pinMode(E, OUTPUT);                // Initializes Arduino pin for the E line
   digitalWrite(E, LOW);              // Sets LOW the E line of the display
//   pinMode(CS, OUTPUT);             // Initializes Arduino pin for the /CS line (optional)
//   digitalWrite(CS, HIGH);          // Sets HIGH the /CS line of the display (optional, can be always low)
   for (byte i=0; i<4; i++)
   {
      pinMode(DATA_PINS[i], OUTPUT);     // Initializes all Arduino pins for the data bus
      digitalWrite(DATA_PINS[i], LOW);   // Sets LOW the data bus
   }
//   pinMode(RES, OUTPUT);            // Initializes Arduino pin for the Reset line (optional)
//   digitalWrite(RES, HIGH);         // Sets HIGH the Reset line of the display (optional, can be always high)
   delayMicroseconds(200);            // Waits 200 us for stabilization purpose
   
   if (ROW_N == 2 || ROW_N == 4)
      rows = 0x08;                    // Display mode: 2/4 lines
   else
      rows = 0x00;                    // Display mode: 1/3 lines
   
   command(0x22 | rows); // Function set: extended command set (RE=1), lines #
   command(0x71);        // Function selection A:
   data(0x5C);           //  enable internal Vdd regulator at 5V I/O mode (def. value) (0x00 for disable, 2.8V I/O)
   command(0x20 | rows); // Function set: fundamental command set (RE=0) (exit from extended command set), lines #
   command(0x08);        // Display ON/OFF control: display off, cursor off, blink off (default values)
   command(0x22 | rows); // Function set: extended command set (RE=1), lines #
   command(0x79);        // OLED characterization: OLED command set enabled (SD=1)
   command(0xD5);        // Set display clock divide ratio/oscillator frequency:
   command(0x70);        //  divide ratio=1, frequency=7 (default values)
   command(0x78);        // OLED characterization: OLED command set disabled (SD=0) (exit from OLED command set)
   
   if (ROW_N > 2)
      command(0x09);  // Extended function set (RE=1): 5-dot font, B/W inverting disabled (def. val.), 3/4 lines
   else
      command(0x08);  // Extended function set (RE=1): 5-dot font, B/W inverting disabled (def. val.), 1/2 lines
   
   command(0x06);        // Entry Mode set - COM/SEG direction: COM0->COM31, SEG99->SEG0 (BDC=1, BDS=0)
   command(0x72);        // Function selection B:
   data(0x0A);           //  ROM/CGRAM selection: ROM C, CGROM=250, CGRAM=6 (ROM=10, OPR=10)
   command(0x79);        // OLED characterization: OLED command set enabled (SD=1)
   command(0xDA);        // Set SEG pins hardware configuration:
   command(0x10);        //  alternative odd/even SEG pin, disable SEG left/right remap (default values)
   command(0xDC);        // Function selection C:
   command(0x00);        //  internal VSL, GPIO input disable
   command(0x81);        // Set contrast control:
   command(0x7F);        //  contrast=127 (default value)
   command(0xD9);        // Set phase length:
   command(0xF1);        //  phase2=15, phase1=1 (default: 0x78)
   command(0xDB);        // Set VCOMH deselect level:
   command(0x40);        //  VCOMH deselect level=1 x Vcc (default: 0x20=0,77 x Vcc)
   command(0x78);        // OLED characterization: OLED command set disabled (SD=0) (exit from OLED command set)
   command(0x20 | rows); // Function set: fundamental command set (RE=0) (exit from extended command set), lines #
   command(0x01);        // Clear display
   delay(2);             // After a clear display, a minimum pause of 1-2 ms is required
   command(0x80);        // Set DDRAM address 0x00 in address counter (cursor home) (default value)
   command(0x0C);        // Display ON/OFF control: display ON, cursor off, blink off
   delay(250);           // Waits 250 ms for stabilization purpose after display on
   
   if (ROW_N == 2)
      new_line[1] = 0xC0;             // DDRAM address for each line of the display (only for 2-line mode)
}
// _______________________________________________________________________________________

void loop(void)                       // MAIN PROGRAM
{  
   output();                          // Execute subroutine "output"
   delay(2000);                       // Waits, only for visual effect purpose
   blocks();                          // Execute subroutine "blocks"
   delay(2000);                       // Waits, only for visual effect purpose
}
