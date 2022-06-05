#pragma once
#include "TSUtilities.h"
#include "Oscillator.h"
#include "Speaker.h"
#include "Notes.h";

int main(int argc, char* argv[]) {

	std::vector<std::unique_ptr<Box>> pBoxes;

	pBoxes.emplace_back(new Speaker());
	auto pSpeaker = dynamic_cast<Speaker*>(pBoxes.back().get());
	pSpeaker->setVol(0.5f);

	std::vector<Note> notes = { Note::C4, Note::EF4, Note::G4, /*Note::F4, Note::A4*/};
	for (auto& note : notes) {
		pBoxes.emplace_back(new Oscillator());
		auto pOsc = dynamic_cast<Oscillator*>(pBoxes.back().get());
		pOsc->setAmp(0.2f);
		pOsc->setFreq(getFreq(note));
		pOsc->setType(Oscillator::OscType::Sine);

		pBoxes.emplace_back(new Oscillator());
		auto pFreqMod = dynamic_cast<Oscillator*>(pBoxes.back().get());
		pFreqMod->setAmp(10.0f);
		pFreqMod->setFreq(0.1f);
		pFreqMod->setType(Oscillator::OscType::Sine);
		pOsc->setFreqMod(pFreqMod->getOutput());

		pBoxes.emplace_back(new Oscillator());
		auto pAmpMod = dynamic_cast<Oscillator*>(pBoxes.back().get());
		pAmpMod->setAmp(0.8f);
		pAmpMod->setFreq(2.0f);
		pAmpMod->setType(Oscillator::OscType::Sine);
		pOsc->setAmpMod(pAmpMod->getOutput());
		
		pSpeaker->addInput(pOsc->getOutput());
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