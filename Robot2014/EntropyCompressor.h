
#ifndef __ENTROPY_COMPRESSOR__
#define __ENTROPY_COMPRESSOR__

#include "WPILib.h"
#include "EntropySubsystemTemplate.h"
	

class EntropyCompressor : public EntropySubsystemTemplate 
{
	
private:
	// The robot shooter left, right, up, down control
	Compressor * airCompressor;
		
public:
	EntropyCompressor( );
	
	bool Initialize ();
	
	void Cleanup ();
	
	char * GetFeedback();

	};
#endif

