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
#ifndef __V_MOT_CONF__
#define __V_MOT_CONF__

#include <QWidget>
#define NOMINMAX
#define NOMINMAX 
#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "debug.hpp"
#include "server/factory.hpp"
#include "client/clientfactory.hpp"
#include "client/storstream.hpp"
#include "vsmotalgo.hpp"
#include <QTimer>
#include <QMenu>
#include <QMimeData>
using  namespace tthread;

namespace Ui {
class VMotConf;
}

class VMotConfPri;

class VE_LIBRARY_API VMotConf : public QWidget
{
	Q_OBJECT

public:
	VMotConf(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VMotConf();
public:
	bool GetCurrentMot(VVidMotReg &pMot);
	bool SetCurrentMot(VVidMotReg &pMot);
	bool UpdateImage(astring strUrl);

public slots:
	void SlotMotOff();
	void SlotMotOn();
	void SlotMotAll();
	void SlotMotClear();

public:
	Ui::VMotConf *p_ui;
	Ui::VMotConf &ui;
	
private:
	VMotConfPri * m_pPri;
    
};


#endif // __V_Mot_CONF__
