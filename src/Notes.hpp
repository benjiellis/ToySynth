#pragma once

enum class Note {
	C2, DF2, D2, EF2, E2, F2, GF2, G2, AF2, A2, BF2, B2, 
	C3, DF3, D3, EF3, E3, F3, GF3, G3, AF3, A3, BF3, B3,
	C4, DF4, D4, EF4, E4, F4, GF4, G4, AF4, A4, BF4, B4
};

float getFreq(Note note) {
	float f = 0.0f;
	switch (note) {
	case Note::C2:
		f = 65.4064f;
		break;
	case Note::DF2:
		f = 69.2957f;
		break;
	case Note::D2:
		f = 73.4162f;
		break;
	case Note::EF2:
		f = 77.7817f;
		break;
	case Note::E2:
		f = 82.4069f;
		break;
	case Note::F2:
		f = 87.3071f;
		break;
	case Note::GF2:
		f = 92.4968f;
		break;
	case Note::G2:
		f = 97.9989f;
		break;
	case Note::AF2:
		f = 103.826f;
		break;
	case Note::A2:
		f = 110.0f;
		break;
	case Note::BF2:
		f = 116.541f;
		break;
	case Note::B2:
		f = 123.471f;
		break;
	case Note::C3:
		f = 130.831f;
		break;
	case Note::DF3:
		f = 138.591f;
		break;
	case Note::D3:
		f = 146.832f;
		break;
	case Note::EF3:
		f = 155.563f;
		break;
	case Note::E3:
		f = 164.814f;
		break;
	case Note::F3:
		f = 174.614f;
		break;
	case Note::GF3:
		f = 184.997f;
		break;
	case Note::G3:
		f = 195.998f;
		break;
	case Note::AF3:
		f = 207.652f;
		break;
	case Note::A3:
		f = 220.0f;
		break;
	case Note::BF3:
		f = 233.08f;
		break;
	case Note::B3:
		f = 246.942f;
		break;
	case Note::C4:
		f = 261.626f;
		break;
	case Note::DF4:
		f = 277.183f;
		break;
	case Note::D4:
		f = 293.665f;
		break;
	case Note::EF4:
		f = 311.217f;
		break;
	case Note::E4:
		f = 329.628f;
		break;
	case Note::F4:
		f = 349.228f;
		break;
	case Note::GF4:
		f = 369.994f;
		break;
	case Note::G4:
		f = 391.995f;
		break;
	case Note::AF4:
		f = 415.305f;
		break;
	case Note::A4:
		f = 440.0f;
		break;
	case Note::BF4:
		f = 466.164f;
		break;
	case Note::B4:
		f = 493.883f;
		break;
	}
	return f;
}