//##################################################################################################
//!\defgroup HDLC HDLC Class
//!
//!
//!
//!\note
//!
//!\author
//!
//!\version 0.0.0
//!
//!@{
//!\file HDLC.h
//!\file HDLC.cpp
//##################################################################################################
#ifndef HDLC_H_
#define	HDLC_H_

//##################################################################################################
// Include Files
//==================================================================================================
// Standard includes

// Project includes

//##################################################################################################
// Public MACRO Definitions
//==================================================================================================

//##################################################################################################
// Public Type Definitions
//==================================================================================================

//##################################################################################################
// Forward Declaration of Classes
//==================================================================================================

//##################################################################################################
// Public Function Prototypes
//==================================================================================================
#ifdef	__cplusplus
extern "C" {
#endif
//==================================================================================================
//! .
//!
//! \param .
//!
//!
//! \return Does not return any parameters.
//==================================================================================================

#ifdef	__cplusplus
}
#endif

//##################################################################################################
// Public Class Definitions
//==================================================================================================
//!
//! \class [class name]
//!
//! \brief [brief description]
//!
//! [detailed description]
//!
//! \tparam
//==================================================================================================
class HDLC_t
{
private:
	//!\private

protected:
	//!\protected
    //!\property
	uint32_t maxRawLen = 0;
    //!\property
	uint32_t maxHDLClen = 0;
    //!\property
	uint16_t addr = 0x0000;
    //!\property
	uint8_t control = 0x00;
    //!\property
	const char *msgData = nullptr;
    //!\property
	uint32_t msgDataLen = 0;
    //!\property
	bool escape = false;
    //!\property
	uint32_t rcvlen = 0;
    //!\property
	bool msgPending = false;
    //!\property
	char *rcvBuf = nullptr;
    //!\property
	char *createBuf = nullptr;

	//----------------------------------------------------------------------------------------------
	//!\fn bool ExtractMsg(char *data, uint32_t dataLen)
	//!
	//! ExtractMsg validate a HDLC messages content once all the byte stuffing has been removed. If a
	//! valid message exists then the address, control flags, data pointer and length are updated.
	//!
	//!\param data is the message content being extracted. I.e. HDLC message once all the byte
	//!		stuffing has been removed.
	//!\param dataLen is the length of the data being extracted. I.e. HDLC message length once all
	//!		the byte stuffing has been removed.
	//!
	//!\return Returns true if the messages data matches the embedded checksum.
	//----------------------------------------------------------------------------------------------
	bool ExtractMsg(const char *data, uint32_t dataLen);

public:
	//!\public
	//! Msg_t defines a structure that describes where to find and how large the contents of a
	//! message are in a dedicated buffer. The Msg_t structure may refer to a raw message or a HDLC
	//! message depending on how the implementation is using the structure.
	typedef struct Msg
	{
		uint16_t address;
		uint8_t control;
		const char *data;
		uint32_t len;
	} Msg_t;

	//----------------------------------------------------------------------------------------------
	//!\fn HDLC_t(uint32_t maxRawMsgLen)
	//!
	//! The constructor initiialises the internal state of the message parser. Buffers are not
	//! allocated at this time as the object is unaware of the size of messages it is expected to
	//! handle.
	//!
	//!\return Does not return any parameters.
	//----------------------------------------------------------------------------------------------
	HDLC_t();

	//----------------------------------------------------------------------------------------------
	//!\fn ~HDLC_t()
	//!
	//! The destructor releases any memory that has been allocated and performs any required clean
	//! up.
	//!
	//!\return Does not return any parameters.
	//----------------------------------------------------------------------------------------------
    ~HDLC_t();

	//----------------------------------------------------------------------------------------------
	//!\fn HDLC_t(uint32_t maxRawMsgLen)
	//!
	//! Init() allocates buffers for processing message parsing and also generation. The internal
	//! state for message parsing is also initialised to a known start state.
	//!
	//!\param maxRawMsgLen defines the largest message the HDLC_t object is guaranteed to handle.
	//!
	//!\return Does not return any parameters.
	//----------------------------------------------------------------------------------------------
    void Init(uint32_t maxRawMsgLen);

	//----------------------------------------------------------------------------------------------
	//!\fn bool ParseRcvdData(char *rcvdData, uint32_t rcvdLen)
	//!
	//! ParseRcvdData processes incoming data, building up the raw message that is contained in a
    //! HDLC message. rcvdData may contain part of a message or multiple messages. ParseRcvdData
    //! will only parse the data that relates to the current message. Once a message has been
    //! received, GetPendingMsg must be called to retrieve the current pending message. No more data
    //! is parsed until any pending message is accessed.
	//!
	//!\param rcvdData points to the data that is to be parsed.
	//!\param rcvdLen indicates how much data needs to be parsed.
	//!
	//!\return Returns the number of bytes that were parsed. If not all the data was parsed then the
    //! unparsed data should be passed back to ParseRcvdData() once the pending message has been
    //! accessed (GetPendingMsg()).
	//----------------------------------------------------------------------------------------------
    uint32_t ParseRcvdData(const char *rcvdData, uint32_t rcvdLen);

	//----------------------------------------------------------------------------------------------
	//!\fn Msg_t GetPendingMsg(void)
	//!
	//! GetPendingMsg returns the details of any pending messages that have been completely parsed.
	//!
	//!\return Returns a Msg_t giving the details of any pending fully parsed message. If no message
    //! is pending then the structure will contain a NULL pointer and indicate a message length of
    //! 0.
	//----------------------------------------------------------------------------------------------
    Msg_t GetPendingMsg(void);

	//----------------------------------------------------------------------------------------------
	//!\fn Msg_t CreateMsg(char *rawData, uint32_t rawLen)
	//!
	//! CreateMsg takes a raw data and encapsulates it in a HDLC message.
	//!
	//!\param addr adding as a placeholder. HDLC protocol calls out an address parameter, currently
    //!		not using this parameter.
	//!\param cntrl adding as a placeholder. HDLC protocol calls out an control parameter, currently
    //!		not using this parameter.
	//!\param rawData is a pointer to the memory that contains the raw data to encapsulate in the
    //!		HDLC message.
	//!\param rawLen indicates the length of the raw data to encapsulate in the HDLC message.
	//!
	//!\return Returns a Msg_t giving the details of the created HDLC message.
	//----------------------------------------------------------------------------------------------
    Msg_t CreateMsg(uint16_t addr, uint8_t cntrl, const char *rawData, uint32_t rawLen);
};

#endif	// #ifndef HDLC_H_
//!@}
