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
#ifndef VSC_HDD_EDIT_H
#define VSC_HDD_EDIT_H

#include <QWidget>
#include "ui_vschddedit.h"
#include "utility.hpp"
#include <QTimer>
#include "config/vidstor/disksetting/vschddoneedit.h"

#define VSC_SUPPORTED_DISKS 128

class VSCHddEdit : public QWidget
{
	Q_OBJECT

public:
	VSCHddEdit(VidStor &stor, QWidget *parent = 0);
	~VSCHddEdit(){}
	
public:
	Ui::VSCHddEdit ui;
	
private:
	VSCHddOneEdit *  m_Disks[VSC_SUPPORTED_DISKS];
	VidStor m_pStor;

    
};

#endif // VSC_HDD_DEVICE_H
