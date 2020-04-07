#pragma once


////////////////////////////////////////////////////////////////////////////////////////////////////
// Application Manager Module Commands
////////////////////////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<HaWC::CmdMsg> GetCloseAppCmd(uint32_t seqNum, HaWC::CmdMsg::CloseAction closeOption);

std::shared_ptr<HaWC::CmdMsg> GetSaveDefaultConfigCmd(uint32_t seqNum);

std::shared_ptr<HaWC::CmdMsg> GetRestoreFactoryDefaultsCmd(uint32_t seqNum);

std::shared_ptr<HaWC::CmdMsg> GetSaveConfigFileCmd(uint32_t seqNum, const std::string &filePath);

std::shared_ptr<HaWC::CmdMsg> GetLoadConfigFileCmd(uint32_t seqNum, const std::string &filePath);

std::shared_ptr<HaWC::CmdMsg> GetFileHashCmd(uint32_t seqNum, const std::string &filePath);

std::shared_ptr<HaWC::CmdMsg> GetParametersCmd(	uint32_t seqNum,
												HaWC::Targets target,
												std::vector<std::string> &paramIDs);

std::shared_ptr<HaWC::CmdMsg> GetParameterConfigsCmd(uint32_t seqNum,
													HaWC::Targets target,
													std::vector<std::string> &paramIDs);

std::shared_ptr<HaWC::CmdMsg> GetWriteParametersCmd(uint32_t seqNum);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Display Module Commands
////////////////////////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<HaWC::CmdMsg> GetSaveDefaultDisplayCmd(uint32_t seqNum);

std::shared_ptr<HaWC::CmdMsg> GetRestoreDefaultDisplayCmd(uint32_t seqNum);

std::shared_ptr<HaWC::CmdMsg> GetSaveDisplayCmd(uint32_t seqNum, const std::string &filePath);

std::shared_ptr<HaWC::CmdMsg> GetLoadDisplayCmd(uint32_t seqNum, const std::string &filePath);

std::shared_ptr<HaWC::CmdMsg> GetLoadFontCmd(uint32_t seqNum, std::vector<std::string> &fontFiles);

std::shared_ptr<HaWC::CmdMsg> GetSaveOverlayCmd(uint32_t seqNum,
												const std::string &filePath,
												bool mainOverlay,
												uint32_t viewPortID);

std::shared_ptr<HaWC::CmdMsg> GetLoadOverlayCmd(uint32_t seqNum,
												const std::string &filePath,
												bool mainOverlay,
												uint32_t viewPortID);

// SetDisplayCfgCmd methods
std::shared_ptr<HaWC::CmdMsg> GetSetDisplayCmd(uint32_t seqNum);
std::shared_ptr<HaWC::CmdMsg> GetCreateViewOverlayCmd(uint32_t seqNum);
std::shared_ptr<HaWC::CmdMsg> GetUpdateDisplayCmd(uint32_t seqNum);

std::shared_ptr<HaWC::CmdMsg> GetDisplayCmd(uint32_t seqNum);

#ifdef JSON_ENABLE
std::shared_ptr<HaWC::CmdMsg> GetJsonCmd(std::string filePath);
#endif
