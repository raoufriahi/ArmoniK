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
#ifndef __VSC_VID_SEARCH_PB_H__
#define __VSC_VID_SEARCH_PB_H__

#include "common/vscvidinf.h"
#include "searchpb/vscvideventsearch.h"

class VSCVidSearchPB : public VSCVidInf
{
    Q_OBJECT
public:
    VSCVidSearchPB(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent);
    ~VSCVidSearchPB();
	
public:
	virtual void VidShow();
	virtual void VidHide();
	virtual void VidNewEventSearch();
	virtual void VidNewMotionSearch();
	virtual VSCVidTreeInf *GetCameraTree();
	virtual bool CheckClosed(QWidget * pTab);

private:
	VSCVidTreeInf *m_pCameraTree;
	VSCVidEventSearch *m_pSearch;

};
#endif
	
