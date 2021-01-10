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
#ifndef __VEVENT_DB_HPP__
#define __VEVENT_DB_HPP__
#include "utility.hpp"
#include "Poco/Data/Session.h"
#include "utility/videotype.hpp"
#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"

using namespace UtilityLib;
using namespace XSDK;

/*
Event Time:
MotionStart
MotionEnd

*/

class VE_LIBRARY_API VEventDB
{
public:
    VEventDB(astring & strPath);
    ~VEventDB();
	
public:
	BOOL Init();
	BOOL CreateTablesIfNecessary();
	BOOL DeInit();
public:
	BOOL NewEvent(astring strDev, astring strDevName, astring strType, s64 nEvtTime, astring strDesc);
	
private:
	XMutex m_cMutex;
    astring m_strIndexPath;
    Poco::Data::Session *m_DB;
};
#endif /* __VEVENT_DB_HPP__ */
