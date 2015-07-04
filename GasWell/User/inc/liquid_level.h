#ifndef _LIQUID_LEVEL_H_
#define _LIQUID_LEVEL_H_

typedef enum {NORMAL,ALARM} liquid_level_state_e; 

void LIQUID_LEVEL_Init(void);
void LIQUID_LEVEL_Interrupt_Handler(void);

#endif  // End of #ifndef _LIQUID_LEVEL_H_
