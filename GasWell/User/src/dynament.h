#ifndef __DYNAMENT_H__
#define __DYNAMENT_H__

#include <stdint.h>

#define DYNAMENT_RD                     (0x13)
#define DYNAMENT_DLE                    (0x10)
#define DYNAMENT_WR                     (0x15)
#define DYNAMENT_ACK                    (0x16)
#define DYNAMENT_NAK                    (0x19)
#define DYNAMENT_DAT                    (0x1A)
#define DYNAMENT_EOF                    (0x1F)

#define DYNAMENT_READ_LIVEDATA          (0x01)
#define DYNAMENT_READ_LIVEDATA_SIMPLE   (0x06)

#define DYNAMENT_USART_RX_MAX_LEN               30

typedef struct {
        uint8_t     *pu8Data;
        uint8_t     len;
        uint16_t    u16Crc;
}dynament_rx_t;

typedef struct {
        uint8_t     u8Size;
        uint16_t    u16Version;
        uint16_t    u16StatusFlags;
        float       fReading;
}dynament_rx_simple_t;

typedef enum {
        RX_IDLE,
        RX_START,
        RX_OK,
        RX_ERROR
}dynament_rx_status_t;

extern dynament_rx_status_t dynament_usart_rx_status;
extern int8_t dynament_usart_rx_index;
extern int8_t dynament_usart_rx_flag;
extern uint8_t dynament_usart_rx_buffer[];

void ReadLiveData(void);
int8_t CheckSum(uint8_t *buffer,int16_t size,uint16_t checksum);

#endif      // End of #ifndef __DYNAMENT_H__
