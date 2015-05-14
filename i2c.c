/****************************************************************************/
/***        i2c	                                                          ***/
/****************************************************************************/

#include "i2c.h"

/****************************************************************************/
/***        i2c_open                                                      ***/
/****************************************************************************/
int i2c_open(char *buspath)
{
  /* check if we can access to the i2c bus */
  return open(buspath, O_RDWR);   
}

/****************************************************************************/
/***        i2c_select_slave                                              ***/
/****************************************************************************/
int i2c_select_slave(int devicebus, int slave_address)
{
  if (ioctl(devicebus, I2C_SLAVE, slave_address) < 0) 
  {
    return -1;
  }
  return 0;
}

/****************************************************************************/
/***        i2c_write_data                                                ***/
/****************************************************************************/
int i2c_write_data(int devicebus, unsigned char register_number, unsigned char * data, unsigned char length)
{
  char frame[10];
  
  /* check limit */
  if(length>9) return -1;
  
  /* create i2c frame : register number + data */
  frame[0] = register_number;
  memcpy(&frame[1],data,length);
  
  /* send the frame */
  if (write(devicebus, frame, length+1) != (length+1) ) {
    return -1;
  }  
  return 0;
}

/****************************************************************************/
/***        i2c_write_byte                                                ***/
/****************************************************************************/
int i2c_write_byte(int devicebus, unsigned char register_number, unsigned char data)
{
  char frame[2];
  
  /* create i2c frame : register number + data */
  frame[0] = register_number;
  frame[1] = data;
  
  /* send the frame */
  if (write(devicebus, frame, 2) != 2 ) {   
    return -1;
  }  
  return 0;
}


/****************************************************************************/
/***        i2c_read_byte                                                 ***/
/****************************************************************************/
unsigned char i2c_read_byte(int devicebus, unsigned char register_number)
{
  char data = register_number;  
  
  /* send the frame */
  if (read(devicebus, &data, 1) != 1 ) {
    return -1;
  }  
  return data;
}


/****************************************************************************/
/***        i2c_read_data                                                 ***/
/****************************************************************************/
char i2c_read_data(int devicebus, unsigned char *data, unsigned char length)
{ 
  /* send the frame */
  if (read(devicebus, data, length) != length ) {
    return -1;
  }  
  return 0;
}


/* EOF */