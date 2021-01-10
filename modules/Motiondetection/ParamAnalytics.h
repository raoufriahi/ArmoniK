#ifndef _PARAM_ANALYTICS_METADATA
#define _PARAM_ANALYTICS_METADATA

typedef int  BOOL;
typedef long  LONG;
typedef unsigned char BYTE;

/***************************************************************************************************/
/***********************     Structure d'info et evenement en provenance  du sensor  ***************/
/***************************************************************************************************/
#define AR_SSR_MAX_BLOB_LIST	  20
#define	MAX_TRAJECT				  1000
#define	SSR_NBMAX_CHANNEL		  33
#define	SSR_NBMAX_PRESET		  16
#define AR_SSR_MAX_LINE		      16
#define AR_SSR_MAX_DBLINE	      16
#define AR_SSR_MAX_POLY		      16
#define AR_SSR_MAX_POLYMASK	      16
#define AR_SSR_MAX_STICK	      16
#define AR_SSR_MAX_PATH		      16
#define AR_SRS_MAX_POINT_POLYMASK 16

typedef struct 
{
	uint32_t x;
	uint32_t y;
	uint32_t dx;
	uint32_t dy;
} AR_SSR_POINT;

typedef enum
{
	AR_ANALYTICS_TYPE_EVT_SSR_OnAbandonnedObject     = 1,
	AR_ANALYTICS_TYPE_EVT_SSR_OnEnterInZone		     = 2,
	AR_ANALYTICS_TYPE_EVT_SSR_OnCrossFromRightToLeft = 3, 
	AR_ANALYTICS_TYPE_EVT_SSR_OnCrossFromLeftToRight = 4,
	AR_ANALYTICS_TYPE_EVT_SSR_OnCrossFromL1_to_L2    = 5,
	AR_ANALYTICS_TYPE_EVT_SSR_OnCrossFromL2_to_L1    = 6,
	AR_ANALYTICS_TYPE_EVT_TRK_OnEnterInZone          = 101,

} AR_ANALYTICS_TYPE_EVT;

typedef enum
{
	AR_SSR_GEOMETRIC_OBJECT_TYPE_NONE		= 0,
	AR_SSR_GEOMETRIC_OBJECT_TYPE_LINE		= 1,
	AR_SSR_GEOMETRIC_OBJECT_TYPE_DBLINE 	= 2,
	AR_SSR_GEOMETRIC_OBJECT_TYPE_POLY		= 3,
	AR_SSR_GEOMETRIC_OBJECT_TYPE_STICK		= 4,
	AR_SSR_GEOMETRIC_OBJECT_TYPE_RECTANGLE	= 5,
	AR_SSR_GEOMETRIC_OBJECT_TYPE_ELLIPSE	= 6,
	AR_SSR_GEOMETRIC_OBJECT_TYPE_POLYMASK	= 7,

} AR_SSR_RULE_TYPE;

typedef struct{
	uint32_t  analysis_result;
	uint32_t  analysis_threshold;
	uint32_t  width ;    //channel width
	uint32_t  height ;   //channel height
	char filter_type[64];
} AR_SSR_EVT_DESCRIPTION;


/* BLOB STRUCTURE*/
struct CvBlob_ // 20 octets
{
    float       x,y; // blob position
    float       w,h; // blob sizes
    uint32_t    ID;  // blbo ID
};

typedef struct //4 + (20 * AR_SSR_MAX_BLOB_LIST)= 404 octets
{
	uint32_t  nbBlobInList;
	CvBlob_   blobList[AR_SSR_MAX_BLOB_LIST];
} AR_SSR_TRAJ_PACKET;

typedef struct 
{
	uint32_t a;
} AR_SSR_BLOB_PACKET;


typedef struct{

	int		SSR_Seg_BGalpha;							// segmentation module: persistence coefficient
	int		SSR_Seg_Dilate;								// segmentation module: dilatation coefficient
	int		SSR_Seg_Erode;								// segmentation module: erosion coefficient

	int		SSR_PostTrk_TrackNum ;						// post-tracking module: first trajectory id
	float	SSR_PostTrk_HMin ;							// post-tracking module: minimum blob height
	float	SSR_PostTrk_WMin ;							// post-tracking module: minimum blob width
	float	SSR_PostTrk_MinDistToBorder ;				// post-tracking module: minimum distance to image border
	int		SSR_PostTrk_Clastering ;					// post-tracking module: activate clastering module (1) or not (0)
	int		SSR_PostTrk_SeqSize ;						// post-tracking module: minimum persistance to create a new blob (in number of pictures)

	float	SSR_Trk_AlphaSize;							// tracking module: size refreshment speed
	float	SSR_Trk_AlphaPos  ;							// tracking module: position refreshment speed
	int		SSR_Trk_Collision;							// tracking module: try to manage collision (1) or not (0)
	int		SSR_Trk_ClearHyp;							// tracking module: see default value
	int		SSR_Trk_ConfidenceType;						// tracking module: see default value
	float	SSR_Trk_Alpha;								// tracking module: see default value

	double	SSR_Traj_TimeWithoutMvt;					// trajectory module: maximum time of life  for left objects
	double	SSR_Traj_TimeToLive; 						// trajectory module: maximum time of life  for active objects//

	bool	SSR_Alr_AlarmON;							// alarm module:	is true in case of alarm
	int		SSR_Alr_DelaiAlarm;							// alarm module: minimum delay in seconds between 2 alarms

	int		SSR_Flt_ActiveFilter1;						// filter module: activate (1) or not (0) filter 1
	int		SSR_Flt_ActiveFilter2;						// filter module: activate (1) or not (0) filter 2
	int		SSR_Flt_ContactFilter1;						// filter module: output id on I/O device for filter 1
	int		SSR_Flt_ContactFilter2;						// filter module: output id on I/O device for filter 2
	int		SSR_Flt_ThresholdFilter11;					// filter module: threshold 1 for filter 1
	int		SSR_Flt_ThresholdFilter12;					// filter module: threshold 2 for filter 1
	int		SSR_Flt_ThresholdFilter21;					// filter module: threshold 1 for filter 2

	int		SSR_CarSpeedMAX;						// maximum speed for cars
	int		SSR_CarspeedMIN;						// minimum speed for cars
	int		SSR_CarSizeMAX;							// maximum size for cars
	int		SSR_CarSizeMIN;							// minimum size for cars
	int		SSR_PedSpeedMAX;						// maximum speed for pedestrians
	int		SSR_PedspeedMIN;						// minimum speed for pedestrians
	int		SSR_PedSizeMAX;							// maximum size for pedestrians
	int		SSR_PedSizeMIN;							// minimum size for pedestrians

} AR_SSR_GENERAL_SETTINGS;

/***************************************************************************************************/
/***********************     Structure ou enum utilis� par les logiciels en interne  ***************/
/***************************************************************************************************/
typedef enum
{
	AR_SSR_GEOMETRIC_OBJECT_TYPE_NONE			= 0,
	AR_SSR_GEOMETRIC_OBJECT_TYPE_LINE			= 1,
	AR_SSR_GEOMETRIC_OBJECT_TYPE_DBLINE			= 2,
	AR_SSR_GEOMETRIC_OBJECT_TYPE_POLY			= 3, 
	AR_SSR_GEOMETRIC_OBJECT_TYPE_STICK			= 4,
	AR_SSR_GEOMETRIC_OBJECT_TYPE_RECTANGLE		= 5, 
	AR_SSR_GEOMETRIC_OBJECT_TYPE_ELLIPSE		= 6,
	AR_SSR_GEOMETRIC_OBJECT_TYPE_POLYMASK		= 7,
} AR_SSR_RULE_OBJECT_TYPE;

//////////////////////////////////////////////////////////////////////////////
///////////////:: PARAMETRES PAR CHANNEL/PRESET ////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//  les id sont en fait la position de l'objet dans le tableau
//  ex dans AR_SSR_RULE_LINE  tabLine [3];  l'id est 3 
typedef struct 
{
	uint32_t x1;
	uint32_t y1;
	uint32_t x2;
	uint32_t y2;
} AR_SSR_LINE_COORD;


typedef struct 
{
	uint32_t	Id;								// line identifier
	char		Description[50];				// line nickname
	AR_SSR_LINE_COORD	Coordinate_L1;			// line coordinates
	BOOL		SendAlarmOnCrossFromRightToLeft;// send alarm or not when something match the rule
	BOOL		SendAlarmOnCrossFromLeftToRight;
	BOOL		AcceptCar;						// Accept path with cars
	BOOL		AcceptPedestrian;				// Accept path with cars
} AR_SSR_RULE_LINE;


typedef struct 
{
	uint32_t	Id;								// double line identifier
	char		Description[50];				// double line nickname
	uint32_t	Id_L1;							// line 1 coordinates
	uint32_t	Id_L2;							// line 2 coordinates
	BOOL		SendAlarmWhenL1_to_L2;			// restricted crossing direction (true: send alarm when crossing from L1 to L2)
	BOOL		SendAlarmWhenL2_to_L1;			// restricted crossing direction
	BOOL		AcceptCar;						// Accept path with cars
	BOOL		AcceptPedestrian;				// Accept path with cars
} AR_SSR_RULE_DBLINE;


#define AR_SRS_MAX_POINT_POLY 16
typedef struct 
{
	uint32_t	Id;								// polygon identifier
	char		Description[50];				// polygon nickname
	uint32_t	NbOfPoint;						// number of coordinates
	uint32_t	CoordX[AR_SRS_MAX_POINT_POLY];	// rule coordinates
	uint32_t	CoordY[AR_SRS_MAX_POINT_POLY];	// rule coordinates
	BOOL		SendAlarmOnAbandonnedObject;	// send alarm or not when detecting left object
	BOOL		SendAlarmOnEnterInZone;			// send alarm or not when object entering in zone
	BOOL		AcceptCar;						// Accept path with cars
	BOOL		AcceptPedestrian;				// Accept path with cars
} AR_SSR_RULE_POLY;


typedef struct 
{
	uint32_t	Id;								    // polygon identifier
	uint32_t	NbOfPoint;						    // number of coordinates
	uint32_t	CoordX[AR_SRS_MAX_POINT_POLYMASK];	// rule coordinates
	uint32_t	CoordY[AR_SRS_MAX_POINT_POLYMASK];	// rule coordinates
} AR_SSR_RULE_POLYMASK;


typedef struct 
{
	uint32_t	id;								  // path identifier
	char		description[50];				  // path nickname
	uint32_t    tabId[8];						  // rule's list
	uint32_t	IdObjSendedOnAlarm;				  // In case this path is a double line (for user)
	AR_SSR_RULE_OBJECT_TYPE TypeObjSendedOnAlarm; // In case this path is a double line (for user)

} AR_SSR_RULE_CRITICAL_PATH;


typedef struct 
{//sticks coordinates [0;12000]
	uint32_t	P1_x;		// horizontal stick point 1 coordinate x
	uint32_t	P1_y;		// horizontal stick point 1 coordinate y
	uint32_t	P2_x;		// horizontal stick point 2 coordinate x
	uint32_t	P2_y;		// horizontal stick point 2 coordinate y
	uint32_t	P3_x;		// vertical stick point 3 coordinate x
	uint32_t	P3_y;		// vertical stick point 3 coordinate y
	uint32_t	P4_x;		// vertical stick point 4 coordinate x
	uint32_t	P4_y;		// vertical stick point 4 coordinate y
	uint32_t	Real_h;		// stick real estimated height
	uint32_t	Real_w;		// stick real estimated width

} AR_SSR_RULE_STICK;


typedef struct{
	double  homography[9];	// homography matrix:
							// homography[0]		homography[1]		homography[2]
							// homography[3]		homography[4]		homography[5]
							// homography[6]		homography[7]		homography[8]
} AR_SSR_HOMOGRAPHY_MATRIX;


typedef struct 
{
	uint32_t			        FileVersion;				// for reserve using, Set this value to 1 for first version
	uint32_t			        NbStick;
	uint32_t			        NbRuleLine;					// number of rules (line type)
	uint32_t			        NbRulePoly;					// number of rules (polyline type)
	uint32_t			        NbRuleDbLine;				// number of rules (doubleline type)
	uint32_t			        NbRulePath;					// number of paths
	uint32_t	                NbRulePolyMask;
	AR_SSR_RULE_LINE	        tabLine [AR_SSR_MAX_LINE];
	AR_SSR_RULE_DBLINE	        tabDbLine[AR_SSR_MAX_DBLINE];
	AR_SSR_RULE_POLY	        tabPoly[AR_SSR_MAX_POLY];
	AR_SSR_RULE_STICK			tabStick[AR_SSR_MAX_STICK];
	AR_SSR_RULE_POLYMASK		tabPolyMask[AR_SSR_MAX_POLYMASK];
	AR_SSR_RULE_CRITICAL_PATH	tabPath[AR_SSR_MAX_PATH];
	AR_SSR_HOMOGRAPHY_MATRIX	HomographyMatrix;
	AR_SSR_GENERAL_SETTINGS		GeneralSettings;
	uint32_t			        WindowWidthPx;
	uint32_t			        WindowHeightPx;
} AR_SSR_PARAM_CHANNEL_PRESET;

/***************************************************************************************************/
/***********************     Enum�ration des diff�rents formats d'image			  ***************/
/***************************************************************************************************/
typedef enum
{ 
	FORMAT_UNKNOWN  = -1,
	FORMAT_RGB8     = 10,
	FORMAT_RGB16    = 11,
	FORMAT_RGB24    = 12,
	FORMAT_JPEG     = 20,
	FORMAT_YUV420   = 30,
	FORMAT_YUV422   = 31,
	FORMAT_YUV444   = 32,
	FORMAT_Y        = 33,

}TI_PICTURE_FORMAT;

#endif
