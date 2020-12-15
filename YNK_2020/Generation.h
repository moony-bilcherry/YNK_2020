#pragma once

#include "Parm.h"
#include "LT.h"
#include "IT.h"
#include "LexAnalysis.h"

namespace Generator
{
	struct Gener
	{
		LT::LexTable lexT;
		IT::IdTable idT;
		std::ofstream out;

		Gener(LT::LexTable lexT, IT::IdTable IdT, wchar_t out[]);

		void Head();
		void Constants();
		void Data();
		void Code();
	};
};
