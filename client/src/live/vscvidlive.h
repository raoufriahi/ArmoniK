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
#ifndef __VSC_VID_LIVE_H__
#define __VSC_VID_LIVE_H__

#include "common/vscvidinf.h"
#include "live/vscview.h"
#include <QDockWidget>

class VSCVidLive : public VSCVidInf
{
    Q_OBJECT
public:
    VSCVidLive(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent);
    ~VSCVidLive();
	
public:
	virtual void VidShow();
	virtual void VidHide();
	virtual void VidNewLiveView();
	virtual void VidNewLivePB();
	virtual void VidNewEmap(std::string strId);

	virtual VSCVidTreeInf *GetCameraTree();
	virtual VSCVidTreeInf *GetGroupTree();
	virtual VSCVidTreeInf *GetEmapTree();
	virtual VSCVidTreeInf *GetViewTree();
	virtual QWidget * GetMainView();

private:
	VSCView *m_pView;
	VSCVidTreeInf *m_pCameraTree;
	VSCVidTreeInf *m_pGroupTree;
	VSCVidTreeInf *m_pViewTree;
	VSCVidTreeInf *m_pEmapTree;
	
};
	
#endif
