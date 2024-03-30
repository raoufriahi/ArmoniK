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
#include "Poco/Data/Session.h"
#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"


using namespace XSDK;

/*
Event Time:
MotionStart
MotionEnd
*/

class VE_LIBRARY_API VEventDB
{
public:
    VEventDB(string & strPath);
    ~VEventDB();
	
	BOOL Init();
	BOOL CreateTablesIfNecessary();
	BOOL DeInit();
	BOOL NewEvent(string strDev, string strDevName, string strType, time_t nEvtTime, string strDesc);
	
private:
	XMutex m_cMutex;
    string m_strIndexPath;
    Poco::Data::Session *m_DB;
};

