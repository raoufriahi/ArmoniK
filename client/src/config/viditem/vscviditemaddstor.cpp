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
#include "config/viditem/vscviditemaddstor.h"

VSCVidItemAddStor::VSCVidItemAddStor(ClientFactory &pFactory, QTreeWidgetItem *parent)
: VSCVidItemInf(pFactory, parent)
{
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/device/resources/addstor.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	setIcon(0, icon1);
	setText(0, QApplication::translate(" ", "Stor Add",0));	
	this->setTextColor(0, QColor(41, 42, 43));
	//setHidden(true);
}
VSCVidItemAddStor::~VSCVidItemAddStor()
{
	
}
