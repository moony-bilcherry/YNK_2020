#include "stdafx.h"
#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"
#include "LT.h"
#include "LexAnalysis.h"
#include "Polish.h"
#include "MFST.h"
#include "Greibach.h"
#include "SemAnalysis.h"
#include "Generation.h"

#define EXP1 16

Log::LOG logfile;

int wmain(int argc, wchar_t* argv[])
{
	setlocale(LC_ALL, "ru");
	
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		logfile = Log::getlog(parm.log);
		In::IN in = In::getin(parm);

		Lexer::LEX lex = Lexer::Analyze(in, logfile);
		Semantics::Analyze(lex, logfile);

		Log::WriteLog(logfile);
		Log::WriteParm(logfile, parm);
		Log::WriteIn(logfile, in);
		Log::WriteIDTableLog(lex.idtable, logfile);
		Log::WriteLexTableLog(lex.lextable, logfile);
		Log::WriteTokensLog(lex.lextable, logfile);

		MFST_TRACE_START(logfile);
		unsigned int start_time = clock();
		MFST::Mfst mfst(lex.lextable, GRB::getGreibach());
		mfst.start(logfile);
		unsigned int end_time = clock();
		unsigned int search_time = end_time - start_time;
		mfst.savededucation();
		mfst.printrules(logfile);
		
		*logfile.stream << "\tРазбор выполнен за " << ((float)search_time) / CLOCKS_PER_SEC << " секунд." << std::endl;
		*logfile.stream << std::setfill('-') << std::setw(90) << '-' << std::endl;

		std::cout << "\tРазбор выполнен за " << ((float)search_time) / CLOCKS_PER_SEC << " секунд." << std::endl;
		std::cout << std::setfill('-') << std::setw(90) << '-' << std::endl;

		if (parm.idtable)
			IT::WriteTable(lex.idtable);
		if (parm.lextable)
			LT::WriteLexTable(lex.lextable);

		Poliz::StartPoliz(lex);

		std::cout << "\t\tПОЛЬСКАЯ НОТАЦИЯ ВЫПОЛНЕНА" << std::endl;
		std::cout << std::setfill('-') << std::setw(90) << '-' << std::endl;
		if (parm.tokens)
			LT::WriteTokens(lex.lextable);
		
		*logfile.stream << "\t\tПОЛЬСКАЯ НОТАЦИЯ ВЫПОЛНЕНА" << std::endl;
		*logfile.stream << std::setfill('-') << std::setw(90) << '-' << std::endl;
		Log::WriteTokensLog(lex.lextable, logfile);

		Generator::Gener Gener(lex.lextable, lex.idtable, parm.out);

		IT::Delete(lex.idtable);
		LT::Delete(lex.lextable);
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(logfile, e);
	}

	system("pause");
	return 0;
}