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

#define NOMINMAX
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utility/tinythread.h"
#include "utility/fast_mutex.h"

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <map>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#define closesocket(x) close(x)

using namespace tthread;
using namespace std;

typedef std::string astring;

namespace UtilityLib
{

VE_LIBRARY_API unsigned UtilityGetBufferSize(int bufOptName,
                  int socket);

VE_LIBRARY_API unsigned UtilityGetSendBufferSize(int socket);

VE_LIBRARY_API unsigned UtilityGetReceiveBufferSize(int socket);
VE_LIBRARY_API unsigned UtilitySetBufferTo(int bufOptName, int socket, 
					unsigned requestedSize);

VE_LIBRARY_API unsigned UtilitySetSendBufferTo(int socket, unsigned requestedSize);

VE_LIBRARY_API unsigned UtilitySetReceiveBufferTo(int socket, unsigned requestedSize);

VE_LIBRARY_API BOOL makeSocketNonBlocking(int sock);

VE_LIBRARY_API std::string currentDateTime(time_t nTime);

VE_LIBRARY_API void DumpTxtData(unsigned char *pNuf, int nLen);

VE_LIBRARY_API void DumpHexData(unsigned char *pNuf, int nLen);

VE_LIBRARY_API int image_write_raw (unsigned char *pImg, int size, const char* fname);

VE_LIBRARY_API const std::string Time2String(time_t nTime);

VE_LIBRARY_API bool TryCheckDevice(std::string ip, std::string port);
VE_LIBRARY_API void ve_sleep(int ms);
VE_LIBRARY_API double GetTickCount(void);
}/* UtilityLib */

/* Get the value for a given parameter,  */
VE_LIBRARY_API int uriParserGetParaByName(const char * uri, const char * name,  
        char *out, const int out_len);

