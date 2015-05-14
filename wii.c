/****************************************************************************/
/***        wii	                                                          ***/
/****************************************************************************/

#include "wii.h"

/****************************************************************************/
/***        wii_decode_controller                                           ***/
/****************************************************************************/
void wii_decode_controller(unsigned char *data, struct wii_gamepad *pad)
{
  /* which button has changed ? */
  pad->changed.word.raw1 = (~(data[4]) & 0xFF)^pad->previous_state.word.raw1;
  pad->changed.word.raw2 = (~(data[5]) & 0xFF)^pad->previous_state.word.raw2;
  
  pad->trigger.word.raw1 = ((~(data[4]) & 0xFF)^pad->previous_state.word.raw1)&(~(data[4]) & 0xFF);
  pad->trigger.word.raw2 = ((~(data[5]) & 0xFF)^pad->previous_state.word.raw2)&(~(data[5]) & 0xFF);

  /* store current state for next time */
  pad->previous_state.word.raw1 = (~(data[4]) & 0xFF);
  pad->previous_state.word.raw2 = (~(data[5]) & 0xFF);
  
}


/****************************************************************************/
/***        wii_find_controller_type                                       ***/
/****************************************************************************/
unsigned char wii_find_controller_type(unsigned char *data)
{
  const unsigned char NUNCHUK[6]                = {0x00, 0x00, 0xA4, 0x20, 0x00, 0x00 };
  const unsigned char CLASSIC_CONTROLLER[6]      = {0x00, 0x00, 0xA4, 0x20, 0x01, 0x01 };
  const unsigned char CLASSIC_CONTROLLER_PRO[6]  = {0x01, 0x00, 0xA4, 0x20, 0x01, 0x01 };  
  
  if(memcmp(data,NUNCHUK,6)==0)                return 0;
  if(memcmp(data,CLASSIC_CONTROLLER,6)==0)     return 1;
  if(memcmp(data,CLASSIC_CONTROLLER_PRO,6)==0) return 1;
  
  return 0;
}

/* EOF */