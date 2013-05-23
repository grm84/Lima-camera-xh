//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2011
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
/*
 * XhCamera.h
 * Created on: Feb 04, 2013
 * Author: g.r.mant
 */
#ifndef XHCAMERA_H_
#define XHCAMERA_H_

#include <stdlib.h>
#include <limits>
#include <stdarg.h>
#include <strings.h>
#include "HwMaxImageSizeCallback.h"
#include "HwBufferMgr.h"
#include "HwInterface.h"
#include "XhInterface.h"
#include <ostream>
#include "Debug.h"
#include "XhClient.h"

using namespace std;

namespace lima {
namespace xh {

const int xPixelSize = 1;
const int yPixelSize = 1;

class BufferCtrlObj;

/*******************************************************************
 * \class Camera
 * \brief object controlling the Xh camera
 *******************************************************************/
class Camera {
DEB_CLASS_NAMESPC(DebModCamera, "Camera", "Xh");

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

Camera(string hostname, int port, string configName);
	~Camera();

	void init();
	void reset();
	void prepareAcq();
	void startAcq();
	void stopAcq();
	void getStatus(XhStatus& status);
	int getNbHwAcquiredFrames();

	// -- detector info object
	void getImageType(ImageType& type);
	void setImageType(ImageType type);
//
	void getDetectorType(std::string& type);
	void getDetectorModel(std::string& model);
	void getDetectorImageSize(Size& size);
	void getPixelSize(double& sizex, double& sizey);

	// -- Buffer control object
	HwBufferCtrlObj* getBufferCtrlObj();

	//-- Synch control object
	void setTrigMode(TrigMode mode);
	void getTrigMode(TrigMode& mode);

	void setExpTime(double exp_time);
	void getExpTime(double& exp_time);

	void setLatTime(double lat_time);
	void getLatTime(double& lat_time);

	void getExposureTimeRange(double& min_expo, double& max_expo) const;
	void getLatTimeRange(double& min_lat, double& max_lat) const;

	void setNbFrames(int nb_frames);
	void getNbFrames(int& nb_frames);

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
		XhESRF5468MHz,		///> ESRF 54.68 MHz clcok
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
		XhTrigIn_groupTrigger = 0x1,	///>
		XhTrigIn_frameTrigger = 0x2,	///>
		XhTrigIn_scanTrigger = 0x4,		///>
		XhTrigIn_groupOrbit = 0x8,		///>
		XhTrigIn_frameOrbit = 0x10,		///>
		XhTrigIn_scanOrbit = 0x20,		///>
		XhTrigIn_fallingTrigger = 0x40	///>
	};

	vector<string> getDebugMessages();
	void sendCommand(string cmd);
	void shutDown(string cmd);
	void uninterleave(bool uninterleave);
	void set16BitReadout(bool mode);
	void setDeadPixels(int first, int num, bool reset=false);
	void setXDelay(int value);

	void setHvDac(double value, bool noslew=false, int sign=0, bool direct=false);
	void getHvAdc(double& value, bool hvmon=true, bool v12=false, bool v5=false, int sign=0, bool direct=false);
	void enableHv(bool enable, bool overtemp=false, bool force=false);
	void setCalImage(double imageScale);

	void setHeadDac(double value, HeadVoltageType voltageType, int head = -1, bool direct=false);
	void getHeadAdc(double& value, int head, HeadVoltageType voltageType);
	void setHeadCaps(int capsAB, int capsCD, int head=-1);
	void setCalEn(bool onOff, int head=-1);
	void listAvailableCaps(int32_t* capValues, int& num, bool& alt_cd);

	void setTimingGroup(int groupNum, int nframes, int nscans, int intTime, bool last,
			TriggerControlType trigControl=XhTrigIn_noTrigger,
			int trigMux=-1, int orbitMux=-1, int lemoOut=0, bool correctRounding=false,
			int groupDelay=0, int frameDelay=0, int scanPeriod = 0,
			int auxDelay=0, int auxWidth=1,int longS12=0, int frameTime=0,
			int shiftDown=0, int cyclesStart=1, bool cyclesEnd = false,
			int s1Delay=0, int s2Delay=0, int xclkDelay=0, int rstRDelay=0, int rstFDelay=0,
			bool allowExcess=false);
	void modifyTimingGroup(int group_num, int fixed_reset=-1, bool last=false, bool allowExcess=false);
	void setTimingOrbit(int delay, bool use_falling_edge=false);
	void getTimingInfo(uint32_t* buff, int firstParam, int nParams, int firstGroup, int nGroups);
	void continueAcq();
	void setLedTiming(int pause_time, int frame_time, int int_time, bool wait_for_trig);
	void setExtTrigOutput(int trigNum, TriggerOutputType trigType, int width=0, bool invert=false);

	void setupClock(ClockModeType clockMode, int pll_gain=0, int extra_div=0, int caps=0, int r3=0, int r4=0, bool stage1=false, bool nocheck=false);

	void getSetpoint(int channel, double& value);
	void getTemperature(int channel, double& value);

	void setOffsets(int first, int num, int value, bool direct=false);
	void syncClock();

	void readFrame(void* ptr, int frame_nb);

private:
	// xh specific
	XhClient *m_xh;
	string m_hostname;
	int m_port;
	string m_configName;
	string m_sysName;

	int m_uninterleave;
	int m_npixels;
	int m_nb_groups;
	int m_openHandle;

	class AcqThread;

	AcqThread *m_acq_thread;
	TrigMode m_trigger_mode;
	double m_exp_time;
	ImageType m_image_type;
	int m_nb_frames; // nos of frames to acquire
	bool m_thread_running;
	bool m_wait_flag;
	bool m_quit;
	int m_acq_frame_nb; // nos of frames acquired
	mutable Cond m_cond;


	// Buffer control object
	SoftBufferCtrlObj m_bufferCtrlObj;

};

} // namespace xh
} // namespace lima

#endif /* XHCAMERA_H_ */