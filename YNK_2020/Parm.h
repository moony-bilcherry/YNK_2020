#pragma once
#include "stdafx.h"
#define PARM_IN		L"-in:"
#define	PARM_OUT	L"-out:"
#define	PARM_LOG	L"-log:"

#define PARM_LEX L"-lex"
#define PARM_ID L"-id"
#define PARM_TOKENS L"-tokens"
#define PARM_RULES L"-rules"

#define PARM_MAX_SIZE 300																	// максимальная длина строки параметра
#define PARM_OUT_DEFAULT_EXT L".out.asm"														// расширение файла объектного кода по умолчанию
#define PARM_LOG_DEFAULT_EXT L".log"														// расширение файла протокола по умолчанию

namespace Parm
{
	struct PARM
	{
		wchar_t in[PARM_MAX_SIZE];						// -in: - имя файла с исходным кодом
		wchar_t out[PARM_MAX_SIZE];						// -out: - имя файла с объектным кодом
		wchar_t log[PARM_MAX_SIZE];						// -log: - имя файла протокола

		bool lextable;									// -lex - ключ для вывода ТЛ
		bool idtable;									// -id - ключ для вывода ТИ
		bool tokens;									// -tokens ключ для вывода кода в виде токенов
		bool rules;										// -rules ключ для вывода правил
	};
	PARM getparm(int argc, wchar_t* argv[]);			// сформировать структуру PARM на основе параметров, переданных main
}