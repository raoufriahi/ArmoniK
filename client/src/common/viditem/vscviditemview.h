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
#ifndef __VSC_VID_ITEM_VIEW_H__
#define __VSC_VID_ITEM_VIEW_H__

#include "common/vscviditeminf.h"

class VSCVidItemView : public VSCVidItemInf
{
public:
    VSCVidItemView(VidView cView, ClientFactory &pFactory, QTreeWidgetItem *parent);
    ~VSCVidItemView();
public:
	virtual astring GetId(){return m_cView.strid();}
	virtual astring GetName(){return m_cView.strname();}
	virtual void VidFilter(astring strFilter);
private:
	VidView m_cView;

};

#endif /* __VSC_VID_ITEM_VIEW_H__ */
