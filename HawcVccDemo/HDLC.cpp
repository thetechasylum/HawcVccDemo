//!\addtogroup HDLC
//!@{
//##################################################################################################
//
//
//
//##################################################################################################
//##################################################################################################
// Include Files
//==================================================================================================
// Standard includes
#include <iostream> //ToDo:<-remove

// Project includes
#include "CRC.h"

#include "HDLC.h"

//##################################################################################################
// Local MACRO Definitions
//==================================================================================================
#define BITS_PER_BYTE				(8)

#define FRAME_CHAR					(0x7e)
#define ESCAPE_CHAR					(0x7d)
#define ESCAPE_FLAG					(0x20)

#define MSG_START_FRAME_LOC			(0)
#define MSG_ADDR_LOC				(MSG_START_FRAME_LOC + 1)
#define MSG_ADDR_LOC_BYTE0			(MSG_ADDR_LOC)
#define MSG_ADDR_LOC_BYTE1			(MSG_ADDR_LOC_BYTE0 + 1)
#define MSG_CNTRL_LOC				(MSG_ADDR_LOC_BYTE1 + 1)
#define MSG_DATA_START_LOC			(MSG_CNTRL_LOC + 1)

#define MSG_ADDR_LEN				(2)
#define MSG_CHECKSUM_LEN			(2)

// MSG_STRIPPED_OVERHEAD is the message overhead less the start and end frame characters which get
// stripped by the parsing code. Includes 2 address bytes, one control byte, and 2 CRC bytes.
#define MSG_STRIPPED_OVERHEAD		(5)
#define MSG_STRIPPED_LOC(LOC)		(LOC - 1)
// MSG_OVERHEAD includes 2 address bytes, one control byte, 2 CRC bytes, and 2 frame bytes.
#define MSG_OVERHEAD				(MSG_STRIPPED_OVERHEAD + 2)

#define MAX_HDLC_LEN(MAX_RAW_LEN)	(2 * (MAX_RAW_LEN + MSG_OVERHEAD))

#define MSG_SAFETY_BUF				(1)

//##################################################################################################
// Local Type Definitions
//==================================================================================================

//##################################################################################################
// Local Class Definitions
//==================================================================================================

//##################################################################################################
// Local Function Prototypes
//==================================================================================================
uint16_t GetUInt16(const char *data);
char *InsertMsgByte(char *dataLoc, char val);

//##################################################################################################
// Module Variables
//==================================================================================================

//##################################################################################################
// Functional Implementation
//==================================================================================================
//--------------------------------------------------------------------------------------------------
// Local Functions
//--------------------------------------------------------------------------------------------------
uint16_t GetUInt16(const char *data)
{
	uint16_t val;

	val = (uint8_t)*data;
	data++;
	val |=  ((uint16_t)*data) << BITS_PER_BYTE;
	return (val);
}

char *InsertMsgByte(char *dataLoc, char val)
{
	// Check for characters that must be escaped.
	if((FRAME_CHAR == val) || (ESCAPE_CHAR == val))
	{
		// Insert escape sequence for control characters.
		*dataLoc = ESCAPE_CHAR;
		dataLoc++;
		*dataLoc = ESCAPE_FLAG ^ val;
	}
	else
	{
		// Insert non control characters.
		*dataLoc = val;
	}
	dataLoc++;
	return (dataLoc);
}

//--------------------------------------------------------------------------------------------------
// Public Functions
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// Private Class Implementations
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// Protected Class Implementations
//--------------------------------------------------------------------------------------------------
bool HDLC_t::ExtractMsg(const char *data, uint32_t msgLen)
{
	bool goodMsg = false;

	if(MSG_STRIPPED_OVERHEAD < msgLen)
	{
		uint32_t crcOffset = msgLen - MSG_CHECKSUM_LEN;
		uint16_t crc = CRC16_DEFAULT_SEED;

		crc = CalcCRC16Begin(crc);
		crc = CalcCRC16(crc, data, crcOffset);
		crc = CalcCRC16Complete(crc);

		uint16_t msgCRC;
		msgCRC = GetUInt16(&data[crcOffset]);

		// Validate the messages CRC matches the CRC calculated over the message.
		if(msgCRC == crc)
		{
			goodMsg = true;

			// Extract the details from the received message.
			addr = GetUInt16(data);
			control = *(data + MSG_ADDR_LEN);
			msgData = &data[MSG_STRIPPED_LOC(MSG_DATA_START_LOC)];
			msgDataLen = msgLen - MSG_STRIPPED_OVERHEAD;
		}
	}
	return (goodMsg);
}

//--------------------------------------------------------------------------------------------------
// Public Class Implementations
//--------------------------------------------------------------------------------------------------
HDLC_t::HDLC_t()
{
	Init(0);
}

HDLC_t::~HDLC_t()
{
	// Free the buffer used to store raw data received in HDLC messages.
	if(nullptr != rcvBuf)
	{
		delete [] rcvBuf;
		rcvBuf = nullptr;
	}
	// Free the buffer used to store HDLC messages that are generated from raw data.
	if(nullptr != createBuf)
	{
		delete [] createBuf;
		createBuf = nullptr;
	}
}

void HDLC_t::Init(uint32_t maxRawMsgLen)
{
	// Free any existing buffers and then crreate new ones that are large enough to store the
	// desired maximum message size.
	maxRawLen = maxRawMsgLen;
	if(nullptr != rcvBuf)
	{
		delete [] rcvBuf;
	}
	rcvBuf = new char[maxRawLen];

	maxHDLClen = MAX_HDLC_LEN(maxRawMsgLen);
	if(nullptr != createBuf)
	{
		delete [] createBuf;
	}
	createBuf = new char[maxHDLClen];

	// Initialise the data used to parse incoming HDLC messages.
	addr = 0x0000;
	control = 0x00;
	msgData = nullptr;
	msgDataLen = 0;
	escape = false;
	rcvlen = 0;
	msgPending = false;
}

uint32_t HDLC_t::ParseRcvdData(const char *rcvdData, uint32_t rcvdLen)
{
	uint32_t dataOffset = 0;

	// Keep parsing any remaining data until a complete message is detected or all the received data
	// has been parsed.
	while ((!msgPending) && (dataOffset < rcvdLen))
	{
		// Detect the frame byte that is used to indicate the start and end of a message.
		if(FRAME_CHAR == rcvdData[dataOffset])
		{
			if(ExtractMsg(rcvBuf, rcvlen))
			{
				msgPending = true;
			}
			rcvlen = 0;
			escape = false;
		}
		// Detect the start of an escape sequence, ignoring if the character has already been
		// escaped.
		else if(!escape && ((ESCAPE_CHAR == rcvdData[dataOffset])))
		{
			escape = true;
		}
		// Process any non-control characters.
		else
		{
			char rcvdChar = rcvdData[dataOffset];

			// Handle any characters that appear as part of a escape sequence.
			if(escape)
			{
				rcvdChar ^= ESCAPE_FLAG;
			}

			// Only append new data to the end of the message buffer if there is enough room.
			if(rcvlen < maxRawLen)
			{
				rcvBuf[rcvlen] = rcvdChar;
				rcvlen++;
			}
			escape = false;
		}
		dataOffset++;
	}
	return (dataOffset);
}

HDLC_t::Msg_t HDLC_t::GetPendingMsg(void)
{
	HDLC_t::Msg_t msg;

	if(msgPending)
	{
		// Provide the caller all the details of the currently pending message.
		msg.data = msgData;
		msg.len = msgDataLen;
		msg.address = addr;
		msg.control = control;

		// Clear the message data ready for the next message to be used.
		msgData = nullptr;
		msgDataLen = 0;
		addr = 0x0000;
		control = 0x00;
		msgPending = false;
	}
	else
	{
		msg.data = nullptr;
		msg.len = 0;
		msg.address = 0x0000;
		msg.control = 0x00;
	}
	return (msg);
}

HDLC_t::Msg_t HDLC_t::CreateMsg(uint16_t addr, uint8_t cntrl, const char *rawData, uint32_t rawLen)
{
	char *buf = createBuf;
	char *endOfBuf = buf + maxHDLClen - MSG_SAFETY_BUF;
	char msgData;
	Msg_t result;

	// Initialise the result structure.
	result.address = addr;
	result.control = cntrl;
	result.data = nullptr;
	result.len = 0;

	// Initialise the CRC16 value prior to calculating the messages checksum.
	uint16_t crc = CRC16_DEFAULT_SEED;
	crc = CalcCRC16Begin(crc);

	*buf = FRAME_CHAR;
	buf++;
	// Set the messages address and control data in the message buffer, calculating the CRC across
	// the raw data.
	msgData = (char)addr;
	buf = InsertMsgByte(buf, msgData);
	crc = CalcCRC16(crc, &msgData, sizeof(char));
	msgData = (char)(addr >> BITS_PER_BYTE);
	buf = InsertMsgByte(buf, msgData);
	crc = CalcCRC16(crc, &msgData, sizeof(char));
	buf = InsertMsgByte(buf, cntrl);
	crc = CalcCRC16(crc, &cntrl, sizeof(char));

	// Insert the data into the HDLC message buffer, ensuring the data doesn't run off the end of
	// the buffer.
	uint32_t dataIdx = 0;
	while((dataIdx < rawLen) && (buf < endOfBuf))
	{
		buf = InsertMsgByte(buf, rawData[dataIdx]);
		dataIdx++;
	}

	// Populate the message with the CRC and end of frame character if the message hasn't overrun
	// the end of the buffer.
	if(buf < endOfBuf)
	{
		// Complete the CRC calculation over the raw data.
		crc = CalcCRC16(crc, rawData, rawLen);
		crc = CalcCRC16Complete(crc);

		msgData = (char)crc;
		buf = InsertMsgByte(buf, msgData);
		if(buf < endOfBuf)
		{
			msgData = (char)(crc >> BITS_PER_BYTE);
			buf = InsertMsgByte(buf, msgData);
			// endOfBuf is actually the last valid position of the buffer so it is OK to test '<='
			// as we are adding the final character to the message and no byte stuffing will occur.
			if(buf <= endOfBuf)
			{
				*buf = FRAME_CHAR;
				buf++;

				result.data = createBuf;
				result.len = (uint32_t)(buf - createBuf);
			}
		}
	}
	return (result);
}

//==================================================================================================
//!@}
