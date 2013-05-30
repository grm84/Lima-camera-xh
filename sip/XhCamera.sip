//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2013
// European Synchrotron Radiation Facility
// BP 220, Grenoble 38043
// FRANCE
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//###########################################################################

namespace Xh
{
  /*******************************************************************
   * \class Camera
   * \brief object controlling the xh detector via da.server 
   *******************************************************************/
  class Camera
  {
%TypeHeaderCode
#include <XhCamera.h>
#include <string>
%End

  public:
	struct XhStatus {
	public:
		enum XhState {
			Idle, ///< The detector is not acquiringing data
			PausedAtGroup, ///< The detector has paused at the beginning of a group
			PausedAtFrame, ///< The detector has paused at the beginning of a frame
			PausedAtScan, ///< The detector has paused at the beginning of a scan
			Running ///< The detector is acquiring data.
		};
		XhState state;
		int group_num; ///< The current group number being acquired, only valid when not {@link #Idle}
		int frame_num; ///< The current frame number, within a group, being acquired, only valid when not {@link #Idle}
		int scan_num; ///< The current scan number, within a frame, being acquired, only valid when not {@link #Idle}
		int cycle; ///< Not supported yet
		int completed_frames; ///< The number of frames completed, only valid when not {@link #Idle}
	};

	Camera(std::string hostname, int port, std::string configName);
	~Camera();

	void init();
	void reset();
	void prepareAcq();
	void startAcq();
	void stopAcq();
	void getStatus(XhStatus& status /Out/);
	int getNbHwAcquiredFrames();

	// -- detector info object
	void getImageType(ImageType& type /Out/);
	void setImageType(ImageType type);

	void getDetectorType(std::string& type /Out/);
	void getDetectorModel(std::string& model /Out/);
	void getDetectorImageSize(Size& size /Out/);
	void getPixelSize(double& sizex, double& sizey /Out/);

	// -- Buffer control object
	HwBufferCtrlObj* getBufferCtrlObj();

	//-- Synch control object
	void setTrigMode(TrigMode mode);
	void getTrigMode(TrigMode& mode /Out/);

	void setExpTime(double exp_time);
	void getExpTime(double& exp_time /Out/);

	void setLatTime(double lat_time);
	void getLatTime(double& lat_time /Out/);
        
	// not implemented yet
	//void getExposureTimeRange(double& min_expo /Out/, double& max_expo /Out/) const;
	//void getLatTimeRange(double& min_lat /Out/, double& max_lat /Out/) const;

	void setNbFrames(int nb_frames);
	void getNbFrames(int& nb_frames /Out/);

	bool isAcqRunning() const;

	///////////////////////////
	// -- xh specific functions
	///////////////////////////

	enum HeadVoltageType {
		XhVdd, 		///>
		XhVref, 	///>
		XhVrefc, 	///>
		XhVres1, 	///>
		XhVres2, 	///>
		XhVpupref, 	///>
		XhVclamp, 	///>
		XhVled 		///>
	};

	enum ClockModeType {
		XhInternalClock,		///> internal clock
		XhESRF5468MHz,			///> ESRF 54.68 MHz clcok
		XhESRF1136MHz			///> ESRF Clock settings for RF div 31 = 11.3 MHz
	};

	enum TriggerOutputType {
		XhTrigOut_dc,				///> DC value from software polarity control only
		XhTrigOut_wholeGroup,		///> Asserted for full duration of enabled groups
		XhTrigOut_groupPreDel,		///> Pulse At start of Group before delay or trigger in
		XhTrigOut_groupPostDel,		///> Pulse At start of Group after delay and trigger in
		XhTrigOut_framePreDel,		///> Pulse At start of Frames before delay or trigger in
		XhTrigOut_framePostDel,		///> Pulse At start of Frames after delay and trigger in
		XhTrigOut_scanPreDel,		///> Pulse At start of Scans before delay or trigger in
		XhTrigOut_scanPostDel,		///> Pulse At start of Scans after delay and trigger in
		XhTrigOut_integration,		///> Assert During integration time
		XhTrigOut_aux1,				///> Derive from aux 1 pulse
		XhTrigOut_waitExtTrig,		///> Assert while waiting for external trigger
		XhTrigOut_waitOrbitSync		///> Assert while waiting for Orbit sync
	};

	enum TriggerControlType {
		XhTrigIn_noTrigger = 0,			///>
		XhTrigIn_groupTrigger = 0x1,		///>
		XhTrigIn_frameTrigger = 0x2,		///>
		XhTrigIn_scanTrigger = 0x4,		///>
		XhTrigIn_groupOrbit = 0x8,		///>
		XhTrigIn_frameOrbit = 0x10,		///>
		XhTrigIn_scanOrbit = 0x20,		///>
		XhTrigIn_fallingTrigger = 0x40		///>
	};

	//vector<std::string> getDebugMessages();
	void sendCommand(std::string cmd);
	void shutDown(std::string cmd);
	void uninterleave(bool uninterleave);
	void set16BitReadout(bool mode);
	void setDeadPixels(int first, int num, bool reset=false);
	void setXDelay(int value);

	void setHvDac(double value, bool noslew=false, int sign=0, bool direct=false);
	void getHvAdc(double& value, bool hvmon=true, bool v12=false, bool v5=false, int sign=0, bool direct=false);
	void enableHv(bool enable, bool overtemp=false, bool force=false);
	void setCalImage(double imageScale);

	void setHeadDac(double value, HeadVoltageType voltageType, int head = -1, bool direct=false);
	void getHeadAdc(double& value /Out/, int head, HeadVoltageType voltageType);
	void setHeadCaps(int capsAB, int capsCD, int head=-1);
	void setCalEn(bool onOff, int head=-1);
	//void listAvailableCaps(int32_t* capValues, int& num /Out/, bool& alt_cd /Out/);

// sip is limited to 20 args max, (see MAX_NR_ARGS in sip.h
//	void setTimingGroup(int groupNum, int nframes, int nscans, int intTime, bool last,
//			TriggerControlType trigControl=XhTrigIn_noTrigger,
//			int trigMux=-1, int orbitMux=-1, int lemoOut=0, bool correctRounding=false,
//			int groupDelay=0, int frameDelay=0, int scanPeriod = 0,
//			int auxDelay=0, int auxWidth=1,int longS12=0, int frameTime=0,
//			int shiftDown=0, int cyclesStart=1, bool cyclesEnd = false,
//			int s1Delay=0, int s2Delay=0, int xclkDelay=0, int rstRDelay=0, int rstFDelay=0,
//			bool allowExcess=false);
	void modifyTimingGroup(int group_num, int fixed_reset=-1, bool last=false, bool allowExcess=false);
	void setTimingOrbit(int delay, bool use_falling_edge=false);
	//void getTimingInfo(uint32_t* buff, int firstParam, int nParams, int firstGroup, int nGroups);
	void continueAcq();
	void setLedTiming(int pause_time, int frame_time, int int_time, bool wait_for_trig);
	void setExtTrigOutput(int trigNum, TriggerOutputType trigType, int width=0, bool invert=false);

	void setupClock(ClockModeType clockMode, int pll_gain=0, int extra_div=0, int caps=0, int r3=0, int r4=0, bool stage1=false, bool nocheck=false);

	void getSetpoint(int channel, double& value);
	void getTemperature(int channel, double& value /Out/);

	void setOffsets(int first, int num, int value, bool direct=false);
	void syncClock();

	void readFrame(void* ptr, int frame_nb);

  };

};
