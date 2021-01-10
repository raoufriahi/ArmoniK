/*
 * Copyright (c) 2017-2018 Heimdall
 *
 * The computer program contained herein contains proprietary
 * information which is the property of Heimdall.
 * The program may be used and/or copied only with the written
 * permission of Heimdall or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#ifndef __V_SMOT_ALGO_HPP__
#define __V_SMOT_ALGO_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "utility/videotype.hpp"

using namespace UtilityLib;

#define VVID_MOT_PROC_W 320
#define VVID_MOT_PROC_H 240

#define VVID_MOT_REGION_W (VVID_MOT_PROC_W/8)
#define VVID_MOT_REGION_H (VVID_MOT_PROC_H/8)

class VVidMotRegLine
{
public:
	VVidMotRegLine()
	{
		for (int i = 0; i < VVID_MOT_REGION_W; i++)
		{
			enable[i] = 0;
		}
	}
	VVidMotRegLine(bool bEnable)
	{
		for (int i = 0; i < VVID_MOT_REGION_W; i ++)
		{
			enable[i] = bEnable;
		}
	}
	VVidMotRegLine(const VVidMotRegLine &pLine)
	{
		for (int i = 0; i < VVID_MOT_REGION_W; i ++)
		{
			enable[i] = pLine.enable[i];
		}
	}

public:
	u8 enable[VVID_MOT_REGION_W];
};

class VVidMotReg
{
public:
	VVidMotReg()
	{
		VVidMotRegLine lineIn;
		for (int i = 0; i < VVID_MOT_REGION_H; i ++)
		{
			line[i] = lineIn;
		}
	}
	VVidMotReg(VVidMotRegLine pLine)
	{
		for (int i = 0; i < VVID_MOT_REGION_H; i ++)
		{
			line[i] = pLine;
		}
	}
	VVidMotReg(const VVidMotReg &pReg)
	{
		for (int i = 0; i < VVID_MOT_REGION_H; i ++)
		{
			line[i] = pReg.line[i];
		}
	}
	VVidMotReg(std::string &pReg)
	{
		for (int i = 0; i < VVID_MOT_REGION_H; i ++)
		{
			for (int j = 0; j < VVID_MOT_REGION_W; j ++)
			{
				line[i].enable[j] = pReg[i * VVID_MOT_REGION_W + j];
			}
			
		}
	}
	std::string ToString()
	{
		std::string reg;
		for (int i = 0; i < VVID_MOT_REGION_H; i ++)
		{
			for (int j = 0; j < VVID_MOT_REGION_W; j ++)
			{
				char c = line[i].enable[j];
				reg.push_back(c);
			}
			
		}

		return reg;
	}

public:
	static VVidMotReg CreateOff()
	{
		VVidMotRegLine line(0);
		
		VVidMotReg reg(line);
		
		return reg;
	}

	static VVidMotReg CreateAll()
	{
		VVidMotRegLine line(1);
		
		VVidMotReg reg(line);
		return reg;
	}
	static bool CheckStringForReg(std::string &pMot)
	{
		if (pMot.length() != VVID_MOT_REGION_W * VVID_MOT_REGION_H)
		{
			return false;
		}else
		{
			return true;
		}
	}
public:
	VVidMotRegLine line[VVID_MOT_REGION_H];
};


class VSMotAlgoData;
class VE_LIBRARY_API VSMotAlgo
{
public:

	VSMotAlgo(bool bFullFrame);
	~VSMotAlgo();
public:
	bool ProcessFrame(RawFrame& frame);
	bool SetMotReg(VVidMotReg &motReg);
	bool GetOutput(VBlobVec &pOutput, s32 &w, s32 &h);

public:
    VSMotAlgoData * m_data;
};
#endif /* __V_SMOT_ALGO_HPP__ */
