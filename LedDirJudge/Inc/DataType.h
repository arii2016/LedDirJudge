
// datatype.h

#ifndef DATATYPE_H
#define DATATYPE_H

/*===========================================================*\
	AE/AWB callback function	dw1         Returns the parameters after AE/AWB, 
					            dw2			Returns the parameters after AE/AWB, 
					            lpContext	Transfer in setting the frame callback function when the context

	2011-04-12 The callback callback format used as equipment removed and inserted into the message
	dw1 Said device is inserted or removed. 0£rremoved;1£¬inserted
	dw2 Said device handle, forced conversion
\*=========================================================//*/
typedef VOID (WINAPI *DL_AUTOCALLBACK )( DWORD dw1, DWORD dw2, LPVOID lpContext );

/*===========================================================*\
	Frame the callback function:	lpParam :The pointer to the frame data, 
				lpPoint  Retain, 
\*=========================================================//*/
typedef VOID (WINAPI *DL_FRAMECALLBACK)( LPVOID lpParam1, LPVOID lpPoint, LPVOID lpContext );

/*-------------------------------------------------------------
	The return value definition
  *===========================================================*/
#define		ResSuccess					0x0000		// Return success           
#define		ResNullHandleErr			0x0001		// Invalid handle           
#define		ResNullPointerErr			0x0002		// Pointer is null          
#define		ResFileOpenErr				0x0003		// File / open failed       
#define		ResNoDeviceErr				0x0004		// No device available      
#define		ResInvalidParameterErr		0x0005		// Memory allocation problem
#define		ResOutOfMemoryErr			0x0006		// Do not open the preview  
#define		ResNoPreviewRunningErr		0x0007		// The preview didn't open  
#define		ResOSVersionErr				0x0008
#define		ResUsbNotAvailableErr		0x0009
#define		ResNotSupportedErr			0x000a
#define		ResNoSerialString			0x000b
#define		ResVerificationErr			0x000c
#define		ResTimeoutErr	            0x000d		
#define		ResScaleModeErr				0x000f
#define		ResUnknownErr				0x00ff

#define		ResDisplayWndExist			0x0011		// Should close the preview window 
#define		ResAllocated				0x0012		// Memory has been allocated       
#define		ResAllocateFail				0x0013		// Memory allocation failure       
#define		ResReadError				0x0014      // USB failed to read              
#define		ResWriteError				0x0015		// USB command failed              
#define		ResUsbOpen					0x0016      // The USB port is already open    
#define     ResCreateStreamErr			0x0017		// Create a avi flow failure       
#define     ResSetStreamFormatErr		0x0018		// Set the AVI stream format failed


typedef struct _tagDLVIDEORECT
{
	int     Left;		// Relative to the horizontal displacement of the parent window
	int     Top;		// With respect to the vertical migration of the parent window 
	int     Width;		// The video window width                                      
	int     Height;		// The video window height                                     
}DLVIDEORECT, *PDLVIDEORECT;

/*-------------------------------------------------------------
	The camera parameters structure
  *===========================================================*/
struct CapInfoStruct 
{
	UCHAR	*Buffer;		//User distribution, for the return of 8bit raw data             
	ULONG	Height;			//The sampling height                                            
	ULONG	Width;			//The sampling width                                             
	ULONG	OffsetX;		//Horizontal offset, disable the CCD camera                      
	ULONG	OffsetY;		//Vertical offset, disable CCD camera                            
	ULONG	Exposure;		//The exposure value: 1-500MS                                    
	UCHAR	Gain[3];		//Gain[0] :overall gain: 1-63; Gain[1], Gain[2] :reserves        
	UCHAR	Control;		//Control bit                                                    
	UCHAR	InternalUse;	//Users don't byte operation                                     
	UCHAR	ColorOff[3];	//The user from the outside do not change the values of the array
	UCHAR	Reserved[4];	//Reserved bit                                                   
};
//CapInfoStruct m_tCamerParam;
/*-----------------------------------------------------------
	Control 
	BIT7       BIT6      BIT5     BIT4     BIT3     BIT2     BIT1     BIT0
													HDR    	 Trigger  cutting / resizing
													0		 0			0 cutting
													1 open	 1	Trigger collection  1 
  ===========================================================*/


/*-------------------------------------------------------------
	color mode
  *===========================================================*/
enum	COLOR_MODE {
					COLOR_RGB24,
					COLOR_RGB24_CLEAR,
					COLOR_BW24,
					COLOR_GRAY,
					COLOR_RAWDATA,
					COLOR_Microscope	//color correction model (microscope mode)
				};


/*-------------------------------------------------------------
	ture mode
  *===========================================================*/
enum	FLIP_MODE {	
					FLIP_NATURAL,		// The normal display
					FLIP_LEFTRIGHT,	    // Flips around      
					FLIP_UPDOWN,		
					FLIP_ROTATE180,		// Rotating 180      
					FLIP_ROTATE90,
					FLIP_ROTATE270
				};

enum	VTIMAGETYPR {  
						BMP,		// Save the image format
					    JPEG,	    // 
					};

enum	VTCAMERA{	VTUNKOWN,		//The type of camera
					VT130,			
					VT300,
					VT130CF,
					VT200CF,
					VT300CF,
					VT500CF,
					VT900CF,
					VT500P,			//
					VT130C_1_3,
					VT36M,
					VT300C_FPGA,
					VT130C_FPGA,
					VT130M_FPGA,
					VT80C_SP16,
					VT130SC_YUV,
					VT120C_FPGA,
					VT300_LED,
					VTTEST,
					VTF1400CF,
					VTF1000CF
				};

enum	VT_COLOR_TYPE{
					VT_COLORS,
					VT_GRAY
				};


enum COLOR_ADJUST_TYPE {					
					COLOR_ADJUST_NO,	
					COLOR_ADJUST_COLD,	
					COLOR_ADJUST_WARM	
				};

//Setting automatic exposure luminance 
enum	VT_AE_LEVEL
{
		VT_AE_LEVEL_HIGH,	// brighter
		VT_AE_LEVEL_NORMAL,	// normal  
		VT_AE_LEVEL_LOW		// darker  
};

#endif