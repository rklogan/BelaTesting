#include <Bela.h>
#include <cmath>

//Sample rate and wavelength
float Fs;
float WL;

float TWO_PI = 2.0 * M_PI;

//phase for the current frame to be used in the generators
float phase;

/**Headers*/
float sinGenerator(float freq, float amplitude);
void incrementPhase(float freq);
float limitAudio(float output);



bool setup(BelaContext *context, void *userData)
{
	Fs = context->audioSampleRate;
	WL = 1.0 / Fs;		//calculate the wavelength
	phase = 0.0;
	return true;
}

void render(BelaContext *context, void *userData)
{
	//Outer loop to process all frames in the buffer; we should always see this in the render fn
	for(int frame = 0; frame < context->audioFrames; frame++){
		
		
		//Inner loop to process each of the channels; in most cases left/right; 
		for(int channel = 0; channel < context->audioOutChannels; channel++){
			
			float output = sinGenerator(440.0, 0.7);
			
			//output audio
			audioWrite(context, frame, channel, output);
		}
	}
}

void cleanup(BelaContext *context, void *userData)
{

}

// Generates a sin wave
float sinGenerator(float freq, float amplitude){
	incrementPhase(freq);
	return limitAudio( amplitude * sinf(phase) );
}
// Adjusts the phase to the next step
void incrementPhase(float freq){
	phase += TWO_PI * freq * WL;
	if(phase > 2.0 * TWO_PI) phase -= TWO_PI;
}
// Clips signals that are out of the range [-1.0, 1.0]
float limitAudio(float output){
	if(output > 1.0) return 1.0;
	if(output < -1.0) return -1.0;
	return output;
}