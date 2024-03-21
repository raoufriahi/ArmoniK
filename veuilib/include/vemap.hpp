/*
 * Copyright (c) 2017-2024 UbVideo
 *
 * The computer program contained herein contains proprietary
 * information which is the property of UbVideo.
 * The program may be used and/or copied only with the written
 * permission of UbVideo or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#pragma once
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include <QTabWidget>
#include "client/clientfactory.hpp"
#include <QThread>

using namespace UtilityLib;

class UB_LIBRARY_API VEMap : public QWidget
{
	Q_OBJECT
public:
	VEMap(QWidget *parent = 0);
	~VEMap();
public:
	static VEMap * CreateObject(std::string strId, ClientFactory &pFactory, QTabWidget &pTabbed, QWidget *parent);
	
};

