#include "Arduino.h"
#include "lfo.h"

lfo::lfo(int dacSize)
{
	_dacSize = dacSize;
	_ampl = dacSize - 1;
	_ampl_offset = dacSize/2;
}

void lfo::setWaveForm(int l_waveForm)
{
	if(l_waveForm < 0)
		l_waveForm = 0;
	if(l_waveForm > 4)
		l_waveForm = 4;
	_waveForm = l_waveForm;
}

void lfo::setAmpl(int l_ampl)
{
	if(l_ampl < 0)
		l_ampl = 0;
	if(l_ampl >= _dacSize)
		l_ampl = _dacSize - 1;
	_ampl = l_ampl;
}

void lfo::setAmplOffset(int l_ampl_offset)
{
	if(l_ampl_offset < 0)
		l_ampl_offset = 0;
	if(l_ampl_offset >= _dacSize)
		l_ampl_offset = _dacSize - 1;
	_ampl_offset = l_ampl_offset;
}

void lfo::setMode(bool l_mode)
{
	_mode = l_mode;
}

void lfo::setMode0Freq(float l_mode0_freq)
{
	if(l_mode0_freq < 0)
		l_mode0_freq = 0;
	_mode0_freq = l_mode0_freq;
}

void lfo::setMode0Freq(float l_mode0_freq, unsigned long l_t)
{
	if(l_mode0_freq < 0)
		l_mode0_freq = 0;

	// synchronize the phase of the old freq with the new freq
	float l_phase_old = (double)(l_t - _t0) * _mode0_freq / 1000000 + _mode0_phase;
	float l_phase_new = (double)(l_t - _t0) * l_mode0_freq / 1000000;
	l_phase_old = l_phase_old - floor(l_phase_old);
	l_phase_new = l_phase_new - floor(l_phase_new);
	_mode0_phase = l_phase_old - l_phase_new;

	_mode0_freq = l_mode0_freq;
}

void lfo::setMode1Bpm(float l_mode1_bpm)
{
	if(l_mode1_bpm < 0)
		l_mode1_bpm = 0;
	_mode1_bpm = l_mode1_bpm;
}

void lfo::setMode1Rate(float l_mode1_rate)
{
	if(l_mode1_rate < 0)
		l_mode1_rate = 0;
	_mode1_rate = l_mode1_rate;
}

void lfo::setMode1Phase(float l_mode1_phase)
{
	_mode1_phase = l_mode1_phase;
}

void lfo::sync(unsigned long l_t)
{
	_t0 = l_t;
	_mode0_phase = 0;
}

int lfo::getWaveForm()
{
	return _waveForm;
}

int lfo::getAmpl()
{
	return _ampl;
}

int lfo::getAmplOffset()
{
	return _ampl_offset;
}

float lfo::getMode0Freq()
{
	return _mode0_freq;
}

int lfo::getWave(unsigned long l_t)
{
	int result = 0;
	int l_ampl = _ampl;
	double l_phase = 0;

	if(_mode == false)         			// LFO free running
		l_phase = (double)(l_t - _t0) * _mode0_freq / 1000000 + _mode0_phase;
	else    												// LFO synced
		l_phase = (double)(l_t - _t0) * _mode1_rate * _mode1_bpm / 60000000 + _mode1_phase;

	// Compute correct _ampl_offsetoffset (wave not to exceed 0 to dacSize)
	int l_ampl_offset = 0;
	int l_ampl_half = (int) l_ampl / 2;
	if(_ampl_offset < _dacSize / 2)
		l_ampl_offset = ( _ampl_offset > l_ampl_half ) ? _ampl_offset : l_ampl_half; 												// l_ampl_offset must be large enough not to go below 0
	else
		l_ampl_offset = ( _dacSize - _ampl_offset > l_ampl_half ) ? _ampl_offset : _dacSize - l_ampl_half - 1;

	switch(_waveForm)
	{
		case 0: // Off
			result = _ampl_offset;
			return result;
			break;
		case 1: // Saw
			return result = l_ampl * (_dacSize - 1 - ((int)(l_phase * _dacSize)) % _dacSize) / (_dacSize - 1) + l_ampl_offset - l_ampl_half;
			break;
		case 2: // Triangle
			if(((int)(2 * l_phase))%2) // up flank
				return result = abs(l_ampl * (((int)(l_phase * _dacSize * 2)) % _dacSize) / (_dacSize - 1) + l_ampl_offset - l_ampl_half - 1);
			else // down flank
				return result = abs(l_ampl * (_dacSize - ((int)(l_phase * _dacSize * 2)) % _dacSize - 1) / (_dacSize - 1) + l_ampl_offset - l_ampl_half);
			break;
		case 3: // Sin
			return result = (int) l_ampl_half * cos(2 * _PI * l_phase) + l_ampl_offset;
			break;
		case 4: // Square
			if(((int)(2 * l_phase))%2)
				return result = l_ampl_offset - l_ampl_half;
			else
				return result = l_ampl_offset + l_ampl_half;
			break;
	}
	return result;
}
