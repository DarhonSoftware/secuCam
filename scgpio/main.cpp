#include <stdio.h>
#include <iostream>
#include "pigpio.h"

/***************************************/
/*  Latency needs to be less than 10s  */
/***************************************/


int main(int argc, char *argv[])
{
  if (gpioInitialise() < 0)
  {
    printf("Initialisation failed\n");
    return 1;
  }

  bool bExit=false;
  do
  {
    int iGpio, iPulseWidth;
    std::cin >> iGpio >> iPulseWidth;
    if ((iGpio==-1)&&(iPulseWidth==-1))
    {
      bExit=true;
    }
    else
    {
      int iResult=gpioServo(iGpio, iPulseWidth);
      if (iResult==0)
        printf("OK");
      else
        printf("Unable to change the position of the servo.");
    }
  } while (!bExit);

  //Release resources
  gpioTerminate();
  return 0;
}
