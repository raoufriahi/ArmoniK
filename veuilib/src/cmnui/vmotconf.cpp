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
#include "vmotconf.hpp"
#include "ui_vmotconf.h"
#include "recordwrapper.hpp"
#include <QRubberBand>
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>

class VMotConfPri
{
public:
	VMotConfPri();
	~VMotConfPri();
public:
};

VMotConfPri::VMotConfPri()
{
}
VMotConfPri::~VMotConfPri()
{
}

VMotConf::VMotConf(QWidget *parent, Qt::WindowFlags flags)
	: QWidget(parent, flags), p_ui(new Ui::VMotConf), ui(*p_ui), 
	m_pPri(NULL)
{
	m_pPri = new VMotConfPri();
	ui.setupUi(this);


	connect(ui.pbOff, SIGNAL(clicked()), this, SLOT(SlotMotOff()));
	connect(ui.pbOn, SIGNAL(clicked()), this, SLOT(SlotMotOn()));
	connect(ui.pbAll, SIGNAL(clicked()), this, SLOT(SlotMotAll()));
	connect(ui.pbClear, SIGNAL(clicked()), this, SLOT(SlotMotClear()));
}

VMotConf::~VMotConf()
{
	if (m_pPri)
	{
		delete m_pPri;
		m_pPri = NULL;
	}
	
}

bool VMotConf::GetCurrentMot(VVidMotReg &pMot)
{	
	ui.widget->GetCurrentMot(pMot);
	return true;
}

bool VMotConf::SetCurrentMot(VVidMotReg &pMot)
{
	ui.widget->SetCurrentMot(pMot);
	return true;
}

bool VMotConf::UpdateImage(astring strUrl)
{
	ui.widget->UpdateImage(strUrl);
	return true;
}

void VMotConf::SlotMotOff()
{
	ui.widget->UpdateMotType(false);
}
void VMotConf::SlotMotOn()
{
	ui.widget->UpdateMotType(true);
}

void VMotConf::SlotMotAll()
{
	ui.widget->SetMotAll();
}

void VMotConf::SlotMotClear()
{
	ui.widget->SetMotClear();
}





