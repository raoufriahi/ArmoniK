#ifndef MOTION_DETECTION_EXPORTS
#define MOTION_DETECTION_EXPORTS

#include <stdint.h>

/*#*****************************************************************************
*	Fonction		: MD_CreateInstance
*-------------------------------------------------------------------------------
*	Parameters		:	
*    Input			- void
*    Output			- null
*-------------------------------------------------------------------------------
*   Action			: creates a Motion detection instance
*-------------------------------------------------------------------------------
*	Value back		: null
******************************************************************************#*/
uint32_t
MD_CreateInstance(void);


/*#*****************************************************************************
*	Fonction		: MD_OnNewContext
*-------------------------------------------------------------------------------
*	Parameters		:	
*    Input			- ui_instance: Motion detection instance identifier
*					- sz_configuration_path: whole configuration file's path for current executable
*					- sz_channel_id: current executable id
*    Output			- null
*-------------------------------------------------------------------------------
*   Action			: load  Motion detection's parameters for a specific channel
*-------------------------------------------------------------------------------
*	Value back		: true if parameter's files have been correctly loaded
******************************************************************************#*/
bool
MD_OnNewContext (
		uint32_t ui_instance,
		char *sz_configuration_path,
		uint32_t sz_channel_id);


/*#*****************************************************************************
*	Fonction		: MD_AskSaveContext
*-------------------------------------------------------------------------------
*	Parameters		:	
*    Input			- ui_instance: Motion detection instance identifier
*					- sz_configuration_path: complete configuration file's path for current channel
*					- sz_channel_id: current executable id
*    Output			- null
*-------------------------------------------------------------------------------
*   Action			: save motion detection's parameters for a specific channel
*-------------------------------------------------------------------------------
*	Value back		: null
******************************************************************************#*/
bool
MD_AskSaveContext (
		uint32_t ui_instance,
		char *sz_configuration_path,
		uint32_t ui_channel_id);


/*#*****************************************************************************
*	Fonction		: MD_PutImage
*-------------------------------------------------------------------------------
*	Parameters		:	
*    Input			- ui_instance: Motion detection instance identifier
*					- p_buffer_y: input buffer 8 bits
*					- ui_width: input buffer width
*					- ui_height: input buffer height
*					- p_buffer_out: output buffer with drawn metadata
*					- p_buf_out_size: output buffer's size
*    Output			- null
*-------------------------------------------------------------------------------
*   Action			: process Sensor algorithm
*-------------------------------------------------------------------------------
*	Value back		: true in case of alarm
******************************************************************************#*/
bool
MD_PutImage(
		uint32_t ui_instance,
		unsigned char *p_buffer_8bits,
		uint32_t ui_width,
		uint32_t ui_height,
		unsigned char *pbufferOut,
		long* p_buf_out_size,
		char* sz_key);


/*#*****************************************************************************
*	Fonction		: MD_GetAlarmInf
*-------------------------------------------------------------------------------
*	Parameters		:	
*    Input			- ui_instance: Motion detection instance identifier
*    Output			- p_return: alarm info structure
*-------------------------------------------------------------------------------
*   Action			: in case of alarm returns infos like channel and
*					  preset numbers, rule id and type of id.
*-------------------------------------------------------------------------------
*	Value back		: returns true if called after FA_PutImage_()==true
******************************************************************************#*/
bool
MD_GetAlarmInfo(
		uint32_t ui_instance,
		char* p_return);
