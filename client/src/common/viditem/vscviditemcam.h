/*
 * Copyright (c) 2017-2018 Heimdall
 *
 * The computer program contained herein contains proprietary
 * information which is the property of Heimdall.
 * The program may be used and/or copied only with the written
 * permission of Heimdall or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#ifndef __VSC_VID_ITEM_CAM_H__
#define __VSC_VID_ITEM_CAM_H__

#include "common/vscviditeminf.h"

class VSCVidItemCam : public VSCVidItemInf
{
public:
    VSCVidItemCam(VidCamera cCam, VidCameraId cCamId, ClientFactory &pFactory, QTreeWidgetItem *parent);
    ~VSCVidItemCam();
public:
	virtual astring GetId(){return m_cCam.strid();}
	virtual astring GetName(){return m_cCam.strname();}
	virtual VidCameraId GetCameraId(){return m_cCamId;}
	virtual void VidFilter(astring strFilter);
	virtual void VidGetSelectedItems(VidCameraIdMap &pMap);
private:
	VidCamera m_cCam;
	VidCameraId m_cCamId;/* The ID include the Stor ID can Camera ID */

};

#endif /* __VSC_VID_ITEM_CAM_H__ */
