#pragma once

#include "Error.h"

#define LEXEMA_FIXSIZE		1																// фиксированный размер лексемы
#define LT_MAXSIZE			4096															// максимальное количество строк в таблице лексем
#define LT_TI_NULLIDX		0xfffffff														// нет элемента таблицы идентификаторов

#define LEX_TYPE			't'						// лексема для объявления типа (int_ str_ bool_)
#define LEX_CREATE			'c'						// лексема для создания переменной (create)
#define LEX_ID				'i'						// лексема для идентификатора
#define LEX_LITERAL			'l'						// лексема для литерала
#define LEX_MAIN			'm'						// лексема для main
#define LEX_FUNCTION		'f'						// лексема для function
#define LEX_PROCEDURE		'p'						// лексема для procedure
#define LEX_GIVE			'r'						// лексема для возвращения значения (give)
#define LEX_SPEAK			'o'						// лексема для вывода в консоль (speak)
#define LEX_OPERATOR		'v'						// лексема для операторов (+ - * / %)
#define LEX_EQUAL			'='						// лексема для оператора присваивания (=)
#define LEX_LOGOPERATOR		'g'						// лексема для логического оператора (> < ~ #)
#define LEX_СYCLE			'u'						// лексема для cycle (цикл)
#define LEX_WHERE			'w'						// лексема для where (условный оператор)
#define LEX_OTHERWISE		'!'						// лексема для otherwise (условный оператор)
#define LEX_SEMICOLON		';'						// лексема для ;
#define LEX_COMMA			','						// лексема для ,
#define LEX_LEFTBRACE		'{'						// лексема для {
#define LEX_BRACELET		'}'						// лексема для }
#define LEX_LEFTTHESIS		'['						// лексема для [
#define LEX_RIGHTTHESIS		']'						// лексема для ]
#define LEX_LEFTBRACKET		'('						// лексема для (
#define LEX_RIGHTBRACKET	')'						// лексема для )

#define LEX_COMBINE			'+'						// лексема для конкатенации строк (combine)
#define LEX_MEASURE			'%'						// лексема для измерения длины строки (measure)
#define LEX_EXPONENT		'e'						// лексема для возведения в степень (exponent)
#define LEX_BREAKL			'b'						// лексема для перехода на новую строку (breakl)
#define LEX_RANDOMIZE		'z'						// лексема для генерации случайного числа в диапазоне (randomize)

namespace LT																				// таблица лексем
{
	enum OPER { NOT = -1, PLUS = 1, MINUS, MUL, DIV, REST, MORE, LESS, EQU, NOEQU };

	struct Entry								// строка таблицы лексем
	{
		unsigned char lexema;					// лексема
		int line;								// номер строки в исходном тексте
		int idxTI;								// индекс в таблице идентификаторов или LT_TI_NULLIDX
		int priority;							// приоритет
		OPER operation;
	};

	struct LexTable								// экземпляр таблицы лексем
	{
		int maxsize;							// емкость таблицы лексем < LT_MAXSIZE
		int size;								// текущий размер таблицы лексем < maxsize
		Entry* table;							// массив строк таблицы лексем
	};

	LexTable Create(																		// создать таблицу лексем
		int size																			// емкость таблицы лексем < LT_MAXSIZE
		);

	void Add(																				// добавить строку в таблицу лексем
		LexTable& lextable,																	// экземпляр таблицы лексем
		Entry entry																			// строка таблицы лексем
		);

	Entry GetEntry(																			// получить строку таблицы лексем
		LexTable& lextable,																	// экземпляр таблицы лексем
		int n																				// номер получаемой строки
		);

	void Delete(LexTable& lextable);														// удалить таблицу лексем (освободить память)

	Entry writeEntry(																		// заполнить строку таблицы лексем
		Entry& entry,
		unsigned char lexema,
		int indx,
		int line,
		int priority = -1,
		OPER operation = NOT
		);

	void WriteLexTable(LexTable& lextable);

	void WriteTokens(LexTable lextable);
};