
#include "EntropyCompressor.h"
#include "IODefinitions.h"


EntropyCompressor::EntropyCompressor() {
	
	int compressorRelayChannel = IODefinitions::COMPRESSOR_RELAY_CHANNEL;
	int pressureSwitchChannel = IODefinitions::PRESSURE_SWITCH_CHANNEL;
	int compressorRelayModule = IODefinitions::COMPRESSOR_RELAY_MODULE;
	int pressureSwitchModule = IODefinitions::PRESSURE_SWITCH_MODULE;
		
	airCompressor = new Compressor (pressureSwitchModule, pressureSwitchChannel, compressorRelayModule, compressorRelayChannel);
}

bool EntropyCompressor::Initialize()
{
	
		
	airCompressor->Start();
	return true;
}

void EntropyCompressor::Cleanup()
{ 
	
}

char * EntropyCompressor::GetFeedback(){ return NULL;}


