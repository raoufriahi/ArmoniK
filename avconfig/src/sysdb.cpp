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

#include "sysdb.h"

/**
 * \brief Locks the mutex of the SysDB object.
 *
 * This function locks the mutex of the SysDB object, preventing concurrent access to its data.
 */
void SysDB::Lock()
{
    m_Lock.lock(); /**< Locks the mutex of the SysDB object. */
}

/**
 * \brief Unlocks the mutex of the SysDB object.
 *
 * This function unlocks the mutex of the SysDB object, allowing concurrent access to its data.
 */
void SysDB::UnLock()
{
    m_Lock.unlock(); /**< Unlocks the mutex of the SysDB object. */
}

/**
 * \brief Retrieves the system path from the database.
 *
 * This function retrieves the system path from the database and stores it in the provided string.
 * The system path is identified by the key "videoSystemPath" in the database.
 *
 * \param strPath Reference to the string where the retrieved system path will be stored.
 * \return True if the system path was successfully retrieved, false otherwise.
 */
inline bool SysDB::GetSystemPath(string &strPath)
{
    leveldb::Slice key("videoSystemPath"); /**< Represents the key used to retrieve the system path from the database. */
    leveldb::Slice sysValue; /**< Represents the value containing the retrieved system path. */

    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions()); /**< Represents the LevelDB iterator used to traverse the database. */
    if(it == nullptr) {
      VDC_DEBUG("System Config is not initialized\n");
      return false; /**< Indicates failure to initialize the database iterator. */
    }

    it->Seek(key);
    if (it->Valid()) {
        sysValue = it->value();
    }
    else {
        VDC_DEBUG("System Config is not initialized\n");
        delete it;
        return false; /**< Indicates failure to find the system path in the database. */
    }

    if (sysValue.size() == 0) {
        VDC_DEBUG("System Config is not initialized\n");
        delete it;
        return false; /**< Indicates failure to retrieve the system path from the database. */
    }

    strPath = sysValue.ToString(); /**< Converts the system path value to a string and stores it in the provided variable. */

    // Check for any errors found during the scan
    assert(it->status().ok());

    delete it;
    return true; /**< Indicates successful retrieval of the system path from the database. */
}

/**
 * \brief Sets the system path in the database.
 *
 * This function sets the system path in the database with the provided path string.
 * The system path is identified by the key "videoSystemPath" in the database.
 *
 * \param strPath Reference to the string containing the system path to be set.
 * \return True if the system path was successfully set, false otherwise.
 */
inline bool SysDB::SetSystemPath(string &strPath)
{
    leveldb::WriteOptions writeOptions; /**< Represents options for the database write operation. */
    leveldb::Slice sysKey("videoSystemPath"); /**< Represents the key used to set the system path in the database. */
    leveldb::Slice sysValue(strPath); /**< Represents the value containing the system path to be set. */

    // Write the system path to the database
    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true; /**< Indicates successful setting of the system path. */
}

/**
 * \brief Opens or creates a LevelDB database.
 *
 * This function opens or creates a LevelDB database with the specified path.
 *
 * \param pPath Reference to the string containing the path to the LevelDB database.
 * \return True if the database was successfully opened or created, false otherwise.
 */
inline bool SysDB::Open(string &pPath)
{
    m_Options.create_if_missing = true; /**< Specifies whether the database should be created if it doesn't exist. */
    leveldb::Status status = leveldb::DB::Open(m_Options, pPath, &m_pDb); /**< Attempts to open or create the LevelDB database. */
    // Check if the database was opened or created successfully
    if (!status.ok()){
        // If opening or creating the database fails, print error messages and return false
        VDC_DEBUG("Unable to open/create sys database %s\n", pPath.c_str());
        return false; /**< Indicates failure to open or create the database. */
    }

    return true; /**< Indicates successful opening or creation of the database. */
}

