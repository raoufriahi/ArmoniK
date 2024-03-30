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
#include "type.hpp"


#define VE_OBJ_GROUP_MAX 64  /* Max object in each frame.*/
#define VE_MOT_BOX_MAX 4096  /* (1024/16) * (1024/16) =  4096, every 16 x 16 has a Box.*/

typedef struct __VeBox
{
	unsigned int x0; 
	unsigned int y0; 
	unsigned int w;/* box width */
	unsigned int h;/* box height */	
	unsigned int rw;/* image width */
	unsigned int rh;/* image height */
}VeBox;

typedef struct __VeObj
{
	long id;
	VeBox box;
}VeObj;


typedef struct __VeObjGroup
{
	VeObj obj[VE_OBJ_GROUP_MAX];
}VeObjGroup;


typedef struct __VeMotionBox
{
	unsigned int size;/* Total num of BOX */
	unsigned int rw; /* image width */
	unsigned int rh; /* image heigh */
	unsigned int survival;/* survival frames */ 
	VeBox mot[VE_MOT_BOX_MAX];
}VeMotionBox;


