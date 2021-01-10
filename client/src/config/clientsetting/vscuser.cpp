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
#include "config/clientsetting/vscuser.h"
#include "debug.hpp"
#include "vscloading.hpp"

VSCUser::VSCUser(ClientFactory &pFactory, QWidget *parent)
	: QWidget(parent), m_pFactory(pFactory)
{
	ui.setupUi(this);
	ui.user->setText("admin");
	ui.autoLogin->setToggle(false);
	/* Setup the default value */
	if (m_pFactory.GetAutoLogin() == true)
	{
	        ui.autoLogin->setToggle(true);
	}

	if (m_pFactory.GetAutoFullScreen() == true)
	{
	        ui.autoFullScreen->setToggle(true);
	}
	connect( this->ui.pushButtonApply, SIGNAL( clicked() ), this, SLOT(applyConfig()));
}

void VSCUser::applyConfig()
{
	astring strCurUser = "admin";
	astring strCurPasswd;
	astring strPasswd;
	astring strPasswd2;

	astring oldPasswd;
	m_pFactory.GetAdminPasswd(oldPasswd);
	
	strCurPasswd = ui.curPasswd->text().toStdString();
	strPasswd = ui.newPasswd->text().toStdString();
	strPasswd2 = ui.repeatPasswd->text().toStdString();

	if (ui.changePasswd->isToggled())
	{
		if (m_pFactory.AuthUser(strCurUser, strCurPasswd) == true
			&& strPasswd == strPasswd2)
		{

			VSCLoading *loading = new VSCLoading(this);
			loading->Processing(3);
			SimpleCrypt crypt;

			QString strPassCrypt = strPasswd.c_str();
			m_pFactory.SetAdminPasswd(crypt.encryptToString(strPassCrypt).toStdString());
			
			delete loading;
		}else
		{
			QMessageBox msgBox(this);
			//Set text
			msgBox.setText(tr("Password not correct ..."));
			//Set predefined icon, icon is show on left side of text.
			msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));

			msgBox.setStandardButtons(QMessageBox::Ok);
			//Set focus of ok button
			msgBox.setDefaultButton(QMessageBox::Ok);

			//execute message box. method exec() return the button value of cliecke button
			int ret = msgBox.exec();				
		}
	}

	VSCLoading *loading = new VSCLoading(this);
	loading->Processing(3);

	
	m_pFactory.SetAutoLogin(ui.autoLogin->isToggled());
	m_pFactory.SetAutoFullScreen(ui.autoFullScreen->isToggled());
	
	delete loading;
	return;

}


