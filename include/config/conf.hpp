/*
 * Copyright (c) 2017-2023 Heimdall
 *
 * The computer program contained herein contains proprietary
 * information which is the property of Heimdall.
 * The program may be used and/or copied only with the written
 * permission of Heimdall or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#ifndef _CONF_H_
#define _CONF_H_

#include "utility.hpp"
#include "config/confver.hpp"

#define LINK_PROTO_WS_PATH         "/linkproto"    /**< This macro defines the path used for establishing a WebSocket connection for the link protocol.*/
#define LINK_PROTO_WS_STREAM_PATH  "/linkstream"   /**< This macro defines the path used for streaming data over the established link protocol WebSocket connection.*/


/* 0xFF FFFF to 0xFFFF FFFF is for status for the map */
/**
 * \brief Minimum value for an invalid configuration map.
 *
 * This constant defines the minimum value that represents an invalid
 * configuration map. Any configuration map with a value less than or equal
 * to this constant is considered invalid.
 *
 * \note The value of this constant is 0xFFFFFF.
 */
#define CONF_MAP_INVALID_MIN 0xFFFFFF

/**
 * \brief Maximum length for a configuration key string.
 *
 * This constant defines the maximum number of characters allowed in a
 * configuration key string. Any configuration key string that exceeds this
 * length will be truncated.
 *
 * \note The value of this constant is 16.
 */
#define CONF_KEY_STR_MAX 16


/* The length of key max < 16 !!! */
#define VSC_CONF_KEY             "ConfVSCSystem"
#define VSC_CONF_STOR_SERVER_KEY "ConfVSCStorS"  /* server */
#define VSC_CONF_CLIENT_KEY      "ConfVSCClient" /* Client */
#define VSC_CONF_LIC_KEY         "ConfVSCLicense"
#define VSC_CONF_CAMERA_KEY      "ConfVSCCAMERA"
#define VSC_CONF_VIDSTOR_KEY     "ConfVSCVidStor"
#define VSC_CONF_VIEW_KEY        "ConfVSCView"
#define VSC_CONF_TOUR_KEY        "ConfVSCTour"
#define VSC_CONF_VGROUP_KEY      "ConfVSCVGroup"
#define VSC_CONF_HDFS_RECORD_KEY "ConfVSCHdfsRec"
#define VSC_CONF_EMAP_KEY        "ConfVSCEmap"
#define VSC_CONF_USER_KEY        "ConfVSCUserCnf"

#ifdef WIN32
#define VSC_DEFAULT_SYSPATH  "C:\\"
#else
#define VSC_DEFAULT_SYSPATH  "ve/"
#endif

#define REC_SCHED_OFF      "0e4adca1-a2d6-499c-84fc-68c450e46d8f"
#define REC_SCHED_ALL_DAY  "44241a90-8927-4907-a3d1-f32f29c2266e"
#define REC_SCHED_WORK_DAY "2010e075-f937-461f-a1a0-be39b385e0a5"
#define VVID_UUID_NULL     "a17bdf7d-9588-4bf3-8667-3fbaeaabe59e"
#define VVID_UUID_ALL      "a1947501-930e-476f-a8b1-9002b0a6ca66"
#define VVID_MAIN_VIEW     "18531236-e5df-415b-b547-2440115d1164"

//------------------------------------------------------------------------------
// Conf keys, for leveldb
//
//------------------------------------------------------------------------------

#pragma pack(push,  1 )
typedef struct __VSCConfSystemKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfSystemKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_KEY);
    }
}VSCConfSystemKey;

typedef struct __VSCConfStorServerKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfStorServerKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_STOR_SERVER_KEY);
    }
}VSCConfStorServerKey;

typedef struct __VSCConfClientKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfClientKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_CLIENT_KEY);
    }
}VSCConfClientKey;

typedef struct __VSCConfLicenseKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfLicenseKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_LIC_KEY);
    }
}VSCConfLicenseKey;

typedef struct __VSCConfCameraKey {
    s8 Key[CONF_KEY_STR_MAX];
	__VSCConfCameraKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_CAMERA_KEY);
    }
}VSCConfCameraKey;

/* HDFS Reocrd key */
typedef struct __VSCConfHdfsRecordKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfHdfsRecordKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_HDFS_RECORD_KEY);
    }
}VSCConfHdfsRecordKey;

typedef struct __VSCConfVidStorKey {
    s8 Key[CONF_KEY_STR_MAX];
	__VSCConfVidStorKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_VIDSTOR_KEY);
    }
}VSCConfVidStorKey;

typedef struct __VSCConfViewKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfViewKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_VIEW_KEY);
    }
}VSCConfViewKey;

typedef struct __VSCConfTourKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfTourKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_TOUR_KEY);
    }
}VSCConfTourKey;

/* Camera Group key */
typedef struct __VSCConfGroupKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfGroupKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_VGROUP_KEY);
    }
}VSCConfVGroupKey;

/* Emap configuration, storage the emap conf, such as the camera location*/
typedef struct __VSCConfEmapKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfEmapKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_EMAP_KEY);
    }
}VSCConfEmapKey;

/* User key */
typedef struct __VSCConfUserKey {
    s8 Key[CONF_KEY_STR_MAX];
    __VSCConfUserKey()
    {
        memset(Key, 0, CONF_KEY_STR_MAX);
        strcpy(Key, VSC_CONF_USER_KEY);
    }
}VSCConfUserKey;

/**
 * @brief Video wall layout mode enumeration.
 *
 * This enumeration represents the different layout modes available for a video wall.
 * Each mode specifies the arrangement of video tiles on the wall.
 */
typedef enum
{
    LAYOUT_MODE_1 = 1,      /**< Layout mode 1: Single tile. */
    LAYOUT_MODE_2X2,        /**< Layout mode 2x2: 2 rows and 2 columns of tiles. */
    LAYOUT_MODE_3X3,        /**< Layout mode 3x3: 3 rows and 3 columns of tiles. */
    LAYOUT_MODE_4X4,        /**< Layout mode 4x4: 4 rows and 4 columns of tiles. */
    LAYOUT_MODE_6,          /**< Layout mode 6: 6 tiles arranged in a specific pattern. */
    LAYOUT_MODE_8,          /**< Layout mode 8: 8 tiles arranged in a specific pattern. */
    LAYOUT_MODE_12p1,       /**< Layout mode 12+1: 12 tiles arranged in a specific pattern with an additional tile. */
    LAYOUT_MODE_5x5,        /**< Layout mode 5x5: 5 rows and 5 columns of tiles. */
    LAYOUT_MODE_6x6,        /**< Layout mode 6x6: 6 rows and 6 columns of tiles. */
    LAYOUT_MODE_8x8,        /**< Layout mode 8x8: 8 rows and 8 columns of tiles. */
    LAYOUT_MODE_ONE,        /**< Layout mode ONE: A single large tile. */
    LAYOUT_MODE_1x3,        /**< Layout mode 1x3: 1 row and 3 columns of tiles. */
    LAYOUT_MODE_1p6,        /**< Layout mode 1+6: 1 large tile with 6 smaller tiles arranged around it. */
    LAYOUT_MODE_LAST        /**< Layout mode LAST: The last mode in the enumeration. */
} VideoWallLayoutMode;


#pragma pack(pop)

#endif /* _CONF_H_ */
