#include "usbd_cdc_if.h"
#include "fifo.h"
#include "ush.h"
#include "adt7420.h"
#include "displed.h"

fifo_def fifo_rx, fifo_tx;
ush_def ush;
uint8_t flag_auto_send = 1;

void usb2stream_init(void){
	static uint8_t rxbuff[256], txbuff[512], ush_buff[256];
	fifo_init(&fifo_rx, rxbuff, 256);
	fifo_init(&fifo_tx, txbuff, 512);
  ush_init(&ush, ush_buff, 128);
}

uint8_t UserTxBufferFS[128];
void temperature_main(void){
  uint32_t count = 0;
  float temp;
  uint32_t data_count;
	usb2stream_init();
  adt7420_init();
  displed_init();
	while(1){
    count ++;
    if(count == 700000){
      char str[8];
      count = 0;
      temp = adt7420_read_temp();
      //printf("Temp: %f\n", temp);
      temp += 0.005;
      snprintf(str, 8, "%.2fC", temp);
      str[6] = 0; //only display 5 characters.
      displed_str(str);
      if(flag_auto_send){
        USH_Print("temp: %f\n", temp);
      }
    }
    if(CDC_is_busy() == 0){
      data_count = fifo_status(&fifo_tx);
      if(data_count){
        data_count = data_count>63?63:data_count;
        fifo_read(&fifo_tx, UserTxBufferFS, &data_count);
        while(CDC_Transmit_FS(UserTxBufferFS, data_count) == USBD_BUSY);
      }
    }
    data_count = fifo_status(&fifo_rx);
		if(data_count){
      data_count = data_count>128?128:data_count;
			fifo_read(&fifo_rx, UserTxBufferFS, &data_count);
      ush_process_input(&ush, UserTxBufferFS, data_count);
    }
	}
}

uint32_t disable_auto_send(uint32_t para1, uint32_t para2){
  flag_auto_send = 0;
  return 0;
}
USH_REGISTER(disable_auto_send, disautoread, disable auto read option);\

#ifdef __GNUC__
int _write(int file, char *ptr, int len)
{
  int count = 0;
  if(is_uart_inited == 0)
  {
    is_uart_inited = 1;
    __usart_init(DEFAULT_BAUDRATE);
  }
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  while(len--)
  {
    USART_SendData(USART2, (uint8_t) *ptr++);

    /* Loop until transmit data register is empty */
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
    {}
    count++;
  }

  return count;
}
#elif defined(__ARMCC_VERSION)
void _putchar(char character)
{
  uint8_t c = character;
  uint32_t len = 1;
  fifo_write(&fifo_tx, &c, &len); 
}
#endif

