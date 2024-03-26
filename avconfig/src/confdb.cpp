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

#include <iostream>
#include <thread>
#include <mutex>
#include "confdb.h"
using namespace std;

#define VSC_OAPI_DEFAULT_PORT 9080

/**
 * \brief Sets default values for the configuration of a HDFS record.
 *
 * This function sets default values for the configuration of a HDFS record, including the name node,
 * port, user, password, and file interval.
 *
 * \param pData Reference to the HDFS configuration object to be initialized with default values.
 */
void VSCHdfsRecordDefault(VidHDFSConf &pData)
{
    // Set default values for HDFS configuration parameters
    // Set the name node to "localhost"
    pData.set_strnamenode("localhost");
    // Set the port to "8020"
    pData.set_strport("8020");
    // Set the user to "admin"
    pData.set_struser("admin");
    // Set the password to "admin"
    pData.set_strpasswd("admin");
    // Set the file interval to 30 seconds
    pData.set_nfileinterval(30);
}


/**
 * \brief Sets default values for the configuration of a video storage server.
 *
 * This function sets default values for the configuration of a video storage server.
 * Currently, the function is commented out and does not perform any operations.
 *
 * \param pData Reference to the video storage server configuration object to be initialized with default values.
 */
void VSCStorServerConfDefault(VidStorServerConf &pData)
{
#if 0
    // This section of code is currently commented out
    // It was originally intended to set default values for the video storage server configuration
    
    // SimpleCrypt crypt; // Uncomment if SimpleCrypt is used
    
    // Set the default port for the NoAPI interface
    pData.set_noapiport(VSC_OAPI_DEFAULT_PORT);
    
    // Default password is "admin"
    // string strPass = "admin";
    // pData.set_stradminpasswd(crypt.encryptToString(strPass).toStdString());
    
#endif
}


/**
 * \brief Opens or creates a LevelDB database at the specified path.
 *
 * This function opens or creates a LevelDB database at the specified path. If the database does not exist, it will be created.
 *
 * \param pPath The path to the LevelDB database.
 * \return True if the database was successfully opened or created, false otherwise.
 */
bool ConfDB::Open(string pPath)
{
    m_Options.create_if_missing = true; /**< Sets the option to create the database if it does not exist. */

    // Attempt to open or create the LevelDB database
    leveldb::Status status = leveldb::DB::Open(m_Options, pPath, &m_pDb);

    // Check if the database was opened or created successfully
    if (!status.ok()) {
        // If opening or creating the database fails, log an error message and return false
        VDC_DEBUG("Unable to open/create test database %s\n", pPath.c_str());
        return false;
    }

    // Populate the database with a fake key-value pair
    string fakeKey = "fakeKey";
    string fakeValue = "fakeValue";
    SetCmnParam(fakeKey, fakeValue);
    return true; /**< Indicates successful opening or creation of the database. */
}



/**
 * \brief Searches for a camera with the specified ID in the configuration database.
 *
 * This function searches for a camera entry with the specified ID in the configuration database.
 *
 * \param strCameraId The ID of the camera to search for.
 * \return True if a camera with the specified ID is found, false otherwise.
 */
bool ConfDB::FindCamera(string strCameraId)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */
    
    VidCameraList cameraList; /**< Represents the list of cameras retrieved from the configuration. */
    GetCameraListConf(cameraList); /**< Retrieves the camera list from the configuration. */
    
    // Iterate through the camera list to find the specified camera ID
    for (int i = 0; i < cameraList.cvidcamera_size(); i++) {
        const VidCamera &cam = cameraList.cvidcamera(i); /**< Represents the camera object at index i. */
        
        // If a camera with the specified ID is found, return true
        if (cam.strid() == strCameraId) {
            return true; /**< Indicates successful finding of the camera with the specified ID. */
        }
    }
    return false; /**< Indicates failure to find a camera with the specified ID. */
}

/**
 * \brief Deletes a camera entry from the configuration database.
 *
 * This function deletes a camera entry identified by the provided camera ID from the configuration database.
 *
 * \param strCameraId The ID of the camera to be deleted.
 * \return True if the camera was successfully deleted, false otherwise.
 */
bool ConfDB::DeleteCamera(string strCameraId)
{
    std::lock_guard<std::mutex> lock(m_cMutex);  /**< Locks the mutex to ensure thread safety. */
    VidCameraList cameraList;                    /**< Represents the list of cameras retrieved from the configuration. */
    VidCameraList cameraListNew;                 /**< Represents the new camera list after deletion. */
    GetCameraListConf(cameraList);               /**< Retrieves the camera list from the configuration. */

    // Iterate through the camera list and copy cameras except the one to be deleted
    for (int i = 0; i < cameraList.cvidcamera_size(); i ++) {
        const VidCamera &cam = cameraList.cvidcamera(i);  /**< Represents the camera object at index i. */
        
        // If the camera ID does not match the ID to be deleted, add it to the new list
        if (cam.strid() != strCameraId) {
            VidCamera *pCam = cameraListNew.add_cvidcamera();  /**< Represents a pointer to the new camera object. */
            *pCam = cam;                                       /**< Copies the camera object to the new list. */
        }
    }

    UpdateCameraListConf(cameraListNew);  /**< Updates the configuration with the new camera list. */
    return true;                          /**< Indicates successful deletion of the camera. */
}

/**
 * \brief Adds a new camera entry to the configuration database.
 *
 * This function adds a new camera entry to the configuration database.
 *
 * \param pAddCam Reference to the camera object to be added.
 * \return True if the camera was successfully added, false otherwise.
 */
bool ConfDB::AddCamera(VidCamera &pAddCam)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */
    VidCameraList cameraList; /**< Represents the list of cameras retrieved from the configuration. */
    GetCameraListConf(cameraList);  /**< Retrieves the camera list from the configuration. */
   
    VidCamera *pCam = cameraList.add_cvidcamera(); /**< Represents a pointer to the new camera object. */
    *pCam = pAddCam; /**< Copies the provided camera object to the new camera object. */
    
    UpdateCameraListConf(cameraList); /**< Updates the configuration with the new camera list. */
    return true;  /**< Indicates successful addition of the camera. */
}

/**
 * \brief Retrieves the configuration of a specific camera from the database.
 *
 * This function retrieves the configuration of a specific camera identified by the provided camera ID from the database.
 *
 * \param strCameraId The ID of the camera whose configuration is to be retrieved.
 * \param pCam Reference to the camera object where the retrieved configuration will be stored.
 * \return True if the camera configuration was successfully retrieved, false otherwise.
 */
bool ConfDB::GetCameraConf(string strCameraId, VidCamera &pCam)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */
    
    VidCameraList cameraList; /**< Represents the list of cameras retrieved from the configuration. */
    GetCameraListConf(cameraList); /**< Retrieves the camera list from the configuration. */
    
    // Iterate through the camera list to find the specified camera
    for (int i = 0; i < cameraList.cvidcamera_size(); i++) {
        const VidCamera &cam = cameraList.cvidcamera(i); /**< Represents the camera object at index i. */
        
        // If the camera ID matches the specified ID, retrieve its configuration and return true
        if (cam.strid() == strCameraId) {
            pCam = cam; /**< Copies the configuration of the found camera to the provided camera object. */
            return true; /**< Indicates successful retrieval of the camera configuration. */
        }
    }
    return false; /**< Indicates failure to retrieve the camera configuration (camera not found). */
}

/**
 * \brief Sets the recording template for a specific camera in the database.
 *
 * This function sets the recording template for a specific camera in the database.
 *
 * \param strCameraId The ID of the camera for which the recording template will be set.
 * \param strTempl The recording template to set for the specified camera.
 * \return True if the recording template was successfully set, false otherwise.
 */
bool ConfDB::CameraRecordTemplSet(string strCameraId, string strTempl)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    // Implement the logic to set the recording template for the specified camera in the database
    // (This function currently returns true without performing any operation)

    return true; /**< Indicates successful setting of the recording template. */
}


/**
 * \brief Retrieves the license key from the configuration database.
 *
 * This function retrieves the license key stored in the configuration database.
 *
 * \param strLicense Reference to a string where the retrieved license key will be stored.
 * \return True if the license key was successfully retrieved, false otherwise.
 */
bool ConfDB::GetLicense(string &strLicense)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */
    VSCConfLicenseKey sLicKey; /**< Represents the license key object used as the database key. */
    // Convert the license key object to a LevelDB slice to use as the database key
    leveldb::Slice key((char *)&sLicKey, sizeof(sLicKey));
    // Attempt to retrieve the license key from the database
    leveldb::Status status = m_pDb->Get(leveldb::ReadOptions(), key, &strLicense);

    // Check if the license key was retrieved successfully
    if (!status.ok()){
        // If retrieval fails, set the license key to an empty string and return false
        strLicense = "";
        return false;
    }
    return true; /**< Indicates successful retrieval of the license key. */
}

/**
 * \brief Sets the license key in the database.
 *
 * This function sets the license key in the database with the provided data.
 *
 * \param strLicense Reference to the string containing the license key to be set.
 * \return True if the license key was successfully set, false otherwise.
 */
bool ConfDB::SetLicense(string &strLicense)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    VSCConfLicenseKey sLic; /**< Represents the key used to store the license key in the database. */
    leveldb::WriteOptions writeOptions; /**< Represents options for the database write operation. */

    leveldb::Slice licKey((char *)&sLic, sizeof(sLic)); /**< Represents the LevelDB key corresponding to the license key. */
    leveldb::Slice licValue(strLicense); /**< Represents the LevelDB value containing the license key. */

    // Write the license key to the database
    m_pDb->Put(writeOptions, licKey, licValue);

    return true; /**< Indicates successful setting of the license key. */
}

/**
 * \brief Retrieves a common parameter from the database.
 *
 * This function retrieves a common parameter with the specified key from the database.
 *
 * \param strKey Reference to the string containing the key of the common parameter to retrieve.
 * \param strParam Reference to the string where the retrieved common parameter value will be stored.
 * \return True if the common parameter was successfully retrieved, false otherwise.
 */
bool ConfDB::GetCmnParam(string &strKey, string &strParam)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */
    leveldb::Slice key(strKey); /**< Represents the LevelDB key corresponding to the common parameter. */
    
    // Attempt to retrieve the common parameter value from the database
    leveldb::Status status = m_pDb->Get(leveldb::ReadOptions(), key, &strParam);
    // Check if the retrieval was successful
    if (!status.ok()){
        // If retrieval fails, set the parameter value to an empty string and return false
        strParam = "";
        return false;
    }
    return true; /**< Indicates successful retrieval of the common parameter. */
}

/**
 * \brief Sets a common parameter in the database.
 *
 * This function sets a common parameter with the specified key and value in the database.
 *
 * \param strKey Reference to the string containing the key of the common parameter to set.
 * \param strParam Reference to the string containing the value of the common parameter to set.
 * \return True if the common parameter was successfully set, false otherwise.
 */
bool ConfDB::SetCmnParam(string &strKey, string &strParam)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    leveldb::WriteOptions writeOptions; /**< Represents options for the database write operation. */

    leveldb::Slice licKey(strKey); /**< Represents the LevelDB key corresponding to the common parameter. */
    leveldb::Slice licValue(strParam); /**< Represents the LevelDB value containing the common parameter. */

    // Write the common parameter to the database
    m_pDb->Put(writeOptions, licKey, licValue);

    return true; /**< Indicates successful setting of the common parameter. */
}

/**
 * \brief Retrieves the configuration of the HDFS record from the database.
 *
 * This function retrieves the configuration of the HDFS record from the database and stores it in the provided VidHDFSConf object.
 * If the configuration does not exist or cannot be parsed, default values are used, and the configuration is updated in the database.
 *
 * \param pData Reference to the VidHDFSConf object where the retrieved configuration will be stored.
 * \return True if the configuration was successfully retrieved or initialized, false otherwise.
 */
bool ConfDB::GetHdfsRecordConf(VidHDFSConf &pData)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    VSCConfHdfsRecordKey sKey; /**< Represents the key used to retrieve the HDFS record configuration from the database. */
    string strValue; /**< Represents the string value containing the retrieved configuration. */

    leveldb::Slice key((char *)&sKey, sizeof(sKey)); /**< Represents the LevelDB key corresponding to the HDFS record configuration. */

    // Attempt to retrieve the HDFS record configuration from the database
    leveldb::Status status = m_pDb->Get(leveldb::ReadOptions(), key, &strValue);

    // Check if the retrieval was successful
    if (!status.ok()) {
        // If retrieval fails, set a default string value and proceed
        strValue = "fake";
    }

    // Parse the retrieved configuration string into the VidHDFSConf object
    if (pData.ParseFromString(strValue) == false) {
        // If parsing fails, log a debug message and set default values for the HDFS record configuration
        VDC_DEBUG("Hdfs Record Config is not initialized\n");
        VSCHdfsRecordDefault(pData);
        
        // Update the HDFS record configuration in the database with the default values
        UpdateHdfsRecordConf(pData);
        return true; /**< Indicates successful retrieval or initialization of the configuration. */
    }
    return true; /**< Indicates successful retrieval of the configuration. */
}


/**
 * \brief Updates the configuration of the HDFS record in the database.
 *
 * This function updates the configuration of the HDFS record in the database with the provided data.
 *
 * \param pData Reference to the VidHDFSConf object containing the updated HDFS record configuration.
 * \return True if the update was successful, false otherwise.
 */
bool ConfDB::UpdateHdfsRecordConf(VidHDFSConf &pData)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    VSCConfHdfsRecordKey sKey; /**< Represents the key used to store the HDFS record configuration in the database. */
    string strOutput; /**< Represents the serialized string containing the updated HDFS record configuration. */

    leveldb::WriteOptions writeOptions; /**< Represents options for the database write operation. */
    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey)); /**< Represents the LevelDB key corresponding to the HDFS record configuration. */
    
    // Serialize the HDFS record configuration object to a string
    if (pData.SerializeToString(&strOutput) != true) {
        // If serialization fails, return false
        return false;
    }

    leveldb::Slice sysValue(strOutput); /**< Represents the LevelDB value corresponding to the serialized configuration. */

    // Write the HDFS record configuration to the database
    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true; /**< Indicates successful update of the HDFS record configuration. */
}


/**
 * \brief Retrieves the configuration of the camera list from the database.
 *
 * This function retrieves the configuration of the camera list from the database and stores it in the provided VidCameraList object.
 * If the configuration does not exist or cannot be parsed, default values are used.
 *
 * \param pData Reference to the VidCameraList object where the retrieved configuration will be stored.
 * \return True if the configuration was successfully retrieved, false otherwise.
 */
bool ConfDB::GetCameraListConf(VidCameraList &pData)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    VSCConfCameraKey sKey; /**< Represents the key used to retrieve the camera list configuration from the database. */
    string strValue; /**< Represents the string value containing the retrieved configuration. */

    leveldb::Slice key((char *)&sKey, sizeof(sKey)); /**< Represents the LevelDB key corresponding to the camera list configuration. */

    // Attempt to retrieve the camera list configuration from the database
    leveldb::Status status = m_pDb->Get(leveldb::ReadOptions(), key, &strValue);

    // Check if the retrieval was successful
    if (!status.ok()) {
        // If retrieval fails, set a default string value and proceed
        strValue = "fake";
    }

    // Parse the retrieved configuration string into the VidCameraList object
    if (pData.ParseFromString(strValue) == false) {
        // If parsing fails, set default values for the camera list configuration
        VidCameraList listDefault;
        pData = listDefault;
        VDC_DEBUG("Camera List Config is not initialized\n");
        return false; /**< Indicates failure to retrieve or parse the configuration. */
    }
    return true; /**< Indicates successful retrieval and parsing of the configuration. */
}


/**
 * \brief Updates the configuration of the camera list in the database.
 *
 * This function updates the configuration of the camera list in the database with the provided data.
 *
 * \param pData Reference to the VidCameraList object containing the updated camera list configuration.
 * \return True if the update was successful, false otherwise.
 */
bool ConfDB::UpdateCameraListConf(VidCameraList &pData)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    VSCConfCameraKey sKey; /**< Represents the key used to store the camera list configuration in the database. */
    leveldb::WriteOptions writeOptions; /**< Represents options for the database write operation. */
    string strOutput; /**< Represents the serialized string containing the updated camera list configuration. */

    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey)); /**< Represents the LevelDB key corresponding to the camera list configuration. */

    // Serialize the camera list configuration object to a string
    if (pData.SerializeToString(&strOutput) != true) {
        // If serialization fails, return false
        return false;
    }

    leveldb::Slice sysValue(strOutput); /**< Represents the LevelDB value corresponding to the serialized configuration. */

    // Write the camera list configuration to the database
    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true; /**< Indicates successful update of the camera list configuration. */
}


/**
 * \brief Retrieves the configuration of a storage server from the database.
 *
 * This function retrieves the configuration of a storage server from the database and stores it in the provided VidStorServerConf object.
 * If the configuration does not exist or cannot be parsed, default values are used.
 *
 * \param pData Reference to the VidStorServerConf object where the retrieved configuration will be stored.
 * \return True if the configuration was successfully retrieved, false otherwise.
 */
bool ConfDB::GetStorServerConf(VidStorServerConf &pData)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    VSCConfStorServerKey sKey; /**< Represents the key used to retrieve the storage server configuration from the database. */
    string strValue; /**< Represents the string value containing the retrieved configuration. */

    leveldb::Slice key((char *)&sKey, sizeof(sKey)); /**< Represents the LevelDB key corresponding to the storage server configuration. */

    // Attempt to retrieve the storage server configuration from the database
    leveldb::Status status = m_pDb->Get(leveldb::ReadOptions(), key, &strValue);

    // Check if the retrieval was successful
    if (!status.ok()) {
        // If retrieval fails, set a default string value and proceed
        strValue = "fake";
    }

    // Parse the retrieved configuration string into the VidStorServerConf object
    if (pData.ParseFromString(strValue) == false) {
        // If parsing fails, set default values for the storage server configuration
        VSCStorServerConfDefault(pData);
        VDC_DEBUG("Stor Server Config is not initialized\n");
        return false; /**< Indicates failure to retrieve or parse the configuration. */
    }
    return true; /**< Indicates successful retrieval and parsing of the configuration. */
}

/**
 * \brief Sets the configuration of a storage server in the database.
 *
 * This function sets the configuration of a storage server in the database.
 *
 * \param pData Reference to the VidStorServerConf object containing the configuration data to be set.
 * \return True if the configuration was successfully set, false otherwise.
 */
bool ConfDB::SetStorServerConf(VidStorServerConf &pData)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    VSCConfStorServerKey sKey; /**< Represents the key used to store the storage server configuration in the database. */
    string strOutput; /**< Represents the serialized string containing the storage server configuration. */

    leveldb::WriteOptions writeOptions; /**< Represents options for the database write operation. */
    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey)); /**< Represents the LevelDB key corresponding to the storage server configuration. */

    // Serialize the storage server configuration object to a string
    if (pData.SerializeToString(&strOutput) != true) {
        // If serialization fails, return false
        return false;
    }
    leveldb::Slice sysValue(strOutput); /**< Represents the LevelDB value corresponding to the serialized configuration. */
    // Write the storage server configuration to the database
    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true; /**< Indicates successful setting of the storage server configuration. */
}


/**
 * \brief Retrieves a recording schedule for a specific ID from the configuration database.
 *
 * This function retrieves a recording schedule for a specific ID from the configuration database.
 * The recording schedule is represented by a RecordSchedWeek object.
 *
 * \param strId The ID of the recording schedule to retrieve.
 * \param pSched Reference to a RecordSchedWeek object where the retrieved recording schedule will be stored.
 * \return True if the recording schedule was successfully retrieved, false otherwise.
 */
bool ConfDB::GetRecSched(string strId, RecordSchedWeek &pSched)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    // Check if the ID corresponds to a predefined recording schedule
    if (strId == REC_SCHED_OFF){
        // Set the recording schedule to Off
        pSched = RecordSchedWeek::CreateOff();
        return true; /**< Indicates successful retrieval of the recording schedule. */
    }
    if (strId == REC_SCHED_ALL_DAY){
        // Set the recording schedule to All Day
        pSched = RecordSchedWeek::CreateAllDay();
        return true; /**< Indicates successful retrieval of the recording schedule. */
    }
    if (strId == REC_SCHED_WORK_DAY){
        // Set the recording schedule to Work Day
        pSched = RecordSchedWeek::CreateWorkDay();
        return true; /**< Indicates successful retrieval of the recording schedule. */
    }
    return false; /**< Indicates failure to retrieve the recording schedule (ID not recognized). */
}


#if 0
bool ConfDB::GetSystemConf(VSCConfData &pSys)
{
    VSCConfSystemKey sSysKey;

    leveldb::Slice key((char *)&sSysKey, sizeof(sSysKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCConfData))
    {
        VDC_DEBUG( "System Config is not init\n");
        delete it;
        memset(&pSys, 0, sizeof(VSCConfData));
        SysConfDataDefault(pSys);
        UpdateSysData(pSys);
        string strLicense = " ";
        SetLicense(strLicense);//set the default license
        /* Call get system again */
        return true;
    }

    memcpy(&pSys, sysValue.data(), sizeof(VSCConfData));
	if (pSys.data.conf.RTSPServerPort == 0)
	{
		pSys.data.conf.RTSPServerPort = VSC_RTSP_DEFAULT_PORT;
	}
	if (pSys.data.conf.OAPIPort == 0)
	{
		pSys.data.conf.OAPIPort = VSC_OAPI_DEFAULT_PORT;
	}
	if (pSys.data.conf.VHTTPServerPort == 0)
	{
		pSys.data.conf.VHTTPServerPort = VSC_VHTTPS_DEFAULT_PORT;
	}
	if (pSys.data.conf.VHLSServerPort == 0)
	{
		pSys.data.conf.VHLSServerPort = VSC_VHLSS_DEFAULT_PORT;
	}
	if (pSys.data.conf.VHTTPSSLServerPort == 0)
	{
		pSys.data.conf.VHTTPSSLServerPort = VSC_VHTTPS_SSL_DEFAULT_PORT;
	}

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;

}

bool ConfDB::GetVmsConf(VSCVmsData &pVms)
{
    VSCConfVmsKey sVmsKey;

    leveldb::Slice key((char *)&sVmsKey, sizeof(sVmsKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCVmsData))
    {
        VDC_DEBUG( "VMS Config is not init\n");
        delete it;
        memset(&pVms, 0, sizeof(VSCVmsData));
        VmsConfDataDefault(pVms);
        UpdateVmsData(pVms);
        /* Call get system again */
        return true;
    }

    memcpy(&pVms, sysValue.data(), sizeof(VSCVmsData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;

}

bool ConfDB::GetViewConf(VSCViewData &pView)
{
    VSCConfViewKey sViewKey;

    leveldb::Slice key((char *)&sViewKey, sizeof(sViewKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCViewData))
    {
        VDC_DEBUG( "View Config is not init\n");
        delete it;
        memset(&pView, 0, sizeof(VSCViewData));
        ViewConfDataDefault(pView);
        UpdateViewData(pView);
        /* Call get system again */
        return true;
    }

    memcpy(&pView, sysValue.data(), sizeof(VSCViewData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;

}


bool ConfDB::GetVGroupConf(VSCVGroupData &pVGroup)
{
    VSCConfVGroupKey sVGroupKey;

    leveldb::Slice key((char *)&sVGroupKey, sizeof(sVGroupKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCVGroupData))
    {
        VDC_DEBUG( "Group Config is not init\n");
        delete it;
		memset(&pVGroup, 0, sizeof(VSCVGroupData));
		VGroupConfDataDefault(pVGroup);
		UpdateVGroupData(pVGroup);
        /* Call get system again */
        return true;
    }

	memcpy(&pVGroup, sysValue.data(), sizeof(VSCVGroupData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;

}

bool ConfDB::GetUserConf(VSCUserData &pData)
{
    VSCConfUserKey sKey;

    leveldb::Slice key((char *)&sKey, sizeof(sKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCUserData))
    {
        VDC_DEBUG( "User Config is not init\n");
        delete it;
	memset(&pData, 0, sizeof(VSCUserData));
	VSCUserDataItemDefault(pData.data.conf);
	UpdateUserData(pData);
        /* Call get system again */
        return true;
    }

	memcpy(&pData, sysValue.data(), sizeof(VSCUserData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;

}

bool ConfDB::GetTourConf(VSCTourData &pData)
{
    VSCConfTourKey sKey;

    leveldb::Slice key((char *)&sKey, sizeof(sKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCTourData))
    {
        VDC_DEBUG( "Tour Config is not init\n");
        delete it;
	memset(&pData, 0, sizeof(VSCTourData));
	VSCTourDataDefault(pData.data.conf);
	UpdateTourData(pData);
        /* Call get system again */
        return true;
    }

	memcpy(&pData, sysValue.data(), sizeof(VSCTourData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;

}


bool ConfDB::GetEmapConf(VSCEmapData &pData)
{
    VSCConfEmapKey sKey;

    leveldb::Slice key((char *)&sKey, sizeof(sKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCEmapData))
    {
        VDC_DEBUG( "User Config is not init\n");
        delete it;
	memset(&pData, 0, sizeof(VSCEmapData));
	VSCEmapDataDefault(pData.data.conf);
	UpdateEmapData(pData);
        /* Call get system again */
        return true;
    }

	memcpy(&pData, sysValue.data(), sizeof(VSCEmapData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;

}


s32 ConfDB::UpdateSysData(VSCConfData &pSysData)
{
    VSCConfSystemKey sSysKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sSysKey, sizeof(sSysKey));
    leveldb::Slice sysValue((char *)&pSysData, sizeof(VSCConfData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true;
}

s32 ConfDB::GetSysData(VSCConfData &pSysData)
{
    GetSystemConf(pSysData);

    return true;
}

bool ConfDB::UpdateDeviceData(u32 nId, VSCDeviceData &pData)
{
    VSCConfDeviceKey sChKey(nId);
    leveldb::WriteOptions writeOptions;

    leveldb::Slice chKey((char *)&sChKey, sizeof(sChKey));
    leveldb::Slice chValue((char *)&pData, sizeof(VSCDeviceData));
    m_pDb->Put(writeOptions, chKey, chValue);

    return true;
}

bool ConfDB::GetDeviceData(u32 nId, VSCDeviceData &pData)
{
    VSCConfDeviceKey sChKey(nId);

    leveldb::Slice key((char *)&sChKey, sizeof(sChKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCDeviceData))
    {
        VDC_DEBUG( "Device Can not find !!!\n");

        delete it;
        return false;
    }

    memcpy(&pData, sysValue.data(), sizeof(VSCDeviceData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;
}

bool ConfDB::UpdateVIPCData(u32 nId, VSCVIPCData &pData)
{
    VSCConfVIPCKey sChKey(nId);
    leveldb::WriteOptions writeOptions;

    leveldb::Slice chKey((char *)&sChKey, sizeof(sChKey));
    leveldb::Slice chValue((char *)&pData, sizeof(VSCDeviceData));
    m_pDb->Put(writeOptions, chKey, chValue);

    return true;
}

bool ConfDB::GetVIPCData(u32 nId, VSCVIPCData &pData)
{
    VSCConfVIPCKey sChKey(nId);

    leveldb::Slice key((char *)&sChKey, sizeof(sChKey));


    leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

    it->Seek(key);
    leveldb::Slice sysValue;

    if (it->Valid())
    {
        sysValue = it->value();
    }

    if (sysValue.size() != sizeof(VSCVIPCData))
    {
        VDC_DEBUG( "Device Can not find !!!\n");

        delete it;
        return false;
    }

    memcpy(&pData, sysValue.data(), sizeof(VSCVIPCData));

    // Check for any errors found during the scan
    assert(it->status().ok());
    delete it;

    return true;
}



s32 ConfDB::GetVmsData(VSCVmsData &pVmsData)
{
	GetVmsConf(pVmsData);
	
	return true;
}
s32 ConfDB::UpdateVmsData(VSCVmsData &pVmsData)
{
    VSCConfVmsKey sVmsKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sVmsKey, sizeof(sVmsKey));
    leveldb::Slice sysValue((char *)&pVmsData, sizeof(VSCVmsData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true;
}

s32 ConfDB::GetViewData(VSCViewData &pViewData)
{
	GetViewConf(pViewData);
	
	return true;
}
s32 ConfDB::UpdateViewData(VSCViewData &pViewData)
{
    VSCConfViewKey sViewKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sViewKey, sizeof(sViewKey));
    leveldb::Slice sysValue((char *)&pViewData, sizeof(VSCViewData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true;
}

/* Camera Group  */
s32 ConfDB::GetVGroupData(VSCVGroupData &pGroupData)
{
	GetVGroupConf(pGroupData);
	
	return true;
}
/* Camera Group  */
s32 ConfDB::UpdateVGroupData(VSCVGroupData &pVGroupData)
{
    VSCConfVGroupKey sVGroupKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sVGroupKey, sizeof(sVGroupKey));
    leveldb::Slice sysValue((char *)&pVGroupData, sizeof(VSCVGroupData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true;
}

/* HDFS record  */
s32 ConfDB::GetHdfsRecordData(VSCHdfsRecordData &pData)
{
	GetHdfsRecordConf(pData);
	
	return true;
}

/* User  */
s32 ConfDB::GetUserData(VSCUserData &pData)
{
	GetUserConf(pData);
	
	return true;
}

s32 ConfDB::UpdateUserData(VSCUserData &pData)
{
    VSCConfUserKey sKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));
    leveldb::Slice sysValue((char *)&pData, sizeof(VSCUserData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true;
}

/* Tour  */
s32 ConfDB::GetTourData(VSCTourData &pData)
{
	GetTourConf(pData);
	
	return true;
}

s32 ConfDB::UpdateTourData(VSCTourData &pData)
{
    VSCConfTourKey sKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));
    leveldb::Slice sysValue((char *)&pData, sizeof(VSCTourData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true;
}


/* Emap */
s32 ConfDB::GetEmapData(VSCEmapData &pData)
{
	GetEmapConf(pData);
	
	return true;
}
s32 ConfDB::UpdateEmapData(VSCEmapData &pData)
{
    VSCConfEmapKey sKey;

    leveldb::WriteOptions writeOptions;

    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));
    leveldb::Slice sysValue((char *)&pData, sizeof(VSCEmapData));

    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true;
}

/* Emap file Get & Set */
  bool ConfDB::GetEmapFile(string &strFile)
{
	bool ret = false;
	VSCConfEmapFileKey sMapKey;
	

	leveldb::Slice key((char *)&sMapKey, sizeof(sMapKey));


	leveldb::Iterator* it = m_pDb->NewIterator(leveldb::ReadOptions());

	it->Seek(key);
	leveldb::Slice sysValue;

	if (it->Valid())
	{
		sysValue = it->value();
		strFile = sysValue.ToString();
		ret = true;
	}
	
	// Check for any errors found during the scan
	assert(it->status().ok());
	delete it;

	return ret;

}

  bool ConfDB::SetEmapFile(string &strFile)
{
	VSCConfEmapFileKey sMapKey;
	leveldb::WriteOptions writeOptions;

	leveldb::Slice Key((char *)&sMapKey, sizeof(sMapKey));
	leveldb::Slice Value(strFile);
	m_pDb->Put(writeOptions, Key, Value);
	return true;
    
}

s32 ConfDB::AddDevice(VSCDeviceData &pData, u32 nId)
{
    if (nId > CONF_MAP_MAX)
    {
        return false;
    }
    VSCConfData SysData;
    GetSysData(SysData);

    SysData.data.conf.DeviceMap[nId] = nId;
    SysData.data.conf.DeviceNum ++;
    pData.data.conf.nId = nId;

    UpdateSysData(SysData);

    UpdateDeviceData(nId, pData);


    return true;
}

s32 ConfDB::DelDevice(u32 nId)
{
    if (nId > CONF_MAP_MAX)
    {
        return false;
    }

    VSCConfData SysData;
    GetSysData(SysData);

    SysData.data.conf.DeviceMap[nId] = CONF_MAP_INVALID_MIN;

    UpdateSysData(SysData);

    return true;
}

s32 ConfDB::AddVIPC(VSCVIPCData &pData, u32 nId)
{
    if (nId > CONF_MAP_MAX)
    {
        return false;
    }
    VSCConfData SysData;
    GetSysData(SysData);

    SysData.data.conf.VIPCMap[nId] = nId;
    SysData.data.conf.VIPCNum ++;
    pData.data.conf.nId = nId;

    UpdateSysData(SysData);

    UpdateVIPCData(nId, pData);


    return true;
}

s32 ConfDB::DelVIPC(u32 nId)
{
    if (nId > CONF_MAP_MAX)
    {
        return false;
    }

    VSCConfData SysData;
    GetSysData(SysData);

    SysData.data.conf.VIPCMap[nId] = CONF_MAP_INVALID_MIN;

    UpdateSysData(SysData);

    return true;
}

#endif
