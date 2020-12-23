#include "Arduino.h"

#ifndef _PI
#define _PI 3.14
#endif

#ifndef lfo_h
#define lfo_h


// Midi trigger -> on/off
class lfo
{
	public:
		// constructor
		lfo(int dacSize);

		void setAmpl(int l_ampl);																							// use this function to set the amplitude from 0 to DACSIZE-1
		void setAmplOffset(int l_ampl_offset);																// use this function to set the amplitude offset from 0 to DACSIZE-1
		void setWaveForm(int l_waveForm);																			// use this function to set the waveForm (see "private variables")
		void setFreqSync(bool l_freq_sync);																		// use this function to set sync to free running (false) or BPM locked (true)
	  void setFreqMode0Free(float l_freq_mode0_free);												// set Freq in Hz of free-running mode
		void setFreqMode0Free(float l_freq_mode0_free, unsigned long l_t);		// set Freq in Hz of free-running mode, but by adding the current time the freq will be changed without phase jump -> just use micros() as second parameter
	  void setFreqMode1Bpm(float l_freq_mode1_bpm);													// set BPM of track for sync mode
	  void setFreqMode1Rate(int l_freq_mode1_rate);													// set rate of sync mode -> see _freq_array -> if for instance l_freq_mode1_rate is set to 15 this means the that _freq_array[15] = 1, so 1/1 will be the sync to the BPM.
		void setFreqMode1Phase(float l_freq_mode1_phase);											// set phase offset for sync mode (free mode does not have phase offset as it is free running, though you can use sync(micros) in free mode to set phase to 0)
		void sync(unsigned long l_t);																					// function to sync LFO to external trigger -> use sync(micros())

		int getWaveForm();																										// simple get functions as I made the variables private
		int getAmpl();
		int getAmplOffset();
		float getFreqMode0Free();
		int getWave(unsigned long l_t);																				// main function that gives the waveformshape at time l_t -> use with getWave(micros())

	private:
		int 						_dacSize;														// DAC size
		int 						_waveForm = 0;											// mode 0 -> off, mode 1 -> saw, mode 2 -> triangle, mode 3 -> sin, mode 4 -> square [0,4]
		int 						_ampl = 0;													// amplitude, scales from 0 to _dacSize
		int 						_ampl_offset = 0;										// amplitude offset, scales from 0 to _dacSize
		bool            _freq_sync = false;        	      	// sync mode: false -> free, true -> synced to track
		float           _freq_mode0_free = 30;           		// frequency in Hz
		float           _freq_mode1_bpm = 120;           		// BPM of track
		int             _freq_mode1_rate = 15;           		// Rate to link to BPM of track
		float           _freq_mode1_phase = 0;           		// phase of LFO -> scales from 0 to 1

		// lookup table for freq MODE 1:
		float _freq_array[23] = {64, 48, 32, 24, 16, 12, 8, 6, 5.3333, 4, 3.2, 3, 2.667, 2, 1.333, 1, 0.667, 0.5, 0.333, 0.25, 0.167, 0.125, 0.0625};

		// time reference needed for sync waves:
		unsigned long _t0 = 0;

		// internal value to keep phase with freq changes in free mode:
		float           _freq_mode0_phase = 0;           		// phase of LFO in free-running mode -> this parameter is used internally to enable a seamless freq change with this function: setFreqMode0Free(float l_freq_mode0_free, unsigned long l_t);
};

#endif
