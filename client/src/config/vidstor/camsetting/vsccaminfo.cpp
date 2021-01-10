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
#include "config/vidstor/camsetting/vsccaminfo.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include <QDesktopWidget>
#include "client/storsyncinf.hpp"



VSCCamInfo::VSCCamInfo(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent)
    : m_pFactory(pFactory), m_pStor(stor), m_strCam(strCam), QWidget(parent), 
    m_strCamName("Loading...")
{
	//VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	syncInf.Connect();
	
	ui.setupUi(this);
	
	VidCamera pCam;
	
	syncInf.GetVidCamera(m_strCam, pCam);
	switch(pCam.ntype())
	{
		case VID_FILE:
			ui.radioButtonFile->setChecked(true);
			break;
		case VID_RTSP:
			ui.radioButtonRtsp->setChecked(true);
			break;
		case VID_ONVIF_S:
			ui.radioButtonOnvif->setChecked(true);
			break;
		case VID_MJPEG:
			ui.radioButtonMJPEG->setChecked(true);
			break;
		default:
			break;
	}
	m_strCamName = pCam.strname();
	ui.lineEditName->setText(pCam.strname().c_str());
	ui.lineEditIP->setText(pCam.strip().c_str());
	ui.lineEditPort->setText(pCam.strport().c_str());
	ui.lineEditFile->setText(pCam.strfile().c_str());
	ui.lineEditRtspAddr->setText(pCam.strrtspurl().c_str());

	//delete pLoading;
}




void VSCCamInfo::applyConfig()
{

	return;

}


