#include "stdafx.h"
#include "MFST.h"
#include "Error.h"

namespace MFST
{
#pragma region CONSTRUCTORS
	MfstState::MfstState()										 // конструктор по умолчанию
	{
		lenta_position = 0;										//Позиция на ленте 
		nrule = -1;												//Номер текущего правила 
		nrulechain = -1;										//Номер текущей цепочки 
	};

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain)					//Конструктор 
	{
		lenta_position = pposition;								//Позиция на ленте 
		st = pst;												//Стек автомата 
		nrulechain = pnrulechain;								//Номер текущего правила и цепочки 
	};

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain)
	{
		lenta_position = pposition;								//Позиция в ленте 
		st = pst;												//Стек автомата 
		nrule = pnrule;											//Номер текущего правила 
		nrulechain = pnrulechain;								//Номер текущей цепочки 
	};

	Mfst::MfstDiagnosis::MfstDiagnosis()						//Диагностика 
	{
		lenta_position = -1;
		rc_step = SURPRISE;
		nrule = -1;
		nrule_chain = -1;
	};
	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
	{
		lenta_position = plenta_position;
		rc_step = prc_step;										//Код завершения шага 
		nrule = pnrule;											//Номер правила 
		nrule_chain = pnrule_chain;								//Номер цепочки правила
	};
	Mfst::Mfst() { lenta = 0; lenta_size = lenta_position = 0; };					//Конструктор по умолчанию 
	Mfst::Mfst(LT::LexTable& lextable, GRB::Greibach pgrebach)						// Результат работы лекс анализатора, Грамматика Греибах
	{
		grebach = pgrebach;										//Грейбах 
		lex = lextable;											//Лекс
		lenta = new short[lenta_size = lex.size];				// Размер ленты =	текущий размер таблицы 
		for (int k = 0; k < lenta_size; k++)					//Занесение терминалов 
			lenta[k] = GRB::Rule::Chain::T(lex.table[k].lexema);
		lenta_position = 0;
		st.push(grebach.stbottomT);								//Добавл дна стека 
		st.push(grebach.startN);								//Добавление стартового символа 
		nrulechain = -1;										//Правила = -1 на старте 
	}
#pragma endregion
	Mfst::RC_STEP Mfst::step(Log::LOG log)										//Шаг по ленте 
	{
		RC_STEP rc = SURPRISE;									//Код возврата = ошибка 
		if (lenta_position < lenta_size)
		{
			if (GRB::Rule::Chain::isN(st.top()))				//Извлечение последнего элемента стека и проверка на нетерминал 
			{
				GRB::Rule rule;
				if ((nrule = grebach.getRule(st.top(), rule)) >= 0)			//Если такое правило есть, то идем дальше 
				{
					GRB::Rule::Chain chain;
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0)		//Подключение след цепочки и вывод ее номера 
					{
						MFST_TRACE1(log)								//Вывод 
							savestate(log);						//Сохранение состояния
						st.pop();								//Выталкиваем 
						push_chain(chain);						//Посещение цепочки в стек 
						rc = NS_OK;								//Найдено правило и цепочка ... запись в стек 
						MFST_TRACE2(log)								// Вывод 
					}
					else
					{
						MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE")
							savediagnosis(NS_NORULECHAIN);	//Код завершения 
						rc = resetstate(log) ? NS_NORULECHAIN : NS_NORULE;			//Восстановка состояния автомата 
					};
				}
				else rc = NS_ERROR;								//Неизвестный нетерминальный символ 
			}
			else if ((st.top() == lenta[lenta_position]))		//Если $
			{
				lenta_position++;
				st.pop();
				nrulechain = -1;
				rc = TS_OK;
				MFST_TRACE3(log)
			}
			else { MFST_TRACE4(TS_NOK / NS_NORULECHAIN) rc = resetstate(log) ? TS_NOK : NS_NORULECHAIN; };
		}
		else
		{
			rc = LENTA_END;
			MFST_TRACE4(LENTA_END, log);
		};
		return rc;
	};

	bool Mfst::push_chain(GRB::Rule::Chain chain)				//Поместить цепочку правила в стек 
	{
		for (int k = chain.size - 1; k >= 0; k--) st.push(chain.nt[k]);
		return true;
	};

	bool Mfst::savestate(Log::LOG log)										//Сохранение состояния автомата для отката 
	{
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain));
		MFST_TRACE6("SAVESTATE:", storestate.size());
		return true;
	};

	bool Mfst::resetstate(Log::LOG log)										//Восстановка состояния автомата 
	{
		bool rc = false;
		MfstState state;
		if (rc = (storestate.size() > 0))
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			MFST_TRACE5("RESSTATE")
				MFST_TRACE2(log)
		};
		return rc;
	};

	bool Mfst::savediagnosis(RC_STEP prc_step)
	{
		bool rc = false;
		short k = 0;

		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
			k++;

		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain);

			for (int i = k + 1; i < MFST_DIAGN_NUMBER; i++)
				diagnosis[i].lenta_position = -1;
		}

		return rc;
	};

	bool Mfst::start(Log::LOG log)		//Старт синтаксического анализа 
	{
		bool rc = false;
		RC_STEP rc_step = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE]{};
		rc_step = step(log);
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
			rc_step = step(log);

		switch (rc_step)
		{
		case LENTA_END:
		{
			MFST_TRACE4("------>LENTA_END");
			
			std::cout << "\tСинтаксический анализ выполнен без ошибок." << std::endl;
			*log.stream << std::setfill('-') << std::setw(89) << '-' << std::endl;
			*log.stream << "\tСинтаксический анализ выполнен без ошибок." << std::endl;
			*log.stream << std::setfill('-') << std::setw(90) << '-' << std::endl;
			rc = true;
			break;
		}

		case NS_NORULE:
		{
			MFST_TRACE4("------>NS_NORULE", log)
				std::cout << "------------------------------------------------------------------------------------------   ------" << std::endl;
			std::cout << getDiagnosis(0, buf) << std::endl;
			std::cout << getDiagnosis(1, buf) << std::endl;
			std::cout << getDiagnosis(2, buf) << std::endl;
			break;
		}

		case NS_NORULECHAIN:	MFST_TRACE4("------>NS_NORULECHAIN", log) break;
		case NS_ERROR:			MFST_TRACE4("------>NS_ERROR", log) break;
		case SURPRISE:			MFST_TRACE4("------>NS_SURPRISE", log) break;


		}
		return rc;
	};

	char* Mfst::getCSt(char* buf) //вывод стека 
	{
		short p;
		for (int k = (signed)st.size() - 1; k >= 0; --k)
		{
			p = st.c[k];
			buf[st.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p);
		}
		buf[st.size()] = '\0';
		return buf;
	}
	char* Mfst::getCLenta(char* buf, short pos, short n) //вывод ленты 
	{
		short i = 0, k = (pos + n < lenta_size) ? pos + n : lenta_size;

		for (int i = pos; i < k; i++)
			buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);

		buf[i - pos] = '\0';
		return buf;
	}
	char* Mfst::getDiagnosis(short n, char* buf)				//Вывод ошибок 
	{
		char* rc = new char[200]{};
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = grebach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = Error::geterror(errid);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d,	%s", err.id, lex.table[lpos].line, err.message);
			rc = buf;
			/*throw ERROR_THROW(123);*/
		}
		return rc;
	}
	void Mfst::printrules(Log::LOG log)										//вывод правил 
	{
		*log.stream << "\tДерево разбора:" << std::endl;
		*log.stream << std::setfill('-') << std::setw(90) << '-' << std::endl;
		MfstState state;
		GRB::Rule rule;
		for (unsigned short i = 0; i < storestate.size(); i++)
		{
			state = storestate.c[i];
			rule = grebach.getRule(state.nrule);
			MFST_TRACE7(log);
		};
		*log.stream << std::setfill('-') << std::setw(90) << '-' << std::endl;
	};

	bool Mfst::savededucation()									//Вывод дерева разбора 
	{
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storestate.size()];
		deducation.nrulechains = new short[deducation.size];

		for (unsigned short i = 0; i < storestate.size(); i++)
		{
			state = storestate.c[i];
			deducation.nrules[i] = state.nrule;
			deducation.nrulechains[i] = state.nrulechain;
		}
		return true;
	}
}


