#include "CMotiondetection_exports.h"
#include "CMotiondetection.h"

CMotiondetection* obj_FA[SSR_NBMAX_CHANNEL] = {NULL};

uint32_t
MD_CreateInstance()
{
	uint32_t ui_numsession = 0;
	
	while(ui_numsession < SSR_NBMAX_CHANNEL && obj_FA[ui_numsession] != NULL)
		ui_numsession++ ;
	
	if(ui_numsession < SSR_NBMAX_CHANNEL){
		obj_FA[ui_numsession] = new CMotiondetection();
		//CC_OnNewContext: Session %d is opened", iNumSession
	}else {
		// CC_OnNewContext: iNumSession = %d > NB_INSTANCE_DECODER", iNumSession
		return -1;	
	}

	return ui_numsession;
}

bool
MD_OnNewContext (
		uint32_t ui_instance,
		char *sz_configuration_path,
		uint32_t ui_channel_id)
{
	if(	ui_instance < 0 ||
			ui_instance >= SSR_NBMAX_CHANNEL)
		return false;

	return obj_FA[ui_instance] ? obj_FA[ui_instance]->md_OnNewContext(
			sz_configuration_path,
			ui_channel_id): false;
}

bool
MD_AskSaveContext (
		uint32_t ui_instance,
		char *sz_configuration_path,
		uint32_t uichannel_id)
{

	if(	ui_instance < 0 ||
			ui_instance >= SSR_NBMAX_CHANNEL )
		return false;		

	return true;
}

bool
MD_PutImage(
		uint32_t ui_instance,
		unsigned char *p_buffer_8bits,
		uint32_t ui_width,
		uint32_t ui_height,
		unsigned char *sz_buffer_out,
		long* p_buf_out_size,
		char* sz_key)
{
	return obj_FA[ui_instance] ? obj_FA[ui_instance]->md_PutImage(
			p_buffer_8bits,
			ui_width,
			ui_height,
			sz_buffer_out,
			p_buf_out_size,
			sz_key) :
			false;
 }
bool
MD_GetAlarmInfo(
		uint32_t ui_instance,
		char* p_return)
{
	return  obj_FA[ui_instance] ? obj_FA[ui_instance]->md_GetAlarmInfo(
			p_return): false;
}
