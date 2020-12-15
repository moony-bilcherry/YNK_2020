#pragma once
#include "stdafx.h"
#include "LexAnalysis.h"
#include "Error.h"

namespace Poliz
{
	void StartPoliz(Lexer::LEX& lex);
	bool Poliz(int i, Lexer::LEX& lex);
}
