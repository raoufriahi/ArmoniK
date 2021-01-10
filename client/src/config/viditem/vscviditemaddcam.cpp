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
#include "config/viditem/vscviditemaddcam.h"

VSCVidItemAddCam::VSCVidItemAddCam(VidStor cStor, ClientFactory &pFactory, QTreeWidgetItem *parent)
: VSCVidItemInf(pFactory, parent), m_cStor(cStor)
{
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/device/resources/addcam.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	setIcon(0, icon1);
	setText(0, QApplication::translate(" ", "Camera Add",0));	
	this->setTextColor(0, QColor(41, 42, 43));

}
VSCVidItemAddCam::~VSCVidItemAddCam()
{
	
}

void VSCVidItemAddCam::VidFilter(astring strFilter)
{
	int cnt = childCount();
	setExpanded(true);

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem)
		{
			pItem->VidFilter(strFilter);
		}
	}
}
