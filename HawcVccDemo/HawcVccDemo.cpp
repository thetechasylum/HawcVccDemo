// HawcVccDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <locale>
#include <cctype>
#include <vector>

#include <windows.h>

#include "Serial.h"
#include "HDLC.h"
#include "HaWC.pb.h"
#include "ProtoBufReceive.h"
#include "ProtoBufCommand.h"
#include"IOTest.h"

//new include 
#include<string>

#if defined(_DEBUG)
#pragma warning ("Disabling Json support when operating in Debug mode as the protobuf library causes an exception.")
#pragma message ("Disabling Json support when operating in Debug mode as the protobuf library causes an exception.")
#endif
#if defined(JSON_ENABLE) && defined(_DEBUG)
#error "The Json code was causing an exception to occur when running in Debug mode, so not allowing\
 JSON_ENABLE and _DEBUG settings to be selected together."
#endif

typedef enum CommandID {
	NoCommandCmdID,
	QuitCmdID,
	CloseHaWCapp,
	SaveDefaultConfig,
	RestoreFactoryDefaults,
	SaveConfigFile,
	LoadConfigFile,
	GetFileHash,
	GetParameters,
	GetParameterConfigs,
	WriteParameters,
	SaveDefaultDisplay,
	RestoreFactoryDisplay,
	SaveDisplayFile,
	LoadDisplayFile,
	LoadFont,
	SaveOverlay,
	LoadOverlay,
	CreateDisplay,
	CreateViewOverlay,
	UpdateDisplay,
	GetDisplay,
#ifdef JSON_ENABLE
	JsonCmd,
#endif
} CommandID_t;

const std::map<std::string, CommandID_t> CommandMap = {
	{"", NoCommandCmdID},
	{"quit", QuitCmdID},
	{"close hawc app", CloseHaWCapp},
	{"save default config", SaveDefaultConfig},
	{"restore default config", RestoreFactoryDefaults},
	{"save config file", SaveConfigFile},
	{"load config file", LoadConfigFile},
	{"get file hash", GetFileHash},
	{"get params", GetParameters},
	{"get param configs", GetParameterConfigs},
	{"write params", WriteParameters},
	{"save default display", SaveDefaultDisplay},
	{"restore default display", RestoreFactoryDisplay},
	{"save display file", SaveDisplayFile},
	{"load display file", LoadDisplayFile},
	{"load font", LoadFont},
	{"save overlay", SaveOverlay},
	{"load overlay", LoadOverlay},
	{"create display", CreateDisplay},
	{"create view overlay", CreateViewOverlay},
	{"update display", UpdateDisplay},
	{"get display", GetDisplay},
#ifdef JSON_ENABLE
	{"json cmd", JsonCmd},
#endif
};

const uint32_t MaXMsgSize = 0xffff;
const uint32_t MaXBufSize = 200;
const uint32_t MsgWaitMs = 5000;

uint32_t seqNum = 1;

HANDLE hSerial;
HANDLE hRxThread;
HANDLE hMsgRcvdEvt;
HANDLE hCloseEvt;
HANDLE hThreadClosedEvt;

HDLC_t hdlc;


std::string StrToLower(std::string str);

void PrintSupportedCommands(void);

void ProcessReceivedHdlcData(const uint8_t *data, uint32_t dataLen);
DWORD WINAPI ReceiveThreadFn(LPVOID lpParam);


std::string StrToLower(std::string str)
{
	std::transform(	str.begin(),
					str.end(),
					str.begin(),
					[](unsigned char c) { return std::tolower(c); });
	return(str);
}

#if false
std::vector<std::string> StrSplit(const std::string str2Parse, std::string delim);
std::vector<std::string> StrSplit(const std::string str2Parse, std::string delim)
{
	std::vector<std::string> splitVec;

	size_t start = 0;
	size_t end = str2Parse.find(delim);
	while (end != std::string::npos)
	{
		splitVec.push_back(str2Parse.substr(start, end - start));
		start = end + delim.length();
		end = str2Parse.find(delim, start);
	}
	splitVec.push_back(str2Parse.substr(start, str2Parse.length() - start));
	return (splitVec);
}
#endif

void PrintSupportedCommands(void)
{
    std::cout << "Supported Commands:" << std::endl;

	std::map<std::string, CommandID_t>::const_iterator iter = CommandMap.begin();
	for (; (iter != CommandMap.end()); iter++)
	{
		if (iter->first.length())
		{
			std::cout << iter->first << std::endl;
		}
	}
	std::cout << std::endl << std::endl << std::endl;
}

void ProcessReceivedHdlcData(const uint8_t *data, uint32_t dataLen)
{
	uint32_t lenParsed = 0;

	while (lenParsed < dataLen)
	{
		lenParsed += hdlc.ParseRcvdData((char *)(data + lenParsed), (dataLen - lenParsed));

		HDLC_t::Msg rcvdMsg = hdlc.GetPendingMsg();
		if (rcvdMsg.len != 0)
		{
			if (ProcessReceivedProtoBufMsg((uint8_t *)rcvdMsg.data, rcvdMsg.len))
			{
				SetEvent(hMsgRcvdEvt);
			}
		}
	}
}

DWORD WINAPI ReceiveThreadFn(LPVOID lpParam)
{
	static uint8_t data[MaXBufSize];
	static char szBuff[100] = { 0 };
	DWORD dwBytesRead = 0;
	bool running = true;

	while (running)
	{
		// Could use overlap mode so that can wait on multiple handles to make the app more
		// responsive, but just using a simple timeout on the read instead.
		int32_t numBytesRead;
		
		numBytesRead = SerialRead(hSerial, data, sizeof(data));
		if (numBytesRead > 0)
		{
			// Note: HDLC is only required for Serial and CAN interfaces, because unlike Ethernet
			// (UDP), these interfaces are unable to packetise a complete message.
			ProcessReceivedHdlcData(data, numBytesRead);
		}

		if (WaitForSingleObject(hCloseEvt, IGNORE) == WAIT_OBJECT_0)
		{
			running = false;
		}
	}
	std::cout << "Receive Thread closed" << std::endl;
	SetEvent(hThreadClosedEvt);
	return (0);
}


int main()
{
	bool running = true;

	PrintSupportedCommands();

	hdlc.Init(MaXMsgSize);

	hSerial = SerialOpenPort("COM3", 115200);
	//hSerial = SerialOpenPort("COM1", 115200);

	hMsgRcvdEvt = CreateEvent(	NULL,               // default security attributes
								TRUE,				// manual-reset event
								FALSE,              // initial state is nonsignaled
								NULL);				// object name

	hCloseEvt = CreateEvent(NULL,               // default security attributes
							TRUE,               // manual-reset event
							FALSE,              // initial state is nonsignaled
							NULL);				// object name

	hThreadClosedEvt = CreateEvent(	NULL,		// default security attributes
									TRUE,		// manual-reset event
									FALSE,		// initial state is nonsignaled
									NULL);		// object name

	hRxThread = CreateThread(NULL,                  // default security attributes
							0,                      // use default stack size  
							ReceiveThreadFn,		// thread function name
							NULL,					// argument to thread function 
							0,                      // use default creation flags 
							NULL);					// returns the thread identifier 

	while (running)
	{
		std::cout << "Input Command: ";

		std::string userInput;
		std::getline(std::cin, userInput);

		std::string cmd = StrToLower(userInput);
		std::map<std::string, CommandID_t>::const_iterator cmdIter = CommandMap.find(cmd);

		HaWC::CmdMsg::CloseAction action;
		HaWC::Targets target;
		std::vector<std::string> paramIDs;
		std::vector<std::string> fontFiles;

		std::shared_ptr<HaWC::CmdMsg> protobufCmd = nullptr;
		CommandID_t cmdID = (cmdIter != CommandMap.end()) ? cmdIter->second: NoCommandCmdID;
		
	

		switch (cmdID)
		{
		case QuitCmdID:
			SetEvent(hCloseEvt);
			running = false;
			break;

		////////////////////////////////////////////////////////////////////////////////////////////
		// Application Manager Module Commands
		////////////////////////////////////////////////////////////////////////////////////////////
		case CloseHaWCapp:
			// The Close App coomand can tell the HaWC to close the HaWC application, restart the
			// application, Shutdown the OS, and to restart the OS. No response is sent for this
			// message as the message may not get sent before the command takes affect. For the
			// close application and shutdown OS options the unit will stop sending IO broadcast
			// messages when the command is successfully processed. For restart application and
			// restart OS options the session ID in the IO broadcast message will change when the
			// command is successfully processed.
			//action = HaWC::CmdMsg::RestartApp;
			//action = HaWC::CmdMsg::CloseApp;
			action = HaWC::CmdMsg::RestartOS;
			//action = HaWC::CmdMsg::ShutdownOS;
			protobufCmd = GetCloseAppCmd(seqNum, action);
			break;

		case SaveDefaultConfig:
			// Saves the current settings configuration to file to be loaded as a default
			// configuration.
			protobufCmd = GetSaveDefaultConfigCmd(seqNum);
			break;

		case RestoreFactoryDefaults:
			// Restores the HaWC's configuration to factory defaults. This causes the HaWC to go
			// through a reset, so like the Close App command no response is sent in reply to this
			// command.
			// Note: This command deletes the file that contains the default configuration and then
			// resets the OS. If you don't want to lose these settings, then they should be saved to
			// a file with  a different name. You can always load the new file, save its
			// configuration as the default configuration and reset the OS to initialise all the
			// HaWC settings to the saved configuration.
			protobufCmd = GetRestoreFactoryDefaultsCmd(seqNum);
			break;

		case SaveConfigFile:
			// Save the HaWC's current configuration to a file. As long as name and path of the file
			// isn't the same as the default configuration file, then this file will not get loaded
			// on power up. Multiple configurations can be saved and then loaded on demand.
			protobufCmd = GetSaveConfigFileCmd(seqNum, "configFile.pb");
			break;

		case LoadConfigFile:
			// Loads the HaWC's configuration from a file. Some setting don't take affect until
			// either the HaWC application is restarted or the OS is restarted. To ensure all
			// settings take affect then once the load command completes successfully the
			// configuration should be saved as the default configuration and the OS restarted.
			protobufCmd = GetLoadConfigFileCmd(seqNum, "configFile.pb");
			
			break;

		case GetFileHash:
			// The get file hash can be used to get a checksum for any file in the user
			// configuration area. This command is useful to verify that the files in the user
			// configuration area are a known good version.
			protobufCmd = GetFileHashCmd(seqNum, "configFile.pb");
			break;

		case GetParameters:
			// The get parameters command retrieves the current state of module variables on the
			// HaWC. Module variable represent an assortment of settings from serial port BAUD rate
			// and ip addresses to screen brightnes and contrast. The get parameters command will
			// retrieve the parameters associated with a specific module on the HaWC. The command
			// takes a list of parameters being requested, if the list is empty then all the
			// supported variables for the target module is returned.
			target = HaWC::App;
			paramIDs.clear();
			paramIDs.push_back("swVer");
			protobufCmd = GetParametersCmd(seqNum, target, paramIDs);
			break;

		case GetParameterConfigs:
			// The get parameter configs command retrieves the configuration details of module
			// variables on the HaWC. Module variable represent an assortment of settings from
			// serial port BAUD rate and ip addresses to screen brightnes and contrast. The get
			// parameter configs command will retrieve the details of parameters associated with a
			// specific module on the HaWC. The command takes a list of parameters being requested,
			// if the list is empty then all the supported variables for the target module is
			// returned. This command is useful in development to find the support module variable
			// and to get a human readable description, but this command should not be used in
			// production releases.
			target = HaWC::App;
			paramIDs.clear();
			paramIDs.push_back("swVer");
			protobufCmd = GetParameterConfigsCmd(seqNum, target, paramIDs);
			break;

		case WriteParameters:
			// The write parameters command set the current state of module variables on the
			// HaWC. Module variable represent an assortment of settings from serial port BAUD rate
			// and ip addresses to screen brightnes and contrast. The command has the capability to
			// set multiple module variables from a single command.
			protobufCmd = GetWriteParametersCmd(seqNum);
			break;

		////////////////////////////////////////////////////////////////////////////////////////////
		// Display Module Commands
		////////////////////////////////////////////////////////////////////////////////////////////
		case SaveDefaultDisplay:
			// Saves the current display screen to file to be loaded as a default screen at power
			// up.
			// Note: the overlays are not saved as part of this command. The system must save the
			// overlays seperately using the Save Overlay command.
			protobufCmd = GetSaveDefaultDisplayCmd(seqNum);
			break;

		case RestoreFactoryDisplay:
			// Restores the HaWC's power up screen to the factory default power up screen.
			protobufCmd = GetRestoreDefaultDisplayCmd(seqNum);
			break;

		case SaveDisplayFile:
			// Save the current display screen to a file so it can be commanded to load at a later
			// time.
			// Note: the overlays are not saved as part of this command. The system must save the
			// overlays seperately using the Save Overlay command.
			protobufCmd = GetSaveDisplayCmd(seqNum, "displayFile.pb");
			break;

		case LoadDisplayFile:
			// Loads a new screen from a file.
			// Note: Overlays are not loaded using this command as they are not saved as part of the
			// Save Display command. Overlay's must be saved and loaded using the Save Overlay and
			//Load Overlay commands.
			protobufCmd = GetLoadDisplayCmd(seqNum, "displayFile.pb");
			break;

		case LoadFont:
			// Loads a font file into the system. The system must be reset before the new fonts can
			// be accessed by the Font management system.
			fontFiles.clear();
			fontFiles.push_back("TestFont.file");
			protobufCmd = GetLoadFontCmd(seqNum, fontFiles);
			break;

		case SaveOverlay:
			// Saves an overlay to file.
			// Note: The saved overlay represents the state of the overlay when it was created.
			// Subsequent updates to an overlay have no affects on the overlay that is saved to
			// file.
			// Note: Certain commands cause the HaWC to restart. If a restart occurs the overlay
			// does not get reinitialised. So there may situations where the main overlay has
			// content yet you are unable to save the main overlay. So if an overlay is intended to
			// be saved then it should be saved straight after it was created/loaded.
			protobufCmd = GetSaveOverlayCmd(seqNum, "overlayFile.pb", true, 0);
			break;

		case LoadOverlay:
			// Loads an overlay from file.
			protobufCmd = GetLoadOverlayCmd(seqNum, "overlayFile.pb", true, 0);
			break;

		case CreateDisplay:
			// Updates the screen configuration.
			// Couldn't generate a view port and overlay on the new view port, so split the creation
			// of the view port (CreateDisplay) and the creation of the overlay (CreateViewOverlay)
			// into two seperate operations.
			protobufCmd = GetSetDisplayCmd(seqNum);

			

			break;

		case CreateViewOverlay:
			// Updates the screen configuration.
			// Couldn't generate a view port and overlay on the new view port, so split the creation
			// of the view port (CreateDisplay) and the creation of the overlay (CreateViewOverlay)
			// into two seperate operations.
			protobufCmd = GetCreateViewOverlayCmd(seqNum);
			break;

		case UpdateDisplay:
			// Updates the screen configuration.
			protobufCmd = GetUpdateDisplayCmd(seqNum);
			break;

		case GetDisplay:
			// Gets the current screen configuration from the HaWc.
			protobufCmd = GetDisplayCmd(seqNum);
			break;

#ifdef JSON_ENABLE
		case JsonCmd:
			protobufCmd = GetJsonCmd("JsonCmd.json");
			break;
#endif

		default:
			std::cout << "Command not recognised!" << std::endl;
			break;
		}

		//input test
		//HaWC::RspMsg::IO_Rsp ioRsp;
		
		//HaWC::RspMsg::IO_Bcast ioBcast = ioRsp.iobcast();

		//std::string msgStr;

		//msgStr += ";leftx:" + std::to_string(ioBcast.leftx());
		//msgStr = std::to_string(ioBcast.leftx());

		//std::cout << msgStr << std::endl;
		//std::cout << testMsg << std::endl;
		if (protobufCmd != nullptr)
		{
			int32_t len;
			uint8_t *data;
			len = protobufCmd->ByteSizeLong(); // ToDo: Probably should use ByteSize() but unable to test at this
												//time, current HaWC code uses ByteSizeLong() currently.
			data = new uint8_t[len];
			protobufCmd->SerializeToArray(data, len);

			HDLC_t::Msg_t msg2Transmit = hdlc.CreateMsg(0, 0, (char *)data, len);

			ResetEvent(hMsgRcvdEvt);

			int32_t lenWritten = SerialWrite(hSerial, (const uint8_t *)msg2Transmit.data, msg2Transmit.len);
			// ToDo: handle partial writes to the serial port.

			seqNum++;

			// Free the data used to contain the serialised data.
			delete data;

			WaitForSingleObject(hMsgRcvdEvt, MsgWaitMs);
		}
	}
	
	WaitForSingleObject(hThreadClosedEvt, INFINITE);

	SerialClosePort(hSerial);

	// Close events and thread.
	if (hMsgRcvdEvt != nullptr)
	{
		CloseHandle(hMsgRcvdEvt);
	}
	if (hCloseEvt != nullptr)
	{
		CloseHandle(hCloseEvt);
	}
	if (hThreadClosedEvt != nullptr)
	{
		CloseHandle(hThreadClosedEvt);
	}
	if (hRxThread != nullptr)
	{
		CloseHandle(hRxThread);
	}

	std::cout << "Closed!!!" << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
