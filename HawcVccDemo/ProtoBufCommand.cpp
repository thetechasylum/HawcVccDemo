#include <fstream>
#include <sstream>

#include <google/protobuf/util/json_util.h>

#include "HaWC.pb.h"
#include "ProtoBufCommand.h"

std::shared_ptr<HaWC::CmdMsg> GetCmdMsg(uint32_t seqNum);
void CreateMainOverlay(HaWC::CmdMsg::OverlayCmd *overlay);
void CreateViewOverlay(HaWC::CmdMsg::OverlayCmd *overlay, uint32_t viewID);
void UpdateMainOverlay(HaWC::CmdMsg::OverlayCmd *overlay);
void UpdateViewOverlay(HaWC::CmdMsg::OverlayCmd *overlay, uint32_t viewID);


std::shared_ptr<HaWC::CmdMsg> GetCmdMsg(uint32_t seqNum)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = std::make_shared<HaWC::CmdMsg>();

	// Set the sequence number, this is used to match a response with the initial command
	cmdMsg->set_seqnum(seqNum);

	return (cmdMsg);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Application Manager Module Commands
////////////////////////////////////////////////////////////////////////////////////////////////////
// Parameters:
// set_closeoption(HaWC::CmdMsg::CloseAction): Determines what the close command is to do. I.e.
// Restart the application (HaWC::CmdMsg::RestartApp), close the application but leave the OS
// running (HaWC::CmdMsg::CloseApp), Restart the OS (HaWC::CmdMsg::RestartOS), Shutdown the OS
// (HaWC::CmdMsg::ShutdownOS).
std::shared_ptr<HaWC::CmdMsg> GetCloseAppCmd(uint32_t seqNum, HaWC::CmdMsg::CloseAction closeOption)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the close message.
	// Can use the set_allocated_*(...) or mutable_*() methods to initiate the object.
//	HaWC::CmdMsg::AppManagerCmd *appcmd = new HaWC::CmdMsg::AppManagerCmd();
//	cmdMsg->set_allocated_appcmd(appcmd);
//	HaWC::CmdMsg::CloseCmd* closeCmd = new HaWC::CmdMsg::CloseCmd();
//	appcmd->set_allocated_close(closeCmd);
	HaWC::CmdMsg::CloseCmd* closeCmd = cmdMsg->mutable_appcmd()->mutable_close();
	
	// Set the close option.
	closeCmd->set_closeoption(closeOption);

	return (cmdMsg);
}

// Parameters:
// set_reporterrstr(bool): is useful for developmment and logging otherwise in production should be
// turned off. "reporterrstr" causes a human readable error string to be sent from the HaWC in
// the event of an error. To save on bandwidth and memory usage, this feature should be disabled
// in production releases.
std::shared_ptr<HaWC::CmdMsg> GetSaveDefaultConfigCmd(uint32_t seqNum)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the save message.
	HaWC::CmdMsg::SaveDefaultConfigCmd *saveCmd
										= cmdMsg->mutable_appcmd()->mutable_savedefaultconfig();

	saveCmd->set_reporterrstr(true);
	return (cmdMsg);
}


std::shared_ptr<HaWC::CmdMsg> GetRestoreFactoryDefaultsCmd(uint32_t seqNum)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the restore message.
	HaWC::CmdMsg::RestoreFactoryConfigCmd *restoreCmd
										= cmdMsg->mutable_appcmd()->mutable_restorefactoryconfig();

	return (cmdMsg);
}

// Parameters:
// set_filename(std::string): specifies a path (inside the user configurable area) and file name
// used to save the current configuration to file.
// Note: the path should use '/' character as a directory seperator, not the windows style '\'.

// set_reporterrstr(bool): is useful for developmment and logging otherwise in production should be
// turned off. "reporterrstr" causes a human readable error string to be sent from the HaWC in
// the event of an error. To save on bandwidth and memory usage, this feature should be disabled
// in production releases.
std::shared_ptr<HaWC::CmdMsg> GetSaveConfigFileCmd(uint32_t seqNum, const std::string &filePath)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the save message.
	HaWC::CmdMsg::SaveConfigToFileCmd *saveCmd
											= cmdMsg->mutable_appcmd()->mutable_saveconfigtofile();

	saveCmd->set_filename(filePath);
	saveCmd->set_reporterrstr(true);
	return (cmdMsg);
}

// Parameters:
// set_filename(std::string): specifies a path (inside the user configurable area) and file name
// of the file that contains the settings to load.
// Note: the path should use '/' character as a directory seperator, not the windows style '\'.

// set_reporterrstr(bool): is useful for developmment and logging otherwise in production should be
// turned off. "reporterrstr" causes a human readable error string to be sent from the HaWC in
// the event of an error. To save on bandwidth and memory usage, this feature should be disabled
// in production releases.
std::shared_ptr<HaWC::CmdMsg> GetLoadConfigFileCmd(uint32_t seqNum, const std::string &filePath)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the load message.
	HaWC::CmdMsg::LoadConfigFromFileCmd *loadCmd
										= cmdMsg->mutable_appcmd()->mutable_loadconfigfromfile();

	loadCmd->set_filename(filePath);
	loadCmd->set_reporterrstr(true);
	return (cmdMsg);
}

// Parameters:
// set_filename(std::string): specifies a path (inside the user configurable area) and file name
// of the file to get the hash for.
// Note: the path should use '/' character as a directory seperator, not the windows style '\'.

// set_reporterrstr(bool): is useful for developmment and logging otherwise in production should be
// turned off. "reporterrstr" causes a human readable error string to be sent from the HaWC in
// the event of an error. To save on bandwidth and memory usage, this feature should be disabled
// in production releases.
std::shared_ptr<HaWC::CmdMsg> GetFileHashCmd(uint32_t seqNum, const std::string &filePath)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the get hash message.
	HaWC::CmdMsg::GetFileHashCmd *getHashCmd = cmdMsg->mutable_appcmd()->mutable_getfilehash();

	getHashCmd->set_filename(filePath);
	getHashCmd->set_reporterrstr(true);
	return (cmdMsg);
}

// Parameters:
// set_target(HaWC::Targets): Specifies which module on the HaWC that the App manager is going to
// access to get parameters from.

// mutable_locations() gets a list to be updated with the parameters to be requested from the HaWC.
// leave the list empty to request information on every module variable supported by the given
// target module.
//		set_id(std::string): ID of the parameter being requested.
//		set_offset(uint32_t): Offset into the data being requested. Leave as 0 as none of the
//							variables are configured to be multiple values.

// set_reporterrstr(bool): is useful for developmment and logging otherwise in production should be
// turned off. "reporterrstr" causes a human readable error string to be sent from the HaWC in
// the event of an error. To save on bandwidth and memory usage, this feature should be disabled
// in production releases.
std::shared_ptr<HaWC::CmdMsg> GetParametersCmd(	uint32_t seqNum,
												HaWC::Targets target,
												std::vector<std::string> &paramIDs)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the get parameters message.
	HaWC::CmdMsg::GetParamsCmd *getParamsCmd = cmdMsg->mutable_appcmd()->mutable_getparams();

	getParamsCmd->set_target(target);

	google::protobuf::RepeatedPtrField<HaWC::CmdMsg::ParamLoc> *params = 
																getParamsCmd->mutable_locations();
	std::vector<std::string>::iterator iter = paramIDs.begin();
	for (; (iter != paramIDs.end()); iter++)
	{
		HaWC::CmdMsg::ParamLoc *param = new HaWC::CmdMsg::ParamLoc();
		param->set_id(*iter);
		param->set_offset(0);
		
		params->AddAllocated(param);
	}

	getParamsCmd->set_reporterrstr(true);
	return (cmdMsg);
}

// Parameters:
// set_target(HaWC::Targets): Specifies which module on the HaWC that the App manager is going to
// access to get parameter configurationss from.

// mutable_getparamscfgn() gets a list to be updated with the parameters to be requested from the HaWC.
// leave the list empty to request information on every module variable supported by the given
// target module.
//		std::string: ID of the parameter being requested.

// set_reportdescription(bool): indicates if a human readable description should be sent in the
// response message. Useful for developers.

// set_reporterrstr(bool): is useful for developmment and logging otherwise in production should be
// turned off. "reporterrstr" causes a human readable error string to be sent from the HaWC in
// the event of an error. To save on bandwidth and memory usage, this feature should be disabled
// in production releases.
std::shared_ptr<HaWC::CmdMsg> GetParameterConfigsCmd(uint32_t seqNum,
													HaWC::Targets target,
													std::vector<std::string> &paramIDs)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the get parameter configuration message.
	HaWC::CmdMsg::GetParamsCfgnCmd *getParamCfgsCmd
												= cmdMsg->mutable_appcmd()->mutable_getparamscfgn();

	getParamCfgsCmd->set_target(target);

	google::protobuf::RepeatedPtrField<std::string> *params = getParamCfgsCmd->mutable_paramids();
	std::vector<std::string>::iterator iter = paramIDs.begin();
	for (; (iter != paramIDs.end()); iter++)
	{
		std::string *paramID = new std::string();
		*paramID = *iter;
		params->AddAllocated(paramID);
	}

	getParamCfgsCmd->set_reportdescription(true);

	getParamCfgsCmd->set_reporterrstr(true);
	return (cmdMsg);
}

// Parameters:
// set_target(HaWC::Targets): Specifies which module on the HaWC that the App manager is going to
// access to set parameter vaslues to.

// set_reportparamvals(bool): indicates whether the response message should contain the updated
// values from the parameters being set.

// add_params(): Adds a new parameter item to be updated on the target HaWC.
//		set_paramid: ID of the parameter to be updated.
//		set_offset: offset into the data that needs to be updated. Set to 0 as multi value
//					parameters have not been implemented on the HaWC.
//		set_databytes(uint8_t): Set a uint8_t value.
//		set_boolval(bool): Set a bool value.
//		set_uintval(uint32_t): Set a uint32_t value.
//		set_intval(int32_t): Set a int32_t value.
//		set_doubleval(double): Set a double value.
//		set_strval(std::string): Set a string value.
//		The datat type of the target parameter must match the data type of the value being set.

// set_reporterrstr(bool): is useful for developmment and logging otherwise in production should be
// turned off. "reporterrstr" causes a human readable error string to be sent from the HaWC in
// the event of an error. To save on bandwidth and memory usage, this feature should be disabled
// in production releases.
std::shared_ptr<HaWC::CmdMsg> GetWriteParametersCmd(uint32_t seqNum)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the set parameters message.
	HaWC::CmdMsg::SetParamsCmd *setParamsCmd = cmdMsg->mutable_appcmd()->mutable_setparams();

	// Seting parameters in the App Manager (HaWC::App). The valid modules to address are HaWC::App,
	// HaWC::Display, HaWC::IO, HaWC::MsgHandler, HaWC::Serial_RS422_485, HaWC::Serial_USB,
	// HaWC::Ethernet, and HaWC::CAN.
	setParamsCmd->set_target(HaWC::App);

	setParamsCmd->set_reportparamvals(true);

	HaWC::CmdMsg::ParamSet *param;
	// Add a parameter to set a new value to. Use the GUI to query the unit on what each module
	// supports. I.e. In the GUI, select the comms interface you are using, the module to query in the
	// drop down, leave the list of parameters empty, hit the "Get Parameters" button.
	// This message is enabling the configuration mode (enabling the USB device) and also giving the
	// HaWC unit a name ("NVTS HaWC").
	param = setParamsCmd->add_params();
	param->set_paramid("enableConfigMode");  // Parameter’s name
	param->set_offset(0); // index, should be set to 0 as there are no arrays being supported currently.
	param->set_boolval(true); // bool data

	param = setParamsCmd->add_params();
	param->set_paramid("devName");  // Parameter’s name
	param->set_offset(0); // index, should be set to 0 as there are no arrays being supported currently.
	param->set_strval("Herman the HaWC"); // std::string data

#if false
	param->set_databytes(uint8_tVal); // uint8_t data
	param->set_boolval(boolVal); // bool data
	param->set_uintval(uint32_tVal); // uint32_t data
	param->set_intval(int32_tVal); // int32_t data
	param->set_doubleval(doubleVal); // double data
	param->set_strval(std_stringVal); // std::string data
#endif

	setParamsCmd->set_reporterrstr(true);

	return (cmdMsg);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Display Module Commands
////////////////////////////////////////////////////////////////////////////////////////////////////
// Parameters:
// set_reporterrstr(bool): is useful for developmment and logging otherwise in production should be
// turned off. "reporterrstr" causes a human readable error string to be sent from the HaWC in
// the event of an error. To save on bandwidth and memory usage, this feature should be disabled
// in production releases.
std::shared_ptr<HaWC::CmdMsg> GetSaveDefaultDisplayCmd(uint32_t seqNum)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the save message.
	HaWC::CmdMsg::SaveDefaultDisplayCmd *saveCmd
									= cmdMsg->mutable_displaycmd()->mutable_savedefaultdisplaycmd();

	saveCmd->set_reporterrstr(true);
	return (cmdMsg);
}

std::shared_ptr<HaWC::CmdMsg> GetRestoreDefaultDisplayCmd(uint32_t seqNum)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the save message.
	HaWC::CmdMsg::RestoreFactoryDisplayCmd *restoreCmd
								= cmdMsg->mutable_displaycmd()->mutable_restorefactorydisplaycfgn();

	return (cmdMsg);
}

std::shared_ptr<HaWC::CmdMsg> GetSaveDisplayCmd(uint32_t seqNum, const std::string &filePath)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the save message.
	HaWC::CmdMsg::SaveDisplayToFileCmd *saveCmd
										= cmdMsg->mutable_displaycmd()->mutable_savedisplaycmd();

	saveCmd->set_filename(filePath);

	saveCmd->set_reporterrstr(true);
	return (cmdMsg);
}

std::shared_ptr<HaWC::CmdMsg> GetLoadDisplayCmd(uint32_t seqNum, const std::string &filePath)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the load message.
	HaWC::CmdMsg::LoadDisplayFromFileCmd *loadCmd
										= cmdMsg->mutable_displaycmd()->mutable_loaddisplaycmd();

	loadCmd->set_filename(filePath);

	loadCmd->set_reporterrstr(true);
	return (cmdMsg);
}

std::shared_ptr<HaWC::CmdMsg> GetLoadFontCmd(uint32_t seqNum, std::vector<std::string> &fontFiles)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the load message.
	HaWC::CmdMsg::LoadFontsCmd *loadCmd = cmdMsg->mutable_displaycmd()->mutable_loadfontscmd();

	google::protobuf::RepeatedPtrField<std::string> *files = loadCmd->mutable_fontfiles();
	std::vector<std::string>::iterator iter = fontFiles.begin();
	for (; (iter != fontFiles.end()); iter++)
	{
		std::string *fontFile = new std::string(*iter);
		files->AddAllocated(fontFile);
	}

	loadCmd->set_overwritefiles(true);

	loadCmd->set_reportparamvals(true);

	loadCmd->set_reporterrstr(true);
	return (cmdMsg);
}

std::shared_ptr<HaWC::CmdMsg> GetSaveOverlayCmd(uint32_t seqNum,
												const std::string &filePath,
												bool mainOverlay,
												uint32_t viewPortID)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the save message.
	HaWC::CmdMsg::SaveOverlayToFileCmd *saveCmd
									= cmdMsg->mutable_displaycmd()->mutable_saveoverlaytofilecmd();

	saveCmd->set_filename(filePath);

	saveCmd->set_mainoverlay(mainOverlay);

	saveCmd->set_viewportid(viewPortID);

	saveCmd->set_reporterrstr(true);
	return (cmdMsg);
}

std::shared_ptr<HaWC::CmdMsg> GetLoadOverlayCmd(uint32_t seqNum,
												const std::string &filePath,
												bool mainOverlay,
												uint32_t viewPortID)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the load message.
	HaWC::CmdMsg::LoadOverlayFromFileCmd *loadCmd
								= cmdMsg->mutable_displaycmd()->mutable_loadoverlayfromfilecmd();
	
	loadCmd->set_mainoverlay(mainOverlay);

	loadCmd->set_filename(filePath);

	loadCmd->set_viewportid(viewPortID);

	loadCmd->set_reporterrstr(true);
	return (cmdMsg);
}

void CreateMainOverlay(HaWC::CmdMsg::OverlayCmd *overlay)
{
	// viewportid is ignored for the main overlay.
	overlay->set_viewportid(0xffffffff);

	HaWC::CmdMsg::ComponentList *compList = overlay->mutable_createscreen();
	google::protobuf::RepeatedPtrField<HaWC::CmdMsg::Component> *components = compList->mutable_components();
	HaWC::CmdMsg::Component *component;
	
	// Start set_objectname value with a lowercase character.

	// Rectangle
	/*component = new HaWC::CmdMsg::Component();
	component->set_objectname("rectangle");
	HaWC::CmdMsg::RectangleProperties *rectProps = component->mutable_rectangle();
	rectProps->add_properties()->set_width(100);
	rectProps->add_properties()->set_height(100);
	rectProps->add_properties()->set_x(50);
	rectProps->add_properties()->set_y(0);
	rectProps->add_properties()->set_color("Red");
	components->AddAllocated(component);*/

	// Text
	component = new HaWC::CmdMsg::Component();
	component->set_objectname("text");
	HaWC::CmdMsg::LabelProperties *labelProps = component->mutable_text();
	labelProps->add_properties()->set_text("HaWC Controller");
	labelProps->add_properties()->set_x(320);
	labelProps->add_properties()->set_y(0);
	labelProps->add_properties()->set_color("Yellow");
	labelProps->add_properties()->set_textformat(HaWC::CmdMsg_TextFormat::CmdMsg_TextFormat_PlainText);
	labelProps->add_properties()->set_font_pointsize(20);
	components->AddAllocated(component);

	// List
	std::string listName = "list"; // The list name must start with a lowercase character to work correctly.
	component = new HaWC::CmdMsg::Component();
	component->set_objectname(listName); // The list name must start with a lowercase character to work correctly.
	HaWC::CmdMsg::ListProperties *listProps = component->mutable_list();
	listProps->add_properties()->set_item("Visible");
	listProps->add_properties()->set_item("Thermal");
	listProps->add_properties()->set_item("LRF");
	//listProps->add_properties()->set_item("4. Ite");
	components->AddAllocated(component);

	// Menu
	component = new HaWC::CmdMsg::Component();
	component->set_objectname("menu");
	HaWC::CmdMsg::MenuProperties *menuProps = component->mutable_menu();
	menuProps->add_properties()->set_options___(listName);
	menuProps->add_properties()->set_x(580);
	menuProps->add_properties()->set_y(83);
	menuProps->add_properties()->set_width(210);
	menuProps->add_properties()->set_itemheight(70);
	menuProps->add_properties()->set_selected(2);
	menuProps->add_properties()->set_backcolor("Blue");
	menuProps->add_properties()->set_textcolor("White");
	menuProps->add_properties()->set_selectedcolor("Red");
	components->AddAllocated(component);


	std::string pics = "images";
	component = new HaWC::CmdMsg::Component();
	component->set_objectname(pics);
	HaWC::CmdMsg::FileListProperties* listProps2 = component->mutable_filelist();
	listProps2->add_properties()->set_item("battery_4Qtrs.png");
	listProps2->add_properties()->set_item("gauge_1_third.png");
	listProps2->add_properties()->set_item("magnifyingglass.png");
	listProps2->add_properties()->set_item("three_bullets.png");
	components->AddAllocated(component);

	std::string logo = "logo";
	component = new HaWC::CmdMsg::Component();
	component->set_objectname(logo);
	HaWC::CmdMsg::ImageProperties* listProps3 = component->mutable_image();
	listProps3->add_properties()->set_x(580);
	listProps3->add_properties()->set_y(2);
	listProps3->add_properties()->set_width(200);
	listProps3->add_properties()->set_height(80);
	listProps3->add_properties()->set_source("NVTSLogo.png");
	listProps3->add_properties()->set_visible(true);
	
	components->AddAllocated(component);

	
	component = new HaWC::CmdMsg::Component();
	component->set_objectname("toolbar");
	HaWC::CmdMsg::ToolBarProperties* toolbar = component->mutable_toolbar();
	toolbar->add_properties()->set_x(10);
	toolbar->add_properties()->set_y(5);
	toolbar->add_properties()->set_width(300);
	toolbar->add_properties()->set_height(100);
	toolbar->add_properties()->set_options___(pics);
	toolbar->add_properties()->set_isvertical(false);
	toolbar->add_properties()->set_selected(1);
	toolbar->add_properties()->set_iconsspacing(5);
	toolbar->add_properties()->set_margins(10);
	toolbar->add_properties()->set_backcolor("Blue");
	toolbar->add_properties()->set_selectedcolor("Red");
	toolbar->add_properties()->set_selectedbordercolor("Black");
	toolbar->add_properties()->set_selectedborderwidth(3);
	toolbar->add_properties()->set_visible(true);
	components->AddAllocated(component);
}

void CreateViewOverlay(HaWC::CmdMsg::OverlayCmd *overlay, uint32_t viewID)
{
	// viewportid is ignored for the main overlay.
	/*overlay->set_viewportid(viewID);

	HaWC::CmdMsg::ComponentList *compList = overlay->mutable_createscreen();
	google::protobuf::RepeatedPtrField<HaWC::CmdMsg::Component> *components = compList->mutable_components();
	HaWC::CmdMsg::Component *component;

	// Toolbar Images
	// Copy png files to the user configuration area (USB storage area) and either rename to match
	// the set_item() values or change the set_item() values to match your filenames.
	std::string toolbarListName = "images";
	component = new HaWC::CmdMsg::Component();
	component->set_objectname(toolbarListName);
	HaWC::CmdMsg::FileListProperties *listProps = component->mutable_filelist();
	listProps->add_properties()->set_item("battery_4Qtrs.png");
	listProps->add_properties()->set_item("item2.png");
	listProps->add_properties()->set_item("item3.png");
	listProps->add_properties()->set_item("item4.png");
	components->AddAllocated(component);

	component = new HaWC::CmdMsg::Component();
	component->set_objectname("toolbar");
	HaWC::CmdMsg::ToolBarProperties *toolbar = component->mutable_toolbar();
	toolbar->add_properties()->set_x(50);
	toolbar->add_properties()->set_y(50);
	toolbar->add_properties()->set_width(300);
	toolbar->add_properties()->set_height(100);
	toolbar->add_properties()->set_options___(toolbarListName);
	toolbar->add_properties()->set_isvertical(false);
	toolbar->add_properties()->set_selected(1);
	toolbar->add_properties()->set_iconsspacing(5);
	toolbar->add_properties()->set_margins(10);
	toolbar->add_properties()->set_backcolor("Blue");
	toolbar->add_properties()->set_selectedcolor("Red");
	toolbar->add_properties()->set_selectedbordercolor("Black");
	toolbar->add_properties()->set_selectedborderwidth(3);
	toolbar->add_properties()->set_visible(true);
	components->AddAllocated(component);*/
}

void UpdateMainOverlay(HaWC::CmdMsg::OverlayCmd *overlay)
{
	// viewportid is ignored for the main overlay.
	overlay->set_viewportid(0xffffffff);

	HaWC::CmdMsg::ComponentList *compList = overlay->mutable_updatesreen();
	google::protobuf::RepeatedPtrField<HaWC::CmdMsg::Component> *components = compList->mutable_components();
	HaWC::CmdMsg::Component *component;

	// Start set_objectname value with a lowercase character.
	// Rectangle
	component = new HaWC::CmdMsg::Component();
	component->set_objectname("rectangle");
	HaWC::CmdMsg::RectangleProperties *rectProps = component->mutable_rectangle();
	rectProps->add_properties()->set_border_width(10);
	rectProps->add_properties()->set_border_color("Black");
	components->AddAllocated(component);

	// Text
	component = new HaWC::CmdMsg::Component();
	component->set_objectname("text");
	HaWC::CmdMsg::LabelProperties *labelProps = component->mutable_text();
	labelProps->add_properties()->set_text("Hi there!!!");
	labelProps->add_properties()->set_color("White");
	components->AddAllocated(component);

	// Menu
	component = new HaWC::CmdMsg::Component();
	component->set_objectname("menu");
	HaWC::CmdMsg::MenuProperties *menuProps = component->mutable_menu();
	menuProps->add_properties()->set_selected(0);
	components->AddAllocated(component);
}

void UpdateViewOverlay(HaWC::CmdMsg::OverlayCmd *overlay, uint32_t viewID)
{
	// viewportid is ignored for the main overlay.
	overlay->set_viewportid(viewID);

	HaWC::CmdMsg::ComponentList *compList = overlay->mutable_updatesreen();
	google::protobuf::RepeatedPtrField<HaWC::CmdMsg::Component> *components = compList->mutable_components();
	HaWC::CmdMsg::Component *component;

	component = new HaWC::CmdMsg::Component();
	component->set_objectname("toolbar");
	HaWC::CmdMsg::ToolBarProperties *toolbar = component->mutable_toolbar();
	toolbar->add_properties()->set_x(0);
	toolbar->add_properties()->set_y(0);
	toolbar->add_properties()->set_selected(2);
	components->AddAllocated(component);
}

std::shared_ptr<HaWC::CmdMsg> GetSetDisplayCmd(uint32_t seqNum)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the set message.
	HaWC::CmdMsg::SetDisplayCfgCmd *setCmd = cmdMsg->mutable_displaycmd()->mutable_setdisplaycfg();

	setCmd->set_reportparamvals(true);

	HaWC::CmdMsg::OverlayCmd *mainOverlay = setCmd->mutable_mainoverlay();
	CreateMainOverlay(mainOverlay);

	//test source feed

	//google::protobuf::RepeatedPtrField<HaWC::CmdMsg::SetFeedSourceCmd> *feedSrcs = setCmd->mutable_feedsrcs();
	//HaWC::CmdMsg::SetFeedSourceCmd *feedSrc = new HaWC::CmdMsg::SetFeedSourceCmd();
	//feedSrc->set_feedid(1);
	//feedSrc->set_type(HaWC::TestSource);
	//feedSrc->set_sourcestr("0");
//	feedSrc->set_qostimeout(500);
//	feedSrc->set_qosrefresh(1000);
	//feedSrcs->AddAllocated(feedSrc);

	//google::protobuf::RepeatedPtrField<HaWC::CmdMsg::SetViewFeedCmd> *viewFeeds = setCmd->mutable_viewfeeds();
	//HaWC::CmdMsg::SetViewFeedCmd *viewFeed = new HaWC::CmdMsg::SetViewFeedCmd();
	//viewFeed->set_feedid(1);
	//viewFeed->set_viewportid(1);
	//viewFeeds->AddAllocated(viewFeed);

	google::protobuf::RepeatedPtrField<HaWC::CmdMsg::SetViewWindowCmd> *viewWndows = setCmd->mutable_viewwindows();
	HaWC::CmdMsg::SetViewWindowCmd *viewWndow = new HaWC::CmdMsg::SetViewWindowCmd();
	viewWndow->set_viewportid(1);
	viewWndow->set_x(100);
	viewWndow->set_y(100);
	viewWndow->set_z(4);
	viewWndow->set_width(400);
	viewWndow->set_height(280);
	viewWndow->set_alpha(1.0f);
	viewWndow->set_enableoverlay(true);
	viewWndows->AddAllocated(viewWndow);

#if false
	google::protobuf::RepeatedPtrField<HaWC::CmdMsg::OverlayCmd> *viewOverlays = setCmd->mutable_viewoverlays();
	HaWC::CmdMsg::OverlayCmd *viewOverlay = new HaWC::CmdMsg::OverlayCmd();
	CreateViewOverlay(viewOverlay, 1);
	viewOverlays->AddAllocated(viewOverlay);
#endif

	setCmd->set_reporterrstr(true);
	return (cmdMsg);
}

std::shared_ptr<HaWC::CmdMsg> GetCreateViewOverlayCmd(uint32_t seqNum)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the set message.
	HaWC::CmdMsg::SetDisplayCfgCmd *setCmd = cmdMsg->mutable_displaycmd()->mutable_setdisplaycfg();

	setCmd->set_reportparamvals(true);

	google::protobuf::RepeatedPtrField<HaWC::CmdMsg::OverlayCmd> *viewOverlays = setCmd->mutable_viewoverlays();
	HaWC::CmdMsg::OverlayCmd *viewOverlay = new HaWC::CmdMsg::OverlayCmd();
	CreateViewOverlay(viewOverlay, 1);
	viewOverlays->AddAllocated(viewOverlay);

	setCmd->set_reporterrstr(true);
	return (cmdMsg);
}

std::shared_ptr<HaWC::CmdMsg> GetUpdateDisplayCmd(uint32_t seqNum)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the set message.
	HaWC::CmdMsg::SetDisplayCfgCmd *setCmd = cmdMsg->mutable_displaycmd()->mutable_setdisplaycfg();

	setCmd->set_reportparamvals(true);

	HaWC::CmdMsg::OverlayCmd *mainOverlay = setCmd->mutable_mainoverlay();
	UpdateMainOverlay(mainOverlay);

	google::protobuf::RepeatedPtrField<HaWC::CmdMsg::OverlayCmd> *viewOverlays = setCmd->mutable_viewoverlays();
	HaWC::CmdMsg::OverlayCmd *viewOverlay = new HaWC::CmdMsg::OverlayCmd();
	UpdateViewOverlay(viewOverlay, 1);
	viewOverlays->AddAllocated(viewOverlay);

	setCmd->set_reporterrstr(true);
	return (cmdMsg);
}

std::shared_ptr<HaWC::CmdMsg> GetDisplayCmd(uint32_t seqNum)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(seqNum);

	// Create the get message.
	HaWC::CmdMsg::GetDisplayCfgCmd *getCmd = cmdMsg->mutable_displaycmd()->mutable_getdisplaycfg();

	google::protobuf::RepeatedField<int> *feedSrcs = getCmd->mutable_feedsrcids();
	/*feedSrcs->Add(0);
	feedSrcs->Add(1);
	feedSrcs->Add(2);
	feedSrcs->Add(3);
	feedSrcs->Add(4);*/

	google::protobuf::RepeatedField<uint32_t> *viewFeeds = getCmd->mutable_viewfeedids();
	/*viewFeeds->Add(0);
	viewFeeds->Add(1);
	viewFeeds->Add(2);
	viewFeeds->Add(3);
	viewFeeds->Add(4);*/

	google::protobuf::RepeatedField<uint32_t> *viewWindows = getCmd->mutable_viewwindowids();
	viewWindows->Add(0);
	viewWindows->Add(1);
	viewWindows->Add(2);
	viewWindows->Add(3);
	viewWindows->Add(4);

	getCmd->set_reporterrstr(true);
	return (cmdMsg);
}


#ifdef JSON_ENABLE
std::shared_ptr<HaWC::CmdMsg> GetJsonCmd(std::string filePath)
{
	// Create the base message.
	std::shared_ptr<HaWC::CmdMsg> cmdMsg = GetCmdMsg(0);

	std::ifstream jsonFile(filePath);
	std::stringstream jsonTxt;
	jsonTxt << jsonFile.rdbuf();

	google::protobuf::util::JsonParseOptions options2;
	JsonStringToMessage(jsonTxt.str(), &(*cmdMsg), options2);

	return (cmdMsg);
}
#endif