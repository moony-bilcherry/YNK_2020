#include "stdafx.h"
#include "Polish.h"

extern Log::LOG logfile;

namespace Poliz
{
	bool Poliz(int i, Lexer::LEX& lex)
	{
		std::stack<LT::Entry> stack;
		std::queue<LT::Entry> queue;

		LT::Entry temp;
		temp.idxTI = LT_TI_NULLIDX;
		temp.lexema = '#';
		temp.line = lex.lextable.table[i].line;

		LT::Entry func;
		func.idxTI = LT_TI_NULLIDX;
		func.lexema = '@';
		func.line = lex.lextable.table[i].line;

		int countLex = 0;
		int countParm = 1;
		int posLex = i;
		char* buf = new char[1];

		bool findComma = false;

		for (i; lex.lextable.table[i].lexema != LEX_SEMICOLON; i++, countLex++)
		{
			switch (lex.lextable.table[i].lexema)
			{
			case LEX_ID:
			{
				queue.push(lex.lextable.table[i]);
				continue;
			}
			case LEX_COMBINE:
			{
				queue.push(lex.lextable.table[i]);
				continue;
			}
			case LEX_MEASURE:
			{
				queue.push(lex.lextable.table[i]);
				continue;
			}
			case LEX_RANDOMIZE:
			{
				queue.push(lex.lextable.table[i]);
				continue;
			}
			case LEX_EXPONENT:
			{
				queue.push(lex.lextable.table[i]);
				continue;
			}
			case LEX_LITERAL:
			{
				queue.push(lex.lextable.table[i]);
				continue;
			}
			case LEX_OPERATOR:
			{
				while (!stack.empty() && lex.lextable.table[i].priority <= stack.top().priority)																					// меньше или равен приоритету оператора в вершине стека
				{
					queue.push(stack.top());
					stack.pop();
				}
				stack.push(lex.lextable.table[i]);
				continue;
			}
			case LEX_COMMA:
			{
				findComma = true;
				continue;
			}
			case LEX_LEFTBRACKET:
			{
				stack.push(lex.lextable.table[i]);
				continue;
			}
			case LEX_RIGHTBRACKET:
			{
				while (stack.top().lexema != LEX_LEFTBRACKET)
				{
					queue.push(stack.top());
					stack.pop();
					if (stack.empty())
						return false;
				}
				stack.pop();
				continue;
			}
			case LEX_LEFTTHESIS:
			{
				stack.push(lex.lextable.table[i]);
				continue;
			}
			case LEX_RIGHTTHESIS:
			{
				while (stack.top().lexema != LEX_LEFTTHESIS)
				{
					queue.push(stack.top());
					stack.pop();
					if (stack.empty())
						return false;
				}
				if (findComma)
					countParm++;
				lex.lextable.table[i] = func;
				queue.push(lex.lextable.table[i]);
				_itoa_s(countParm, buf, 2, 10);
				stack.top().lexema = buf[0];
				stack.top().idxTI = func.idxTI;
				stack.top().line = func.line;
				queue.push(stack.top());
				stack.pop();
				continue;
			}
			}
		}

		while (!stack.empty())
		{
			if (stack.top().lexema == LEX_LEFTTHESIS || stack.top().lexema == LEX_RIGHTTHESIS || stack.top().lexema == LEX_LEFTBRACKET || stack.top().lexema == LEX_RIGHTBRACKET)
				return false;
			queue.push(stack.top());
			stack.pop();
		}

		while (countLex)
		{
			if (!queue.empty())
			{
				lex.lextable.table[posLex++] = queue.front();
				queue.pop();
			}
			else
			{
				lex.lextable.table[posLex++] = temp;
			}
			countLex--;
		}

		for (int i = 0; i < posLex; i++)
		{
			if (lex.lextable.table[i].lexema == LEX_LITERAL)
				lex.idtable.table[lex.lextable.table[i].idxTI].idxfirstLE = i;
		}
	}

	void StartPoliz(Lexer::LEX& lex)
	{
		bool rc = false;
		bool equ = false;
		for (int i = 0; i < lex.lextable.size; i++)
		{
			if (lex.lextable.table[i].lexema == LEX_EQUAL || (lex.lextable.table[i].lexema == LEX_ID && lex.lextable.table[i - 1].lexema != LEX_EQUAL &&
				lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::F && lex.lextable.table[i - 1].lexema != LEX_PROCEDURE && lex.lextable.table[i - 2].lexema != LEX_FUNCTION))
			{
				rc = Poliz(i + 1, lex);
			}
		}
	}
}