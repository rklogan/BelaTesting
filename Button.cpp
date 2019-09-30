#include <Bela.h>
#include <cmath>
#include <stdlib.h>

float Fs;		//sample rate
float WL;		//time between samples = 1/Fs
float TWO_PI = 2.0 * M_PI;
float frequency = 440.0;	//frequency of tone to be generated
float phase;				//current phase of the tone
float amplitude = 0.8;

//pins to be used
int DIGITAL_IN = 1;
int DIGITAL_OUT = 0;


bool setup(BelaContext *context, void *userData)
{
	//setup basic audio parameters
	Fs = context->audioSampleRate;
	WL = 1.0 / Fs;		//this is to save computation in render
	phase = 0.0;
	
	//tell Bela which pins are inputs or outputs
	pinMode(context, 0, DIGITAL_IN, INPUT);
	pinMode(context, 0, DIGITAL_OUT, OUTPUT);
	return true;
}

void render(BelaContext *context, void *userData)
{
	int button_status;
	for(int frame = 0; frame < context->digitalFrames; frame++){
		
		//read if the button is depressed...
		button_status = digitalRead(context, 0, DIGITAL_IN);
		
		//reverse the button's status
		if(button_status != 0) button_status = 0;
		else button_status = 1;
		
		//turn on the light
		digitalWriteOnce(context, frame, DIGITAL_OUT, button_status);
	}
	
	for(int frame = 0; frame < context->audioFrames; frame++){
		//adjust phase
		phase += TWO_PI * frequency * WL;
		if(phase > TWO_PI) phase -= TWO_PI;			//probably don't need this line

		//generate the sin
		float output_audio = amplitude * sinf(phase);
		
		//clip the signal if we're out of bounds. only relevant if amplitude > 1
		if(output_audio > 1.0) output_audio = 1.0;
		if(output_audio < -1.0) output_audio = -1.0;
		
		//output the audio
		for(int channel = 0; channel < context->audioOutChannels; channel++){
			//when the button is pressed output audio, otherwise output silence
			if(button_status) audioWrite(context, frame, channel, output_audio);
			//You get a high pitched ringing if the following line is ommitted, not sure why, but something is wrong.
			else audioWrite(context, frame, channel, 0.0);	
		}
	}
}

void cleanup(BelaContext *context, void *userData)
{

}