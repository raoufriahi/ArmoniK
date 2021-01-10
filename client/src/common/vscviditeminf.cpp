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
#include "common/vscviditeminf.h"

VSCVidItemInf::VSCVidItemInf(ClientFactory &pFactory, QTreeWidgetItem *parent)
: QTreeWidgetItem(parent), m_pFactory(pFactory), m_bOnline(false), m_bRec(false), 
m_bSetChecked(false)
{
	UpdateOnline(false);
}
VSCVidItemInf::~VSCVidItemInf()
{
	
}

void VSCVidItemInf::VidSetChecked(QTreeWidgetItem * item, bool bChecked)
{
	setFlags(flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
	//setCheckState(Qt::Unchecked);
	/* The event is send from myself */
	if (item == this)
	{
		return;
	}
	if (bChecked == true)
	{
		setCheckState(0, Qt::Checked);
	}else
	{
		setCheckState(0, Qt::Unchecked);
	}

	return;
}

bool VSCVidItemInf::VidGetChecked()
{
	if (checkState(0) == Qt::Checked)
	{
		return true;
	}else
	{
		return false;
	}
}

void VSCVidItemInf::UpdateOnline(bool bOnline)
{
	if (bOnline == true)
	{
		this->setTextColor(0, QColor(0, 170, 0));
		m_bOnline = true;
		if (m_bRec == true)
		{
			this->setTextColor(0, QColor(255, 0, 0));
		}
	}else
	{
	    	this->setTextColor(0, QColor(194, 194, 194));
		m_bOnline = false;
	}
}

void VSCVidItemInf::UpdateRec(bool bRec)
{
	m_bRec = bRec;

	if (bRec == true && m_bOnline == true)
	{
		this->setTextColor(0, QColor(255, 0, 0));
	}

	if (bRec == false && m_bOnline == true)
	{
		this->setTextColor(0, QColor(0, 170, 0));
	}
	
}
