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
#ifndef VSCCALCULATE_H
#define VSCCALCULATE_H
#include "utility.hpp"
#include "debug.hpp"
#include <QObject>
#include <QLabel>
#include <QTimer>

using namespace UtilityLib;

class cpuOccupy;

class VE_LIBRARY_API VSCCalculate: public QObject
{
	Q_OBJECT
public:
	VSCCalculate(QLabel* label);
	~VSCCalculate();
public slots:
	void showCpuMemoryState();

private:
	QLabel* m_label;
	QTimer* m_Timer;
#ifndef WIN32
	cpuOccupy *m_LastCPU;
#endif
};

#endif
