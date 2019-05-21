
// VTapi.h

#ifndef _VTAPI_H
#define _VTAPI_H

#include "DataType.h"

#ifndef _LIB
	#ifdef VTSDK_EXPORTS
	#define VT_API extern "C" __declspec(dllexport) int WINAPI
	#else
	#define VT_API extern "C" __declspec(dllimport) int WINAPI
	#endif
#else
	#ifdef VTSDK_EXPORTS
	#define VT_API extern "C" int WINAPI
	#else
	#define VT_API extern "C" int WINAPI
	#endif
#endif


 /*==============================================================
	Function: initialize the device, back to camera handle, used to call the other function
 
	Parameters:	pFilterName	:	reserves
			pIndex			camera number
			pCapInfo		initialization camera parameters   
	Note: must allocate enough memory space to pCapInfo->Buffer,
	 general distribution than the largest collection of space to be big,
	 recommend the use of MAXWIDTH*MAXHEIGHT+512 for transfer the collection data 
			
  --------------------------------------------------------------*/
VT_API 
VT_Initialize(	IN  LPCTSTR			pFilterName,
				IN	OUT	int				*pIndex,	
				IN  CapInfoStruct	*pCapInfo,
				OUT HANDLE			*hCamera
				);	



/*==============================================================
	Function:	VT_Uninitialize
	Function:	Uninitialized device   
	Note:  must call in program exit, to release the allocated memory
  --------------------------------------------------------------*/
VT_API
VT_Uninitialize( IN HANDLE hCamera );


/*==============================================================
	Function:	VT_StartView
	Function:	Open the preview window, and start the video stream
   
   
  --------------------------------------------------------------*/
VT_API
VT_StartView( IN HANDLE	hCamera, 
			  IN LPCTSTR	lpszWindowName, 
			  IN DWORD		dwStyle, 
			  IN int		x, 
			  IN int		y, 
			  IN int		nWidth, 
			  IN int		nHeight, 
			  IN HWND		hwndParent, 
			  IN HMENU		nIDorHMenu
			  );



/*==============================================================
	Function:	VT_StopView
	Function:	stop the video stream
 
 
-----------------------------------------------------*/
VT_API
VT_StopView( IN HANDLE  hCamera );


/*==============================================================
	Function:	VT_SetCapInfo
	Function:	Real time video parameters
   
   
  --------------------------------------------------------------*/
VT_API
VT_SetCapInfo( IN HANDLE hCamera , IN CapInfoStruct  *pCapInfo );


/*==============================================================
	Function:	VT_GetCapInfo
	Function:	Get real time video parameters   
  --------------------------------------------------------------*/
VT_API
VT_GetCapInfo( IN HANDLE hCamera , OUT CapInfoStruct  *pCapInfo );


/*==============================================================
	Function:	VT_SetParam
	Function:	Set the image parameters
	  

	  
  --------------------------------------------------------------*/
VT_API
VT_SetParamColor(IN HANDLE hCamera, COLOR_MODE  DLParam);
VT_API
VT_SetParamFlip(IN HANDLE hCamera, FLIP_MODE  DLParam);




/*==============================================================	
	parameters:   VOID (WINAPI *DL_AUTOCALLBACK )( DWORD dw1, LPVOID lpContext );
			To obtain the AWB results as follows by lpParam1:
			ULONG   Result = dw1;				// 0xFF202020
			BYTE btRed		= ( BYTE )(Result);				// 0x20
			BYTE btGreen	= ( BYTE )( Result>>8 );		// 0x20
			BYTE btBlue		= ( BYTE )( Result>>16 );		// 0x20
			ULONG Exposure  = ( BYTE )( Result>>24 ) * 2;	// 255 * 2 = 510 ms
			  
  --------------------------------------------------------------*/
VT_API 
VT_SetDoAWB( IN HANDLE hCamera, IN BOOL bAWB, IN BYTE btTarget,
			  IN DL_AUTOCALLBACK pfAWBCallback,	 IN LPVOID lpContext );

/*==============================================================
	Function:	VT_SetDoAE
	Function:	To start the automatic exposure operation	  
	  
  --------------------------------------------------------------*/
VT_API 
VT_SetDoAE( IN HANDLE hCamera, IN BOOL bAE, IN BYTE btTarget, 
			 IN DL_AUTOCALLBACK pfAECallback, IN LPVOID lpContext );


/*==============================================================
	Function:	VT_GetFrameRate
	Function:	Get the current frame rate of the camera	 
  --------------------------------------------------------------*/
VT_API 
VT_GetFrameRate( IN HANDLE	hCamera, OUT float *pfFrameRate );


/*==============================================================
	Function:	VT_GetFrameRate                         
  --Function:	Get the current frame rate of the camera------------------------------------------------------------*/
VT_API
VT_GetRawFrame(IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo);


/*==============================================================
	Function:	VT_GetRawFrameToBmp
	Function:	Collect one frame of original data to pCapInfo->Buffer, and save the data into a 8 bit BMP. 
  --------------------------------------------------------------*/
VT_API
VT_GetRawFrameToBmp(IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo, LPCTSTR  strFileName );


/*==============================================================
	Function:	VT_GetRgbFrame
	Function:	Collecttion of a 24bitRGB data into pDest, 
	               at the same time, the original data will be stored in pCapInfo->Buffer.    
  --------------------------------------------------------------*/
VT_API
VT_GetRgbFrame(IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo, OUT BYTE* pDest, BOOL bFlip=FALSE);


/*==============================================================
	Function:	VT_GetRgbFrameToBmp
	Function:	Collect one frame of image to a file, the file format based on CAMPARAM settings change.                
			    If set to bClearView, pDest is the result of optimization algorithm and the results of 24bit conversion                                           
			    If you set the UPDOWN image data will do flip vertical corresponding                                    
  --------------------------------------------------------------*/
VT_API
VT_GetRgbFrameToBmp( IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo, OUT BYTE* pDest, LPCTSTR  strFileName, IN BOOL SaveData=FALSE ); 


/*==============================================================
	Function:	VT_GetRgbFrameToJpeg
	Function:	Collect one frame of image to a Jpeg file,  
  --------------------------------------------------------------*/
VT_API
VT_GetRgbFrameToJpeg( IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo, OUT BYTE* pDest, LPCTSTR  strFileName, int nQuality ); 


/*==============================================================
	Function:	VT_SetContrastValue
	Function:	Add contrast adjusting function
	Parameters:   fValue range: 1 - 32   
  --------------------------------------------------------------*/
VT_API 
VT_SetContrastValue( IN HANDLE hCamera, IN int fValue);


/*==============================================================
	Function:	VT_GetContrastValue
	Function:	Gets the contrast
	Parameters:   fValue Returns the contrast   
  --------------------------------------------------------------*/
VT_API 
VT_GetContrastValue( IN HANDLE hCamera, OUT int *fValue);


/*==============================================================
	Function:	VT_CaptureToAvi
	Function:	AVI video
	Parameters:   bStart = TRUE:  start video FALSE: stop video 
  --------------------------------------------------------------*/
VT_API
VT_CaptureToAvi( IN HANDLE hCamera, BOOL bStart, LPCTSTR  strFileName );


/*==============================================================
	Function:	VT_GetTrigger
	Function:	Start the external trigger mode, used for image acquisition
	Parameters:   ImageType£ºspecifies the save image type,   BMP and JPG two format is now supported
	              pFilterNameStart:setting file name ,If the input is empty, at the time named syste
	              JpegCompressibility:if selected images saved as JPEG format, 
	                                  the parameters for a given compression ratio range of 1~100
	Note:  Once received a trigger signal gathering a frame of image                    
	       Only supports low speed camera, the VT-F series does not provide the function
  --------------------------------------------------------------*/
VT_API
VT_GetTrigger( IN HANDLE hCamera, BOOL bStart, 
			   IN  VTIMAGETYPR ImageType, 
			   IN  LPCTSTR	pFileNameStart,
			   IN  int JpegCompressibility=50);

/*==============================================================
	Function:	VT_SetScrollOffset
	Function:	Set the video window scroll offset  
  --------------------------------------------------------------*/
VT_API 
VT_SetScrollOffset(IN HANDLE hCamera,int Xoffset, int Yoffset);


/*==============================================================
	Function:	VT_SetVideoSize
	Function:	Set video size   
  --------------------------------------------------------------*/
VT_API 
VT_SetVideoSize(IN HANDLE hCamera,int nWidth, int nHeight);



/*==============================================================
	Function:	VT_GetTotalDeviceNum
	Function:	Get installed cameras number  
  --------------------------------------------------------------*/
VT_API
VT_GetTotalDeviceNum(IN HANDLE hCamera, int * pNum);



/*==============================================================
	Function:	VT_GetDeviceID
	Function:	To get the camera serial number
  --------------------------------------------------------------*/
VT_API
VT_GetDeviceID(IN HANDLE hCamera, IN USHORT * pDeviceID);



/*==============================================================
	Function:	VT_SetFrameCallBack
	Function:	Set  frame  callback  function
	Parameters:   FrameCB   Frame callback function
			      lpContext The user context   
  --------------------------------------------------------------*/
VT_API
VT_SetFrameCallBack(IN HANDLE hCamera, IN DL_FRAMECALLBACK FrameCB, LPVOID lpContext);

/*==============================================================
	Function:		VT_SetDeviceEvent
	Function:		Set message callback function
	Parameters:     FrameCB     Frame callback function 
			        lpContext   The user context        
  --------------------------------------------------------------*/
VT_API
VT_SetDeviceEvent(IN HANDLE hCamera, IN DL_AUTOCALLBACK Event, LPVOID lpContext);

/*==============================================================
	Function:		VT_GetFriendlyName
	Function:		To get the equipment device name
	Parameters:    pName  Back to hCamera designated by the camera's friendly name   
  --------------------------------------------------------------*/
VT_API
VT_GetFriendlyName(IN HANDLE hCamera, OUT TCHAR * pName);


/*==============================================================
	Function:	VT_SetClearView
	Function:	Set clear video
	Parameters:   bOn   Clear video mode of open mark, TRUE for open, FALSE for closed
  --------------------------------------------------------------*/
VT_API
VT_SetClearView(IN HANDLE hCamera, IN BOOL bOn);


/*==============================================================
	Function:		VT_GetClearViewState
	Function:		Inquires the clear video mode whether are open
	Parameters:    bOn Clear video mode of open mark, TRUE for open, FALSE for closed     
  --------------------------------------------------------------*/
VT_API
VT_GetClearViewState(IN HANDLE hCamera, OUT BOOL &bOn);


/*==============================================================
	Function:	VT_GetDeviceType
	Function:	Get the type of camera
	Parameters:   CamType   Returns the type of camera   
  --------------------------------------------------------------*/
VT_API
VT_GetDeviceType(IN HANDLE hCamera, OUT VTCAMERA *CamType);


/*==============================================================
	Function:		VT_SetIO
	Function:		Set IO control bit,
	Parameters:     Data
	Note:   
			Pin order
			(2)		(4)		(6)		(8)		(10)
			[1]		(3)		(5)		(7)		(9)	

				data
	data bit:		BIT7    BIT6	BIT5	BIT4	BIT3	BIT2	BIT1	BIT0
	The corresponding pin													[1]		(4)
  --------------------------------------------------------------*/
VT_API
VT_SetIO(IN HANDLE hCamera,IN BYTE Data);


/*==============================================================
	Function:	VT_GetIO
	Function:	Get IO control bit, 
	Parameters:   Data  Returns a IO control bit
	Note:         
				Pin order
				(2)		(4)		(6)		(8)		(10)
				[1]		(3)		(5)		(7)		(9)	

				data
	data bit:		BIT7    BIT6	BIT5	BIT4	BIT3	BIT2	BIT1	BIT0
	The corresponding pin									(7)		(8)		(5)		(6)
  --------------------------------------------------------------*/
VT_API
VT_GetIO(IN HANDLE hCamera, IN OUT BYTE *pData);


/*==============================================================
	Function:		VT_GetSerialNumber
	Function:		Get the serial number
	Parameters:    data    Returns the serial number 
	Note:          Call the function, users need to apply for 16 bytes for pData
  --------------------------------------------------------------*/
VT_API
VT_GetSerialNumber(IN HANDLE hCamera, IN BYTE *pData);


/*==============================================================
	Function:		VT_PixelCalibration
	Function:		Open bad point detection function
	Parameters:    
	Note:            The results will be automatically saved to the VTMOS.bpt file
  --------------------------------------------------------------*/
VT_API
VT_PixelCalibration(IN HANDLE hCamera);


/*==============================================================
	Function:		VT_SetShowAutoMark
	Function:		Set up automatic exposure  mark displays a status
	Parameters:    
	Note:          
  --------------------------------------------------------------*/
VT_API
VT_SetShowAutoMark(IN HANDLE hCamera, IN BOOL bShow);



/*==============================================================
	Function:		VT_SetShowVideo
	Function:		In the use of RZ_StartView began after the acquisition, set up if display images
	Parameters:    
	Note:          
  --------------------------------------------------------------*/
VT_API
VT_SetShowVideo(IN HANDLE hCamera, IN BOOL bShow);


/*==============================================================
	Function:	VT_SelectAviCompressor
	Function:	Select the  video compressor, and returned by fccHandler
	Parameters:   
	Note:         
  --------------------------------------------------------------*/
VT_API
VT_SelectAviCompressor(IN HANDLE hCamera, OUT DWORD *fccHandler);


/*==============================================================
	Function:	VT_SetAviCompressor
	Function:	Set video compressor
	Parameters:   
	Note:         
  --------------------------------------------------------------*/
VT_API
VT_SetAviCompressor(IN HANDLE hCamera, IN DWORD fccHandler);


/*==============================================================
	Function:		VT_SetGainR
	Function:		set red gain
	Parameters:     
	Note:           
  --------------------------------------------------------------*/
VT_API
VT_SetGainR(IN HANDLE hCamera, IN int nIdxR);


/*==============================================================
	Function:	VT_SetGainG
	Function:	set green  gain
	Parameters:   
	Note:         
  --------------------------------------------------------------*/
VT_API
VT_SetGainG(IN HANDLE hCamera, IN int nIdxG);


/*==============================================================
	Function:	VT_SetGainB
	Function:	set blue  gain 
	Parameters:  
	Note:        
  --------------------------------------------------------------*/
VT_API
VT_SetGainB(IN HANDLE hCamera, IN int nIdxB);


/*==============================================================
	Function:	VT_GetGainR
	Function:	get red  gain
	Parameters:  
	Note:        
  --------------------------------------------------------------*/
VT_API
VT_GetGainR(IN HANDLE hCamera, OUT int* pIndexR);


/*==============================================================
	Function:		VT_GetGainB
	Function:		get blue gain
	Parameters:    
	Note:          
  --------------------------------------------------------------*/
VT_API
VT_GetGainB(IN HANDLE hCamera, OUT int* pIndexB);


/*==============================================================
	Function:	VT_GetGainG
	Function:	get green gain
	Parameters:    
	Note:          
  --------------------------------------------------------------*/
VT_API
VT_GetGainG(IN HANDLE hCamera, OUT int* pIndexG);


/*==============================================================
	Function:	VT_GetDeviceColorType
	Function:	To distinguish the current camera photosensitive chip type, color or black and white
	Parameters:   
	Note:         
  --------------------------------------------------------------*/
VT_API
VT_GetDeviceColorType(IN HANDLE hCamera, OUT VT_COLOR_TYPE *ColorType);


VT_API
VT_SetColorAdjust(IN HANDLE hCamera, COLOR_ADJUST_TYPE type);


VT_API
VT_GetColorAdjust(IN HANDLE hCamera, COLOR_ADJUST_TYPE *type);


VT_API
VT_ResetLightAvgTable(IN HANDLE hCamera);


VT_API
VT_EnableLightAvg(IN HANDLE hCamera, BOOL bOn);


VT_API
VT_SetSourceFormat(IN HANDLE hCamera, DWORD dwType);
//===============
// Internal use
VT_API
VT_SetEC(IN HANDLE hCamera,BYTE *pData);
VT_API
VT_GetEC(IN HANDLE hCamera, BYTE *pData);

/*==============================================================
/*--------------------------------------------------------------*/
VT_API
VT_SetAELevel(IN HANDLE hCamera, IN VT_AE_LEVEL AELevel);

/*==============================================================
Function:		VT_SetOffCenter
Function:		Set the display centered in the cutting mode 
Parameters:    
Note:          
--------------------------------------------------------------*/
VT_API
VT_SetOffCenter(IN HANDLE hCamera, IN BOOL bCenter);


/*==============================================================
Function:		VT_SetOffCenter                     
Function:		Set the display in the cutting mode centered           
Parameters:                                           
Note:                                                 
--------------- --------------------------------------------------------*/
VT_API
VT_GetOffCenter(IN HANDLE hCamera, OUT BOOL *bCenter);

/*==============================================================
Function:	VT_ErasureStripe
Function:	Remove the fringe algorithm
Parameters:     
Note:           
--------------------------------------------------------------*/
VT_API
VT_ErasureStripe(IN HANDLE hCamera);

/*==============================================================
Function:	VT_ApplyErasure
Function:	Remove the fringe algorithm
Parameters:   
Note:         
--------------------------------------------------------------*/
VT_API
VT_ApplyErasure(IN HANDLE hCamera, IN BOOL bOn);

/*==============================================================
Function:		VT_SetGamma
Function:		Set the Gamma value
Parameters:    
Note:          
--------------------------------------------------------------*/
VT_API
VT_SetGamma(IN HANDLE hCamera, IN int _nGammaIdx);

/*==============================================================
Function:		VT_SetGamma
Function:		get the Gamma value
Parameters:    
Note:          
--------------------------------------------------------------*/
VT_API
VT_GetGamma(IN HANDLE hCamera, OUT int *_nGammaIdx);

/*==============================================================
Function:		VT_SetTriggerMode
Function:		¿ªÆô»ò¹Ø±Õ´¥·¢²É¼¯Ä£Ê½                                  
Parameters:      bEnable£¬TRUE£open trigger acquisition mode,FALSE, close the trigger acquisition mode
Note:          
--------------------------------------------------------------*/
VT_API
VT_SetTriggerMode(IN HANDLE hCamera, IN BOOL bEnable);


/*==============================================================
Function:		VT_SoftTrigger
Function:		Soft trigger collection 
Parameters:   

--------------------------------------------------------------*/
VT_API
VT_SoftTrigger(IN HANDLE hCamera);

/*==============================================================
Function:		VT_VideoPause
Function:		Pause the video
Parameters:   
Note:          
--------------------------------------------------------------*/
VT_API
VT_VideoPause(IN HANDLE hCamera, IN BOOL bPause);


/*==============================================================
Function:	VT_GetRgbFrameToBmpZoom
Function:	Will a frame image save for the wide high half the size of 24 (bit) BMP file
Parameters:  
Note:        
  --------------------------------------------------------------*/
VT_API
VT_GetRgbFrameToBmpZoom( IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo, OUT BYTE* pDest, LPCTSTR  strFileName, IN BOOL SaveData=FALSE ); 


/*==============================================================
	Function:	VT_GetRgbFrameToJpegZoom
	Function:	 Will a frame image save for the wide high half  the size of 24 (bit) JPEG file 
    Parameters:  Jpeg files file quality, value (1- 100)  
	Note:         
	  
  --------------------------------------------------------------*/
VT_API
VT_GetRgbFrameToJpegZoom( IN HANDLE hCamera, IN struct CapInfoStruct *pCapInfo, OUT BYTE* pDest, LPCTSTR  strFileName, int nQuality ); 


/*==============================================================
Function:		VT_SetHDR
Function:		Open camera high dynamic range effect (HDR).   
Parameters:     TURE for open, FALSE for closed 
Note:       2011-4-19 Currently only support VT-F500CF ¡¢VT-F900CF and VT36M.
--------------------------------------------------------------*/
VT_API
VT_SetHDR(IN HANDLE hCamera, IN BOOL bEnable);
/*================================================================
Function:		VT_EEProm_Set
Function:		Set save data, 40 bytes available
Parameters: 	pData is data
Note:       	Currently only support VT-F300CF
================================================================*/
VT_API
VT_EEProm_Set(IN HANDLE hCamera, IN BYTE *pData);

/*================================================================
Function:		VT_EEProm_Get
Function:	    Get save data, 40 bytes available
Parameters: 	pData is data                    
Note:       	Currently only support VT-F300CF 
================================================================*/
VT_API
VT_EEProm_Get(IN HANDLE hCamera, IN BYTE *pData);
/*================================================================
Function:		VT_EEProm_SetByte
Function:		Set a fixed position of fixed size data, 40 bytes available
Parameters: 	nIndex is the number of starting bytes 
      		    nCount is data size
		        Data is data
	            
Note:		   Currently only support VT-F300CF 
================================================================*/
VT_API
VT_EEProm_SetByte(IN HANDLE hCamera, IN int nIndex, IN int nCount, IN BYTE *pData);
/*================================================================
Function:		VT_EEProm_GetByte
Function:		
Parameters: 	nIndex is the number of starting bytes 
		        nCount is data size                    
		        Data is data                           
Note:        Currently only support VT-F300CF 
================================================================*/
VT_API
VT_EEProm_GetByte(IN HANDLE hCamera, IN int nIndex, IN int nCount, OUT BYTE *pData);

/*==============================================================
	Function:	VT_SetTriggerDelayTime
	Function:	  Set the camera trigger delay acquisition time
	Parameters:   hCamera:		Camera handle
			      nTime			Delay time, unit: ms
	Note:        Currently only support VT-F300CF 
  --------------------------------------------------------------*/
VT_API
VT_SetTriggerDelayTime(IN HANDLE hCamera, IN LONG nTime);

/*==============================================================
	Function:		VT_SetCustomIO
	Function:		Set the user defined IO.
	Parameters:     hCamera:		Camera handle.
			        data			 0 low level, 1  high level.
	Note:  At present, only for VT- F300CF, VT- F130MF, VT- F500CF effective. 
  --------------------------------------------------------------*/
VT_API
VT_SetCustomIO(IN HANDLE hCamera,IN BYTE data);

/*==============================================================
	Function:		VT_SetTriggerSource
	Function:		Set the camera that can trigger source
	Parameters:     hCamera:		Camera handle
			        data			Control , 0 close£¬1 open
	Note:           At present, only for VT- F300CF effective  
  --------------------------------------------------------------*/
VT_API
VT_SetTriggerSource(IN HANDLE hCamera,IN BYTE data);

/*==============================================================
	Function:		VT_SetTriggerSourceTime
	Function:		Set the camera trigger source time interval.
	Parameters:     hCamera:		Camera handle.
			        nTime			delaytime, unit: ms.
	Note:           At present, only for VT- F300CF effective 
  --------------------------------------------------------------*/
VT_API
VT_SetTriggerSourceTime(IN HANDLE hCamera, IN LONG nTime);


/*==============================================================
	Function:		VT_SetHBlank
	Function:		Set the camera line blank.
	Parameters:     hCamera: Camera handle.		

  --------------------------------------------------------------*/
VT_API
VT_SetHBlank(IN HANDLE hCamera, IN int nHBlank);

/*==============================================================
	Function:		VT_SetTriggerSourceTime
	Function:		Set the camera frame blank.
	Parameters:    hCamera:		Camera handle
			  		
	
	
  --------------------------------------------------------------*/
VT_API
VT_SetVBlank(IN HANDLE hCamera, IN int nVBlank);

/*==============================================================
	Function:		VT_AEParam
	Function:		Set up automatic exposure parameters, real - time automatic exposure
	Parameters:    hCamera:		 Camera handle
			       bLiving:		Whether open real- time automatic exposure
			       AELevel:		Exposure levels
			       pfAECallback:	 Exposure callback function
			       lpContext: The user context  
	 
  --------------------------------------------------------------*/
VT_API
VT_AEParam(IN HANDLE hCamera, IN BOOL bLiving, IN VT_AE_LEVEL AELevel,
		   IN DL_AUTOCALLBACK pfAECallback, IN LPVOID lpContext);

/*==============================================================
Function:	VT_GetNewCalibrationRawFrame
Function:	According to primitive calibration  image  generation  new calibration  image, 
            function  is set up according to the new gain and contrast, 
             by the original image genera tion new calibration image, 
            the new calibration image is still as the primary data format (8bit).
Parameters:   hCamera,[IN] Camera handle 
		      lHeight_In[IN] Data acquisition height £»
              lWidth_In[IN] Data acquisition width
		      iColourType_In,[IN]Reserves
		      iRawDateType_In£¬[IN]Reserves
              cTotalGain_In£¬[IN]Reserves
              iContrastValue_In£¬[IN] Reserves
		      pRawFrame_In£¬[IN] Need to convert image data pointer  
		      cNewTotalGain_In£¬[IN] After converting the brightness of the image data£¨1-63 £©  
		      iNewContrastValue_In£¬[IN]After converting image data contrast£¨1-32 £©
              pRawFrame_Out£¬[OUT] After converting the image of the pointer  
   
--------------------------------------------------------------*/
VT_API
VT_GetNewCalibrationRawFrame(IN HANDLE hCamera,
			IN ULONG lHeight_In,		
			IN ULONG lWidth_In,			
			IN int	 iColourType_In,	
			IN int  iRawDateType_In,	
			IN UCHAR cTotalGain_In,		
			IN int iContrastValue_In,	
			IN UCHAR *pRawFrame_In,		
			IN UCHAR cNewTotalGain_In,	
			IN int iNewContrastValue_In,
			OUT UCHAR *pRawFrame_Out	
			);

/*==============================================================
Function:	VT_RawFrameToRgb                         
Function:	The original data to create BMP image data. 

Parameters:   hCamera,[IN] Camera handle
		     lHeight_In£¬[IN] Data acquisition height
             lWidth_In£¬[IN]Data acquisition width
		     iColourType_In,[IN] Reserves 
		     iRawDateType_In£¬[IN] Reserves 
             pRawFrame_In£¬[IN] A pointer to the original data 
             pRgbData_Out£¬[OUT] The output of the image data area RGB pointer 
  
--------------------------------------------------------------*/
VT_API
VT_RawFrameToRgb(IN HANDLE hCamera,
			IN ULONG lHeight_In,		
			IN ULONG lWidth_In,			
			IN int	 iColourType_In,	
			IN int  iRawDateType_In,	
			IN UCHAR *pRawFrame_In,		
			OUT UCHAR *pRgbData_Out		
		);

/*==============================================================
Function:	VT_SetLight
Function:	Set the brightness values of output image.

Parameters:   hCamera,[IN]Camera handle
		      cTotalGain_In£¬[IN]the brightness values of output image
 
--------------------------------------------------------------*/
VT_API
VT_SetLight(IN HANDLE hCamera, IN UCHAR cTotalGain_In);


/*==============================================================
Function:	VT_GetLight
Function:	 Get the brightness values of output image
Parameters:   hCamera,[IN] Camera handle
		      cTotalGain_Out£¬[OUT] Return to the brightness values of current output image.
   
--------------------------------------------------------------*/
VT_API
VT_GetLight(IN HANDLE hCamera, OUT UCHAR * cTotalGain_Out);

/*==============================================================
Function:	VT_WriteDataToDevice
Function:	Save data to a camera device (data length 40 byte) 

Parameters:   hCamera,[IN]Camera handle 
		      bData_In£¬[IN] Writing to the device data pointer 
              iOffSet_In,[IN]Write down the position offset data (unit: BYTE)
              iNum_In,[IN] Write data length (unit: BYTE)  
 
--------------------------------------------------------------*/
VT_API
VT_WriteDataToDevice(IN HANDLE hCamera, IN BYTE *bData_In, 
			IN int iOffSet_In,
			IN int iNum_In);


/*===========================================================
Function:    VT_ReadDataFromDevice
Function:    Read the stored in the camera equipment in the data (40 byte)

Parameters:   hCamera,[IN] Camera handle  
		      bData_Out£¬[OUT]The output data pointer.
		      iOffSet_In,[IN]read first offset data (unit: BYTE)
              iNum_In,[IN] To read the data length (unit: BYTE) 
  
--------------------------------------------------------------*/
VT_API
VT_ReadDataFromDevice(IN HANDLE hCamera, IN BYTE *bData_In, 
			IN int iOffSet_In,
			IN int iNum_In);

/*==============================================================
Function:	VT_DeviceIoControl
Function:	
		    
Parameters:       hCamera,[IN]Camera handle 
--------------------------------------------------------------*/

VT_API
VT_DeviceIoControl(IN HANDLE hCamera, IN BYTE *bIoData_In);

/*==============================================================
Function:	VT_MCResolutionShow
Function:	Set up if application custom cut resolution show preview.¤ÀÀ
Parameters:   hCamera,		[IN]Camera handle
		      nEnable:		 TRUE, open ; FALSE, close..
		
Note:	 In the display resolution after cutting open the window, preview images to display 
		 the long image data original image data is specified in the location of the image.  
--------------------------------------------------------------*/
VT_API
VT_MCResolutionShow(IN HANDLE hCamera, IN BOOL nEnable);

/*==============================================================
Function:	VT_IsMCResolutionShow
Function:	Get whether open cuting resolution display.   
Parameters:	hCamera,		[IN]Camera handle.
		    nEnable:		 TRUE, open ; FALSE, close
		
--------------------------------------------------------------*/
VT_API
VT_IsMCResolutionShow(IN HANDLE hCamera, OUT BOOL *nEnable);

/*==============================================================
Function:	VT_SetMCResolution
Function:	Setting cutting resolution display related parameters.
Parameters:  				   
Note:		
--------------------------------------------------------------*/
VT_API
VT_SetMCResolution(IN HANDLE hCamera, IN int nOffX, IN int nOffY, IN int nWidth, IN int nHeight);

/*==============================================================
Function:	VT_GetMCResolution
Function:	get cutting resolution display related parameters.
Parameters:   

--------------------------------------------------------------*/
VT_API
VT_GetMCResolution(IN HANDLE hCamera, OUT int *nOffX, OUT int *nOffY, OUT int *nWidth, OUT int *nHeight);

/*==============================================================
Function:		VT_SetExpouseDelayTime
Function:		 Set the exposure time delay.
Parameters:     hCamera,		[IN]Camera handle.
		        nTime			[IN]the exposure time delay £¬range 0- 2047
		
Note:	Currently only support VT130CF£¬VT300CF£¬VT36M£¬VT500CF
--------------------------------------------------------------*/
VT_API
VT_SetExpouseDelayTime(IN HANDLE hCamera, IN int nTime);

/*==============================================================
Function:	 VT_SetExpouseUnit
Function:	 Exposure unit set£¬default for line number.
Parameters:  hCamera,		[IN] Camera handle..
             nUnit			[IN] Exposure unit, 0  for line number, 1 for us
		
Note:	Currently only support VT130CF£¬VT300CF£¬VT36M£¬VT500CF
--------------------------------------------------------------*/
VT_API
VT_SetExpouseUnit(IN HANDLE hCamera, IN int nUnit);

/*==============================================================
	Function:		VT_FrameToImage
	Function:		Incoming data in memory is saved as an image 
	Parameters:   			      			      			      			      			      			
--------------------------------------------------------------*/
VT_API
VT_FrameToImage(IN LPCTSTR strFileName, IN LPVOID pImageData, IN ULONG _ulWidth, IN ULONG _ulHeight, IN ULONG nColor, IN VTIMAGETYPR ImgType);

/*==============================================================
	Function:		VT_SetTriggerCheckSignalTime
	Function:		Set the camera to trigger the detection time
	Parameters:    
	Note:         Currently only support VT36M
--------------------------------------------------------------*/
VT_API
VT_SetTriggerCheckSignalTime(IN HANDLE hCamera, IN USHORT nTime);
/*==============================================================
	Function:	VT_SetSaturntion
	Function:	Set the saturation
	Parameters:   nSharp:      Saturation value:1-16£Ddefault:8
    Note:       
	            
--------------------------------------------------------------*/
VT_API
VT_SetSaturntion(IN HANDLE hCamera, IN int nSaturntion);
/*==============================================================
	Function:	VT_SetSharpness
	Function:	Set the sharpness.
	Parameters:   nSharp:     The Sharpness value, the range of 0-7, the default 1. 
    Note:       
	 
--------------------------------------------------------------*/
VT_API
VT_SetSharpness(IN HANDLE hCamera,int nSharp);
/*==============================================================
	Function:		VT_CapFastMode
	Function:		Set whether to open the fast acquisition mode
	Parameters:    bStart:     Switch setting		     
--------------------------------------------------------------*/
VT_API
VT_CapFastMode(IN HANDLE hCamera, IN BOOL bStart);
#endif