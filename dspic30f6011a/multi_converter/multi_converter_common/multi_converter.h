

#include  "ComPort1.h"
#include  "ComPort2.h"

#define		BAUD_RATE_9600		104
#define		BAUD_RATE_19200		52	
#define		BAUD_RATE_38400		26	
#define		BAUD_RATE_57600		16	


#define         EOT             0x04
#define         ETX             0x03
#define         ENQ             0x05
#define         ACK             0x06



#define         STX_CHK         0
#define         ADDRESS_CHK     1
#define         COMMAND_CHK     2
#define         DATACNT_CHK     3
#define         DATA_CHK        4
#define         CHKSUM_CHK      5
#define         ETX_CHK         6
#define         RX_ERROR        7
#define         RX_GOOD         8
#define         TX_SET          9
#define         CHKSUM_LOW_CHK  10
#define         CHKSUM_HIGH_CHK 11
#define         TX_ALL          12
#define         BCC1_CHK        13
#define         BCC2_CHK        14
