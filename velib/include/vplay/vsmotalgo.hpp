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


#define VVID_MOT_PROC_W 320
#define VVID_MOT_PROC_H 240

#define VVID_MOT_REGION_W (VVID_MOT_PROC_W/8)
#define VVID_MOT_REGION_H (VVID_MOT_PROC_H/8)

class VVidMotRegLine
{
public:
	VVidMotRegLine()
	{
		for (int i = 0; i < VVID_MOT_REGION_W; i++) {
			enable[i] = 0;
		}
	}
	VVidMotRegLine(bool bEnable)
	{
		for (int i = 0; i < VVID_MOT_REGION_W; i ++) {
			enable[i] = bEnable;
		}
	}
	VVidMotRegLine(const VVidMotRegLine &pLine)
	{
		for (int i = 0; i < VVID_MOT_REGION_W; i ++) {
			enable[i] = pLine.enable[i];
		}
	}

public:
	unsigned char enable[VVID_MOT_REGION_W];
};

class VVidMotReg
{
public:
	VVidMotReg()
	{
		VVidMotRegLine lineIn;
		for (int i = 0; i < VVID_MOT_REGION_H; i ++) {
			line[i] = lineIn;
		}
	}
	VVidMotReg(VVidMotRegLine pLine)
	{
		for (int i = 0; i < VVID_MOT_REGION_H; i ++) {
			line[i] = pLine;
		}
	}
	VVidMotReg(const VVidMotReg &pReg)
	{
		for (int i = 0; i < VVID_MOT_REGION_H; i ++) {
			line[i] = pReg.line[i];
		}
	}
	VVidMotReg(std::string &pReg)
	{
		for (int i = 0; i < VVID_MOT_REGION_H; i ++) {
			for (int j = 0; j < VVID_MOT_REGION_W; j ++) {
				line[i].enable[j] = pReg[i * VVID_MOT_REGION_W + j];
			}
		}
	}
	std::string ToString()
	{
		std::string reg;
		for (int i = 0; i < VVID_MOT_REGION_H; i ++) {
			for (int j = 0; j < VVID_MOT_REGION_W; j ++) {
				char c = line[i].enable[j];
				reg.push_back(c);
			}
			
		}
		return reg;
	}

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
		if (pMot.length() != VVID_MOT_REGION_W * VVID_MOT_REGION_H) {
			return false;
		}
		else {
			return true;
		}
	}
private:
	VVidMotRegLine line[VVID_MOT_REGION_H];
};


class VSMotAlgoData;
class VE_LIBRARY_API VSMotAlgo
{
public:

	VSMotAlgo(bool bFullFrame);
	~VSMotAlgo();
	bool ProcessFrame(RawFrame& frame);
	bool SetMotReg(VVidMotReg &motReg);
	bool GetOutput(VBlobVec &pOutput, int &w, int &h);
    
private:
    VSMotAlgoData * m_data;
};

/**
 * \brief The VSMotAlgoData class represents the data used by the VSMotAlgo algorithm.
 */
class VE_LIBRARY_API VSMotAlgoData
{
public:
    /**
     * \brief Constructs a VSMotAlgoData object.
     *
     * \param bFullFrame A boolean indicating whether to process the full frame or not.
     */
    VSMotAlgoData(bool bFullFrame);

    /**
     * \brief Destroys the VSMotAlgoData object.
     */
    ~VSMotAlgoData();

    /**
     * \brief Processes a frame.
     *
     * This function processes the provided frame.
     *
     * \param frame The RawFrame object representing the frame to be processed.
     * \return A boolean indicating whether the frame was processed successfully or not.
     */
    bool ProcessFrame(RawFrame& frame);

    /**
     * \brief Gets the output of the algorithm.
     *
     * This function retrieves the algorithm's output, storing it in the provided output vector.
     *
     * \param pOutput The output vector to store the algorithm's output.
     * \param w The width of the output.
     * \param h The height of the output.
     * \return A boolean indicating whether the output was retrieved successfully or not.
     */
    bool GetOutput(VBlobVec& pOutput, int& w, int& h);

    /**
     * \brief Sets the motion registration data.
     *
     * This function sets the motion registration data used by the algorithm.
     *
     * \param motReg The VVidMotReg object representing the motion registration data.
     * \return A boolean indicating whether the motion registration data was set successfully or not.
     */
    bool SetMotReg(VVidMotReg& motReg);

private:
    int m_ProcWidth;          ///< The width of the processed frame.
    int m_ProcHeight;         ///< The height of the processed frame.
    bool m_bFullFrame;        ///< A flag indicating whether to process the full frame or not.
    char* m_buf;              ///< A buffer used for internal processing.
    unsigned char* m_mot;                ///< A buffer used for motion processing.
    struct timeval m_lastTime;///< The timestamp of the last processed frame.
    VVidMotReg m_motReg;      ///< The motion registration data.
};