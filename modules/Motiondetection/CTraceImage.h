/*
  Titre        : TraceImage.h
  description  : 
  auteur       :
  date         : 12.01.2010
  evolutions   : 
*/
#ifndef TRACE_IMAGE
#define TRACE_IMAGE

#include "include.h"

using namespace std;

#define MAX_SIZE 1024
#define IPRINT1(x1,x2) sprintf(x2,"%s\\*",x1);
typedef list<string> LIST_STR;

class CTraceImage
{
	public:
		CTraceImage();
		CTraceImage(char* PathDir);
		~CTraceImage();	
		void set_folder(char* cameraKey);
		void trace_image(IplImage* im, char* cameraKey);
		void trace_image_processed(IplImage* im, char* cameraKey);
		void trace_bin(char* cameraKey, char* bin, int sz_bin);
		void trace_bin2(char* cameraKey, char* bin, int sz_bin);
		void trace_ini(char* section, char* key, int value);
		void trace_results(char* cameraKey, float analysisResult, int alarm_threshold);
		void init_trace(char* path);
		void ThreadCleanUpthread(void);
	private:
			
		void WritePrivateProfileInt(LPCSTR lpszSection,LPCSTR lpszEntry,int Value, LPCSTR lpszFilename);
		void DeleteOldestDirectory();
		void DeleteDirectory(char *PathDir);
		INT FindOldestCreationTime(char *szPathName);
		int DirSize(string pt_path);

	private:
		char m_globalTraceFolder[MAX_SIZE];
		char m_alarmSubFolder[MAX_SIZE];	
		char m_alarmSubFolderName[MAX_SIZE];
		char m_cameraKey[MAX_SIZE];
		int m_iPicId;
		int m_sizeMaxDirAlr;
		bool m_bCUTIsStopped;

		int m_id_processed;
		int m_id;
};

#endif // TRACE_IMAGE
/*--------------------------- End of TraceImage.h ----------------------*/


