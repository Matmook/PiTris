/****************************************************************************/
/***        uc1608                                                        ***/
/****************************************************************************/

#include "uc1608.h"

/**
 * uc1608_send		send chunk over SPI
 * @param		spi config
 * @param		command or data array ptr
 * @param		size of command or data to send
 * @param		command or data mode
 * @param		io pin for data/cmd mode
 * @author		Matthieu Barreteau
 * @date		04/26/2014
 */
int uc1608_send(struct spi_config *config,unsigned char *data, int len,uint8_t mode,uint8_t cmdPin)
{
		delayMicroseconds(10);
		if(mode == UC1608_DATA) {			
      GPIO_SET = 1<<cmdPin;
		} else {
			GPIO_CLR = 1<<cmdPin;
		}
		delayMicroseconds(75);

		if(spi_write_data(config, data, len) == -1) {
      fprintf (stderr, "Unable to send over SPI : %s\n", strerror (errno)) ;
      return -1;
		}

		return 0;
}


/**
 * uc1608_setup		screen setup
 * @param		spi config
 * @param		io pin for data/cmd mode
 * @author		Matthieu Barreteau
 * @date		04/26/2014
 */
void uc1608_setup(struct spi_config *config,uint8_t cmdPin)
{
  unsigned char cmdToSend[14];
  
  cmdToSend[0] = 0b11100010;		// SYSTEM RESET
	uc1608_send(config, cmdToSend, 1, UC1608_CMD, cmdPin) ;
	delay(300);
	cmdToSend[0] = 0b00000000;		// SET COLUMN LSB
	cmdToSend[1] = 0b00010000;		// SET COLUMN MSB
	cmdToSend[2] = 0b01000000;		// SET START LINE
	cmdToSend[3] = 0b00100100;		// SET MUX & TEMP. COMPENSATION
	cmdToSend[4] = 0b00101101;		// SET POWER CONTROL
	cmdToSend[5] = 0b10000001;		// SET GAIN & POTENTIOMETER
	cmdToSend[6] = 0b10000000;		// SET GAIN & POTENTIOMETER (second part)
	cmdToSend[7] = 0b10001001;		// RAM CONTROL
	cmdToSend[8] = 0b10100100;		// ALL PIX OFF
	cmdToSend[9] = 0b10100100;		// ALL PIX OFF
	cmdToSend[10] = 0b10101111;		// SLEEP MODE OFF
	cmdToSend[11] = 0b11001100;		// MY=1 MX=1 MSF=0
	cmdToSend[12] = 0b11101010;		// BIAS=12
	cmdToSend[13] = 0b10010000;		// fixed line
	uc1608_send(config, cmdToSend, 14, UC1608_CMD, cmdPin) ;
}
/* EOF */