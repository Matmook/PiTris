/**   
 * iotools.c
 * @author		Matthieu Barreteau 
 */	

#include "iotools.h"
 
volatile unsigned *iotoolsUart;
volatile unsigned *iotoolsGpio;
 
void iotoolsSetupMaps(void)
{
  iotoolsUart = iotoolsMapPeripheral(UART_BASE);
  iotoolsGpio = iotoolsMapPeripheral(GPIO_BASE);
}

// Map a block of peripheral registers into memory for easy access
volatile unsigned * iotoolsMapPeripheral(unsigned int address)
{
  void *map;
  int  mem_fd;

  if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
          printf("can't open /dev/mem \n");
          exit(-1);
  }

  map = mmap(NULL, BLOCK_SIZE, PROT_READ|PROT_WRITE,
          MAP_SHARED, mem_fd, address);

  close(mem_fd);

  if (map == MAP_FAILED) {
          printf("mmap error %d\n", (int)map);//errno also set!
          exit(-1);
  }

  return (volatile unsigned *)map;
}

void iotoolsWaitEndOfTX(void)
{
  /* Following the transmit, monitor the TXFE flag (Transmit Fifo Empty). */
  while ( !(iotoolsUart[FR] & FR_TXFE)) 
  {
    continue;
  }
  
  /* Once the FIFO is empty monitor the BUSY flag.  This will be true */
  /* while the UART is transmitting the last character from the FIFO. */
  while ((iotoolsUart[FR] & FR_BUSY))
  {
    continue;
  }
}

int iotoolsGetRPiRevision(void)
{
  /* from WiringPi */
  FILE *cpuFd;
  char line[120];
  char *c ;
   
  if ((cpuFd = fopen ("/proc/cpuinfo", "r")) == NULL)
  {
    printf("Unable to open /proc/cpuinfo");
    exit(0);
  }
  
  // Start by looking for the Architecture, then we can look for a B2 revision....

  while (fgets (line, 120, cpuFd) != NULL)
    if (strncmp (line, "Revision", 8) == 0)
      break ;

  if (strncmp (line, "Revision", 8) != 0)
  {
    printf("Unable to find CPU revision");
    exit(0);
  }
  
  // Chomp trailing CR/NL
  for (c = &line [strlen (line) - 1] ; (*c == '\n') || (*c == '\r') ; --c)
    *c = 0 ;

  // Scan to first digit
  for (c = line ; *c ; ++c)
    if (isdigit (*c))
      break ;

  if (!isdigit (*c) || (strlen (c) < 4))
  {
    printf("Bad CPU revision format");
    exit(0);
  } 
  
  c = c + strlen (c) - 4;
  
  return atoi(c);
}

/* end of file */