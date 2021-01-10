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
#ifndef _V_POCO_H_
#define _V_POCO_H_

#include "utility.hpp"
#include "fast_mutex.h"

/* 
#include "Poco/Notification.h"
#include "Poco/NotificationQueue.h"
#include "Poco/ThreadPool.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/Random.h"
#include "Poco/AutoPtr.h"
#include "Poco/Net/HTTPCredentials.h" 


using Poco::NotificationCenter;
using Poco::NotificationQueue;
using Poco::Notification;
using Poco::AutoPtr;
*/



VE_LIBRARY_API bool VHttpClientGet(astring strHost, astring strPath, astring strUser, astring strPasswd, 
							astring &pResponse);
VE_LIBRARY_API bool VHttpClientPost(astring strHost, astring strPath, astring strUser, astring strPasswd, , astring strBody
							astring &pResponse);


#endif /* _VDC_UTILITY_H_ */
