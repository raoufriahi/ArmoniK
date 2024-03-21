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
#include "leveldb/db.h"
#include "config/conf.hpp"
#include "debug.hpp"

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


void SysDB::Lock()
{
    m_Lock.lock();
}
void SysDB::UnLock()
{
    m_Lock.unlock();
}

inline bool SysDB::GetSystemPath(string &strPath)
{
    leveldb::Slice key("videoSystemPath");
    leveldb::Slice sysValue;

    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());
    if(it == nullptr) {
      VDC_DEBUG( "System Config is not init\n");
      return false;
    }

    it->Seek(key);
    if (it->Valid()) {
        sysValue = it->value();
    }
    else {
        VDC_DEBUG( "System Config is not init\n");
        return false;
    }

    if (sysValue.size() == 0) {
        VDC_DEBUG( "System Config is not init\n");
        delete it;
        return false;
    }

    strPath = sysValue.ToString();
    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;
    return true;
}

inline bool SysDB::SetSystemPath(string &strPath)
{
    leveldb::WriteOptions writeOptions;
    leveldb::Slice sysKey("videoSystemPath");
    leveldb::Slice sysValue(strPath);

    m_pDb->Put(writeOptions, sysKey, sysValue);
    return true;
}


inline bool SysDB::Open(string & pPath)
{
    m_Options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(m_Options, pPath, &m_pDb);
    if (false == status.ok()) {
        cerr << "Unable to open/create test database "<< pPath << endl;
        cerr << status.ToString() << endl;
        VDC_DEBUG( "Unable to open/create sys database %s\n", pPath.c_str());
        return false;
    }
    return true;
}

typedef SysDB* LPSysDB;