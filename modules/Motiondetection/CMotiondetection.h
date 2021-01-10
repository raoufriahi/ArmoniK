#ifndef MOTION_DETECTION_INCLUDE
#define MOTION_DETECTION_INCLUDE
/*!
* \file CMotiondetection.h
* \brief 
* \date 16.03.11 
* \author
* \version 0.1
*/
#include "include.h"
#include "CPictureAnalyser.h"

/*! \class CMotiondetection
* \brief 
* Classe principale appelant les differents sous modules du sensor:
* - construction de l'image de mouvement (ForeGround)
* - module de detection et d'identification des blobs
* - module de calcul d'intersection entre les trajectoires et les barrieres
* - gestion des alarmes par combinaison de franchissement de barrieres
*/
	class CMotiondetection
	{
	public:
		/*!
		* \brief Constructeur
		*
		* constructeur de la classe CFilmAnalysis
		*
		*/
		CMotiondetection();

		/*!
		* \brief Destructeur
		*
		* destructeur de la classe CFilmAnalysis
		*/
		virtual ~CMotiondetection();

		/*!
		* \brief Chargement des parametres
		*
		* methode qui permet de recharger dynamiquement les parametres
		*
		* \param channel : numero de voie
		* \param prepo : numero de preposition
		* \return : null
		*/
		//void FA_LoadParam_(char* cameraKey);
		bool md_OnNewContext_(char *configuration_path, int channel_id);
	
		/*!
		* \brief Fonction principale de detection sensor
		*
		* methode a qui l'on fournit l'image courante et qui renvoie le 
		* statut de la detection: en alarme ou non
		* 
		* \param BufferY : buffer de l'image
		* \param format : format de l'image
		* \param width : largeur de l'image
		* \param height : hauteur de l'image
		* \param channel : numero de voie
		* \param prepo : numero de preposition
		* \param metadata : n/c
		* \param existingMD : n/c
		* \param BufferOut : buffer de debuggage
		* \param bufOutSize : n/c
		* \return : vrai 1 fois en cas d'alarme faux sinon
		*/
		bool md_PutImage(unsigned char *buffer_24bits, int width, int height, unsigned char *bufferOut, long* bufOutSize, char* key);
	
		/*!
		* \brief Reception d'informations sur l'alarme
		*
		* methode qui remplit la structure AR_SSR_EVT_DESRIPTION contenant des informations
		* sur la nature de l'alarme dont l'identifiant barriere
		*
		* \param pRet : informations sur l'alarme
		* \return : retourne vrai si SSR_PutImage_() retourne vrai
		*/
		bool md_GetAlarmInfo(char* pRet);

	private:

		void md_init(int width, int height);
		bool md_fillRGB(unsigned char *BufferY, TI_PICTURE_FORMAT format, int width, int height);

	private:	
		CPictureAnalyser* obj_pa;	

		IplImage* m_src_;		 //!<  image courante en niveaux de gris (8 bpp)
		IplImage* m_src24b;		 //!<  image courante en couleur (24 bpp)
		IplImage* m_maskImg;	 //!<  image 8bpp contenant les zones masquees en noir le reste en blanc
		IplImage* m_srcWithMask; //!<  image courante en 8 bpp avec les zones masquees en noir
		IplImage* m_srcTmp_;

		int m_height;		     //!<  hauteur de l'image en entree
		int	m_width;		     //!<  largeur de l'image en entree
		char m_cameraKey[256];
		int m_idAlarm;
		unsigned long m_timeLastAlarm;	//!< date de la derniere alarme en secondes depuis 1970
		double m_dataFilter1;		    //!<  seuil du filtre anti-eblouissement
		double m_dataSpeedFilter;
		double m_dataSizeFilter;
		bool m_reloadParam;			   //!<  declenche la (re)-initialisation des parametres si = true, active par SSR_LoadParam_()
	};

#endif
				
				

