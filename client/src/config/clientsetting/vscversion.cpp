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
#include "config/clientsetting/vscversion.h"
#include "debug.hpp"


VSCVersion::VSCVersion(ClientFactory &pFactory, QWidget *parent)
    : m_pFactory(pFactory), QWidget(parent)
{
	
	ui.setupUi(this);
	ui.ver->setText(VE_VERSION);
	ui.info->setText(VE_INFO);
}




void VSCVersion::applyConfig()
{

	return;

}


