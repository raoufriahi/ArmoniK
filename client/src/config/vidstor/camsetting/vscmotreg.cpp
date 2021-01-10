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
#include <QMessageBox>
#include <QVariant>
#include "config/vidstor/camsetting/vscmotreg.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include "client/storsyncinf.hpp"
#include "server/camera.hpp"

VSCMotRegConf::VSCMotRegConf(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory), m_pStor(stor), m_strCam(strCam)
{

	ui.setupUi(this);

	VSCLoading loading(NULL);
	StorSyncInf syncInf(m_pStor);
	
	syncInf.Connect();
	VidCamera pCam;
	astring url = "http://" + m_pStor.struser() + ":" + m_pStor.strpasswd() + "@" + 
				m_pStor.strip() + ":9081/vapi/GetImage?Camera=" + m_strCam;

	ui.conf->UpdateImage(url);
	
	if (syncInf.GetVidCamera(m_strCam, pCam)  == true)
	{
		//return ;
		
		astring strMotReg = pCam.strmotreg();
		if (VVidMotReg::CheckStringForReg(strMotReg) == true)
		{
			VVidMotReg mot(strMotReg);
			ui.conf->SetCurrentMot(mot);
		}
	}


	
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}

void VSCMotRegConf::applyConfig()
{
	VVidMotReg mot;
	if (ui.conf->GetCurrentMot(mot) == false)
	{
		return;
	}
	VSCLoading loading(NULL);
	StorSyncInf syncInf(m_pStor);
	
	syncInf.Connect();
	VidCamera pCam;
	if (syncInf.GetVidCamera(m_strCam, pCam)  == false)
	{
		return ;
	}
	pCam.set_strmotreg(mot.ToString());

	syncInf.AddCam(pCam);

	return;
}


