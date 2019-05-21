#include "adt7420.h"
#include "i2c.h"
#include "printf.h"
#include "ush.h"

float latest_temp;

void adt7420_write_reg(uint8_t regaddr, uint8_t data){
	IIC_WriteOneByte(ADT7420_ADDR,regaddr,data);
}

float adt7420_read_temp(void){
  int16_t temp;
	uint8_t data;
	IIC_ReadOneByte(ADT7420_ADDR,0,&data);
  temp = data;
  temp <<= 8;
	IIC_ReadOneByte(ADT7420_ADDR,1,&data);
  temp |= data;
  latest_temp = temp*0.0078f;
  adt7420_write_reg(3, 0x80|(1<<5));  //16bit resolution. one shot 
	return latest_temp;
}

void cmd_read_temp(void){
  USH_Print("temp: %f\n", latest_temp);
}
USH_REGISTER(cmd_read_temp, readtemp, read latest temperature);

uint8_t adt7420_readid(void){
	uint8_t data;
	IIC_ReadOneByte(ADT7420_ADDR,0x0b,&data);
	return data;
}

void adt7420_init(void){
  uint8_t id;
  GPIO_InitTypeDef  GPIO_InitStruct;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitStruct.Pin       = GPIO_PIN_5;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_EVENTOUT;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_InitStruct.Pin       = GPIO_PIN_6;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  IIC_Init();
  adt7420_write_reg(3, 0x80|(1<<5));  //16bit resolution. one shot 
  adt7420_read_temp();
  id = adt7420_readid();
  printf("adt7420 id: 0x%02x\n", id);
}

