// 2004.05.28, 2004.10.20, 2005.01.08, 2005.03.25, 2005.04.28, 2005.07.18, 2005.07.28, 2005.09.19, 2007.12.19, 2008.10.15, 2022.5
//****************************************
//**  Copyright  (C)  WCH  1999-2022    **
//**  Web:  http://www.winchiphead.com  **
//****************************************
//**  DLL for USB interface chip CH341  **
//**  C, VC5.0                          **
//****************************************
//
// USB interface chip CH341 parallel application interface lib V2.2
// Nanjing Qinheng Microelectronics Co., Ltd.  Author: W.ch 2008.10
// CH341-DLL  V2.2
// Runtime environment: Windows 98/ME, Windows 2000/XP/7/10/11
// support USB chip: CH341, CH341A, CH347 SPI/I2C
// USB => Parallel, I2C, SPI, ...
//

#ifndef		_CH341_DLL_H
#define		_CH341_DLL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN64
#define		mOFFSET( s, m )			( (ULONG_PTR) & ( ( ( s * ) 0 ) -> m ) )	// Defines a macro that gets the relative offset address of a structure member
#else
#define		mOFFSET( s, m )			( (ULONG) & ( ( ( s * ) 0 ) -> m ) )	// Defines a macro that gets the relative offset address of a structure member
#endif

#ifndef		max
#define		max( a, b )				( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )	// Larger value
#endif

#ifndef		min
#define		min( a, b )				( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )	// Smaller values
#endif

#ifdef		ExAllocatePool
#undef		ExAllocatePool						// Delete a memory allocation with TAG
#endif

#ifndef		NTSTATUS
typedef		LONG	NTSTATUS;					// Return status
#endif


typedef	struct	_USB_SETUP_PKT {				// USB control transmission of the establishment phase of the data request packet structure
	UCHAR			mUspReqType;				// 00H Request type
	UCHAR			mUspRequest;				// 01H Request code
	union	{
		struct	{
			UCHAR	mUspValueLow;				// 02H Value parameter low bytes
			UCHAR	mUspValueHigh;				// 03H Value parameter high bytes
		};
		USHORT		mUspValue;					// 02H-03H Value parameter
	};
	union	{
		struct	{
			UCHAR	mUspIndexLow;				// 04H Index parameter low bytes
			UCHAR	mUspIndexHigh;				// 05H Index parameter high bytes
		};
		USHORT		mUspIndex;					// 04H-05H Index parameter
	};
	USHORT			mLength;					// 06H-07H Data length of the data phase
} mUSB_SETUP_PKT, *mPUSB_SETUP_PKT;


#define		mCH341_PACKET_LENGTH	32			// Length of data packets supported by CH341
#define		mCH341_PKT_LEN_SHORT	8			// Length of short packets supported by CH341


typedef	struct	_WIN32_COMMAND {				// Define WIN32 command interface structure
	union	{
		ULONG		mFunction;					// Specify the function code or pipe number when entering
		NTSTATUS	mStatus;					// Returns operation status on output
	};
	ULONG			mLength;					// Access length, returns the length of subsequent data
	union	{
		mUSB_SETUP_PKT	mSetupPkt;				// USB controls the transmission of data requests during the setup phase
		UCHAR			mBuffer[ 512];      	// Data buffer, 0 to 255B in length
	};
} mWIN32_COMMAND, *mPWIN32_COMMAND;

// WIN32 application layer interface command
#define		IOCTL_CH341_COMMAND		( FILE_DEVICE_UNKNOWN << 16 | FILE_ANY_ACCESS << 14 | 0x0f34 << 2 | METHOD_BUFFERED )	// Special interface

#define		mWIN32_COMMAND_HEAD		mOFFSET( mWIN32_COMMAND, mBuffer )	// WIN32 command interface header length

#define		mCH341_MAX_NUMBER		16			// Maximum number of CH341 connected at the same time

#define		mMAX_BUFFER_LENGTH		0x1000		// Maximum length of the data buffer is 4096

#define		mMAX_COMMAND_LENGTH		( mWIN32_COMMAND_HEAD + mMAX_BUFFER_LENGTH )	// Maximum data length plus the length of the command structure header

#define		mDEFAULT_BUFFER_LEN		0x0400		// Default length of the data buffer is 1024

#define		mDEFAULT_COMMAND_LEN	( mWIN32_COMMAND_HEAD + mDEFAULT_BUFFER_LEN )	// Default data length plus the length of the command structure header

// CH341 endpoint address
#define		mCH341_ENDP_INTER_UP	0x81		// Address of the interrupt data upload endpoint of CH341
#define		mCH341_ENDP_INTER_DOWN	0x01		// Address of the interrupt data download endpoint of CH341
#define		mCH341_ENDP_DATA_UP		0x82		// Address of the data block upload endpoint of CH341
#define		mCH341_ENDP_DATA_DOWN	0x02		// IP address of the data block download endpoint of the CH341 

// Operation command of pipes provided by device layer interfaces
#define		mPipeDeviceCtrl			0x00000004	// Integrated control piping for CH341
#define		mPipeInterUp			0x00000005	// Interrupt data upload pipeline for CH341
#define		mPipeDataUp				0x00000006	// CH341 data block upload pipeline
#define		mPipeDataDown			0x00000007	// CH341 data block dowload pipeline

// Function code of the application interface
#define		mFuncNoOperation		0x00000000	// No operation
#define		mFuncGetVersion			0x00000001	// Gets the driver version number
#define		mFuncGetConfig			0x00000002	// Gets the USB device configuration descriptor
#define		mFuncSetTimeout			0x00000009	// Set USB communication timeout
#define		mFuncSetExclusive		0x0000000b	// Set exclusive use
#define		mFuncResetDevice		0x0000000c	// Reset USB device
#define		mFuncResetPipe			0x0000000d	// Reset USB pipe
#define		mFuncAbortPipe			0x0000000e	// Cancels data requests for USB pipes

//Function code of CH341 parallel port dedicated 
#define		mFuncSetParaMode		0x0000000f	// Set parallel port mode
#define		mFuncReadData0			0x00000010	// Read block 0 from parallel port
#define		mFuncReadData1			0x00000011	// Read block 1 from parallel port
#define		mFuncWriteData0			0x00000012	// Write data block 0 to parallel port
#define		mFuncWriteData1			0x00000013	// Write data block 1 to parallel port
#define		mFuncWriteRead			0x00000014	// Output before input
#define		mFuncBufferMode			0x00000020	// Set buffered upload mode and query the length of data in the buffer
#define		mFuncBufferModeDn		0x00000021	// Set buffered download mode and query the length of data in the buffer
#define		mFuncGetVersionEx		0x00000022	// Get driver version number and chip model

// USB device standard request code
#define		mUSB_CLR_FEATURE		0x01
#define		mUSB_SET_FEATURE		0x03
#define		mUSB_GET_STATUS			0x00
#define		mUSB_SET_ADDRESS		0x05
#define		mUSB_GET_DESCR			0x06
#define		mUSB_SET_DESCR			0x07
#define		mUSB_GET_CONFIG			0x08
#define		mUSB_SET_CONFIG			0x09
#define		mUSB_GET_INTERF			0x0a
#define		mUSB_SET_INTERF			0x0b
#define		mUSB_SYNC_FRAME			0x0c

// Vendor-specific request type of CH341 control transmission 
#define		mCH341_VENDOR_READ		0xC0		// CH341 Vendor-specific read operations which achieved by control transmission
#define		mCH341_VENDOR_WRITE		0x40		// CH341 Vendor-specific write operations which achieved by control transmission

// Vendor-specific request code of CH341 control transmission 
#define		mCH341_PARA_INIT		0xB1		// Initialize parallel port
#define		mCH341_I2C_STATUS		0x52		// Get status of the I2C interface
#define		mCH341_I2C_COMMAND		0x53		// Sends I2C interface command

// CH341 Parallel port operation command code
#define		mCH341_PARA_CMD_R0		0xAC		// Read data 0 from parallel port, subbyte length
#define		mCH341_PARA_CMD_R1		0xAD		// Read data from parallel port 1, subbyte length
#define		mCH341_PARA_CMD_W0		0xA6		// Writes data 0 to parallel port, starting with the next byte as a data stream
#define		mCH341_PARA_CMD_W1		0xA7		// Writes data 1 to parallel port, starting with the next byte as a data stream
#define		mCH341_PARA_CMD_STS		0xA0		// Gets parallel port status

// CH341A Parallel port operation command code
#define		mCH341A_CMD_SET_OUTPUT	0xA1		// Set parallel output
#define		mCH341A_CMD_IO_ADDR		0xA2		// MEM address read&write/ input&output, starting with the next byte as a command stream
#define		mCH341A_CMD_PRINT_OUT	0xA3		// Compatible PRINT print output, starting with the next byte as a code stream
#define		mCH341A_CMD_PWM_OUT		0xA4		// PWM command package for data output, starting with the next byte as a code stream
#define		mCH341A_CMD_SHORT_PKT	0xA5		// Short packet, the next byte is the actual length of the command packet, the next after next byte and the bytes after that are the original command packet
#define		mCH341A_CMD_SPI_STREAM	0xA8		// SPI interface command package, starting with the next byte as data stream
//#define		mCH341A_CMD_SIO_STREAM	0xA9		// SIO interface command package, starting with the next byte as data stream
#define		mCH341A_CMD_I2C_STREAM	0xAA		// I2C interface command package ,I2C command stream starting from subbyte
#define		mCH341A_CMD_UIO_STREAM	0xAB		// The command packet of the UIO interface, starting from the next byte is the command stream
#define		mCH341A_CMD_PIO_STREAM	0xAE		// The command packet of the PIO interface, starting from the second byte is the data stream

// Vendor-specific request code of CH341 control transmission
#define		mCH341A_BUF_CLEAR		0xB2		// Clear unfinished data
#define		mCH341A_I2C_CMD_X		0x54		// Issue the command of the I2C interface and execute it immediately
#define		mCH341A_DELAY_MS		0x5E		// Delay the specified time in milliseconds
#define		mCH341A_GET_VER			0x5F		// Get the chip version

#define		mCH341_EPP_IO_MAX		( mCH341_PACKET_LENGTH - 1 )	// The maximum length of a single read and write data block of CH341 in EPP/MEM mode
#define		mCH341A_EPP_IO_MAX		0xFF		// The maximum length of a single read and write data block for CH341A in EPP/MEM mode

#define		mCH341A_CMD_IO_ADDR_W	0x00		// MEM command stream with address read/write/input and output: write data, bit 6-bit 0 is the address, and the next byte is the data to be written
#define		mCH341A_CMD_IO_ADDR_R	0x80		// MEM command stream with address read/write/input and output: read data, bit 6-bit 0 is the address, read data and return together

#define		mCH341A_CMD_I2C_STM_STA	0x74		// Command flow of I2C interface: generate start bit
#define		mCH341A_CMD_I2C_STM_STO	0x75		// Command flow of I2C interface: generate stop bit
#define		mCH341A_CMD_I2C_STM_OUT	0x80		// Command flow of I2C interface: output data, bit 5-bit 0 is the length, the subsequent bytes are data, and only one byte is sent with a length of 0 and a response is returned
#define		mCH341A_CMD_I2C_STM_IN	0xC0		// Command flow of I2C interface: input data, bit 5-bit 0 is the length, 0 length means only one byte is received and no response is sent
#define		mCH341A_CMD_I2C_STM_MAX	( min( 0x3F, mCH341_PACKET_LENGTH ) )	// The maximum length of the input and output data of a single command in the command stream of the I2C interface
#define		mCH341A_CMD_I2C_STM_SET	0x60		// Command flow of I2C interface: set parameters, bit 2=SPI I/O number (0=single input and single output, 1=double input and double output), bit 1 bit 0=I2C speed (00=low speed, 01=standard, 10=fast, 11=high speed)
#define		mCH341A_CMD_I2C_STM_US	0x40		// Command flow of I2C interface: delay in microseconds, bit 3-bit 0 is the delay value
#define		mCH341A_CMD_I2C_STM_MS	0x50		// Command flow of I2C interface: delay in milliseconds, bit 3-bit 0 is the delay value
#define		mCH341A_CMD_I2C_STM_DLY	0x0F		// The maximum value of the single command delay of the command stream of the I2C interface
#define		mCH341A_CMD_I2C_STM_END	0x00		// Command flow of I2C interface: command packet ends prematurely

#define		mCH341A_CMD_UIO_STM_IN	0x00		// Command flow of UIO interface: input data D7-D0
#define		mCH341A_CMD_UIO_STM_DIR	0x40		// Command flow of UIO interface: set I/O direction D5-D0, bit 5-bit 0 is the direction data
#define		mCH341A_CMD_UIO_STM_OUT	0x80		// Command flow of UIO interface: output data D5-D0, bit 5-bit 0 is data
#define		mCH341A_CMD_UIO_STM_US	0xC0		// Command flow of UIO interface: delay in microseconds, bit 5-bit 0 is the delay value
#define		mCH341A_CMD_UIO_STM_END	0x20		// Command flow of UIO interface: command packet ends prematurely

// CH341 parallen port work model
#define		mCH341_PARA_MODE_EPP	0x00		// CH341 parallel port working mode is EPP mode
#define		mCH341_PARA_MODE_EPP17	0x00		// CH341A parallel port working mode is EPP mode V1.7
#define		mCH341_PARA_MODE_EPP19	0x01		// CH341A parallel port working mode is EPP mode V1.9
#define		mCH341_PARA_MODE_MEM	0x02		// CH341 parallel port working mode is MEM mode
#define		mCH341_PARA_MODE_ECP	0x03		// CH341A parallel port working mode is ECP mode

// I/O direction setting bit definition, direct input status signal bit definition, direct output bit data definition
#define		mStateBitERR			0x00000100	// Read-only and writable, ERR# pin input status, 1: high level, 0: low level
#define		mStateBitPEMP			0x00000200	// Read-only and writable, PEMP pin input state, 1: high level, 0: low level
#define		mStateBitINT			0x00000400	// Read-only and writable, INT# pin input status, 1: high level, 0: low level
#define		mStateBitSLCT			0x00000800	// Read-only and writable, SLCT pin input status, 1: high level, 0: low level
#define		mStateBitWAIT			0x00002000	// Read-only and writable, WAIT# pin input status, 1: high level, 0: low level
#define		mStateBitDATAS			0x00004000	// Only write and read, DATAS#/READ# pin input status, 1: high level, 0: low level
#define		mStateBitADDRS			0x00008000	// Write only read, ADDRS#/ADDR/ALE pin input status, 1: high level, 0: low level
#define		mStateBitRESET			0x00010000	// Write only, RESET# pin input status, 1: high level, 0: low level
#define		mStateBitWRITE			0x00020000	// Write only, WRITE# pin input status, 1: high level, 0: low level
#define		mStateBitSCL			0x00400000	// Read only, SCL pin input status, 1: high level, 0: low level
#define		mStateBitSDA			0x00800000	// Read only, SDA pin input status, 1: high level, 0: low level

#define		MAX_DEVICE_PATH_SIZE	128			// Maximum number of characters for device name
#define		MAX_DEVICE_ID_SIZE		64			// Maximum number of characters for device ID

typedef		VOID	( CALLBACK	* mPCH341_INT_ROUTINE ) (  // interrupt service routine
	ULONG			iStatus );  // Interrupt status data, refer to the following bit description
// Bit 7-bit 0 correspond to D7-D0 pins of CH341
// Bit 8 corresponds to ERR# pin of CH341, Bit 9 corresponds to PEMP pin of CH341, Bit 10 corresponds to INT# pin of CH341, Bit 11 corresponds to SLCT pin of CH341


HANDLE	WINAPI	CH341OpenDevice(  // Open the CH341 device, return the handle, if an error occurs, it will be invalid
	ULONG			iIndex );  // Specify the device serial number of CH341, 0 corresponds to the first device


VOID	WINAPI	CH341CloseDevice(  // Close the CH341 device
	ULONG			iIndex );  // Specify the serial number of the CH341 device


ULONG	WINAPI	CH341GetVersion( );  // Get the DLL version number, return the version number


ULONG	WINAPI	CH341DriverCommand(  // Directly pass the command to the driver, return 0 if there is an error, otherwise return the data length
	ULONG			iIndex,  // Specify the serial number of the CH341 device, the DLL above V1.6 can also be the handle after the device is opened
	mPWIN32_COMMAND	ioCommand );  // pointer to command structure
// The program returns the data length after the call, and still returns the command structure, if it is a read operation, the data is returned in the command structure,
// The returned data length is 0 when the operation fails, and the length of the entire command structure when the operation is successful. For example, if one byte is read, mWIN32_COMMAND_HEAD+1 is returned.
// Before the command structure is called, provide: pipe number or command function code, length of access data (optional), data (optional)
// After the command structure is called, it returns: the operation status code, the length of the subsequent data (optional),
//   The operation status code is the code defined by WINDOWS, you can refer to NTSTATUS.H,
//   The length of the subsequent data refers to the length of the data returned by the read operation. The data is stored in the subsequent buffer, and is generally 0 for the write operation.

ULONG	WINAPI	CH341GetDrvVersion( );  // Get the driver version number, return the version number, or return 0 if there is an error


BOOL	WINAPI	CH341ResetDevice(  // Reset USB device
	ULONG			iIndex );  // Specify the serial number of the CH341 device


BOOL	WINAPI	CH341GetDeviceDescr(  // Read device descriptor
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	PVOID			oBuffer,  // Points to a buffer large enough to hold the descriptor
	PULONG			ioLength );  // Points to the length unit, the length to be read when input is the length to be read, and the actual read length after return


BOOL	WINAPI	CH341GetConfigDescr(  // Read configuration descriptor
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	PVOID			oBuffer,  //  Points to a buffer large enough to hold the descriptor
	PULONG			ioLength );  // Points to the length unit, the length to be read when input is the length to be read, and the actual read length after return


BOOL	WINAPI	CH341SetIntRoutine(  // Set interrupt service routine
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	mPCH341_INT_ROUTINE	iIntRoutine );  // Specify the interrupt service routine, if it is NULL, the interrupt service will be cancelled, otherwise the routine will be called when interrupted


BOOL	WINAPI	CH341ReadInter(  // Read interrupt data
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	PULONG			iStatus );  // Points to a double word location used to store the read interrupt status data, see below
// Bit 7-bit 0 correspond to D7-D0 pins of CH341
// Bit 8 corresponds to err# pin of CH341, bit 9 corresponds to PEMP pin of CH341, bit 10 corresponds to int# pin of CH341, and bit 11 corresponds to SLCT pin of CH341

BOOL	WINAPI	CH341AbortInter(  // Abort interrupt data read operation
	ULONG			iIndex );  // Specify the serial number of the CH341 device


BOOL	WINAPI	CH341SetParaMode(  // Set parallel port mode
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	ULONG			iMode );  // Specify parallel port mode: 0 is EPP mode/EPP mode V1.7, 1 is EPP mode V1.9, 2 is MEM mode


BOOL	WINAPI	CH341InitParallel(  // Reset and initialize the parallel port, RST# outputs a low-level pulse
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	ULONG			iMode );  // Specify parallel port mode: 0 is EPP mode/EPP mode V1.7, 1 is EPP mode V1.9, 2 is MEM mode, >= 0x00000100 keep current mode


BOOL	WINAPI	CH341ReadData0(  // Read data block from 0# port
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	PVOID			oBuffer,  // Points to a buffer large enough to hold the read data
	PULONG			ioLength );  // Points to the length unit, the length to be read when input is the length to be read, and the actual read length after return


BOOL	WINAPI	CH341ReadData1(  // Read data block from port 1#
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	PVOID			oBuffer,  // Points to a buffer large enough to hold the read data
	PULONG			ioLength );  // Points to the length unit, the length to be read when input is the length to be read, and the actual read length after return


BOOL	WINAPI	CH341AbortRead(  // Abort data block read operation
	ULONG			iIndex );  // Specify the serial number of the CH341 device


BOOL	WINAPI	CH341WriteData0(  // Write data block to port 0#
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	PVOID			iBuffer,  // Pointer to a buffer where the data to be written is placed
	PULONG			ioLength );  // Points to the length unit, the length to be written out when input is the length to be written out, and the length actually written out after return


BOOL	WINAPI	CH341WriteData1(  // Write data block to port 1#
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	PVOID			iBuffer,  // Pointer to a buffer where the data to be written is placed
	PULONG			ioLength );  // Points to the length unit, the length to be written out when input is the length to be written out, and the length actually written out after return


BOOL	WINAPI	CH341AbortWrite(  // Abort block write operation
	ULONG			iIndex );  // Specify the serial number of the CH341 device


BOOL	WINAPI	CH341GetStatus(  // Input data and status directly through CH341
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	PULONG			iStatus );  // Points to a double word unit, used to save status data, refer to the following bit description
// Bit 7-bit 0 correspond to D7-D0 pins of CH341
// Bit 8 corresponds to ERR# pin of CH341, Bit 9 corresponds to PEMP pin of CH341, Bit 10 corresponds to INT# pin of CH341, Bit 11 corresponds to SLCT pin of CH341, Bit 23 corresponds to SDA pin of CH341
// Bit 13 corresponds to BUSY/WAIT# pin of CH341, Bit 14 corresponds to AUTOFD#/DATAS# pin of CH341, Bit 15 corresponds to SLCTIN#/ADDRS# pin of CH341


BOOL	WINAPI	CH341ReadI2C(  // Read one byte of data from the I2C interface
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	UCHAR			iDevice,  // The lower 7 bits specify the I2C device address
	UCHAR			iAddr,  // Address of specified data unit
	PUCHAR			oByte );  // Address of specified data unit


BOOL	WINAPI	CH341WriteI2C(  // Write a byte of data to the I2C interface
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	UCHAR			iDevice,  // The lower 7 bits specify the I2C device address
	UCHAR			iAddr,  // Address of specified data unit
	UCHAR			iByte );  // Byte data to be written


BOOL	WINAPI	CH341EppReadData(  // Byte data to be written
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	PVOID			oBuffer,  // Points to a buffer large enough to hold the read data
	PULONG			ioLength );  // Points to a buffer large enough to hold the read data


BOOL	WINAPI	CH341EppReadAddr(  // Read address in EPP mode: WR#=1, DS#=1, AS#=0, D0-D7=input
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	PVOID			oBuffer,  // Point to a buffer large enough to hold the read address data
	PULONG			ioLength );  // Points to the length unit, the length to be read when input is the length to be read, and the actual read length after return


BOOL	WINAPI	CH341EppWriteData(  // Write data in EPP mode: WR#=0, DS#=0, AS#=1, D0-D7=output
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	PVOID			iBuffer,  // Pointer to a buffer where the data to be written is placed
	PULONG			ioLength );  // Points to the length unit, the length to be written out when input is the length to be written out, and the length actually written out after return


BOOL	WINAPI	CH341EppWriteAddr(  // Write address in EPP mode: WR#=0, DS#=1, AS#=0, D0-D7=output
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	PVOID			iBuffer,  // Point to a buffer to place the address data to be written out
	PULONG			ioLength );  // Points to the length unit, the length to be written out when input is the length to be written out, and the length actually written out after return


BOOL	WINAPI	CH341EppSetAddr(  // EPP mode setting address: WR#=0, DS#=1, AS#=0, D0-D7=output
	ULONG			iIndex,  // Specify the serial number of the CH341 device
	UCHAR			iAddr );  // Specify the EPP address


BOOL	WINAPI	CH341MemReadAddr0(  // Read the address in MEM mode 0: WR#=1, DS#/RD#=0, AS#/ADDR=0, D0-D7=input
	ULONG			iIndex,  // Specify the CH341 device number
	PVOID			oBuffer,  // Points to a buffer large enough to hold the data read from address 0
	PULONG			ioLength );  // Refers to the length unit, input is the length to read, return is the actual length to read


BOOL	WINAPI	CH341MemReadAddr1(  // Read the address in MEM mode 1: WR#=1, DS#/RD#=0, AS#/ADDR=1, D0-D7=input
	ULONG			iIndex,  // Specify the CH341 device number
	PVOID			oBuffer,  // Points to a buffer large enough to hold the data read from address 1
	PULONG			ioLength );  // Refers to the length unit, input is the length to read, return is the actual length to read


BOOL	WINAPI	CH341MemWriteAddr0(  // Write the address in MEM mode 0: WR#=0, DS#/RD#=1, AS#/ADDR=0, D0-D7=output
	ULONG			iIndex,  // Specify the CH341 device number
	PVOID			iBuffer,  // Points to a buffer to place the data to be written to address 0
	PULONG			ioLength );  // Refers to the length unit. The input is the intended length and the return is the actual length


BOOL	WINAPI	CH341MemWriteAddr1(  // Write the address in MEM mode 1: WR#=0, DS#/RD#=1, AS#/ADDR=1, D0-D7=output
	ULONG			iIndex,  // Specify the CH341 device number
	PVOID			iBuffer,  // Points to a buffer to place the data to be written to address 1
	PULONG			ioLength );  // Refers to the length unit. The input is the intended length and the return is the actual length


BOOL	WINAPI	CH341SetExclusive(  // Set exclusive use of the current CH341 device
	ULONG			iIndex,  // Specify the CH341 device number
	ULONG			iExclusive );  // If the value is 0, the device can be shared. If the value is not 0, the device can be exclusively used

BOOL	WINAPI	CH341SetTimeout(  // Set the timeout of USB data read and write
	ULONG			iIndex,  // Specify the CH341 device number
	ULONG			iWriteTimeout,  // Specifies the timeout for USB to write data blocks, in milliseconds mS, 0xFFFFFFFF specifies no timeout (default)
	ULONG			iReadTimeout );  // Specifies the timeout for USB to read data blocks, in milliseconds mS, 0xFFFFFFFF specifies no timeout (default)


BOOL	WINAPI	CH341ReadData(  // Read data block
	ULONG			iIndex,  // Specify the CH341 device number
	PVOID			oBuffer,  // Points to a buffer large enough to hold the read data
	PULONG			ioLength );  // Refers to the length unit, input is the length to read, return is the actual length to read


BOOL	WINAPI	CH341WriteData(  // Write out blocks of data
	ULONG			iIndex,  // Specify the CH341 device number
	PVOID			iBuffer,  // Points to a buffer to place data ready to be written out
	PULONG			ioLength );  // Refers to the length unit. The input is the intended length and the return is the actual length


PVOID	WINAPI	CH341GetDeviceName(  // Returns a buffer pointing to the CH341 device name, or NULL on error
	ULONG			iIndex );  // Specify the CH341 device number,0 corresponds to the first device


ULONG	WINAPI	CH341GetVerIC(  // 0= device invalid,0x10=CH341,0x20=CH341A
	ULONG			iIndex );  // Specify the CH341 device number
#define		IC_VER_CH341A		0x20
#define		IC_VER_CH341A3		0x30


BOOL	WINAPI	CH341FlushBuffer(  // Clear the buffer of CH341
	ULONG			iIndex );  // Specify the CH341 device number


BOOL	WINAPI	CH341WriteRead(  // Execute the data flow command, output before input
	ULONG			iIndex,  // Specify the CH341 device number
	ULONG			iWriteLength,  // Write the length, the length you're going to write
	PVOID			iWriteBuffer,  // Points to a buffer to place data ready to be written out
	ULONG			iReadStep,  // The length of a single block to be read. The total length to be read is(iReadStep*iReadTimes)
	ULONG			iReadTimes,  // Number of times you are ready to read
	PULONG			oReadLength,  // Points to the length unit and returns the length actually read
	PVOID			oReadBuffer );  // Points to a buffer large enough to hold the read data


BOOL	WINAPI	CH341SetStream(  // Set the serial port flow mode
	ULONG			iIndex,  // Specify the CH341 device number
	ULONG			iMode );  // To specify the mode, see Downlink
// Bit 1-bit 0: I2C interface speed /SCL frequency, 00= low speed /20KHz,01= standard /100KHz(default),10= fast /400KHz,11= high speed /750KHz
// Bit 2: SPI I/O number /IO pins, 0= single in/single out (D3 clock /D5 out /D7 in)(default),1= double in/double out (D3 clock /D5 out D4 out /D7 in D6 in)
// Bit 7: Bit order in SPI bytes, 0= low first, 1= high first
// All other reservations must be 0


BOOL	WINAPI	CH341SetDelaymS(  // Set the hardware asynchronous delay to a specified number of milliseconds before the next stream operation
	ULONG			iIndex,  // Specify the CH341 device number
	ULONG			iDelay );  // Specifies the number of milliseconds to delay


BOOL	WINAPI	CH341StreamI2C(  // Process I2C data stream, 2-wire interface, clock line for SCL pin, data line for SDA pin (quasi-bidirectional I/O), speed of about 56K bytes
	ULONG			iIndex,  // Specify the CH341 device number
	ULONG			iWriteLength,  // Number of bytes of data to write out
	PVOID			iWriteBuffer,  // Points to a buffer to place data to be written, usually with the I2C device address and read/write direction bits as the first byte
	ULONG			iReadLength,  // Number of bytes of data to be read
	PVOID			oReadBuffer );  // Points to a buffer and returns the data read in


typedef	enum	_EEPROM_TYPE {					// EEPROM type
	ID_24C01,
	ID_24C02,
	ID_24C04,
	ID_24C08,
	ID_24C16,
	ID_24C32,
	ID_24C64,
	ID_24C128,
	ID_24C256,
	ID_24C512,
	ID_24C1024,
	ID_24C2048,
	ID_24C4096
} EEPROM_TYPE;


BOOL	WINAPI	CH341ReadEEPROM(  // Reads data blocks from EEPROM at a speed of about 56 KB
	ULONG			iIndex,  // Specify the CH341 device number
	EEPROM_TYPE		iEepromID,  // Specifies the EEPROM model
	ULONG			iAddr,  // Specifies the address of the data unit
	ULONG			iLength,  // Number of bytes of data to be read
	PUCHAR			oBuffer );  // Points to a buffer and returns the data read in


BOOL	WINAPI	CH341WriteEEPROM(  // Writes a data block to the EEPROM
	ULONG			iIndex,  // Specify the CH341 device number
	EEPROM_TYPE		iEepromID,  // Specifies the EEPROM model
	ULONG			iAddr,  // Specifies the address of the data unit
	ULONG			iLength,  // Number of bytes of data to write out
	PUCHAR			iBuffer );  // Points to a buffer to place data ready to be written out


BOOL	WINAPI	CH341GetInput(  // Using the CH341 to directly enter data and status is more efficient than using the CH341GetStatus function
	ULONG			iIndex,  // Specify the CH341 device number
	PULONG			iStatus );  // Points to a two-word unit that holds state data, as described in the bit description below
// Bits 7-0 correspond to the D7-D0 pins of CH341
// Bit 8 corresponds to the ERR# pin of CH341, bit 9 to the PEMP pin of CH341, bit 10 to the INT# pin of CH341, bit 11 to the SLCT pin of CH341, and bit 23 to the SDA pin of CH341
// Bit 13 corresponds to the BUSY/WAIT# pin of CH341, bit 14 corresponds to the AUTOFD#/ data # pin of CH341, and bit 15 corresponds to the SLCTIN#/ADDRS# pin of CH341


BOOL	WINAPI	CH341SetOutput(  // Set the I/O direction for the CH341 and output data over the CH341
/* ***** Use this API with care to prevent damage to the chip by changing the I/O direction to change the input pin to the output pin, resulting in short circuits with other output pins ***** */
	ULONG			iIndex,  // Specify the CH341 device number
	ULONG			iEnable,  // Data valid flag, refer to the bit description below
// If bit 0 is 1, bits 15 to 8 of iSetDataOut are valid. Otherwise, it is ignored
// If bit 1 is 1, bits 15 to 8 of iSetDirOut are valid. Otherwise, the value is ignored
// If bit 2 is 1, the 7-bit 0 of iSetDataOut is valid. Otherwise, it is ignored
// If bit 3 is 1, the iSetDirOut bit 7 to bit 0 is valid. Otherwise, the iSetDirOut bit is ignored
// If bit 4 is 1, bits 23 to 16 of iSetDataOut are valid. Otherwise, it is ignored
	ULONG			iSetDirOut,  // To set the I/O direction, pin 0 corresponds to input and pin 1 corresponds to output. In parallel port mode, the default value is 0x000FC000. Refer to the bit description below
	ULONG			iSetDataOut );  // Output data. If the I/O direction is output, then a clear 0 corresponds to pin output low level, and a position 1 corresponds to pin output high level, refer to the bit description below
// Bits 7-0 correspond to the D7-D0 pins of CH341
// Bit 8 corresponds to the ERR# pin of CH341, bit 9 to the PEMP pin of CH341, bit 10 to the INT# pin of CH341, and bit 11 to the SLCT pin of CH341
// Bit 13 corresponds to the WAIT# pin of CH341, bit 14 to the DATAS#/READ# pin of CH341, and bit 15 to the ADDRS#/ADDR/ALE pin of CH341
// The following pins can only be output, irrespective of the I/O direction: bit 16 corresponds to set # for CH341, bit 17 corresponds to WRITE# for CH341, bit 18 corresponds to SCL for CH341, and bit 29 corresponds to SDA for CH341


BOOL	WINAPI	CH341Set_D5_D0(  // Set the I/O direction of D5-D0 pins of CH341, and directly output data through THE D5-D0 pins of CH341. The efficiency is higher than that of CH341SetOutput
/* ***** Use this API with care to prevent damage to the chip by changing the I/O direction to change the input pin to the output pin, resulting in short circuits with other output pins ***** */
	ULONG			iIndex,  // Specify the CH341 device number
	ULONG			iSetDirOut,  // Set the I/O direction of each pin d5-D0. Pin 0 corresponds to input, pin 1 corresponds to output. In parallel port mode, the default value is 0x00 for all input
	ULONG			iSetDataOut );  // Set the output data of each pin of D5-D0. If the I/O direction is output, then a clear 0 corresponds to pin output low level, and a position 1 corresponds to pin output high level
// Bits 5-0 of the above data correspond to the D5-D0 pins of CH341 respectively


BOOL	WINAPI	CH341StreamSPI3(  // This API is invalid. Do not use it
	ULONG			iIndex,
	ULONG			iChipSelect,
	ULONG			iLength,
	PVOID			ioBuffer );


BOOL	WINAPI	CH341StreamSPI4(  // SPI data stream processing, 4-line interface, clock line is DCK/D3 pin, output data line is DOUT/D5 pin, input data line is DIN/D7 pin, chip line is D0/D1/D2, speed is about 68K bytes
/* SPI timing: DCK/D3 pin is clock output, default low level, DOUT/D5 pin is output during low level before clock rising edge, DIN/D7 pin is input during high level before clock falling edge */
	ULONG			iIndex,  // Specify the CH341 device number
	ULONG			iChipSelect,  // Chip selection control. If bit 7 is 0, chip selection control is ignored. If bit 7 is 1, the parameter is effective: if bit 1 is 00/01/10, D0/D1/D2 pins are selected as low level effective chip selection respectively
	ULONG			iLength,  // Number of bytes of data to be transferred
	PVOID			ioBuffer );  // Points to a buffer, places data to be written out from DOUT, and returns data to be read in from DIN


BOOL	WINAPI	CH341StreamSPI5(  // SPI data stream processing, 5-line interface, clock line is DCK/D3 pin, output data line is DOUT/D5 and DOUT2/D4 pin, input data line is DIN/D7 and DIN2/D6 pin, chip line is D0/D1/D2, speed is about 30K bytes *2
/* SPI timing: DCK/D3 pins are clock outputs and are low by default, DOUT/D5 and DOUT2/D4 pins are output during the low period before the rising edge of the clock, and DIN/D7 and DIN2/D6 pins are input during the high period before the falling edge of the clock */
	ULONG			iIndex,  // Specify the CH341 device number
	ULONG			iChipSelect,  // Chip selection control. If bit 7 is 0, chip selection control is ignored. If bit 7 is 1, the parameter is effective: if bit 1 is 00/01/10, D0/D1/D2 pins are selected as low level effective chip selection respectively
	ULONG			iLength,  // Number of bytes of data to be transferred
	PVOID			ioBuffer,  // Points to a buffer, places data to be written out from DOUT, and returns data to be read in from DIN
	PVOID			ioBuffer2 );  // Point to the second buffer, place the data to be written from DOUT2, and return the data to be read from DIN2


BOOL	WINAPI	CH341BitStreamSPI(  // SPI bit data flow processing,4 line /5 line interface, clock line for DCK/D3 pin, output data line for DOUT/DOUT2 pin, input data line for DIN/DIN2 pin, chip line for D0/D1/D2, speed about 8K bit *2
	ULONG			iIndex,  // Specify the CH341 device number
	ULONG			iLength,  // The number of bits to be transmitted at a time is a maximum of 896. It is recommended that the number not exceed 256
	PVOID			ioBuffer );  // Point to a buffer, place data to be written from DOUT/DOUT2/ d2-d0, and return data to be read from DIN/DIN2
/* SPI timing: DCK/D3 pins are clock outputs and are low by default, DOUT/D5 and DOUT2/D4 pins are output during the low period before the rising edge of the clock, and DIN/D7 and DIN2/D6 pins are input during the high period before the falling edge of the clock*/
/* One byte of the ioBuffer has 8 bits corresponding to d7-D0 pins. Bit 5 is output to DOUT, bit 4 to DOUT2, bit 2-0 to D2-d0, bit 7 is input from DIN, bit 6 is input from DIN2, bit 3 is ignored */
/* Before calling this API, CH341Set_D5_D0 should be called to set the I/O direction of the D5-D0 pins of CH341 and set the default level of the pins */


BOOL	WINAPI	CH341SetBufUpload(  // Set internal buffered upload mode
	ULONG			iIndex,  // Specify the CH341 device number,0 corresponds to the first device
	ULONG			iEnableOrClear );  // If the value is 0, the internal buffered upload mode is disabled and direct upload is used. If the value is not 0, the internal buffered upload mode is enabled and the existing data in the buffer is cleared
// If internal buffered-upload mode is enabled, the CH341 driver creation thread automatically receives USB upload data to the internal buffer and clears the existing data in the buffer. The existing data in the buffer will be returned immediately when the application calls CH341ReadData


LONG	WINAPI	CH341QueryBufUpload(  // Query the number of existing data packets in the internal upload buffer. The number of data packets is returned successfully, or -1 is returned if an error occurs
	ULONG			iIndex );  // Specify the CH341 device number,0 corresponds to the first device


BOOL	WINAPI	CH341SetBufDownload(  // Set the internal buffer down mode
	ULONG			iIndex,  // Specify the CH341 device number,0 corresponds to the first device
	ULONG			iEnableOrClear );  // If the value is 0, the internal buffered downtransmission mode is disabled and direct downtransmission is used. If the value is not 0, the internal buffered downtransmission mode is enabled and the existing data in the buffer is cleared
// If internal buffer-down mode is enabled, when an application calls CH341WriteData, it simply puts USB downpass data into an internal buffer and returns it immediately, while threads created by the CH341 driver automatically send it to completion


LONG	WINAPI	CH341QueryBufDownload(  // Query the number of remaining packets in the internal downtransmission buffer (that have not been sent). The number of successful packets is returned, and -1 is returned if an error occurs
	ULONG			iIndex );  // Specify the CH341 device number,0 corresponds to the first device


BOOL	WINAPI	CH341ResetInter(  // Reset an interrupted data read operation
	ULONG			iIndex );  // Specify the CH341 device number


BOOL	WINAPI	CH341ResetRead(  // Reset the data block read operation
	ULONG			iIndex );  // Specify the CH341 device number


BOOL	WINAPI	CH341ResetWrite(  // Reset a block write operation
	ULONG			iIndex );  // Specify the CH341 device number


typedef		VOID	( CALLBACK	* mPCH341_NOTIFY_ROUTINE ) (  // Device event notification callback program
	ULONG			iEventStatus );  // Device event and current status (defined downstream): 0= device removal event, 3= device insertion event

#define		CH341_DEVICE_ARRIVAL		3		// Device insertion event, already inserted
#define		CH341_DEVICE_REMOVE_PEND	1		// Device to be pulled out
#define		CH341_DEVICE_REMOVE			0		// The device is removed


BOOL	WINAPI	CH341SetDeviceNotify(  // Set up the device event notification procedure
	ULONG					iIndex,  // Specify the CH341 device number,0 corresponds to the first device
	PCHAR					iDeviceID,  // Optional argument pointing to a string specifying the ID of the device to be monitored, terminated with \0
	mPCH341_NOTIFY_ROUTINE	iNotifyRoutine );  // Specifies a device event callback that cancels event notification if NULL, or is called when an event is detected


BOOL	WINAPI	CH341SetupSerial(  // Set the serial port feature of the CH341. This API can only be used for CH341 chips that work in serial port mode
	ULONG					iIndex,  // Specify the CH341 device number,0 corresponds to the first device
	ULONG					iParityMode,  // Specifies the data verification mode of the CH341 serial port: NOPARITY/ODDPARITY/EVENPARITY/MARKPARITY/SPACEPARITY
	ULONG					iBaudRate );  // Set the baud rate of the CH341 serial port. The value ranges from 50 to 3000000

/*  The following apis can be used for CH341 chips that work in serial port mode. Other apis can only be used for CH341 chips that work in parallel port mode
	CH341OpenDevice
	CH341CloseDevice
	CH341SetupSerial
	CH341ReadData
	CH341WriteData
	CH341SetBufUpload
	CH341QueryBufUpload
	CH341SetBufDownload
	CH341QueryBufDownload
	CH341SetDeviceNotify
	CH341GetStatus
//  The above API is the primary API and the following API is the secondary API
	CH341GetVersion
	CH341DriverCommand
	CH341GetDrvVersion
	CH341ResetDevice
	CH341GetDeviceDescr
	CH341GetConfigDescr
	CH341SetIntRoutine
	CH341ReadInter
	CH341AbortInter
	CH341AbortRead
	CH341AbortWrite
	CH341ReadI2C
	CH341WriteI2C
	CH341SetExclusive
	CH341SetTimeout
	CH341GetDeviceName
	CH341GetVerIC
	CH341FlushBuffer
	CH341WriteRead
	CH341ResetInter
	CH341ResetRead
	CH341ResetWrite
*/
HANDLE	WINAPI	CH341OpenDeviceEx(   // Turn on the CH341 device and return the handle
    ULONG			iIndex );        // Specify the CH341 device number,0 corresponds to the first device to be inserted, and 1 corresponds to the second device to be inserted. To save device serial number resources, shut down the device after it is used up


VOID	WINAPI	CH341CloseDeviceEx(  // Close the CH341
	ULONG			iIndex );        // Specify the CH341 device number


PCHAR	WINAPI	CH341GetDeviceNameEx(   // Returns a buffer pointing to the CH341 device name, or NULL on error
	ULONG			iIndex );           // Specify the CH341 device number,0 corresponds to the first device


BOOL	WINAPI	CH341SetDeviceNotifyEx(       // Set up the device event notification procedure
	ULONG					iIndex,           // Specify the CH341 device number,0 corresponds to the first device
	PCHAR					iDeviceID,        // Optional argument pointing to a string specifying the ID of the device to be monitored, terminated with \0
	mPCH341_NOTIFY_ROUTINE	iNotifyRoutine ); // Specifies a device event callback that cancels event notification if NULL, or is called when an event is detected


#ifdef __cplusplus
}
#endif

#endif		// _CH341_DLL_H
