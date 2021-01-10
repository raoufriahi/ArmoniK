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
#include "config/vidstor/storsetting/vscstorversion.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include <QDesktopWidget>
#include "client/storsyncinf.hpp"



VSCStorVersion::VSCStorVersion(ClientFactory &pFactory, VidStor &stor, QWidget *parent)
    : m_pFactory(pFactory), m_pStor(stor), QWidget(parent)
{
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	astring pVer;
	astring strInfo;
	syncInf.Connect();
	syncInf.GetVer(pVer, strInfo);
	
	ui.setupUi(this);
	ui.ver->setText(pVer.c_str());
	ui.info->setText(strInfo.c_str());

	delete pLoading;
}




void VSCStorVersion::applyConfig()
{

	return;

}


