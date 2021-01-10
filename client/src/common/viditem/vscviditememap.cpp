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
#include "common/viditem/vscviditememap.h"

VSCVidItemEmap::VSCVidItemEmap(VidEmap cEmap, ClientFactory &pFactory, QTreeWidgetItem *parent)
	: m_cEmap(cEmap), VSCVidItemInf(pFactory, parent)
{
	QIcon icon1;

	icon1.addFile(QStringLiteral(":/action/resources/map.png"), 
	            QSize(), QIcon::Normal, QIcon::Off);

	setIcon(0, icon1);

	setText(0, QApplication::translate(" ", m_cEmap.strname().c_str(), 0));
	this->setTextColor(0, QColor(41, 42, 43));
	//setHidden(true);
}
VSCVidItemEmap::~VSCVidItemEmap()
{
	
}

void VSCVidItemEmap::VidFilter(astring strFilter)
{
	VDC_DEBUG( "%s %d Filter %s name %s\n",__FUNCTION__, __LINE__, 
		strFilter.c_str(), m_cEmap.strname().c_str());
	if (strFilter.size() == 0)
	{
		setHidden(false);
		return;
	}
	std::size_t found = m_cEmap.strname().find(strFilter);
	if (found != std::string::npos)
	{
		setHidden(false);
	}else
	{
		setHidden(true);
	}
}
