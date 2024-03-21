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
#ifndef __VSC_LOADING_HPP__
#define __VSC_LOADING_HPP__

#include <QDialog>
#include <QTimer>
#include <QLabel>
#include <QMovie>
#include <QThread>
#include "server/factory.hpp"
#include "utility.hpp"
#include "debug.hpp"

using namespace UtilityLib;

class VE_LIBRARY_API VSCLoading : public QWidget
{
	Q_OBJECT
public:
	VSCLoading(QWidget *parent);
	~VSCLoading();
public:
	void Processing(int cnt);
	static VSCLoading * Create();
private:
	QLabel * m_label;
	QMovie * m_movie;
};


#endif // __VSC_LOADING_HPP__
