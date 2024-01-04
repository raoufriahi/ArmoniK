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
#ifndef _V_POCO_H_
#define _V_POCO_H_

#include "utility.hpp"
#include "fast_mutex.h"


VE_LIBRARY_API bool VHttpClientGet(string strHost, string strPath, string strUser, string strPasswd, string &pResponse);
VE_LIBRARY_API bool VHttpClientPost(string strHost, string strPath, string strUser, string strPasswd,  string strBody, string &pResponse);


#endif /* _VDC_UTILITY_H_ */
