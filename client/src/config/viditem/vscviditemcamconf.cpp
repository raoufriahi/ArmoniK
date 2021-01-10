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
#include "config/viditem/vscviditemcamconf.h"

VSCVidItemCamConf::VSCVidItemCamConf(VidStor cStor, VidCamera cCam, 
ClientFactory &pFactory, QTreeWidgetItem *parent)
: m_cCam(cCam), m_cStor(cStor), VSCVidItemInf(pFactory, parent)
{
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/device/resources/camera.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	setIcon(0, icon1);

	setText(0, QApplication::translate(" ", m_cCam.strname().c_str(), 0));	
	//setHidden(true);
}
VSCVidItemCamConf::~VSCVidItemCamConf()
{
	
}

void VSCVidItemCamConf::VidFilter(astring strFilter)
{
	if (strFilter.size() == 0)
	{
		setHidden(false);
		return;
	}
	std::size_t found = m_cCam.strname().find(strFilter);
	if (found != std::string::npos)
	{
		setHidden(false);
	}else
	{
		setHidden(true);
	}
}
