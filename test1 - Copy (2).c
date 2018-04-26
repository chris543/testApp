#include <C8051F310.h>
#include <math.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

ababasbasdfsd
10:44
11:03
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
#if (SPIS2_ENABLED == 1)
#define SPIS2_CONFIG_SCK_PIN         2
#define SPIS2_CONFIG_MOSI_PIN        3
#define SPIS2_CONFIG_MISO_PIN        4
#define SPIS2_CONFIG_IRQ_PRIORITY    1244

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;
String IR_DATA;
unsigned long codeValue; // The code value if not raw
//RF
const char *message = "aa Christmas";
//adf
int ID_decode = 0;

const int ID_array[8] = {
 0,0,0,0,0,0,0,0,3,3,4,4,5,5}
int ID_data[8];

void main(){
	for(;;){
		
	}
}