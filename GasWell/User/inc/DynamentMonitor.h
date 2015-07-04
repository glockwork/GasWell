#ifndef _DYNAMENTMONITOR_H_
#define _DYNAMENTMONITOR_H_

#include "board.h"

void SetADCDMA_TransferComplete(void);
void DynamentVoltDetect(void);
float DynamentGetvoltValue(void);

#endif  // End of _DYNAMENTMONITOR_H_
