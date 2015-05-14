/**
 * spitris
 * a simple implementation of Tetris using a UC1608 b&w lcd screen (SPI) and a Wii Classic controller (i2c)
 * Just for Fun !
 * @author  Matthieu Barreteau (Matmook)
 * @email   matthieu@barreteau.org
 * @date		05/14/2015
 * ... and yes, I use "goto" in my code... :p
 */	
 
/**
 * Global include
 */
#include "project.h"

/**
 * Global vars
 */	
unsigned char dataToSend[3840];       /* data buffer for spi lcd (128x240 pixels => 3840 bytes )*/
struct spi_config spi_lcd;            /* spi configuration */
pthread_t inputThreadId;              /* wii classic management thread */
pthread_t updateScreenThreadId;       /* screen update thread */
unsigned char screenLock = 0;         /* no to update video buffer while uploading data to LCD */

/* bitmap buffer */
unsigned char	*tiles;                 /* bitmap for tiles and numbers */
unsigned char	*screenbuffer;          /* game screen */

/* game "board" */
unsigned char gamearray[200];         /* 10x20 tiles */

/* next piece to use */
unsigned char nextPiece = 0;          /* which piece to display next ? */

/* current used piece */
unsigned char currentPiece = 0;       /* which piece */
unsigned char currentRotation = 0;    /* which rotation state (0->3) */
int           currentX = 0;           /* current X position on board */
int           currentY = 0;           /* current Y position on board */

/* score */
unsigned int score = 0;               /* not really score but completed line counter */

/* our pad */
struct wii_gamepad pad;               /* wii classic game pad struct */

/* pieces (4 different rotations per piece (duplicates for some, easier to have always 4 possible rotations) */
/* 0 = tranparent   */
/* 1 = tile present */
static unsigned char tetranoid[7][4][4][4] =
{
    { // O
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        }
    },
 
    { // I
        {
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0}
        },
 
        {
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0}
        }
    },
 
    { // S
        {
            {0, 0, 0, 0},
            {0, 0, 1, 1},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 0, 1, 1},
            {0, 0, 0, 1},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 0},
            {0, 0, 1, 1},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 0, 1, 1},
            {0, 0, 0, 1},
            {0, 0, 0, 0}
        }
    },
 
    { // Z
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 1, 1},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 1},
            {0, 0, 1, 1},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 1, 1},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 1},
            {0, 0, 1, 1},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        }
    },
 
    { // L
        {
            {0, 0, 0, 0},
            {0, 1, 1, 1},
            {0, 1, 0, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 1},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 1},
            {0, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 1, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        }
    },
 
    { // J
        {
            {0, 0, 0, 0},
            {0, 1, 1, 1},
            {0, 0, 0, 1},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 1},
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 1, 0, 0},
            {0, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        }
    },
 
    { // T
        {
            {0, 0, 0, 0},
            {0, 1, 1, 1},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 0, 1, 1},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        }
    }
};


/**
 * Read 1bit bmp file and store data in memory
 */	
unsigned char *read_bmp(char *fname)
{
    unsigned char head[54];
    FILE *f = fopen(fname,"rb");

    /* get picture informations from header */
    fread(head, 1, 54, f);

    int w = head[18] + ( ((int)head[19]) << 8) + ( ((int)head[20]) << 16) + ( ((int)head[21]) << 24);
    int h = head[22] + ( ((int)head[23]) << 8) + ( ((int)head[24]) << 16) + ( ((int)head[25]) << 24);

    /* lines are aligned on 4-byte boundary */
    int lineSize = (w / 8 + (w / 8) % 4);
    int fileSize = lineSize * h;

    /* get some memory space */
    unsigned char *screenbuffer = malloc(w * h);
    unsigned char *data = malloc(fileSize);

    /* skip the header */
    fseek(f,54,SEEK_SET);

    /* skip palette, no need (1 bit) */
    fseek(f, 8, SEEK_CUR);

    /* read data */
    fread(data,1,fileSize,f);

    /* decode bits */
    int i, j, k, rev_j;
    for(j = 0, rev_j = h - 1; j < h ; j++, rev_j--) {
        for(i = 0 ; i < w / 8; i++) {
            int fpos = j * lineSize + i, pos = rev_j * w + i * 8;
            for(k = 0 ; k < 8 ; k++)
                screenbuffer[pos + (7 - k)] = (data[fpos] >> k ) & 1;
        }
    }

    free(data);    
    return screenbuffer;
}


/**
 * Transform our video buffer to LCD compliant one
 * lcd screen memory is not linear but use 8bit width column
 */
void bufferToVideo(unsigned char *buffer)
{
	unsigned char point;
  int cpt, x, y, band;
	cpt=0;
  
  /* wait if our video memory is in use */
	while(screenLock!=0) {
		delayMicroseconds(100);
	}
	
  /* remember, 8 bit bands */
	for(band=0;band<(SCREEN_WIDTH/8);band++) {
		for(y=0;y<SCREEN_HEIGHT;y++) {
			point = 0x0;
			for(x=0;x<8;x++) {
				point = point >> 1;
				point = point | (buffer[x +8*band + y*SCREEN_WIDTH] ? 0x00 : 0x80);				
			}
			dataToSend[cpt++] = (unsigned char)point;
		}
	}
}


/**
 * Read Wii Classic controller
 */
void *inputThread()
{
	unsigned char frame[6] = {0x00,0x00,0x00,0x00,0x00,0x00};  
  int i2cbus; /* file pointer for the i2c bus */
  
   /* open the bus */
  i2cbus = i2c_open(I2C_BUS_PATH);
  if( i2cbus < 0 )
  {
    /* ERROR HANDLING: you can check errno to see what went wrong */
    perror("Failed to open the i2c bus");
    goto get_out_of_here;
  }

  /* select our device on the bus */
  if (i2c_select_slave(i2cbus, I2C_WII_CLASSIC_ADDRESS) < 0) 
  {
    /* ERROR HANDLING; you can check errno to see what went wrong */
    printf("Failed to acquire bus access and/or talk to slave.\n");    
    goto get_out_of_here; 
  }
 
  /* setup wii i2c controller (disable encryption) */  
  i2c_write_byte(i2cbus, 0xF0, 0x55);
  i2c_write_byte(i2cbus, 0xFB, 0x00);
  
  /* read device id */
  i2c_write_data(i2cbus, 0xFA, frame, 0);
  i2c_read_data(i2cbus, frame, 6);
  if(!wii_find_controller_type(frame)) {
    printf("No wii compliant controller found !\n");
    goto get_out_of_here; 
  }  
  
  while(1) {
    /* ask for values from controller */
    i2c_write_data(i2cbus, 0x00, frame, 0);
    /* read values */
    i2c_read_data(i2cbus, frame, 6);
    /* decode values */
    wii_decode_controller(frame, &pad);        
    /* not need to -always- read the controller */
    delay(100); 
  }
    
get_out_of_here:
  /* DIRTY DIRTY DIRTY */
  exit(0);
}


/**
 * Update LCD screen
 */
void *updateScreenThread()
{
  unsigned char cmdToSend[5];		/* command buffer for spi lcd */
	while(1) {
		screenLock = 1;
		cmdToSend[0] = 0b00000000;	// SET COLUMN LSB
		cmdToSend[1] = 0b00010000;	// SET COLUMN MSB
		cmdToSend[2] = 0b01000000;	// SET START LINE
		cmdToSend[3] = 0b10110000;	// SET PAGE 0
		cmdToSend[4] = 0b10010000;	// SET FIXED LINE
		uc1608_send(&spi_lcd, cmdToSend, 5, CMD,  LCD_CD) ;
    /* now send our data */
		uc1608_send(&spi_lcd, dataToSend, 3840, DATA, LCD_CD) ;
		screenLock = 0;
		delay(10);
	}
}


/**
 * Bit block copy
 */
void bitblt(unsigned char *input, unsigned char *output, int srcx, int srcy, int srcwidth, int srcheight, int dstx, int dsty) {
	int cptx=0, cpty=0;
	
	for (cptx=0; cptx<srcwidth ; cptx++) {
		for (cpty=0; cpty<srcheight ; cpty++) {
			output[dstx+cptx+(dsty+cpty)*SCREEN_WIDTH] = input[srcx+cptx+(srcy+cpty)*SCREEN_WIDTH];
		}		
	}
	
}


/**
 * Hide current tetranoid (remove from game array)
 */
void hide_tetranoid() {
	unsigned char xx, yy, startx=0, starty=0;
	
  /* if we are outside the visible area... */
	if(currentY <0) { starty = -currentY; } 	
	if(currentX <0) { startx = -currentX;	} 
		
	/* first of all, remove actual piece tiles from board */	
	for(yy=starty;yy<4;yy++) {
		for(xx=startx;xx<4;xx++) {		
			if(tetranoid[currentPiece][currentRotation][yy][xx] != 0 ) {
				gamearray[xx+currentX+(yy+currentY)*10] = 0;			
			} 		
		}
	}	
}


/**
 * Show current tetranoid (add to game array)
 */
void show_tetranoid() {
	unsigned char xx, yy, startx=0 , starty=0;
	
  /* if we are outside the visible area... */
	if(currentY <0) { starty = -currentY; } 	
	if(currentX <0) { startx = -currentX;	} 
	
	/* display new piece on the board */	
	for(yy=starty;yy<4;yy++) {
		for(xx=startx;xx<4;xx++) {		
			if(tetranoid[currentPiece][currentRotation][yy][xx] != 0 ) {
				gamearray[xx+currentX+(yy+currentY)*10] = currentPiece+1;
			} 			
		}
	}	
}


/**
 * Force redraw of game array 
 * exX are lines to bypass (when lines are complete)
 */
void update_gamezone(int ex1,int ex2,int ex3,int ex4) {
	int cptx=0, cpty=0;	
  
	for (cptx=0; cptx<10 ; cptx++) {
		for (cpty=0; cpty<20 ; cpty++) {
      if(cpty==ex1 || cpty==ex2 || cpty==ex3 || cpty==ex4)
      {
        bitblt(tiles, screenbuffer, 0,0, 8,8, cptx*8+GAME_BOARD_X, cpty*8+GAME_BOARD_Y);
      } else {
        bitblt(tiles, screenbuffer, 8*gamearray[cptx+cpty*10],0, 8,8, cptx*8+GAME_BOARD_X, cpty*8+GAME_BOARD_Y);
      }
		}		
	}	
}


/**
 * Display number of lines completed 
 * score is the current number of lines
 */
void update_score(unsigned int score) {  
  bitblt(tiles, screenbuffer, 8*(score/10000),8, 8,8, SCORE_X+0*8,SCORE_Y);
  score = score%10000;
  bitblt(tiles, screenbuffer, 8*(score/1000),8, 8,8, SCORE_X+1*8,SCORE_Y);
  score = score%1000;
  bitblt(tiles, screenbuffer, 8*(score/100),8, 8,8, SCORE_X+2*8,SCORE_Y);
  score = score%100;
  bitblt(tiles, screenbuffer, 8*(score/10),8, 8,8, SCORE_X+3*8,SCORE_Y);
  score = score%10;
  bitblt(tiles, screenbuffer, 8*(score/1),8, 8,8, SCORE_X+4*8,SCORE_Y);
}


/**
 * Display next Tetranoid 
 */
void show_next_tetranoid() {
	unsigned char xx, yy;
	
	/* display new piece on the board */	
	for(yy=0;yy<4;yy++) {
		for(xx=0;xx<4;xx++) {
			if(tetranoid[nextPiece][currentRotation][yy][xx] != 0 ) {
				bitblt(tiles, screenbuffer, 8*(nextPiece+1),0, 8,8, xx*8+NEXT_PIECE_X, yy*8+NEXT_PIECE_Y);
			} else {
				bitblt(tiles, screenbuffer, 0,0, 8,8, xx*8+NEXT_PIECE_X, yy*8+NEXT_PIECE_Y);
			}
		}
	}
}


/**
 * Can we move (direction or rotation) ?
 * @param direction : future or current position
 * @param rotation : future or current rotation
 */
int tetranoid_can_move(int8_t direction,int8_t rotation)
{
	unsigned char x, y;
	int result = 1;
  
  for(y=0;y<4;y++) 
  {
    for(x=0;x<4;x++) 
    {		  
      /* only for non-blank tiles */
      if(tetranoid[currentPiece][rotation][y][x] != 0 ) 
      {       
        /* check left or right limits */
        if(direction+x < 0 || direction+x > 9)
        {
          result = 0;
          goto job_done;
        }            
        
        /* check for game array 'already present' blocks */
        if(gamearray[x+direction+(y+currentY)*10] != 0)
        {
          /* the is a block, could not move */
          result = 0;
          goto job_done;
        }
      } 			
    }
  }	
job_done:
  return result;
}


/**
 * have we reach the floor ?
 */
int tetranoid_check_floor(void)
{
  unsigned char x, y;
  int result = 0;
   
  for(y=0;y<4;y++) 
  {
    
    for(x=0;x<4;x++) 
    {		  
      /* only for non-blank tiles */
      if(tetranoid[currentPiece][currentRotation][y][x] != 0 ) 
      { 
        /* check game array bottom limit */
        if(currentY + y == 19)
        { 
          result = 1;
          goto job_done;
        } else { 
          /* could check only on visible (on screen) tiles */
          if( (currentY + y) >= 0 && gamearray[x+currentX+(y+currentY+1)*10] != 0) 
          {            
            if(currentY+1<0) {
              /* we reach the top !!! */
              result = 2; 
            } else {
              result = 1; 
            }
            goto job_done;
          } 
        }
      } 			
    }
  }	
  
job_done:    
  return result;
}


/**
 * Check for complete lines
 */
int tetranoid_check_complete_lines(char *lines)
{
  int result = 0;
  unsigned char x, y;
  unsigned linecheck = 0;
  
  /* clear full lines result (max 4 lines) */
  memset(lines,-1,4);
  
  for(y=0;y<4;y++) 
  {
    /* could check only on visible (on screen) tiles */
    if( (currentY + y) >= 0 && (currentY + y) < 20)
    {
      linecheck = 0;
      for(x=0;x<10;x++)
      {
        if(gamearray[x+(y+currentY)*10] != 0) linecheck++;
      }
      /* full line ? */
      if(linecheck == 10)
      {
        /* count multiple line...more points :) */
        lines[result++] = y+currentY;
      }
    }
  }
 
  return result;  
}


/**
 * You should always start somewhere 
 */
int main(int argc, char **argv) {
	int frameCounter = 0;
  unsigned char tempRotation = 0;
  char fullLines[4];
  int nbFull = 0;
  int x=0, cpt1=0, cpt2=0;
  
  /* Map the UART and GPIO peripherals into memory */
  iotoolsSetupMaps();
	
  /* lcd command/data selector */
  OUT_GPIO(LCD_CD);
  GPIO_CLR = 1<<LCD_CD;
  
	/* SPI setup */
  memset(&spi_lcd,0,sizeof(spi_lcd));
  spi_lcd.speed = SPI_FREQUENCY;
  spi_lcd.mode = 0|SPI_CS_HIGH;
  spi_lcd.bpw = 8;
  spi_lcd.channel = SPI_CHANNEL;
      
	if (spi_open(&spi_lcd) < 0)
	{
		fprintf (stderr, "Unable to open SPI device 0: %s\n", strerror (errno)) ;
		exit(1);
	}

  /* uc1608 screen setup */
  uc1608_setup(&spi_lcd,LCD_CD);  

  /* load tiles data from file */  
	tiles = read_bmp("tiles.bmp");
  
  /* create some parallel process */
  if(pthread_create(&inputThreadId, NULL, &inputThread, NULL) )
  {
    fprintf (stderr, "inputThread creation failed %s\n", strerror (errno)) ;
		exit(1);
  }

  if(pthread_create(&updateScreenThreadId, NULL, &updateScreenThread, NULL) )
  {
    fprintf (stderr, "inputThread creation failed %s\n", strerror (errno)) ;
		exit(1);
  }  
  
  /* here we go, display "START SCREEN" */
start_game:  
  /* clear the board */
  memset(gamearray,0,sizeof(gamearray));
  /* start on top of board */
	currentX = 3;
	currentY = -4; 	
		
  /* read our "template" */
	screenbuffer = read_bmp("pitris.bmp");  
	bufferToVideo(screenbuffer);
	delay(1000);
	
  /* please push A or B to start ... */
  while(1) {
    /* do some random stuff */
    nextPiece = (rand() % 7);
    if(pad.trigger.bits.a || pad.trigger.bits.b) {
      break;
    }	
  }
  
  /* serious game start here */
  score = 0;
  update_score(score);  
	show_next_tetranoid();

	while(1) {
		hide_tetranoid();	
		
    /* go left */
    if(pad.trigger.bits.left) {
      if(tetranoid_can_move(currentX-1,currentRotation)) currentX--;
    }   
    
    /* go right */
    if(pad.trigger.bits.right) {
      if(tetranoid_can_move(currentX+1,currentRotation)) currentX++;	
    } 
    
    /* rotate left */
    if(pad.trigger.bits.b) {      
      tempRotation = currentRotation + 1;
      if(tempRotation>3) tempRotation = 0;
      
      if(tetranoid_can_move(currentX,tempRotation)) currentRotation = tempRotation;        
    }	
    
    /* rotate right */
    if(pad.trigger.bits.a) {
      tempRotation = currentRotation;
      if(tempRotation==0) tempRotation = 4;
      tempRotation--;	
      
      if(tetranoid_can_move(currentX,tempRotation)) currentRotation = tempRotation;
    }	
    
    /* get out of here */
    if(pad.trigger.bits.home) {			
      exit(0);
    }
		
    /* push this piece down slowly or now if down button is set */
		frameCounter++;
		if(frameCounter>1 || pad.previous_state.bits.down) {
			currentY++;      
			frameCounter = 0;
		}
		
    /* read the floor ? */
    cpt1=tetranoid_check_floor();
		if(cpt1)
    {
      /* we lose ? */
      if(cpt1==2) {
        goto we_loose;
      } 
        
      show_tetranoid();  
      /* check full lines */
      nbFull = tetranoid_check_complete_lines(fullLines);
      if(nbFull>0)
      {    
        /* little animation (blinking lines) */        
        for(cpt2=0;cpt2<5;cpt2++)
        {
          update_gamezone(fullLines[0],fullLines[1],fullLines[2],fullLines[3]);
          bufferToVideo(screenbuffer);
          delay(150);
          update_gamezone(-1,-1,-1,-1);
          bufferToVideo(screenbuffer);
          delay(150);
        }
        for(cpt2=0;cpt2<nbFull;cpt2++)
        {
          /* remove line from game array */
          for(cpt1=fullLines[cpt2];cpt1>0;cpt1--)
          {          
            for(x=0;x<10;x++)
            {
              gamearray[x+cpt1*10] = gamearray[x+(cpt1-1)*10];
            }
          }
        }
        /* clear first line (remember, things go down :) ) */
        for(x=0;x<10;x++)
        {
          gamearray[x] = 0;
        }   

        score += nbFull;
        update_score(score);
        
      }

      /* prepare next piece */
     	currentX = 3;
      currentY = -4;
      currentRotation = 0;
      currentPiece = nextPiece;
      nextPiece = (rand() % 7);
      show_next_tetranoid(); 
    } else {
      show_tetranoid();    
    }    
    
    update_gamezone(-1,-1,-1,-1);    
		bufferToVideo(screenbuffer);
		delay(100);
	}

we_loose:
  /* little animation (fill the game area) */        
  for(cpt1=19;cpt1>=0;cpt1--)
  {
    for(cpt2=0;cpt2<10;cpt2++)
    {
      gamearray[cpt2+cpt1*10] = 9;
    }
    update_gamezone(-1,-1,-1,-1);
    bufferToVideo(screenbuffer);
    delay(50);     
  }      
  goto start_game;
  
	return 0 ;
}
/* this line is empty.... but must exist ! */
