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
/**
 * \brief Enumeration representing PTZ (Pan-Tilt-Zoom) actions.
 *
 * This enumeration defines various PTZ actions that can be performed, such as moving
 * the camera up, down, left, right, zooming in, zooming out, stopping PTZ movement, etc.
 */
typedef enum
{
    F_PTZ_UP       = 1,  /**< Move the PTZ camera upwards. */
    F_PTZ_DOWN     = 2,  /**< Move the PTZ camera downwards. */
    F_PTZ_LEFT     = 3,  /**< Move the PTZ camera to the left. */
    F_PTZ_RIGHT    = 4,  /**< Move the PTZ camera to the right. */
    F_PTZ_ZOOM_IN  = 5,  /**< Zoom in the PTZ camera. */
    F_PTZ_ZOOM_OUT = 6,  /**< Zoom out the PTZ camera. */
    F_PTZ_STOP     = 7,  /**< Stop PTZ movement. */
    F_PTZ_LAST     = 8   /**< Placeholder for the last PTZ action. */
} FPtzAction;

/**
 * \brief Enumeration representing camera status changes.
 *
 * This enumeration defines various camera status changes, such as transitioning from
 * off to on, on to off, no change in status, etc.
 */
typedef enum
{
    DEV_OFF2ON      = 1,  /**< Transition from off to on state. */
    DEV_ON2OFF      = 2,  /**< Transition from on to off state. */
    DEV_NO_CHANGE   = 3,  /**< No change in camera status. */
    DEV_LAST        = 4   /**< Placeholder for the last camera status change. */
} CameraStatus;

/* This is same with QStorageInfo define */
enum HddDriveType {
	HddUnknownDrive = 0,
	HddInternalDrive,
	HddRemovableDrive,
	HddRemoteDrive,
	HddCdromDrive,
	HddRamDrive
};

typedef enum
{
    FACTORY_CAMERA_ADD = 1,
    FACTORY_CAMERA_DEL,
    FACTORY_CAMERA_ONLINE,
    FACTORY_CAMERA_OFFLINE,
    FACTORY_CAMERA_REC_ON,
    FACTORY_CAMERA_REC_OFF,
    FACTORY_CAMERA_HDFS_REC_ON,
    FACTORY_CAMERA_HDFS_REC_OFF,
    FACTORY_CAMERA_LAST
} FactoryCameraChangeType;
