#include <C8051F310.h>
#include <math.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------


#define  SYSCLK         24500000       	// System clock frequency in Hz

#define  SMB_FREQUENCY  50000          	// Target SCL clock rate
                                       	// This example supports between 10kHz
                                       	// and 100kHz

#define  WRITE          0x00           	// SMBus WRITE command
#define  READ           0x01           	// SMBus READ command

// Device addresses (7 bits, lsb is a don't care)
#define TSL2550			0x72			// TSL2550 light sensor address for slave target

// Status vector - top 4 bits only
#define  SMB_MTSTA      0xE0           	// (MT) start transmitted
#define  SMB_MTDB       0xC0           	// (MT) data byte transmitted
#define  SMB_MRDB       0x80           	// (MR) data byte received
// End status vector definition