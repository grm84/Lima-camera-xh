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
//
// XhInterface.h
// Created on: Feb 04, 2013
// Author: g.r.mant

#ifndef XHINTERFACE_H_
#define XHINTERFACE_H_

#include "lima/HwInterface.h"

namespace lima {
namespace Xh {

class Interface;
class Camera;

/*******************************************************************
 * \class DetInfoCtrlObj
 * \brief Control object providing Xh detector info interface
 *******************************************************************/

class DetInfoCtrlObj: public HwDetInfoCtrlObj {
DEB_CLASS_NAMESPC(DebModCamera, "DetInfoCtrlObj","Xh");

public:
	DetInfoCtrlObj(Camera& cam);
	virtual ~DetInfoCtrlObj();

	virtual void getMaxImageSize(Size& max_image_size);
	virtual void getDetectorImageSize(Size& det_image_size);

	virtual void getDefImageType(ImageType& def_image_type);
	virtual void getCurrImageType(ImageType& curr_image_type);
	virtual void setCurrImageType(ImageType curr_image_type);

	virtual void getPixelSize(double& x_size, double &y_size);
	virtual void getDetectorType(std::string& det_type);
	virtual void getDetectorModel(std::string& det_model);

	virtual void registerMaxImageSizeCallback(HwMaxImageSizeCallback& cb);
	virtual void unregisterMaxImageSizeCallback(HwMaxImageSizeCallback& cb);

private:
	Camera& m_cam;
};

/*******************************************************************
 * \class SyncCtrlObj
 * \brief Control object providing Xh synchronization interface
 *******************************************************************/

class SyncCtrlObj: public HwSyncCtrlObj {
DEB_CLASS_NAMESPC(DebModCamera,"SyncCtrlObj","Xh");

public:
	SyncCtrlObj(Camera& cam); //, BufferCtrlObj& buffer);
	virtual ~SyncCtrlObj();

	virtual bool checkTrigMode(TrigMode trig_mode);
	virtual void setTrigMode(TrigMode trig_mode);
	virtual void getTrigMode(TrigMode& trig_mode);

	virtual void setExpTime(double exp_time);
	virtual void getExpTime(double& exp_time);

	virtual void setLatTime(double lat_time);
	virtual void getLatTime(double& lat_time);

	virtual void setNbHwFrames(int nb_frames);
	virtual void getNbHwFrames(int& nb_frames);

	virtual void getValidRanges(ValidRangesType& valid_ranges);

private:
	Camera& m_cam;
};

/*******************************************************************
 * \class Interface
 * \brief Xh hardware interface
 *******************************************************************/

class Interface: public HwInterface {
DEB_CLASS_NAMESPC(DebModCamera, "Interface", "Xh");

public:
	Interface(Camera& cam);
	virtual ~Interface();
	virtual void getCapList(CapList&) const;
	virtual void reset(ResetLevel reset_level);
	virtual void prepareAcq();
	virtual void startAcq();
	virtual void stopAcq();
	virtual void getStatus(StatusType& status);
	virtual int getNbHwAcquiredFrames();

private:
	Camera& m_cam;
	CapList m_cap_list;
	DetInfoCtrlObj m_det_info;
	HwBufferCtrlObj*  m_bufferCtrlObj;
	SyncCtrlObj m_sync;
};

} // namespace Xh
} // namespace lima

#endif /* XHINTERFACE_H_ */
