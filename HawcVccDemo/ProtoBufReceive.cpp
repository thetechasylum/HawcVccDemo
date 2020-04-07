#include <iostream>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/text_format.h>

#include "HaWC.pb.h"
#include "ProtoBufReceive.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// Application Manager Module Commands
////////////////////////////////////////////////////////////////////////////////////////////////////
void ProcessSaveConfigMsg(const HaWC::RspMsg::SaveConfigRsp &saveRsp, std::string &msgStr, bool &processMsg);
void ProcessLoadConfigMsg(const HaWC::RspMsg::LoadConfigRsp &loadRsp, std::string &msgStr, bool &processMsg);
void ProcessFileHashMsg(const HaWC::RspMsg::FileHashRsp &hashRsp, std::string &msgStr, bool &processMsg);
void ProcessParamsMsg(const HaWC::RspMsg::ParamsRsp &paramsRsp, std::string &msgStr, bool &processMsg);
void ProcessParamsCfgMsg(const HaWC::RspMsg::ParamsCfgnRsp &paramcfgsRsp, std::string &msgStr, bool &processMsg);

void ProcessAppMsg(const HaWC::RspMsg::AppManagerRsp &appRsp, std::string &msgStr, bool &processMsg);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Display Module Commands
////////////////////////////////////////////////////////////////////////////////////////////////////
void ProcessSaveDisplayMsg(const HaWC::RspMsg::SaveDisplayRsp &saveRsp, std::string &msgStr, bool &processMsg);
void ProcessLoadDisplayMsg(const HaWC::RspMsg::LoadDisplayRsp &loadRsp, std::string &msgStr, bool &processMsg);
void ProcessprocessMsg(const HaWC::RspMsg::DisplayRsp &displayRsp, std::string &msgStr, bool &processMsg);
void ProcessSaveOverlayMsg(const HaWC::RspMsg::SaveOverlayToFileRsp &saveRsp, std::string &msgStr, bool &processMsg);
void ProcessLoadOverlayMsg(const HaWC::RspMsg::LoadOverlayFromFileRsp &loadRsp, std::string &msgStr, bool &processMsg);

void ProcessOverlayMsg(const HaWC::RspMsg::OverlayRsp *overlayRsp, std::string &msgStr);
void ProcessFeedSourceMsg(const HaWC::RspMsg::FeedSourceRsp *feedSrcRsp, std::string &msgStr);
void ProcessViewFeedMsg(const HaWC::RspMsg::ViewFeedRsp *viewFeedRsp, std::string &msgStr);
void ProcessViewWindowMsg(const HaWC::RspMsg::ViewWindowRsp *viewWindowRsp, std::string &msgStr);
void ProcessDisplayCfgMsg(const HaWC::RspMsg::DisplayCfgRsp &displayCfgRsp, std::string &msgStr, bool &processMsg);

void ProcessQOSbcastMsg(const HaWC::RspMsg::QosBcast &qosBcast, std::string &msgStr, bool &processMsg);

void ProcessprocessMsg(const HaWC::RspMsg::DisplayRsp &displayRsp, std::string &msgStr, bool &processMsg);


////////////////////////////////////////////////////////////////////////////////////////////////////
// IO Module Commands
////////////////////////////////////////////////////////////////////////////////////////////////////
void ProcessIOMsg(const HaWC::RspMsg::IO_Rsp &ioRsp, std::string &msgStr, bool &processMsg);



////////////////////////////////////////////////////////////////////////////////////////////////////
// Application Manager Module Commands
////////////////////////////////////////////////////////////////////////////////////////////////////
void ProcessSaveConfigMsg(const HaWC::RspMsg::SaveConfigRsp &saveRsp, std::string &msgStr, bool &processMsg)
{
	msgStr += "::SaveConfigRsp{";
	msgStr += "result:" + std::to_string(saveRsp.result());
	msgStr += ";errstr:" + saveRsp.errstr();
	msgStr += "}";
	processMsg = true;
}

void ProcessLoadConfigMsg(const HaWC::RspMsg::LoadConfigRsp &loadRsp, std::string &msgStr, bool &processMsg)
{
	msgStr += "::LoadConfigRsp{";
	msgStr += "result:" + std::to_string(loadRsp.result());
	msgStr += ";errstr:" + loadRsp.errstr();
	msgStr += "}";
	processMsg = true;
}

void ProcessFileHashMsg(const HaWC::RspMsg::FileHashRsp &hashRsp, std::string &msgStr, bool &processMsg)
{
	msgStr += "::FileHashRsp{";
	msgStr += "result:" + std::to_string(hashRsp.result());
	msgStr += ";filehash:" + hashRsp.filehash();
	msgStr += ";errstr:" + hashRsp.errstr();
	msgStr += "}";
	processMsg = true;
}

void ProcessParamsMsg(const HaWC::RspMsg::ParamsRsp &paramsRsp, std::string &msgStr, bool &processMsg)
{
	msgStr += "::ParamsRsp{";
	msgStr += "result:" + std::to_string(paramsRsp.result());

	msgStr += ";ParamsRsp{";
	google::protobuf::RepeatedPtrField<::HaWC::RspMsg::ParamDetails> rspParams = paramsRsp.params();
	google::protobuf::RepeatedPtrField<::HaWC::RspMsg::ParamDetails>::iterator iter = rspParams.begin();
	for (; (iter < rspParams.end()); iter++)
	{
		msgStr += "{";
		msgStr += "paramid:" + iter->paramid();
		msgStr += ";offset:" + std::to_string(iter->offset());
		switch (iter->Data_case())
		{
		case HaWC::CmdMsg::ParamSet::DataCase::kBoolVal:
			msgStr += ";boolval:" + std::to_string(iter->boolval());
			break;

		case HaWC::CmdMsg::ParamSet::DataCase::kUintVal:
			msgStr += ";uintval:" + std::to_string(iter->uintval());
			break;

		case HaWC::CmdMsg::ParamSet::DataCase::kIntVal:
			msgStr += ";intval:" + std::to_string(iter->intval());
			break;

		case HaWC::CmdMsg::ParamSet::DataCase::kDoubleVal:
			msgStr += ";doubleval:" + std::to_string(iter->doubleval());
			break;

		case HaWC::CmdMsg::ParamSet::DataCase::kStrVal:
			msgStr += ";strval:" + iter->strval();
			break;

		case HaWC::CmdMsg::ParamSet::DataCase::kDataBytes:
			msgStr += ";databytes:" + std::to_string(iter->databytes());
			break;

		case HaWC::CmdMsg::ParamSet::DataCase::DATA_NOT_SET:
		default:
			break;
		}
		msgStr += ";updated:" + std::to_string(iter->updated());
		msgStr += ";saved:" + std::to_string(iter->saved());
		msgStr += ";takenaffect:" + std::to_string(iter->takenaffect());
		msgStr += "}";
	}
	msgStr += "}";

	msgStr += ";errstr:" + paramsRsp.errstr();
	msgStr += "}";
	processMsg = true;
}

void ProcessParamsCfgMsg(const HaWC::RspMsg::ParamsCfgnRsp &paramcfgsRsp, std::string &msgStr, bool &processMsg)
{
	msgStr += "::ParamsCfgnRsp{";
	msgStr += "result:" + std::to_string(paramcfgsRsp.result());

	msgStr += ";ParamsRsp{";
	google::protobuf::RepeatedPtrField<::HaWC::RspMsg::ParamCfgn> rspParams = paramcfgsRsp.params();
	google::protobuf::RepeatedPtrField<::HaWC::RspMsg::ParamCfgn>::iterator iter = rspParams.begin();
	for (; (iter < rspParams.end()); iter++)
	{
		msgStr += "{";
		msgStr += "paramid:" + iter->paramid();
		msgStr += ";datatype:HaWC::CmdMsg::ParamSet::DataCase::";
		switch (iter->datatype())
		{
		case HaWC::RspMsg::DataTypes::RspMsg_DataTypes_Bools:
			msgStr += "RspMsg_DataTypes_Bools";
			break;

		case HaWC::RspMsg::DataTypes::RspMsg_DataTypes_UInts:
			msgStr += "RspMsg_DataTypes_UInts";
			break;

		case HaWC::RspMsg::DataTypes::RspMsg_DataTypes_Ints:
			msgStr += "RspMsg_DataTypes_Ints";
			break;

		case HaWC::RspMsg::DataTypes::RspMsg_DataTypes_Doubles:
			msgStr += "RspMsg_DataTypes_Doubles";
			break;

		case HaWC::RspMsg::DataTypes::RspMsg_DataTypes_String:
			msgStr += "RspMsg_DataTypes_String";
			break;

		case HaWC::RspMsg::DataTypes::RspMsg_DataTypes_Bytes:
			msgStr += "RspMsg_DataTypes_Bytes";
			break;

		case HaWC::RspMsg::DataTypes::RspMsg_DataTypes_Unknown:
		default:
			msgStr += "RspMsg_DataTypes_Unknown";
			break;
		}
		msgStr += ";description:" + iter->description();
		msgStr += ";count:" + std::to_string(iter->count());
		msgStr += ";readble:" + std::to_string(iter->readble());
		msgStr += ";writeable:" + std::to_string(iter->writeable());
		msgStr += ";scriptable:" + std::to_string(iter->scriptable());
		msgStr += ";appresetreqd:" + std::to_string(iter->appresetreqd());
		msgStr += ";osresetreqd:" + std::to_string(iter->osresetreqd());
	}
	msgStr += "}";

	msgStr += ";errstr:" + paramcfgsRsp.errstr();
	msgStr += "}";
	processMsg = true;
}

void ProcessAppMsg(const HaWC::RspMsg::AppManagerRsp &appRsp, std::string &msgStr, bool &processMsg)
{
	msgStr += "::AppManagerRsp";
	if (appRsp.has_saveconfig())
	{
		ProcessSaveConfigMsg(appRsp.saveconfig(), msgStr, processMsg);
	}
	else if (appRsp.has_saveconfig())
	{
		ProcessSaveConfigMsg(appRsp.saveconfig(), msgStr, processMsg);
	}
	else if (appRsp.has_loadconfigrsp())
	{
		ProcessLoadConfigMsg(appRsp.loadconfigrsp(), msgStr, processMsg);
	}
	else if (appRsp.has_filehash())
	{
		ProcessFileHashMsg(appRsp.filehash(), msgStr, processMsg);
	}
	else if (appRsp.has_params())
	{
		ProcessParamsMsg(appRsp.params(), msgStr, processMsg);
	}
	else if (appRsp.has_getparamscfgn())
	{
		ProcessParamsCfgMsg(appRsp.getparamscfgn(), msgStr, processMsg);
	}
	else
	{
		std::cout << "" << std::endl;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Display Module Commands
////////////////////////////////////////////////////////////////////////////////////////////////////
void ProcessSaveDisplayMsg(const HaWC::RspMsg::SaveDisplayRsp &saveRsp, std::string &msgStr, bool &processMsg)
{
	msgStr += "::SaveDisplayRsp{";
	msgStr += "result:" + std::to_string(saveRsp.result());
	msgStr += ";errstr:" + saveRsp.errstr();
	msgStr += "}";
	processMsg = true;
}

void ProcessLoadDisplayMsg(const HaWC::RspMsg::LoadDisplayRsp &loadRsp, std::string &msgStr, bool &processMsg)
{
	msgStr += "::LoadDisplayRsp{";
	msgStr += "result:" + std::to_string(loadRsp.result());
	msgStr += ";errstr:" + loadRsp.errstr();
	msgStr += "}";
	processMsg = true;
}

void ProcessLoadFontMsg(const HaWC::RspMsg::LoadFontsRsp &loadRsp, std::string &msgStr, bool &processMsg)
{
	msgStr += "::LoadFontsRsp{";
	msgStr += "result:" + std::to_string(loadRsp.result());

	const google::protobuf::RepeatedPtrField<std::string> files = loadRsp.fontfiles();
	google::protobuf::RepeatedPtrField<std::string>::const_iterator iter = files.begin();
	msgStr += ",fontfiles{";
	for (; (iter != files.end()); iter++)
	{
		msgStr += *iter + ",";
	}
	msgStr += "}";

	msgStr += ";errstr:" + loadRsp.errstr();
	msgStr += "}";
	processMsg = true;
}

void ProcessSaveOverlayMsg(const HaWC::RspMsg::SaveOverlayToFileRsp &saveRsp, std::string &msgStr, bool &processMsg)
{
	msgStr += "::SaveOverlayToFileRsp{";
	msgStr += "result:" + std::to_string(saveRsp.result());
	msgStr += ";errstr:" + saveRsp.errstr();
	msgStr += "}";
	processMsg = true;
}

void ProcessLoadOverlayMsg(const HaWC::RspMsg::LoadOverlayFromFileRsp &loadRsp, std::string &msgStr, bool &processMsg)
{
	msgStr += "::LoadOverlayFromFileRsp{";
	msgStr += "result:" + std::to_string(loadRsp.result());
	msgStr += ";errstr:" + loadRsp.errstr();
	msgStr += "}";
	processMsg = true;
}

void ProcessOverlayMsg(const HaWC::RspMsg::OverlayRsp *overlayRsp, std::string &msgStr)
{
	msgStr += "::OverlayRsp{";
	msgStr += "viewportid:" + std::to_string(overlayRsp->viewportid());
	if (overlayRsp->has_createscreen())
	{
		msgStr += "Overlay created";
	}
	else if (overlayRsp->has_updatesreen())
	{
		msgStr += "Overlay updated";
	}
	else
	{
		msgStr += "Overlay no change";
	}

	msgStr += "}";
}

void ProcessFeedSourceMsg(const HaWC::RspMsg::FeedSourceRsp *feedSrcRsp, std::string &msgStr)
{
	msgStr += "::feedSrcRsp{";
	msgStr += "set_feedid:" + std::to_string(feedSrcRsp->feedid());
	msgStr += ",type:";
	switch (feedSrcRsp->type())
	{
	case HaWC::StaticImage:
		msgStr += "StaticImage";
		break;

	case HaWC::TestSource:
		msgStr += "TestSource";
		break;

	case HaWC::Camera:
		msgStr += "Camera";
		break;

	case HaWC::H264RTPCamera:
		msgStr += "H264RTPCamera";
		break;

	case HaWC::H264File:
		msgStr += "H264File";
		break;

	case HaWC::UnconfigdFeedType:
	default:
		msgStr += "UnconfigdFeedType";
		break;
	}

	msgStr += ",sourcestr:" + feedSrcRsp->sourcestr();
	msgStr += ",qosframetimeout:" + std::to_string(feedSrcRsp->qosframetimeout());
	msgStr += ",qosframerefresh:" + std::to_string(feedSrcRsp->qosframerefresh());

	msgStr += "}";
}

void ProcessViewFeedMsg(const HaWC::RspMsg::ViewFeedRsp *viewFeedRsp, std::string &msgStr)
{
	msgStr += "::viewFeedRsp{";
	msgStr += "viewportid:" + std::to_string(viewFeedRsp->viewportid());
	msgStr += ",feedid:" + std::to_string(viewFeedRsp->feedid());
	msgStr += "}";
}

void ProcessViewWindowMsg(const HaWC::RspMsg::ViewWindowRsp *viewWindowRsp, std::string &msgStr)
{
	msgStr += "::ViewWindowRsp{";
	msgStr += "viewportid:" + viewWindowRsp->viewportid();
	msgStr += ",x:" + std::to_string(viewWindowRsp->x());
	msgStr += ",y:" + std::to_string(viewWindowRsp->y());
	msgStr += ",z:" + std::to_string(viewWindowRsp->z());
	msgStr += ",width:" + std::to_string(viewWindowRsp->width());
	msgStr += ",height:" + std::to_string(viewWindowRsp->height());
	msgStr += ",alpha:" + std::to_string(viewWindowRsp->alpha());
	msgStr += ",enableoverlay:" + std::to_string(viewWindowRsp->enableoverlay());

	viewWindowRsp->viewportid();
	msgStr += "}";
}

void ProcessDisplayCfgMsg(const HaWC::RspMsg::DisplayCfgRsp &displayCfgRsp, std::string &msgStr, bool &processMsg)
{
	msgStr += "::DisplayCfgRsp{";
	msgStr += "result:" + std::to_string(displayCfgRsp.result());

	// ToDo:
	msgStr += ",Main Overlay:";
	const HaWC::RspMsg::OverlayRsp &mainOverlay = displayCfgRsp.mainoverlayset();
	ProcessOverlayMsg(&mainOverlay, msgStr);

	msgStr += ",FeedSources{";
	const google::protobuf::RepeatedPtrField<HaWC::RspMsg::FeedSourceRsp> feedSrcs = displayCfgRsp.feedsrcs();
	google::protobuf::RepeatedPtrField<HaWC::RspMsg::FeedSourceRsp>::const_iterator feedSrcIter = feedSrcs.begin();
	for (; (feedSrcIter != feedSrcs.end()); feedSrcIter++)
	{
		ProcessFeedSourceMsg(&(*feedSrcIter), msgStr);
	}
	msgStr += "}";

	msgStr += ",ViewFeeds{";
	const google::protobuf::RepeatedPtrField<HaWC::RspMsg::ViewFeedRsp> viewFeeds = displayCfgRsp.viewfeeds();
	google::protobuf::RepeatedPtrField<HaWC::RspMsg::ViewFeedRsp>::const_iterator viewFeedIter = viewFeeds.begin();
	for (; (viewFeedIter != viewFeeds.end()); viewFeedIter++)
	{
		ProcessViewFeedMsg(&(*viewFeedIter), msgStr);
	}
	msgStr += "}";

	msgStr += ",ViewWindows{";
	const google::protobuf::RepeatedPtrField<HaWC::RspMsg::ViewWindowRsp> viewWindows = displayCfgRsp.viewwindows();
	google::protobuf::RepeatedPtrField<HaWC::RspMsg::ViewWindowRsp>::const_iterator viewWindowIter = viewWindows.begin();
	for (; (viewWindowIter != viewWindows.end()); viewWindowIter++)
	{
		ProcessViewWindowMsg(&(*viewWindowIter), msgStr);
	}
	msgStr += "}";

	msgStr += ",ViewOverlays{";
	const google::protobuf::RepeatedPtrField<HaWC::RspMsg::OverlayRsp> viewOverlays = displayCfgRsp.viewoverlaysset();
	google::protobuf::RepeatedPtrField<HaWC::RspMsg::OverlayRsp>::const_iterator viewOverlayIter = viewOverlays.begin();
	for (; (viewOverlayIter != viewOverlays.end()); viewOverlayIter++)
	{
		ProcessOverlayMsg(&(*viewOverlayIter), msgStr);
	}
	msgStr += "}";

	msgStr += ";errstr:" + displayCfgRsp.errstr();
	msgStr += "}";
	processMsg = true;
}

void ProcessQOSbcastMsg(const HaWC::RspMsg::QosBcast &qosBcast, std::string &msgStr, bool &processMsg)
{
	msgStr += "::QosBcast{";
	msgStr += "feedname:" + qosBcast.feedname();
	msgStr += ",feedtype:";
	switch(qosBcast.feedtype())
	{
	case HaWC::StaticImage:
		msgStr += "StaticImage";
		break;

	case HaWC::TestSource:
		msgStr += "TestSource";
		break;

	case HaWC::Camera:
		msgStr += "Camera";
		break;

	case HaWC::H264RTPCamera:
		msgStr += "H264RTPCamera";
		break;

	case HaWC::H264File:
		msgStr += "H264File";
		break;

	case HaWC::UnconfigdFeedType:
	default:
		msgStr += "UnconfigdFeedType";
		break;
	}

	msgStr += ",sourcestr:" + qosBcast.sourcestr();
	msgStr += ",qosstate:";
	switch (qosBcast.qosstate())
	{
	case HaWC::QosInit:
		msgStr += "QosInit";
		break;

	case HaWC::QosStartOfStream:
		msgStr += "QosStartOfStream";
		break;

	case HaWC::QosReceivingFrames:
		msgStr += "QosReceivingFrames";
		break;

	case HaWC::QosFrameTimeout:
		msgStr += "QosFrameTimeout";
		break;

	case HaWC::QosEndOfStream:
		msgStr += "QosEndOfStream";
		break;

	default:
		msgStr += "Unknown State!!!";
		break;
	}
	msgStr += ",timesinceframe:" + std::to_string(qosBcast.timesinceframe());
	msgStr += ",repeat:" + std::to_string(qosBcast.repeat());
	msgStr += "}";
	processMsg = true;
}

void ProcessprocessMsg(const HaWC::RspMsg::DisplayRsp &displayRsp, std::string &msgStr, bool &processMsg)
{
	msgStr += "::DisplayRsp";
	if (displayRsp.has_savedisplayrsp())
	{
		ProcessSaveDisplayMsg(displayRsp.savedisplayrsp(), msgStr, processMsg);
	}
	else if (displayRsp.has_loaddisplayrsp())
	{
		ProcessLoadDisplayMsg(displayRsp.loaddisplayrsp(), msgStr, processMsg);
	}
	else if (displayRsp.has_loadfontsrsp())
	{
		ProcessLoadFontMsg(displayRsp.loadfontsrsp(), msgStr, processMsg);
	}
	else if (displayRsp.has_saveoverlaytofilersp())
	{
		ProcessSaveOverlayMsg(displayRsp.saveoverlaytofilersp(), msgStr, processMsg);
	}
	else if (displayRsp.has_loadoverlayfromfilersp())
	{
		ProcessLoadOverlayMsg(displayRsp.loadoverlayfromfilersp(), msgStr, processMsg);
	}
	else if (displayRsp.has_displaycfgrsp())
	{
		ProcessDisplayCfgMsg(displayRsp.displaycfgrsp(), msgStr, processMsg);
	}
	else if (displayRsp.has_qosbcast())
	{
		ProcessQOSbcastMsg(displayRsp.qosbcast(), msgStr, processMsg);
	}
	else
	{
		std::cout << "" << std::endl;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// IO Module Commands
////////////////////////////////////////////////////////////////////////////////////////////////////
void ProcessIOMsg(const HaWC::RspMsg::IO_Rsp &ioRsp, std::string &msgStr, bool &processMsg)
{
#if true
	msgStr += "::IO_Rsp";
	// IO broadcast is the only message type currently supported by the IO module.
	if (ioRsp.has_iobcast())
	{
		HaWC::RspMsg::IO_Bcast ioBcast = ioRsp.iobcast();
		msgStr += "::IO_Bcast{";
		msgStr += "sessionid:" + std::to_string(ioBcast.sessionid());
		msgStr += ";btns:" + std::to_string(ioBcast.btns());
		msgStr += ";leftx:" + std::to_string(ioBcast.leftx());
		msgStr += ";lefty:" + std::to_string(ioBcast.lefty());
		msgStr += ";leftrocker:" + std::to_string(ioBcast.leftrocker());
		msgStr += ";rightx:" + std::to_string(ioBcast.rightx());
		msgStr += ";righty:" + std::to_string(ioBcast.righty());
		msgStr += ";rightrocker:" + std::to_string(ioBcast.rightrocker());
		msgStr += "}";


		//mapping out inputs
		//std::cout << msgStr << std::endl;

		//left joystick(leftx)
		if (ioBcast.leftx()<0)
		{
			std::cout << "x in negative direction" << std::endl;
		}

		if (ioBcast.leftx()>0)
		{
			std::cout << "x in positive direction" << std::endl;
		}

		if (ioBcast.lefty()>0)
		{
			std::cout << "y in positive direction" << std::endl;
		}

		if (ioBcast.lefty()<0)
		{
			std::cout << "y in negative direction" << std::endl;
		}

		//right joystick(rightx)
		if (ioBcast.rightx()>0)
		{
			std::cout << "right x in positive direction" << std::endl;
		}

		if (ioBcast.rightx() < 0)
		{
			std::cout << "right x in negative direction" << std::endl;
		}

		if (ioBcast.righty()>0)
		{
			std::cout << "right y in positive direction" << std::endl;
		}

		if (ioBcast.righty() < 0)
		{
			std::cout << "right y in negative direction" << std::endl;
		}
		//std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
		//system("cls");

		//left arrow keys

		//left arrow key up
		if (ioBcast.btns()==1703936)
		{
			std::cout << "left arrow key up is pressed" << std::endl;
		}
		//left arrow key down
		if (ioBcast.btns() == 2752512)
		{
			std::cout << "left arrow key down is pressed" << std::endl;
		}
		
		//left bottom button
		if (ioBcast.btns() == 655361)
		{
			std::cout << "left bottom button is pressed" << std::endl;
		}

		//left dpad buttons

		//dpad up
		if (ioBcast.btns() == 655376)
		{
			std::cout << "left dpad button up is pressed" << std::endl;
		}
		//dpad down
		if (ioBcast.btns() == 655392)
		{
			std::cout << "left dpad button down is pressed" << std::endl;
		}
		//dpad left
		if (ioBcast.btns() == 655364)
		{
			std::cout << "left dpad button left is pressed" << std::endl;
		}

		//dpad right
		if (ioBcast.btns() == 655368)
		{
			std::cout << "left dpad button right is pressed" << std::endl;
		}

		//dpad center
		if (ioBcast.btns() == 655424)
		{
			std::cout << "left dpad button  is center pressed" << std::endl;
		}

		//right diamond buttons

		//diamond north
		if (ioBcast.btns() == 655872)
		{
			std::cout << "right  diamond button north is pressed" << std::endl;
		}

		//diamond south
		if (ioBcast.btns() == 656384)
		{
			std::cout << "right  diamond button south is pressed" << std::endl;
		}

		//diamond west
		if (ioBcast.btns() == 655488)
		{
			std::cout << "right  diamond button west is pressed" << std::endl;
		}

		//diamond east
		if (ioBcast.btns() == 655616)
		{
			std::cout << "right  diamond button east is pressed" << std::endl;
		}

		//right arrow buttons

		//right up arrow button
		if (ioBcast.btns() == 4849664)
		{
			std::cout << "right  up arrow button is pressed" << std::endl;
		}

		//right down arrow button
		if (ioBcast.btns() == 9043968)
		{
			std::cout << "right  down arrow button is pressed" << std::endl;
		}

		//right bottom button
		if (ioBcast.btns() == 655362)
		{
			std::cout << "right  bottom  button is pressed" << std::endl;
		}

		//top faced buttons

		//top face left
		if (ioBcast.btns()==657408)
		{
			std::cout << "top faced left button is pressed" << std::endl;
		}
		 
		//top face center
		if (ioBcast.btns() == 663552)
		{
			std::cout << "top faced center button is pressed" << std::endl;
		}

		//top face right 
		if (ioBcast.btns() == 659456)
		{
			std::cout << "top faced right button is pressed" << std::endl;
		}
		
	}
#else
	processMsg = false;
#endif
}

bool ProcessReceivedProtoBufMsg(const uint8_t *data, uint32_t dataLen)
{
	bool processedMsg = false;
	std::shared_ptr<HaWC::RspMsg> hawcMsg = std::make_shared<HaWC::RspMsg>();

	if (hawcMsg->ParseFromArray(data, dataLen))
	{
		// Using string instead of stream based on:
		// https://stackoverflow.com/questions/18892281/most-optimized-way-of-concatenation-in-strings
		std::string msgStr;

		msgStr = "RspMsg{seqnum:" + std::to_string(hawcMsg->seqnum()) + "}";
		if (hawcMsg->has_apprsp())
		{
			ProcessAppMsg(hawcMsg->apprsp(), msgStr, processedMsg);
		}
		else if (hawcMsg->has_displayrsp())
		{
			ProcessprocessMsg(hawcMsg->displayrsp(), msgStr, processedMsg);
		}
		else if (hawcMsg->has_iorsp())
		{
			ProcessIOMsg(hawcMsg->iorsp(), msgStr, processedMsg);
		}
		else
		{
			processedMsg = false;
			std::cout << "Unknown ProtoBuf Msg Type" << std::endl;
		}

		if (processedMsg)
		{
			std::cout << msgStr << std::endl;
#ifdef JSON_ENABLE
			// Json example
			google::protobuf::util::JsonPrintOptions options;
			options.add_whitespace = true;
			options.always_print_primitive_fields = true;
			options.preserve_proto_field_names = true;
			options.always_print_enums_as_ints = false;

			std::string jsonStr;
			MessageToJsonString(*hawcMsg, &jsonStr, options);
			std::cout << jsonStr << std::endl;
#endif // JSON_ENABLE
		}
	}
	return (processedMsg);
}