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
#ifndef __VSC_VID_ITEM_EMAP_H__
#define __VSC_VID_ITEM_EMAP_H__

#include "common/vscviditeminf.h"

class VSCVidItemEmap : public VSCVidItemInf
{
public:
    VSCVidItemEmap(VidEmap cEmap, ClientFactory &pFactory, QTreeWidgetItem *parent);
    ~VSCVidItemEmap();
public:
	virtual astring GetId(){return m_cEmap.strid();}
	virtual astring GetName(){return m_cEmap.strname();}
	virtual void VidFilter(astring strFilter);
private:
	VidEmap m_cEmap;

};

#endif /* __VSC_VID_ITEM_EMAP_H__ */
