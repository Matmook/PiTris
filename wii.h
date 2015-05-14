#ifndef __WII_I2C_H__
#define __WII_I2C_H__

#include <string.h>

union wii_controller_buttons {
  struct {
    unsigned char always_one:1;
    unsigned char right_trigger:1;
    unsigned char plus:1;
    unsigned char home:1;
    unsigned char minus:1;
    unsigned char left_trigger:1;
    unsigned char down:1;
    unsigned char right:1;
    
    unsigned char up:1;
    unsigned char left:1;
    unsigned char right_z:1;
    unsigned char x:1;
    unsigned char a:1;    
    unsigned char y:1;    
    unsigned char b:1;    
    unsigned char left_z:1;        
  } bits;    
  struct {
    unsigned char raw1;       
    unsigned char raw2;       
  } word;
};


struct wii_gamepad
{		
  union wii_controller_buttons trigger;
  union wii_controller_buttons changed;
  union wii_controller_buttons previous_state;
};

unsigned char wii_find_controller_type(unsigned char *data);
void wii_decode_controller(unsigned char *data, struct wii_gamepad *pad);

#endif
