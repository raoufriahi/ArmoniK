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

#include "utility/utility.hpp"
#include "leveldb/db.h"
#include "config/conf.hpp"
#include "utility/debug.hpp"

using namespace UtilityLib;

class VE_LIBRARY_API SysDB
{
public:
    SysDB(){ 

    }
    ~SysDB(){
    }

    bool Open(string &pPath);
    bool GetSystemPath(string &strPath);
    bool SetSystemPath(string &strPath);

    void Lock();
    void UnLock();

private:

    std::mutex m_Lock;
    leveldb::DB* m_pDb;
    leveldb::Options m_Options;
};

typedef SysDB* LPSysDB;