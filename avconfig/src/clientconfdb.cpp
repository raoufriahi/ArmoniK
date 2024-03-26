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

#include "clientconfdb.h"

using namespace std;

#if 0
bool SysConfDataDefault(VSCConfData &pConf)
{
	pConf.data.conf.DeviceNum = 0;
	for (int i = 0; i < CONF_MAP_MAX; i ++)
	{
		pConf.data.conf.DeviceMap[i] = CONF_MAP_INVALID_MIN;
	}
	for (int i = 0; i < CONF_MAP_MAX; i ++)
	{
		pConf.data.conf.VIPCMap[i] = CONF_MAP_INVALID_MIN;
	}

	pConf.data.conf.RTSPServerPort = VSC_RTSP_DEFAULT_PORT;
	strcpy(pConf.data.conf.MulticastStartIPV4, "226.0.0.1");
	pConf.data.conf.OAPIPort = VSC_OAPI_DEFAULT_PORT;
	pConf.data.conf.VHTTPServerPort = VSC_VHTTPS_DEFAULT_PORT;
	pConf.data.conf.VHLSServerPort = VSC_VHLSS_DEFAULT_PORT;
	pConf.data.conf.VHTTPSSLServerPort = VSC_VHTTPS_SSL_DEFAULT_PORT;

    return true;
}

bool VmsConfDataDefault(VSCVmsData &pConf)
{
	memset(&pConf, 0, sizeof(VSCVmsData));

	for (int i = 0; i < CONF_VMS_NUM_MAX; i ++)
	{
	    	pConf.data.conf.vms[i].nId = i;
		pConf.data.conf.vms[i].Used = 0;
	}

    return true;
}

bool ViewConfDataDefault(VSCViewData &pConf)
{
	memset(&pConf, 0, sizeof(VSCViewData));

	for (int i = 0; i < CONF_VIEW_NUM_MAX; i ++)
	{
	    	pConf.data.conf.view[i].nId = i;
		pConf.data.conf.view[i].Used = 0;
	}

    return true;
}

bool VGroupConfDataDefault(VSCVGroupData &pConf)
{
	memset(&pConf, 0, sizeof(VSCVGroupData));

	for (int i = 0; i < CONF_VGROUP_NUM_MAX; i ++)
	{
	    	pConf.data.conf.group[i].nId = i;
			pConf.data.conf.group[i].Used = 0;
	}

    return true;
}
#endif

/**
 * \brief Set default values for a VidClientConf object.
 *
 * This function sets default values for a VidClientConf object.
 *
 * \param pData Reference to the VidClientConf object where the default values will be set.
 */
void VSCClientConfDefault(VidClientConf &pData)
{
    // Uncomment the following block to enable default value initialization

    /*
    SimpleCrypt crypt;
    pData.set_nlang(VID_LANG_AUTO);
    pData.set_bautologin(false);
    // Default password is "admin"
    QString strPass = "admin";
    pData.set_stradminpasswd(crypt.encryptToString(strPass).toStdString());
    */
}


/**
 * \brief Opens or creates a LevelDB database for storing client configuration data.
 *
 * This function opens or creates a LevelDB database at the specified path for storing client configuration data.
 * If the database cannot be opened or created, it returns false, otherwise, it initializes the database with default values.
 *
 * \param pPath The path to the LevelDB database.
 * \return True if the database was successfully opened or created, false otherwise.
 */
bool ClientConfDB::Open(string & pPath)
{
    m_Options.create_if_missing = true; /**< Indicates that the database should be created if it does not exist. */
    
    // Attempt to open or create the LevelDB database
    leveldb::Status status = leveldb::DB::Open(m_Options, pPath, &m_pDb);
    
    // Check if the database was opened or created successfully
    if (!status.ok()){
        // If opening or creating the database fails, log an error message and return false
        VDC_DEBUG("Unable to open/create test database %s\n", pPath.c_str());
        return false;
    }
    // Set default common parameters in the database
    string fakeKey = "fakeKey";
    string fakeValue = "fakeValue";
    SetCmnParam(fakeKey, fakeValue);
    return true; /**< Indicates successful opening or creation of the database. */
}

/**
 * \brief Retrieves a common parameter from the client configuration database.
 *
 * This function retrieves a common parameter with the specified key from the client configuration database.
 *
 * \param strKey The key of the common parameter to retrieve.
 * \param strParam Reference to the string where the retrieved common parameter value will be stored.
 * \return True if the common parameter was successfully retrieved, false otherwise.
 */
bool ClientConfDB::GetCmnParam(string &strKey, string &strParam)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    leveldb::Slice key(strKey); /**< Represents the key used to retrieve the common parameter from the database. */

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
 * \brief Sets a common parameter in the client configuration database.
 *
 * This function sets a common parameter with the specified key and value in the client configuration database.
 *
 * \param strKey The key of the common parameter to set.
 * \param strParam The value of the common parameter to set.
 * \return True if the common parameter was successfully set, false otherwise.
 */
bool ClientConfDB::SetCmnParam(string &strKey, string &strParam)
{
    leveldb::WriteOptions writeOptions; /**< Represents options for the database write operation. */
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    leveldb::Slice licKey(strKey); /**< Represents the key used to set the common parameter in the database. */
    leveldb::Slice licValue(strParam); /**< Represents the value of the common parameter to be set in the database. */

    // Write the common parameter to the database
    m_pDb->Put(writeOptions, licKey, licValue);
    return true; /**< Indicates successful setting of the common parameter. */
}

/**
 * \brief Retrieves the license key from the client configuration database.
 *
 * This function retrieves the license key from the client configuration database.
 *
 * \param strLicense Reference to the string where the retrieved license key will be stored.
 * \return True if the license key was successfully retrieved, false otherwise.
 */
bool ClientConfDB::GetLicense(string &strLicense)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    VSCConfLicenseKey sLicKey; /**< Represents the key used to retrieve the license key from the database. */

    leveldb::Slice key((char *)&sLicKey, sizeof(sLicKey)); /**< Represents the LevelDB key corresponding to the license key. */

    // Attempt to retrieve the license key from the database
    leveldb::Status status = m_pDb->Get(leveldb::ReadOptions(), key, &strLicense);

    // Check if the retrieval was successful
    if (!status.ok()){
        // If retrieval fails, set the license key to an empty string and return false
        strLicense = "";
        return false;
    }
    return true; /**< Indicates successful retrieval of the license key. */
}

/**
 * \brief Sets the license key in the client configuration database.
 *
 * This function sets the license key in the client configuration database.
 *
 * \param strLicense The license key to be set in the database.
 * \return True if the license key was successfully set, false otherwise.
 */
bool ClientConfDB::SetLicense(string &strLicense)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */
    
    VSCConfLicenseKey sLic; /**< Represents the key used to set the license key in the database. */
    leveldb::WriteOptions writeOptions; /**< Represents options for the database write operation. */

    leveldb::Slice licKey((char *)&sLic, sizeof(sLic)); /**< Represents the LevelDB key corresponding to the license key. */
    leveldb::Slice licValue(strLicense); /**< Represents the value of the license key to be set in the database. */

    // Write the license key to the database
    m_pDb->Put(writeOptions, licKey, licValue);

    return true; /**< Indicates successful setting of the license key. */
}

/**
 * \brief Retrieves the client configuration from the client configuration database.
 *
 * This function retrieves the client configuration from the client configuration database and stores it in the provided VidClientConf object.
 * If the configuration does not exist or cannot be parsed, default values are used.
 *
 * \param pData Reference to the VidClientConf object where the retrieved client configuration will be stored.
 * \return True if the client configuration was successfully retrieved or initialized, false otherwise.
 */
bool ClientConfDB::GetClientConf(VidClientConf &pData)
{
    VSCConfClientKey sKey; /**< Represents the key used to retrieve the client configuration from the database. */
    string strValue; /**< Represents the string value containing the retrieved client configuration. */

    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    leveldb::Slice key((char *)&sKey, sizeof(sKey)); /**< Represents the LevelDB key corresponding to the client configuration. */

    // Attempt to retrieve the client configuration from the database
    leveldb::Status status = m_pDb->Get(leveldb::ReadOptions(), key, &strValue);
    // Check if the retrieval was successful
    if (!status.ok()) {
        // If retrieval fails, set a default string value and proceed
        strValue = "fake";
    }

    // Parse the retrieved configuration string into the VidClientConf object
    if (pData.ParseFromString(strValue) == false) {
        // If parsing fails, set default values for the client configuration
        VSCClientConfDefault(pData);
        VDC_DEBUG("Client Config is not initialized\n");
        return false; /**< Indicates failure to retrieve or parse the client configuration. */
    }
    return true; /**< Indicates successful retrieval or initialization of the client configuration. */
}

/**
 * \brief Sets the client configuration in the client configuration database.
 *
 * This function sets the client configuration in the client configuration database with the provided VidClientConf object.
 *
 * \param pData Reference to the VidClientConf object containing the client configuration to be set.
 * \return True if the client configuration was successfully set, false otherwise.
 */
bool ClientConfDB::SetClientConf(VidClientConf &pData)
{
    VSCConfClientKey sKey; /**< Represents the key used to set the client configuration in the database. */
    string strOutput; /**< Represents the string containing the serialized client configuration. */

    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    leveldb::WriteOptions writeOptions; /**< Represents options for the database write operation. */

    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey)); /**< Represents the LevelDB key corresponding to the client configuration. */

    // Serialize the client configuration object into a string
    if (pData.SerializeToString(&strOutput) != true) {
        return false; /**< Indicates failure to serialize the client configuration. */
    }

    leveldb::Slice sysValue(strOutput); /**< Represents the serialized client configuration as a LevelDB value. */

    // Write the client configuration to the database
    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true; /**< Indicates successful setting of the client configuration. */
}

/**
 * \brief Searches for a storage device with the specified ID in the client configuration database.
 *
 * This function searches for a storage device with the specified ID in the client configuration database.
 *
 * \param strStorId The ID of the storage device to search for.
 * \return True if a storage device with the specified ID is found, false otherwise.
 */
bool ClientConfDB::FindStor(string strStorId)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    VidStorList storList; /**< Represents the list of storage devices retrieved from the client configuration database. */
    GetStorListConf(storList); /**< Retrieves the list of storage devices from the client configuration database. */

    // Iterate through the list of storage devices
    for (int i = 0; i < storList.cvidstor_size(); i ++) {
        const VidStor &stor = storList.cvidstor(i); /**< Represents a storage device from the list. */
        // Check if the ID of the storage device matches the specified ID
        if (stor.strid() == strStorId) {
            return true; /**< Indicates that a storage device with the specified ID is found. */
        }
    }
    return false; /**< Indicates that no storage device with the specified ID is found. */
}

/**
 * \brief Deletes a storage device with the specified ID from the client configuration database.
 *
 * This function deletes a storage device with the specified ID from the client configuration database.
 *
 * \param strStorId The ID of the storage device to delete.
 * \return True if the storage device was successfully deleted, false otherwise.
 */
bool ClientConfDB::DeleteStor(string strStorId)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    VidStorList storList; /**< Represents the list of storage devices retrieved from the client configuration database. */
    VidStorList storListNew; /**< Represents the new list of storage devices without the deleted storage device. */
    GetStorListConf(storList); /**< Retrieves the list of storage devices from the client configuration database. */

    // Iterate through the list of storage devices
    for (int i = 0; i < storList.cvidstor_size(); i ++) {
        const VidStor &stor = storList.cvidstor(i); /**< Represents a storage device from the list. */

        // Check if the ID of the storage device matches the specified ID
        if (stor.strid() != strStorId) {
            // If the IDs do not match, add the storage device to the new list
            VidStor *pAddStor = storListNew.add_cvidstor();
            *pAddStor = stor;
        }
    }

    // Update the client configuration database with the new list of storage devices
    UpdateStorListConf(storListNew);

    return true; /**< Indicates successful deletion of the storage device. */
}

/**
 * \brief Adds a storage device to the client configuration database.
 *
 * This function adds the specified storage device to the client configuration database.
 *
 * \param pStor Reference to the VidStor object representing the storage device to be added.
 * \return True if the storage device was successfully added, false otherwise.
 */
bool ClientConfDB::AddStor(VidStor &pStor)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */
    
    VidStorList storList; /**< Represents the list of storage devices retrieved from the client configuration database. */
    
    GetStorListConf(storList); /**< Retrieves the list of storage devices from the client configuration database. */

    // Add the specified storage device to the list
    VidStor *pAddStor = storList.add_cvidstor();
    *pAddStor = pStor;

    // Update the client configuration database with the new list of storage devices
    UpdateStorListConf(storList);

    return true; /**< Indicates successful addition of the storage device. */
}

/**
 * \brief Retrieves the list of storage devices from the client configuration database.
 *
 * This function retrieves the list of storage devices from the client configuration database and stores it in the provided VidStorList object.
 * If the list does not exist or cannot be parsed, default values are used.
 *
 * \param pData Reference to the VidStorList object where the retrieved list of storage devices will be stored.
 * \return True if the list of storage devices was successfully retrieved or initialized, false otherwise.
 */
bool ClientConfDB::GetStorListConf(VidStorList &pData)
{
    VSCConfVidStorKey sKey; /**< Represents the key used to retrieve the list of storage devices from the database. */
    string strValue; /**< Represents the string value containing the retrieved list of storage devices. */

    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    leveldb::Slice key((char *)&sKey, sizeof(sKey)); /**< Represents the LevelDB key corresponding to the list of storage devices. */

    // Attempt to retrieve the list of storage devices from the database
    leveldb::Status status = m_pDb->Get(leveldb::ReadOptions(), key, &strValue);

    // Check if the retrieval was successful
    if (!status.ok()) {
        // If retrieval fails, set a default string value and proceed
        strValue = "fake";
    }
    // Parse the retrieved list of storage devices string into the VidStorList object
    if (pData.ParseFromString(strValue) == false) {
        // If parsing fails, set default values for the list of storage devices
        VidStorList listDefault;
        pData = listDefault;
        VDC_DEBUG("Stor List Config is not initialized\n");
        return false; /**< Indicates failure to retrieve or parse the list of storage devices. */
    }
    return true; /**< Indicates successful retrieval or initialization of the list of storage devices. */
}

/**
 * \brief Updates the list of storage devices in the client configuration database.
 *
 * This function updates the list of storage devices in the client configuration database with the provided VidStorList object.
 *
 * \param pData Reference to the VidStorList object containing the updated list of storage devices.
 * \return True if the list of storage devices was successfully updated, false otherwise.
 */
bool ClientConfDB::UpdateStorListConf(VidStorList &pData)
{
    VSCConfVidStorKey sKey; /**< Represents the key used to update the list of storage devices in the database. */
    leveldb::WriteOptions writeOptions; /**< Represents options for the database write operation. */
    string strOutput; /**< Represents the string containing the serialized list of storage devices. */

    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey)); /**< Represents the LevelDB key corresponding to the list of storage devices. */

    // Serialize the list of storage devices object into a string
    if (pData.SerializeToString(&strOutput) != true) {
        return false; /**< Indicates failure to serialize the list of storage devices. */
    }

    leveldb::Slice sysValue(strOutput); /**< Represents the serialized list of storage devices as a LevelDB value. */

    // Update the client configuration database with the new list of storage devices
    m_pDb->Put(writeOptions, sysKey, sysValue);

    return true; /**< Indicates successful updating of the list of storage devices. */
}

/**
 * \brief Retrieves the configuration of a storage device with the specified ID from the client configuration database.
 *
 * This function retrieves the configuration of a storage device with the specified ID from the client configuration database
 * and stores it in the provided VidStor object.
 *
 * \param strId The ID of the storage device to retrieve the configuration for.
 * \param pStor Reference to the VidStor object where the retrieved storage device configuration will be stored.
 * \return True if the configuration of the storage device was successfully retrieved, false otherwise.
 */
bool ClientConfDB::GetStorConf(string strId, VidStor &pStor)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    VidStorList storList; /**< Represents the list of storage devices retrieved from the client configuration database. */
    GetStorListConf(storList); /**< Retrieves the list of storage devices from the client configuration database. */

    // Iterate through the list of storage devices
    for (int i = 0; i < storList.cvidstor_size(); i ++) {
        const VidStor &stor = storList.cvidstor(i); /**< Represents a storage device from the list. */

        // Check if the ID of the storage device matches the specified ID
        if (stor.strid() == strId) {
            pStor = stor; /**< Stores the configuration of the matching storage device in the provided VidStor object. */
            return true; /**< Indicates successful retrieval of the configuration of the storage device. */
        }
    }
    return false; /**< Indicates that no storage device with the specified ID is found. */
}

/**
 * \brief Searches for a view with the specified ID in the client configuration database.
 *
 * This function searches for a view with the specified ID in the client configuration database.
 *
 * \param strViewId The ID of the view to search for.
 * \return True if a view with the specified ID is found, false otherwise.
 */
bool ClientConfDB::FindView(string strViewId)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    VidViewList viewList; /**< Represents the list of views retrieved from the client configuration database. */
    GetViewListConf(viewList); /**< Retrieves the list of views from the client configuration database. */

    // Iterate through the list of views
    for (int i = 0; i < viewList.cvidview_size(); i ++) {
        const VidView &view = viewList.cvidview(i); /**< Represents a view from the list. */

        // Check if the ID of the view matches the specified ID
        if (view.strid() == strViewId) {
            return true; /**< Indicates that a view with the specified ID is found. */
        }
    }
    return false; /**< Indicates that no view with the specified ID is found. */
}

/**
 * \brief Deletes a view with the specified ID from the client configuration database.
 *
 * This function deletes a view with the specified ID from the client configuration database.
 *
 * \param strViewId The ID of the view to delete.
 * \return True if the view was successfully deleted, false otherwise.
 */
bool ClientConfDB::DeleteView(string strViewId)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    VidViewList viewList; /**< Represents the list of views retrieved from the client configuration database. */
    VidViewList viewListNew; /**< Represents the new list of views after deletion. */
    GetViewListConf(viewList); /**< Retrieves the list of views from the client configuration database. */

    // Iterate through the list of views
    for (int i = 0; i < viewList.cvidview_size(); i ++) {
        const VidView &view = viewList.cvidview(i); /**< Represents a view from the list. */

        // Check if the ID of the view does not match the specified ID
        if (view.strid() != strViewId) {
            // If the IDs do not match, add the view to the new list of views
            VidView *pAddView = viewListNew.add_cvidview();
            *pAddView = view;
        }
    }
    // Update the client configuration database with the new list of views
    UpdateViewListConf(viewListNew);

    return true; /**< Indicates successful deletion of the view. */
}

/**
 * \brief Adds a new view to the client configuration database.
 *
 * This function adds a new view to the client configuration database.
 *
 * \param pView Reference to the VidView object representing the view to be added.
 * \return True if the view was successfully added, false otherwise.
 */
bool ClientConfDB::AddView(VidView &pView)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */
    
    VidViewList viewList; /**< Represents the list of views retrieved from the client configuration database. */

    // Retrieve the list of views from the client configuration database
    GetViewListConf(viewList);

    // Add the new view to the list of views
    VidView *pAddView = viewList.add_cvidview();
    *pAddView = pView;

    // Update the client configuration database with the new list of views
    UpdateViewListConf(viewList);

    return true; /**< Indicates successful addition of the view. */
}

/**
 * \brief Retrieves the list of views from the client configuration database.
 *
 * This function retrieves the list of views from the client configuration database
 * and stores it in the provided VidViewList object.
 *
 * \param pData Reference to the VidViewList object where the retrieved list of views will be stored.
 * \return True if the list of views was successfully retrieved, false otherwise.
 */
bool ClientConfDB::GetViewListConf(VidViewList &pData)
{
    VSCConfViewKey sKey; /**< Represents the key used to retrieve the list of views from the database. */
    string strValue; /**< Represents the string containing the serialized list of views. */
    
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    leveldb::Slice key((char *)&sKey, sizeof(sKey)); /**< Represents the LevelDB key corresponding to the list of views. */

    // Retrieve the list of views from the client configuration database
    leveldb::Status Status = m_pDb->Get(leveldb::ReadOptions(), key, &strValue);
    if (Status.ok() == false) {
        strValue = "fake"; /**< If retrieval fails, assign a default value to the string. */
    }

    // Parse the retrieved string to populate the VidViewList object
    if (pData.ParseFromString(strValue) == false) {
        VidViewList listDefault; /**< Represents the default list of views. */
        pData = listDefault; /**< Assigns the default list of views to pData. */
        VDC_DEBUG( "View List Config is not init\n"); /**< Outputs a debug message indicating failure to initialize the view list. */
        return false; /**< Indicates failure to retrieve the list of views. */
    }
    return true; /**< Indicates successful retrieval of the list of views. */
}

/**
 * \brief Updates the list of views in the client configuration database.
 *
 * This function updates the list of views in the client configuration database with the provided data.
 *
 * \param pData Reference to the VidViewList object containing the updated list of views.
 * \return True if the list of views was successfully updated, false otherwise.
 */
bool ClientConfDB::UpdateViewListConf(VidViewList &pData)
{
    VSCConfViewKey sKey; /**< Represents the key used to update the list of views in the database. */
    string strOutput; /**< Represents the serialized string containing the updated list of views. */
    
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */

    leveldb::WriteOptions writeOptions; /**< Represents the write options for LevelDB operations. */
    leveldb::Slice sysKey((char *)&sKey, sizeof(sKey)); /**< Represents the LevelDB key corresponding to the list of views. */

    // Serialize the updated list of views to a string
    if (pData.SerializeToString(&strOutput) != true) {
        return false; /**< Indicates failure to serialize the list of views. */
    }

    leveldb::Slice sysValue(strOutput); /**< Represents the LevelDB value containing the serialized list of views. */
    // Update the client configuration database with the new list of views
    m_pDb->Put(writeOptions, sysKey, sysValue);
    return true; /**< Indicates successful update of the list of views. */
}


/**
 * \brief Retrieves the configuration of a specific view from the client configuration database.
 *
 * This function retrieves the configuration of a view with the specified ID from the client configuration database
 * and stores it in the provided VidView object.
 *
 * \param strId The ID of the view whose configuration is to be retrieved.
 * \param pView Reference to the VidView object where the retrieved view configuration will be stored.
 * \return True if the view configuration was successfully retrieved, false otherwise.
 */
bool ClientConfDB::GetViewConf(string strId, VidView &pView)
{
    std::lock_guard<std::mutex> lock(m_cMutex); /**< Locks the mutex to ensure thread safety. */
    
    VidViewList viewList; /**< Represents the list of views retrieved from the client configuration database. */
    GetViewListConf(viewList); /**< Retrieves the list of views from the client configuration database. */

    // Iterate through the list of views
    for (int i = 0; i < viewList.cvidview_size(); i ++) {
        const VidView &view = viewList.cvidview(i); /**< Represents a view from the list. */

        // Check if the ID of the view matches the specified ID
        if (view.strid() == strId) {
            pView = view; /**< Copy the view configuration to the provided VidView object. */
            return true; /**< Indicates successful retrieval of the view configuration. */
        }
    }
    return false; /**< Indicates that no view with the specified ID was found. */
}


bool ClientConfDB::FindTour(string strTourId)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	VidTourList tourList;
	GetTourListConf(tourList);
	int tourSize = tourList.cvidtour_size();

	for (int i = 0; i < tourList.cvidtour_size(); i ++)
	{
		const VidTour &tour = tourList.cvidtour(i);
		if (tour.strid() == strTourId)
		{
			return true;
		}
	}

	return false;
}
bool ClientConfDB::DeleteTour(string strTourId)
{
	std::lock_guard<std::mutex> lock(m_cMutex);

	VidTourList tourList;
	VidTourList tourListNew;
	GetTourListConf(tourList);
	int tourSize = tourList.cvidtour_size();

	for (int i = 0; i < tourList.cvidtour_size(); i ++)
	{
		const VidTour &tour = tourList.cvidtour(i);
		if (tour.strid() != strTourId)
		{
			VidTour *pAddTour = tourListNew.add_cvidtour();
			*pAddTour = tour;
		}
	}

	UpdateTourListConf(tourListNew);
	return true;
}

bool ClientConfDB::AddTour(VidTour &pTour)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	VidTourList tourList;

	GetTourListConf(tourList);
	
	VidTour *pAddTour = tourList.add_cvidtour();
	*pAddTour = pTour;
	UpdateTourListConf(tourList);

	return true;
}



bool ClientConfDB::GetTourListConf(VidTourList &pData)
{
	VSCConfTourKey sKey;
	string strValue;
	
	std::lock_guard<std::mutex> lock(m_cMutex);

	leveldb::Slice key((char *)&sKey, sizeof(sKey));

	leveldb::Status s = m_pDb->Get(leveldb::ReadOptions(), 
					key,  &strValue);
	if (s.ok() == false)
	{
		strValue = "fake";
	}

	if (pData.ParseFromString(strValue) == false)
	{
		VidTourList listDefault;
		pData = listDefault;
		//VDC_DEBUG( "Tour List Config is not init\n");
		return true;
	}

	return true;

}
bool ClientConfDB::UpdateTourListConf(VidTourList &pData)
{
	VSCConfTourKey sKey;

	std::lock_guard<std::mutex> lock(m_cMutex);

	leveldb::WriteOptions writeOptions;

	leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));

	string strOutput;
	if (pData.SerializeToString(&strOutput) != true)
	{
		return false;
	}
	leveldb::Slice sysValue(strOutput);

	m_pDb->Put(writeOptions, sysKey, sysValue);

	return true;
}

bool ClientConfDB::GetTourConf(string strId, VidTour &pTour)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	VidTourList tourList;
	GetTourListConf(tourList);
	int tourSize = tourList.cvidtour_size();

	for (int i = 0; i < tourList.cvidtour_size(); i ++)
	{
		const VidTour &tour = tourList.cvidtour(i);
		if (tour.strid() == strId)
		{
			pTour = tour;
			return true;
		}
	}

	return false;
}

bool ClientConfDB::FindGroup(string strGroupId)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	VidGroupList groupList;
	GetGroupListConf(groupList);
	int groupSize = groupList.cvidgroup_size();

	for (int i = 0; i < groupList.cvidgroup_size(); i ++)
	{
		const VidGroup &group = groupList.cvidgroup(i);
		if (group.strid() == strGroupId)
		{
			return true;
		}
	}

	return false;
}
bool ClientConfDB::DeleteGroup(string strGroupId)
{
	std::lock_guard<std::mutex> lock(m_cMutex);

	VidGroupList groupList;
	VidGroupList groupListNew;
	GetGroupListConf(groupList);
	int groupSize = groupList.cvidgroup_size();

	for (int i = 0; i < groupList.cvidgroup_size(); i ++)
	{
		const VidGroup &group = groupList.cvidgroup(i);
		if (group.strid() != strGroupId)
		{
			VidGroup *pAddGroup = groupListNew.add_cvidgroup();
			*pAddGroup = group;
		}
	}

	UpdateGroupListConf(groupListNew);
	return true;
}
bool ClientConfDB::AddGroup(VidGroup &pGroup)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	VidGroupList groupList;

	GetGroupListConf(groupList);
	
	VidGroup *pAddGroup = groupList.add_cvidgroup();
	*pAddGroup = pGroup;
	UpdateGroupListConf(groupList);

	return true;
}



bool ClientConfDB::GetGroupListConf(VidGroupList &pData)
{
	VSCConfVGroupKey sKey;
	string strValue;
	
	std::lock_guard<std::mutex> lock(m_cMutex);

	leveldb::Slice key((char *)&sKey, sizeof(sKey));

	leveldb::Status s = m_pDb->Get(leveldb::ReadOptions(), 
					key,  &strValue);
	if (s.ok() == false)
	{
		strValue = "fake";
	}

	if (pData.ParseFromString(strValue) == false)
	{
		VidGroupList listDefault;
		pData = listDefault;
		//VDC_DEBUG( "Group List Config is not init\n");
		return true;
	}

	return true;

}
bool ClientConfDB::UpdateGroupListConf(VidGroupList &pData)
{
	VSCConfVGroupKey sKey;

	std::lock_guard<std::mutex> lock(m_cMutex);

	leveldb::WriteOptions writeOptions;

	leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));

	string strOutput;
	if (pData.SerializeToString(&strOutput) != true)
	{
		return false;
	}
	leveldb::Slice sysValue(strOutput);

	m_pDb->Put(writeOptions, sysKey, sysValue);

	return true;
}

bool ClientConfDB::GetGroupConf(string strId, VidGroup &pGroup)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	VidGroupList groupList;
	GetGroupListConf(groupList);
	int groupSize = groupList.cvidgroup_size();

	for (int i = 0; i < groupList.cvidgroup_size(); i ++)
	{
		const VidGroup &group = groupList.cvidgroup(i);
		if (group.strid() == strId)
		{
			pGroup = group;
			return true;
		}
	}

	return false;
}

bool ClientConfDB::FindEmap(string strEmapId)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	VidEmapList emapList;
	GetEmapListConf(emapList);
	int emapSize = emapList.cvidemap_size();

	for (int i = 0; i < emapList.cvidemap_size(); i ++)
	{
		const VidEmap &emap = emapList.cvidemap(i);
		if (emap.strid() == strEmapId)
		{
			return true;
		}
	}

	return false;
}
bool ClientConfDB::DeleteEmap(string strEmapId)
{
	std::lock_guard<std::mutex> lock(m_cMutex);

	VidEmapList emapList;
	VidEmapList emapListNew;
	GetEmapListConf(emapList);
	int emapSize = emapList.cvidemap_size();

	for (int i = 0; i < emapList.cvidemap_size(); i ++)
	{
		const VidEmap &emap = emapList.cvidemap(i);
		if (emap.strid() != strEmapId)
		{
			VidEmap *pAddEmap = emapListNew.add_cvidemap();
			*pAddEmap = emap;
		}
	}

	UpdateEmapListConf(emapListNew);
	return true;
}
bool ClientConfDB::AddEmap(VidEmap &pEmap)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	VidEmapList emapList;

	GetEmapListConf(emapList);
	
	VidEmap *pAddEmap = emapList.add_cvidemap();
	*pAddEmap = pEmap;
	UpdateEmapListConf(emapList);

	return true;
}



bool ClientConfDB::GetEmapListConf(VidEmapList &pData)
{
	VSCConfEmapKey sKey;
	string strValue;
	
	std::lock_guard<std::mutex> lock(m_cMutex);

	leveldb::Slice key((char *)&sKey, sizeof(sKey));

	leveldb::Status s = m_pDb->Get(leveldb::ReadOptions(), 
					key,  &strValue);
	if (s.ok() == false)
	{
		strValue = "fake";
	}

	if (pData.ParseFromString(strValue) == false)
	{
		VidEmapList listDefault;
		pData = listDefault;
		//VDC_DEBUG( "Emap List Config is not init\n");
		return true;
	}

	return true;

}
bool ClientConfDB::UpdateEmapListConf(VidEmapList &pData)
{
	VSCConfEmapKey sKey;

	std::lock_guard<std::mutex> lock(m_cMutex);

	leveldb::WriteOptions writeOptions;

	leveldb::Slice sysKey((char *)&sKey, sizeof(sKey));

	string strOutput;
	if (pData.SerializeToString(&strOutput) != true)
	{
		return false;
	}
	leveldb::Slice sysValue(strOutput);

	m_pDb->Put(writeOptions, sysKey, sysValue);

	return true;
}

bool ClientConfDB::GetEmapConf(string strId, VidEmap &pEmap)
{
	std::lock_guard<std::mutex> lock(m_cMutex);
	
	VidEmapList emapList;
	GetEmapListConf(emapList);
	int emapSize = emapList.cvidemap_size();

	for (int i = 0; i < emapList.cvidemap_size(); i ++)
	{
		const VidEmap &emap = emapList.cvidemap(i);
		if (emap.strid() == strId)
		{
			pEmap = emap;
			return true;
		}
	}

	return false;
}

