#pragma once
#include "TSUtilities.h"
#include "Oscillator.h"
#include "OscillatorComplex.h"
#include "Speaker.h"
#include "Notes.h";

int main(int argc, char* argv[]) {

	std::vector<std::unique_ptr<Box>> pBoxes;

	pBoxes.emplace_back(new Speaker());
	auto pSpeaker = dynamic_cast<Speaker*>(pBoxes.back().get());
	pSpeaker->setVol(0.5f);

	std::vector<Note> notes = { Note::C4, Note::EF4, Note::G4, Note::BF4 };
	for (auto& note : notes) {
		pBoxes.emplace_back(new OscillatorComplex());
		auto pOC = dynamic_cast<OscillatorComplex*>(pBoxes.back().get());

		auto pOsc = pOC->getOsc();
		pOsc->setFreq(getFreq(note));
		pOsc->setAmp(0.5f);
		pOsc->setType(OscType::Sine);

		auto pFreqOsc = pOsc->setFreqMod();
		pFreqOsc->setFreq(60.0f);
		pFreqOsc->setAmp(20.0f);
		pFreqOsc->setType(OscType::Sine);

		auto pAmpOsc = pOsc->setAmpMod();
		pAmpOsc->setFreq(2.0f);
		pAmpOsc->setAmp(0.8f);
		pAmpOsc->setType(OscType::RevSaw);

		pSpeaker->addInput(pOC->getOutput());
	}

	log("Starting boxes...");
	for (auto& pBox : pBoxes) {
		pBox->start();
	}

	char input;
	std::cin >> input;

	log("Stopping boxes.");
	for (auto& pBox : pBoxes) {
		pBox->stop();
	}

	return 0;
}