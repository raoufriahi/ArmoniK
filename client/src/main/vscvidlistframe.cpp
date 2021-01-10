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
#include "main/vscvidlistframe.h"

VSCVidListFrame::VSCVidListFrame(QWidget * parent, Qt::WindowFlags f)
: QFrame(parent, f)
{
	
}
VSCVidListFrame::~VSCVidListFrame()
{
	
}

void VSCVidListFrame::mousePressEvent(QMouseEvent *event)
{
	Qt::MouseButtons mouseButtons = event->buttons();
    if( mouseButtons != Qt::LeftButton )
    {
        return;
    }	
	
	emit SignalClicked();
	VDC_DEBUG( "%s mousePressEvent %d\n",__FUNCTION__);
}
