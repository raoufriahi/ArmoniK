// TraceImage.cpp : Defines the entry point for the DLL application.
//
#include "CTraceImage.h"

extern void util_get_liste_fichier_recursif(string * pt_path,string * filter, string *file , LIST_STR * pt_lst);

static bool bInitCleanUpthread = false;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void fct_CleanUpthread(CTraceImage *Me)
{
	Me->ThreadCleanUpthread();
}

CTraceImage::CTraceImage(char* PathDir)
{
	memset(m_globalTraceFolder, 0, MAX_SIZE);
	memset(m_alarmSubFolderName, 0, MAX_SIZE);
	memset(m_alarmSubFolder, 0, MAX_SIZE);
	memset(m_cameraKey, 0, MAX_SIZE);
	m_sizeMaxDirAlr = 0;
	init_trace(PathDir);

}
CTraceImage::~CTraceImage()
{
	m_bCUTIsStopped = true;
	Sleep(5000);
}



/*#*****************************************************************************
* Function  : ThreadInputStream
*-------------------------------------------------------------------------------
* Parameters  :
*    Input   - void
*    Output  - void
*-------------------------------------------------------------------------------
* Action   :  reception of video streams
*
******************************************************************************#*/
void CTraceImage::ThreadCleanUpthread(void)
{
	char lpszFilename[MAX_SIZE];
	sprintf(lpszFilename, "%s\\analysis_alarm_infos.ini", m_globalTraceFolder);
	m_sizeMaxDirAlr = 1024 * 1024 * GetPrivateProfileInt("general","size_max_for_alarm_directory_Mo", 500, lpszFilename);

	while(	!m_bCUTIsStopped )		
	{ 
		Sleep(5000);

		while(DirSize(m_globalTraceFolder) > m_sizeMaxDirAlr)
		{
			this->DeleteOldestDirectory();
			//Sleep(50);
		}
		
	}

}

void CTraceImage::init_trace(char* path)
{
	sprintf_s(m_globalTraceFolder, path);
	CreateDirectory(m_globalTraceFolder, NULL);
	memset(m_cameraKey, 0, MAX_SIZE);

	m_id_processed=0;
	m_id=0;
}

void CTraceImage::set_folder(char* cameraKey)
{	
	if(strcmp(cameraKey, m_cameraKey) != 0)
	{//new camera key

		/*char lpszFilename[MAX_SIZE];
		sprintf(lpszFilename, "%s\\analysis_alarm_infos.ini", m_globalTraceFolder);

		m_sizeMaxDirAlr = 1024 * 1024 * GetPrivateProfileInt("general","size_max_for_alarm_directory_Mo", 500, lpszFilename);

		while(DirSize(m_globalTraceFolder) > m_sizeMaxDirAlr)
		{ 
			this->DeleteOldestDirectory();
		}*/

		m_id_processed=0;
		m_id=0;
		
		struct	tm * CurrDate;
		time_t	CurrTime;
		time( &CurrTime );
		CurrDate = localtime( &CurrTime );
		int date; int heure;
		date	= (CurrDate->tm_mon + 1)*100 + CurrDate->tm_mday;
		heure	= CurrDate->tm_hour * 10000 + CurrDate->tm_min*100 + CurrDate->tm_sec;
		sprintf_s(m_alarmSubFolder, "%s\\%s_%04d_%06d", m_globalTraceFolder, cameraKey, date, heure);
		sprintf_s(m_alarmSubFolderName, "%s_%04d_%06d", cameraKey, date, heure);
		int CreatDir = CreateDirectory(m_alarmSubFolder, NULL);
	}

	sprintf_s(m_cameraKey, cameraKey);

	return;
}
void CTraceImage::trace_image(IplImage* im, char* cameraKey)
{	
	//Enregistrement de l'image trace
	char path[MAX_SIZE];
	m_id++;
	sprintf_s(path,"%s\\im_%s_%d.jpeg", m_alarmSubFolder, cameraKey, m_id);
	cvSaveImage(path, im);	
	return;
}
void CTraceImage::trace_image_processed(IplImage* im, char* cameraKey)
{	
	m_id_processed++;
	//Enregistrement de l'image trace
	char sub_folder_processed[MAX_SIZE];
	//sprintf_s(sub_folder_processed,"%s\\processed", m_alarmSubFolder);
	sprintf_s(sub_folder_processed,"%s", m_alarmSubFolder);
	int ret = CreateDirectory(sub_folder_processed, NULL);

	char path[MAX_SIZE];
	sprintf_s(path,"%s\\processed_im_%s_%d.jpeg", sub_folder_processed, cameraKey, m_id_processed);
	cvSaveImage(path, im);

	return;
}
void CTraceImage::trace_bin(char* cameraKey, char* bin, int sz_bin)
{
	char file_name[MAX_SIZE];
	sprintf(file_name,"%s\\FAEngineParameters.bin", m_alarmSubFolder);
	FILE* pFile = fopen(file_name, "wb");

	if(pFile != 0)
	{
		int n = fwrite(bin, 1, sz_bin, pFile);
		fclose(pFile);
	}

	return;
}

void CTraceImage::trace_bin2(char* cameraKey, char* bin, int sz_bin)
{
	char file_name[MAX_SIZE];
	sprintf(file_name,"%s\\FAEngineParameters.ben", m_alarmSubFolder);
	FILE* pFile = fopen(file_name, "w");

	if(pFile != 0)
	{
		int n = fwrite(bin, 1, sz_bin, pFile);
		fclose(pFile);
	}

	return;
}


void CTraceImage::trace_ini(char* section, char* key, int value)
{
	char lpszFilename[MAX_SIZE];
	sprintf(lpszFilename, "%s\\film_analysis_options.ini", m_alarmSubFolder);
	WritePrivateProfileInt(section, key, value, lpszFilename);
	return;
}
void CTraceImage::trace_results(char* cameraKey, float analysisResult, int alarm_threshold)
{
	char process_answer[8];
	if(analysisResult > alarm_threshold)
		sprintf(process_answer, "yes");
	else
		sprintf(process_answer, "no");

	char lpszFilename[MAX_SIZE];
	sprintf(lpszFilename, "%s\\analysis_alarm_infos.ini", m_globalTraceFolder);
	WritePrivateProfileString (m_alarmSubFolderName, "real_alarm", process_answer, lpszFilename);
	return;
}

void CTraceImage::WritePrivateProfileInt(LPCSTR lpszSection,LPCSTR lpszEntry,int Value, LPCSTR lpszFilename)
{
	char szConverted [256];
	_itoa (Value, szConverted, 10);
	WritePrivateProfileString (lpszSection, lpszEntry, szConverted,lpszFilename);
}
INT CTraceImage::FindOldestCreationTime(char *szPathName)
{
	WIN32_FIND_DATA FindFileData;
	DWORD time=0;
	HANDLE hFind;
	hFind = FindFirstFile(szPathName, &FindFileData);	//Find the first file in the directory.
	if(hFind != INVALID_HANDLE_VALUE)
	{
		time=FindFileData.ftCreationTime.dwLowDateTime;
		FindClose(hFind);
	}
	return time;
}


void CTraceImage::DeleteOldestDirectory()
{
	DWORD MIN=/*0x55D4A801*/ INFINITE;
    WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	char FilePath[MAX_SIZE];
	char FilePath_to_erase[MAX_SIZE];
	memset(FilePath,0,MAX_SIZE);
	memset(FilePath_to_erase,0,MAX_SIZE);
	IPRINT1(m_globalTraceFolder,FilePath);
	DWORD Time;

	hFind = FindFirstFile(FilePath, &FindFileData);	//Find the first file in the directory.
	if(hFind != INVALID_HANDLE_VALUE)
	{		
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			if(!strcmp(FindFileData.cFileName,"..") ||	!strcmp(FindFileData.cFileName,"analysis_alarm_infos.ini"))	 continue;	
			sprintf(FilePath,"%s%s", m_globalTraceFolder, FindFileData.cFileName);
			Time = (DWORD)FindOldestCreationTime(FilePath);

			//time_t curr_time = time(NULL);
			//DWORD diff_time = (DWORD)time(NULL) - FindFileData.ftCreationTime.dwLowDateTime;
			//if(diff_time > 60)
			//{
			//	DeleteDirectory(FilePath);
			//	continue;
			//}

			if(MIN>Time)
			{
				sprintf(FilePath_to_erase,"%s%s", m_globalTraceFolder, FindFileData.cFileName);
				MIN=Time;
			}
		}
		FindClose(hFind);
	}

	if(strlen(FilePath_to_erase) > 0)
		DeleteDirectory(FilePath_to_erase);

	return;
}
void CTraceImage::DeleteDirectory(char *PathDir)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	char szPathName[MAX_SIZE];
	IPRINT1(PathDir,szPathName);

	hFind = FindFirstFile(szPathName, &FindFileData);	//Find the first file in the directory.

	if(hFind != INVALID_HANDLE_VALUE)
	{		
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			if(!strcmp(FindFileData.cFileName,".."))	 
				continue;	

			sprintf(szPathName,"%s\\%s",PathDir,(char*)FindFileData.cFileName);
			remove(szPathName);
		}
		FindClose(hFind);
		RemoveDirectory(PathDir);
	}
	return;
}
int CTraceImage::DirSize(string pt_path)
{
	int Size=0;
	LIST_STR::iterator txt;
	string filter="*.*";
	string file;
    LIST_STR pt_lst;
    util_get_liste_fichier_recursif(&pt_path,&filter, &file ,&pt_lst);
	for (txt = pt_lst.begin(); txt != pt_lst.end(); ++txt)
	{
		string sPath=pt_path+"\\"+(*txt);
		Size+=GetTailleFichier(sPath.c_str());
	}
	return Size;
}
