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
#ifndef __VSC_VID_ITEM_VIEW_CONF_H__
#define __VSC_VID_ITEM_VIEW_CONF_H__

#include "common/vscviditeminf.h"

class VSCVidItemViewConf : public QObject, public VSCVidItemInf
{
Q_OBJECT
public:
    VSCVidItemViewConf(ClientFactory &pFactory, QTreeWidgetItem *parent);
    ~VSCVidItemViewConf();
public:

};

#endif /* __VSC_VID_ITEM_ADDSTOR_H__ */
