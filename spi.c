/****************************************************************************/
/***        spi	                                                          ***/
/****************************************************************************/

#include "spi.h"

/****************************************************************************/
/***        spi_open                                                      ***/
/****************************************************************************/
int spi_open(struct spi_config *config)
{
  const static char *spiDev0  = "/dev/spidev0.0" ;
  const static char *spiDev1  = "/dev/spidev0.1" ;
  
  config->channel &= 1 ;

  if ((config->fd = open(config->channel == 0 ? spiDev0 : spiDev1, O_RDWR)) < 0)
    return -1;  // Unable to open SPI device   

  if (ioctl(config->fd, SPI_IOC_WR_MODE, &config->mode) < 0)
    return -2; // SPI Mode Change failure
  
  if (ioctl(config->fd, SPI_IOC_WR_BITS_PER_WORD, &config->bpw) < 0)
    return -3;  // SPI BPW Change failure

  if (ioctl(config->fd, SPI_IOC_WR_MAX_SPEED_HZ, &config->speed)   < 0)
    return -4;  // SPI Speed Change failure

  return config->fd;
}


/****************************************************************************/
/***        spi_write_data (no read in this version)                      ***/
/****************************************************************************/
int spi_write_data(struct spi_config *config, unsigned char *data, int len)
{
  struct spi_ioc_transfer spi;
     
  spi.tx_buf        = (unsigned long)data;
  spi.rx_buf        = (unsigned long)NULL;
  spi.len           = len;
  spi.delay_usecs   = 0;  // use zero delay
  spi.speed_hz      = config->speed;
  spi.bits_per_word = config->bpw;
  spi.pad           = 0;
  
  return ioctl(config->fd, SPI_IOC_MESSAGE(1), &spi);
}

/* EOF */