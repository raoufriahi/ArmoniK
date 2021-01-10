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
#include "vsclogin.h"
#include "debug.hpp"


VSCLogin::VSCLogin(QWidget *parent)
    : QDialog(parent), m_isLogin(FALSE)
{
   ui.setupUi(this);
   //setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
   setWindowFlags(windowFlags() &~ Qt::WindowCloseButtonHint); 

   connect(ui.pbLogin, SIGNAL(clicked()), this, SLOT(LoginClicked()));
   connect(ui.pbExit, SIGNAL(clicked()), this, SLOT(ExitClicked()));
   connect(ui.passwd, SIGNAL(returnPressed()), ui.pbLogin, SIGNAL(clicked()));
    


}

void VSCLogin::LoginClicked()
{
	m_isLogin = TRUE;
}
void VSCLogin::ExitClicked()
{
	//m_Type = VSC_IPCGROUP_CONF_CANCEL;
}

BOOL VSCLogin::GetUserPasswd(astring &strUser, astring &strPasswd)
{
 	strUser = ui.username->text().toStdString();
	strPasswd = ui.passwd->text().toStdString();

	return TRUE;
}


void VSCLogin::mousePressEvent(QMouseEvent *event)  
{  
    if (event->button() == Qt::LeftButton) {  
        m_Drag = true;  
        m_DragPosition = event->globalPos() - this->pos();  
        event->accept();  
    }  
}  
  
void VSCLogin::mouseMoveEvent(QMouseEvent *event)  
{  
    if (m_Drag && (event->buttons() && Qt::LeftButton)) {  
        move(event->globalPos() - m_DragPosition);  
        event->accept();  
    }  
}  
  
void VSCLogin::mouseReleaseEvent(QMouseEvent *)  
{  
    m_Drag = false;  
} 



