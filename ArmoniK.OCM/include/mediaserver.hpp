/*
 * Copyright (c) 2017-2023 UbVideo
 *
 * The computer program contained herein contains proprietary
 * information which is the property of UbVideo.
 * The program may be used and/or copied only with the written
 * permission of UbVideo or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#pragma once
#include "server/factory.hpp"

class VEMediaChannel;

typedef std::map<string, VEMediaChannel*> VEMediaChannelMap;
/**
 * \class VEMediaServer
 * \brief Represents a media server.
 */
class VEMediaServer 
{
public:
	VEMediaServer(Factory &pFactory);
	~VEMediaServer();
	
public:
    /**
	 * \brief Initializes the media server.
	 * \return true if the initialization is successful, false otherwise.
	 */
	static bool InitMediaServer();
	
public:
    /**
	 * \brief Callback function for device change events.
	 * \param pData Pointer to additional data.
	 * \param change The FactoryCameraChangeData object representing the change event.
	 * \return true if the callback was successfully executed, false otherwise.
	 */
	static bool DeviceChangeCallbackFunc(void* pData, FactoryCameraChangeData change);
	
	/**
	 * \brief Callback function for device change events.
	 * \param change The FactoryCameraChangeData object representing the change event.
	 * \return true if the callback was successfully executed, false otherwise.
	 */
	bool DeviceChangeCallbackFunc1(FactoryCameraChangeData change);
	
public:
    /**
	 * \brief Initializes all devices.
	 * \return true if the initialization is successful, false otherwise.
	 */
	bool InitDevices();

	/**
	 * \brief Initializes a single device.
	 * \param pParam The VidCamera object representing the device parameters.
	 * \return true if the initialization is successful, false otherwise.
	 */
	bool InitOneDevice(VidCamera  pParam);

	/**
	 * \brief Deletes a single device.
	 * \param strId The ID of the device to delete.
	 * \return true if the deletion is successful, false otherwise.
	 */
	bool DeleteOneDevice(astring strId);
	
private:
	VEMediaChannelMap m_ChannelMap;   // A map of media channel names to VEMediaChannel pointers.
	Factory &m_pFactory;              // A reference to the Factory object.
};

