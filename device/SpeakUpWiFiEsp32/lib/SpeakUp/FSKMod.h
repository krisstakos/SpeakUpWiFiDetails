// FSKDemod
// Rob Dobson 2018
// Binary FSK modulator

#pragma once

#include <vector>
#include "RingBufferPosn.h"

class FSKMod
{
private:
	static constexpr int _sinTable[] =
	{
		0, 201, 402, 603, 804, 1005, 1206, 1407,
		1608, 1809, 2009, 2210, 2410, 2611, 2811, 3012,
		3212, 3412, 3612, 3811, 4011, 4210, 4410, 4609,
		4808, 5007, 5205, 5404, 5602, 5800, 5998, 6195,
		6393, 6590, 6786, 6983, 7179, 7375, 7571, 7767,
		7962, 8157, 8351, 8545, 8739, 8933, 9126, 9319,
		9512, 9704, 9896, 10087, 10278, 10469, 10659, 10849,
		11039, 11228, 11417, 11605, 11793, 11980, 12167, 12353,
		12539, 12725, 12910, 13094, 13279, 13462, 13645, 13828,
		14010, 14191, 14372, 14553, 14732, 14912, 15090, 15269,
		15446, 15623, 15800, 15976, 16151, 16325, 16499, 16673,
		16846, 17018, 17189, 17360, 17530, 17700, 17869, 18037,
		18204, 18371, 18537, 18703, 18868, 19032, 19195, 19357,
		19519, 19680, 19841, 20000, 20159, 20317, 20475, 20631,
		20787, 20942, 21096, 21250, 21403, 21554, 21705, 21856,
		22005, 22154, 22301, 22448, 22594, 22739, 22884, 23027,
		23170, 23311, 23452, 23592, 23731, 23870, 24007, 24143,
		24279, 24413, 24547, 24680, 24811, 24942, 25072, 25201,
		25329, 25456, 25582, 25708, 25832, 25955, 26077, 26198,
		26319, 26438, 26556, 26674, 26790, 26905, 27019, 27133,
		27245, 27356, 27466, 27575, 27683, 27790, 27896, 28001,
		28105, 28208, 28310, 28411, 28510, 28609, 28706, 28803,
		28898, 28992, 29085, 29177, 29268, 29358, 29447, 29534,
		29621, 29706, 29791, 29874, 29956, 30037, 30117, 30195,
		30273, 30349, 30424, 30498, 30571, 30643, 30714, 30783,
		30852, 30919, 30985, 31050, 31113, 31176, 31237, 31297,
		31356, 31414, 31470, 31526, 31580, 31633, 31685, 31736,
		31785, 31833, 31880, 31926, 31971, 32014, 32057, 32098,
		32137, 32176, 32213, 32250, 32285, 32318, 32351, 32382,
		32412, 32441, 32469, 32495, 32521, 32545, 32567, 32589,
		32609, 32628, 32646, 32663, 32678, 32692, 32705, 32717,
		32728, 32737, 32745, 32752, 32757, 32761, 32765, 32766,
	};
	static constexpr int _sinTableLen = sizeof(_sinTable) / sizeof(_sinTable[0]);

private:
	// Sample rate, bit rate and symbols
	int _sampleRate;
	int _symbolRate;
	int _numSymbols;
	int _preambleSymbols;
	int _postambleSymbols;
	std::vector<int> _symbolFreqs;
	bool _manchesterCodec;

	// Generator state vars
	bool _generatorBusy;
	int _curSymbolVal;
	int _generatorCount;
	int _samplesToNextChange;
	int _manchesterPhase;
	int _curFrequency;
	int _generatorInc;

	// Buffer containing symbols to send
	RingBufferPosn _txSymbolFifoPos;
	std::vector<uint8_t> _txSymbolFifoBuf;

public:
	FSKMod(int txBitFifoLen) : _txSymbolFifoPos(txBitFifoLen)
	{
		_txSymbolFifoBuf.resize(txBitFifoLen);
		_sampleRate = 8000;
		_symbolRate = 200;
		_preambleSymbols = 20;
		_postambleSymbols = 5;
		_manchesterCodec = true;
		_manchesterPhase = 0;
		_generatorBusy = false;
		_curSymbolVal = 0;
		_generatorCount = 0;
		_samplesToNextChange = 0;
		_curFrequency = 0;
		_generatorInc = 0;
		_numSymbols = 2;
		_symbolFreqs.resize(_numSymbols);
		_symbolFreqs[0] = 1000;
		_symbolFreqs[1] = 2000;
	}

	// Setup modulator
	void setup(int sampleRate, int symbolRate, int symbolFreqHigh, int symbolFreqLow,
				bool manchesterCodec);

	// Clear symbol buffer
	void clear();

	// Set preamble & postamble length in symbols
	void setPreamble(int lenInSymbols);
	void setPostamble(int lenInSymbols);

	// Add a preamble / postamble to the symbol buffer
	void addPreamble();
	void addPostamble();

	// Add a single symbol to the buffer
	bool addSymbol(int symbol);

	// Get a sample from the modulated output
	bool getSample(int& sampleValue);

private:
	// Helper functions
	void startSymbol(int symbolValue);
	void setFreq(int freqInHz);
};
