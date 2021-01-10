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
#include <QMouseEvent>
#include <QPainter>
#include <QDrag>

#include "common/vidtree/vscvidtreeemap.h"
#include "common/viditem/vscviditememap.h"

VSCVidTreeEmap::VSCVidTreeEmap(ClientFactory &pFactory, QWidget *parent)
: VSCVidTreeInf(pFactory, parent), m_pRoot(NULL), m_bInit(false)
{
	
}
VSCVidTreeEmap::~VSCVidTreeEmap()
{
	
}

void VSCVidTreeEmap::Init()
{
	if (m_bInit == true)
	{
		return;
	}
	m_pRoot = new  QTreeWidgetItem(this);
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/action/resources/map.png"), QSize(), QIcon::Normal, QIcon::Off);
	
	m_pRoot->setIcon(0, icon1);

	m_pRoot->setText(0, QApplication::translate("Emap",
	        "Emap", 0));
	
	addTopLevelItem(m_pRoot);
	m_pRoot->setExpanded(true);
	
	TreeUpdate();
	
	/* Register the device callback */
	//m_pFactory.RegChangeNotify((void *)this, VSCVidTreeEmap::CallChange);
	connect(&(m_pFactory), SIGNAL(SignalCallChange(int, std::string)), this, SLOT(SlotCallChange(int, std::string)));
	m_bInit = true;
}

void VSCVidTreeEmap::mouseMoveEvent(QMouseEvent *event)
{

}

void VSCVidTreeEmap::mousePressEvent(QMouseEvent *event)
{
	QTreeWidgetItem *selectedItem = currentItem();

	if (event->buttons() & Qt::RightButton)
	{
		return;
	}
	VDC_DEBUG( "%s  %d \n",__FUNCTION__, __LINE__);
	
	QTreeWidget::mousePressEvent(event);

}

void VSCVidTreeEmap::mouseDoubleClickEvent(QMouseEvent *event)
{
	QTreeWidgetItem *selectedItem = currentItem();

	if (event->buttons() & Qt::RightButton)
	{
		return;
	}

	VDC_DEBUG( "%s  %d \n",__FUNCTION__, __LINE__);

	if (selectedItem)
	{
		VSCVidItemEmap *pEmap = dynamic_cast<VSCVidItemEmap * >(selectedItem);
		if (pEmap)
		{
			emit EmapSelected(pEmap->GetId());
			QTreeWidget::mouseDoubleClickEvent(event);
			event->accept();
			return ;
		}

	}
	
	QTreeWidget::mouseDoubleClickEvent(event);
	
}

void VSCVidTreeEmap::SlotCallChange(int type, std::string strId)
{
	ClientFactoryChangeData data;
	data.type = (ClientFactoryChangeType)type;
	data.id = strId;
	CallChange(data);
}

bool VSCVidTreeEmap::CallChange(ClientFactoryChangeData data)
{
	switch (data.type)
	{
		case CLIENT_FACTORY_EMAP_ADD:
		{
			TreeUpdate();
			break;
		}
		case CLIENT_FACTORY_EMAP_DEL:
		{
			TreeUpdate();
			break;
		}
		default:
		   	break;
	};

	return true;
}

void VSCVidTreeEmap::TreeUpdate()
{
	/* Delete all the child */
	qDeleteAll(m_pRoot->takeChildren());

	VidEmapList emapList;
	m_pFactory.GetConfDB().GetEmapListConf(emapList);
	int emapSize = emapList.cvidemap_size();
	
	for (s32 i = 0; i < emapList.cvidemap_size(); i++)
	{
		VidEmap pEmap = emapList.cvidemap(i);
		VSCVidItemEmap *pItemView = new  VSCVidItemEmap(pEmap, 
								m_pFactory, m_pRoot);
	}
	
}

void VSCVidTreeEmap::VidFilter(astring strFilter)
{
	int cnt = m_pRoot->childCount();
	m_pRoot->setExpanded(true);

	for (int i = 0; i < cnt; i ++)
	{
		QTreeWidgetItem * pChild = m_pRoot->child(i);
		VSCVidItemInf *pItem = dynamic_cast<VSCVidItemInf*>(pChild);
		if (pItem)
		{
			pItem->VidFilter(strFilter);
		}
	}
}





