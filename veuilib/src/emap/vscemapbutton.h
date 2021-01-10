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
#ifndef __VIDEO_EMAP_BUTTON_H__
#define __VIDEO_EMAP_BUTTON_H__

#include <QWidget>
#include <QMenu>
#include <QPushButton>
#include "utility.hpp"
#include "server/factory.hpp"

#define NOMINMAX
#define NOMINMAX 
//#include <inttypes.h>


#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
using  namespace tthread;


class VSCEmapButton : public QPushButton
{
	Q_OBJECT

public:
	VSCEmapButton(QWidget *parent = 0);
	~VSCEmapButton();
	void mousePressEvent(QMouseEvent *event);
	void DragCommon();
public:
	BOOL setId(s32 nId)
	{
		m_nId = nId;
		return TRUE;
	}
private:
	s32 m_nId;


};

#endif // __VIDEO_EMAP_BUTTON_H__
