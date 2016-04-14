#include <stdlib.h>
#include <string.h>

const char titolo[] = "AAA BBB CCC DDD EEE";
const byte COUNT_KEY_N = 5;                          // Number of counters
int g_counter[COUNT_KEY_N];                          // Counters array
int c = 0;
int incr = 1;

int y = 0;
int z = 0;
int w = 0;

void setup (void) 
{
   g_counter[0] = 0;
   g_counter[1] = 0;
   g_counter[2] = 0;
   g_counter[3] = 0;
   g_counter[4] = 0;
   
   Serial.begin(115200);                             // Sets the data rate for serial transmission 
}
// _______________________________________________________________________________________

void counter_display (void)                          // SUBROUTINE: DISPLAYS THE COUNTER'S VALUES
{
   char buf[5];
   char stringa[21] = "                    ";
   byte space = 0;
   
   for (byte k=0; k<COUNT_KEY_N; k++)                // For each counter, 
   {
      itoa(g_counter[k], buf, 10);
      space = 3 - strlen(buf);
      if (space == 2)
      {
         space = 1;
      }
      for (byte i=0; buf[i]!=NULL; i++)
      {
         stringa[4*k+space+i] = buf[i];
      }
   }
   Serial.println(stringa);
}
// _______________________________________________________________________________________

void loop (void)
{
   byte x = 0;
   
   Serial.println(titolo);
   for (x=0; x<10; x++)
   {
      g_counter[c] += 1;
      counter_display ();
   }

   Serial.println(titolo);
   for (x=1; x<5; x++)
   {
      g_counter[c] += 10;
      counter_display ();
   }
   
   while(g_counter[0]>=100) {delay(1000); }

   c += incr;
   if (c > 4 || c < 0) 
   {
      incr *= -1;
      c += incr;
   }
}
