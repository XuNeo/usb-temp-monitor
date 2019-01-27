#ifndef _IIC_H_
#define _IIC_H_
#include "stdint.h"

/* SDA--> PA6, SCL-->PA5 */
#define SDA_L() GPIOA->ODR &= ~GPIO_PIN_6
#define SDA_H() GPIOA->ODR |=  GPIO_PIN_6
#define SCL_L() GPIOA->ODR &= ~GPIO_PIN_5
#define SCL_H() GPIOA->ODR |=  GPIO_PIN_5

#define SDA_Status() ((GPIOA->IDR&GPIO_PIN_6)==GPIO_PIN_6)

#define IIC_READ 0x01
#define IIC_WRITE 0x00

void IIC_Init(void);

void IIC_Start(void);
void IIC_Stop(void);
unsigned char IIC_WaitACK(void);
void IIC_ACK(void);
void IIC_NACK(void);
void IIC_WriteByte(unsigned char byte);
unsigned char IIC_ReadByte(void);

uint8_t IIC_WriteOneByte(uint8_t DEV_Addr,uint16_t addr,uint8_t Wdata);
uint8_t IIC_ReadOneByte(uint8_t DEV_Addr,uint16_t addr,uint8_t* pRdata);

#endif
