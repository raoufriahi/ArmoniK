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
#ifndef __V_EMAP_HPP__
#define __V_EMAP_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "videotype.hpp"
#include <QWidget>
#include <QTabWidget>
#include "client/clientfactory.hpp"
#include <QThread>

using namespace UtilityLib;


class VE_LIBRARY_API VEMap : public QWidget
{
	Q_OBJECT
public:
	VEMap(QWidget *parent = 0);
	~VEMap();
public:
	static VEMap * CreateObject(std::string strId, ClientFactory &pFactory, QTabWidget &pTabbed, QWidget *parent);
	
};


#endif /* __V_EMAP_HPP__ */
