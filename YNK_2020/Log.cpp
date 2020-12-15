#include "stdafx.h"	
#include "Log.h"	
#include <time.h>

#pragma warning(disable:4996)
using namespace std;

namespace Log
{
	LOG getlog(wchar_t logfile[])
	{
		LOG log;
		log.stream = new ofstream;
		log.stream->open(logfile);
		if (log.stream->fail())
			throw ERROR_THROW(112);
		wcscpy_s(log.logfile, logfile);
		return log;
	}
	void WriteLine(LOG log, const char* c, ...)
	{
		const char** ptr = &c;
		int i = 0;
		while (ptr[i] != "")
			*log.stream << ptr[i++];
		*log.stream << endl;
	}
	void WriteLine(LOG log, const wchar_t* c, ...)
	{
		const wchar_t** ptr = &c;
		char temp[100];
		int i = 0;
		while (ptr[i] != L"")
		{
			wcstombs(temp, ptr[i++], sizeof(temp));
			*log.stream << temp;
		}
		*log.stream << endl;
	}
	void WriteLog(LOG log)
	{
		tm* timeinf;
		time_t timet;
		time(&timet);
		timeinf = localtime(&timet);
		char temp[100];
		strftime(temp, sizeof(temp), "%d.%m.%y %T", timeinf);

		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;
		*log.stream << "\t\t\t\tÏÐÎÒÎÊÎË [ " << temp << " ] " << std::endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;

		std::cout << std::setfill('-') << std::setw(90) << '-' << endl;
		std::cout << "\t\t\t\tÏÐÎÒÎÊÎË [ " << temp << " ] " << std::endl;
		std::cout << std::setfill('-') << std::setw(90) << '-' << endl;
	}
	void WriteIn(LOG log, In::IN in)
	{
		*log.stream << "\t\tÈÑÕÎÄÍÛÅ ÄÀÍÍÛÅ" << std::endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;
		*log.stream << "Êîëè÷åñòâî ñèìâîëîâ:\t" << in.size << std::endl;
		*log.stream << "Ïðîèãíîðèðîâàíî:\t" << in.ignor << std::endl;
		*log.stream << "Êîëè÷åñòâî ñòðîê:\t" << in.lines + 1 << std::endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;
		*log.stream << "\t\tÊÎÄ ÍÀ ßÇÛÊÅ YNK_2020" << std::endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;
		*log.stream << in.text << endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;


		std::cout << "\t\tÈÑÕÎÄÍÛÅ ÄÀÍÍÛÅ" << std::endl;
		std::cout << std::setfill('-') << std::setw(90) << '-' << endl;
		std::cout << "Êîëè÷åñòâî ñèìâîëîâ:\t" << in.size << std::endl;
		std::cout << "Ïðîèãíîðèðîâàíî:\t" << in.ignor << std::endl;
		std::cout << "Êîëè÷åñòâî ñòðîê:\t" << in.lines + 1 << std::endl;
		std::cout << std::setfill('-') << std::setw(90) << '-' << endl;
	}
	void WriteParm(LOG log, Parm::PARM parm)
	{
		char in_text[PARM_MAX_SIZE];
		char out_text[PARM_MAX_SIZE];
		char log_text[PARM_MAX_SIZE];
		wcstombs(in_text, parm.in, PARM_MAX_SIZE);
		wcstombs(out_text, parm.out, PARM_MAX_SIZE);
		wcstombs(log_text, parm.log, PARM_MAX_SIZE);

		*log.stream << "\t\tÏÀÐÀÌÅÒÐÛ" << std::endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;
		*log.stream << "-in:\t" << in_text << "\n-out:\t" << out_text << "\n-log:\t" << log_text << endl;
		*log.stream << "Äîïîëíèòåëüíî: " << (parm.idtable ? "-id  " : "") << (parm.lextable ? "-lex  " : "")
			<< (parm.tokens ? "-tokens  " : "") << (parm.rules ? "-rules " : "") << std::endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;

		std::cout << "\t\tÏÀÐÀÌÅÒÐÛ" << std::endl;
		std::cout << std::setfill('-') << std::setw(90) << '-' << endl;
		std::cout << "-in:\t" << in_text << "\n-out:\t" << out_text << "\n-log:\t" << log_text << endl;
		std::cout << "Äîïîëíèòåëüíî: " << (parm.idtable ? "-id  " : "") << (parm.lextable ? "-lex  " : "")
			<< (parm.tokens ? "-tokens  " : "") << (parm.rules ? "-rules " : "") << std::endl;
		std::cout << std::setfill('-') << std::setw(90) << '-' << endl;
	}
	void WriteError(LOG log, Error::ERROR error)
	{
		if (log.stream)
		{
			*log.stream << "\nERROR CODE " << error.id << ": " << error.message << "\nÑòðîêà " << error.inext.line << " ïîçèöèÿ " << error.inext.position << endl;
			Close(log);
		}
		else
		{
			cout << "\nERROR CODE " << error.id << ": " << error.message << "\nÑòðîêà " << error.inext.line << " ïîçèöèÿ " << error.inext.position << endl;
		}
	}
	void Close(LOG log)
	{
		log.stream->close();
		delete log.stream;
	}

	void WriteIDTableLog(IT::IdTable& idtable, LOG log)
	{
		int i, numberNoneId = 0;
		*log.stream << "\t\tÒÀÁËÈÖÀ ÈÄÅÍÒÈÔÈÊÀÒÎÐÎÂ" << std::endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;
		*log.stream << "   ¹" << " | " << "Èäåíòèôèêàòîð" << " | " << "Òèï äàííûõ" << " | " << "Òèï èäåíòèôèêàòîðà" << " | " << "Èíäåêñ â ÒË" << " | " << "Çíà÷åíèå/Ïàðàìåòðû" << std::endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;
		for (i = 0; i < idtable.size; i++)
		{
			*log.stream << std::setfill('0') << std::setw(4) << std::right << i << " | ";
			*log.stream << std::setfill(' ') << std::setw(13) << std::left << idtable.table[i].id << " | ";
			switch (idtable.table[i].iddatatype)
			{
			case IT::INT:
				*log.stream << std::setw(10) << std::left;
				*log.stream << "int_" << " | ";
				break;
			case IT::STR:
				*log.stream << std::setw(10) << std::left;
				*log.stream << "str_" << " | ";
				break;
			case IT::PROC:
				*log.stream << std::setw(10) << std::left;
				*log.stream << "-" << " | ";
				break;
			case IT::BOOL:
				*log.stream << std::setw(10) << std::left;
				*log.stream << "bool_" << " | ";
				break;
			}
			switch (idtable.table[i].idtype)
			{
			case IT::V: *log.stream << std::setw(18) << std::left << "ïåðåìåííàÿ" << " | "; break;
			case IT::F: *log.stream << std::setw(18) << std::left << "ôóíêöèÿ" << " | "; break;
			case IT::P: *log.stream << std::setw(18) << std::left << "ïàðàìåòð" << " | "; break;
			case IT::L: *log.stream << std::setw(18) << std::left << "ëèòåðàë" << " | ";
				numberNoneId++;
				break;
			case IT::S: *log.stream << std::setw(18) << std::left << "áèáë. ôóíêöèÿ" << " | ";
				numberNoneId++;
				break;
			}
			*log.stream << std::setw(11) << std::left << idtable.table[i].idxfirstLE << " | ";
			if (idtable.table[i].iddatatype == IT::INT && (idtable.table[i].idtype == IT::V || idtable.table[i].idtype == IT::L))
				*log.stream << std::setw(18) << std::left << idtable.table[i].value.vint;
			else if (idtable.table[i].iddatatype == IT::BOOL && (idtable.table[i].idtype == IT::V || idtable.table[i].idtype == IT::L))
				*log.stream << std::setw(18) << std::left << idtable.table[i].value.vint;
			else if (idtable.table[i].iddatatype == IT::STR && (idtable.table[i].idtype == IT::V || idtable.table[i].idtype == IT::L))
				*log.stream << "[" << idtable.table[i].value.vstr.len << "]\"" << idtable.table[i].value.vstr.str << "\"";
			else if (idtable.table[i].idtype == IT::F || idtable.table[i].idtype == IT::S)
			{
				for (int k = 0; k < idtable.table[i].value.params.count; k++)
				{
					*log.stream << std::left << "P" << k << ":";
					switch (idtable.table[i].value.params.types[k])
					{
					case IT::INT:*log.stream << std::left << "I|"; break;
					case IT::STR:*log.stream << std::left << "S|"; break;
					case IT::BOOL:*log.stream << std::left << "B|"; break;
					}
				}
				if (idtable.table[i].value.params.count == NULL)
					*log.stream << "-";
			}
			else
				*log.stream << "-";
			*log.stream << std::endl;
		}
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;
		*log.stream << "\tÂñåãî èäåíòèôèêàòîðîâ: " << i - numberNoneId << std::endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;
	}

	void WriteLexTableLog(LT::LexTable& lextable, LOG log)
	{
		int i;
		*log.stream << "\t\tÒÀÁËÈÖÀ ËÅÊÑÅÌ" << std::endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;
		*log.stream << "   ¹" << " | " << "Ëåêñåìà" << std::setfill(' ') << std::setw(20) << ' ' << std::left << " | " << "Ñòðîêà" << std::setw(21) << ' ' << std::left << " | " << "Èíäåêñ â ÒÈ" << std::endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;
		for (i = 0; i < lextable.size; i++)
		{
			*log.stream << std::setfill('0') << std::setw(4) << std::right << i << " | " << std::setfill(' ') << std::setw(24) << std::left << lextable.table[i].lexema << "    | " << std::setw(24) << std::left
				<< lextable.table[i].line + 1 << "    | ";
			if (lextable.table[i].idxTI == LT_TI_NULLIDX)
				*log.stream << "-" << std::endl;
			else
				*log.stream << std::setw(23) << lextable.table[i].idxTI << std::endl;
		}
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;
		*log.stream << "\tÂñåãî ëåêñåì: " << i << std::endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;
	}

	void WriteTokensLog(LT::LexTable& lextable, LOG log)
	{
		/*
		std::cout << "\t\t\tÏÐÎÌÅÆÓÒÎ×ÍÎÅ ÏÐÅÄÑÒÀÂËÅÍÈÅ ÊÎÄÀ" << std::endl;
		std::cout << std::setfill('-') << std::setw(90) << '-' << endl;
		std::cout << "0001\t";
		*/
		int number = 0;
		*log.stream << "\t\tÏÐÎÌÅÆÓÒÎ×ÍÎÅ ÏÐÅÄÑÒÀÂËÅÍÈÅ ÊÎÄÀ" << std::endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;
		int i = 0;
		*log.stream << "0001\t" << lextable.table[i].lexema;
		number++;
		i++;
		for (i; i < lextable.size; i++)
		{
			if (lextable.table[i].line != number && lextable.table[i].line != 0)
			{
				while (lextable.table[i].line - number > 1)
					number++;
				if (number < 9)
					*log.stream << std::endl << std::setfill('0') << std::setw(4) << std::right << lextable.table[i].line + 1 << "\t";
				else
					*log.stream << std::endl << std::setfill('0') << std::setw(4) << std::right << lextable.table[i].line + 1 << "\t";
				number++;
			}
			*log.stream << lextable.table[i].lexema;
			if (lextable.table[i].lexema == LEX_ID || lextable.table[i].lexema == LEX_LITERAL)
				*log.stream << "(" << lextable.table[i].idxTI << ")";
		}
		*log.stream << std::endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << endl;
	}

	void WriteTraceLog(LOG log)
	{
		std::cout << std::endl << std::setw(25) << '-' << " ÒÐÀÑÑÈÐÎÂÊÀ ÑÈÍÒÀÊÑÈ×ÅÑÊÎÃÎ ÀÍÀËÈÇÀ " << std::setw(27) << '-' << std::endl << std::setw(89) << '-' << std::endl \
			<< " Øàã" << " | " << std::setfill(' ') \
			<< std::setw(25) << std::left << "Ïðàâèëî" << " | " \
			<< std::setw(30) << std::left << "Âõîäíàÿ ëåíòà" << " | " \
			<< std::setw(20) << std::left << "Ñòåê" \
			<< std::endl << std::setfill('-') << std::setw(89) << '-' << std::endl;
	}

}