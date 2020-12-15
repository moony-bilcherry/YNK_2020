#pragma once

#include "stdafx.h"
#include "Error.h"
#include "LT.h"

#define ID_MAXSIZE		10																	// максимальное количество символов в идентификаторе													// максимальное число символов в идентификаторе + область видимости
#define TI_MAXSIZE		4096																// максимальное количество строк в таблице идентификаторов
#define TI_INT_DEFAULT	0x00000000															// значение по умолчанию дл€ типа integer
#define TI_STR_DEFAULT	0x00																// значение по умолчанию дл€ типа string
#define TI_NULLIDX		0xffffffff															// нет элемента таблицы идентификаторов
#define TI_STR_MAXSIZE	255																	// максимальна€ длина строки													
#define MAX_PARAMS_COUNT 3
#define COMBINE_PARAMS_CNT 2
#define MEASURE_PARAMS_CNT 1
#define RANDOMIZE_PARAMS_CNT 2
#define EXPONENT_PARAMS_CNT 2

#define COMBINE_TYPE IT::IDDATATYPE::STR
#define MEASURE_TYPE IT::IDDATATYPE::INT
#define RANDOMIZE_TYPE IT::IDDATATYPE::INT
#define EXPONENT_TYPE IT::IDDATATYPE::INT

namespace IT																				// таблица идентификаторов
{
	enum IDDATATYPE { INT = 1, STR = 2, PROC = 3, BOOL = 4, UNKNOWN };		// типы данных идентификаторов: integer, string, без типа(процедура), bool, неопределенный
	enum IDTYPE {
		V = 1, F = 2, P = 3, L = 4, S = 5						// типы идентификаторов: переменна€, функци€, параметр, литерал, библ. функци€
	};

	static const IDDATATYPE LINK_PARAMS[] = { IT::IDDATATYPE::STR, IT::IDDATATYPE::STR };
	static const IDDATATYPE MEASURE_PARAMS[] = { IT::IDDATATYPE::STR };
	static const IDDATATYPE RANDOMIZE_PARAMS[] = { IT::IDDATATYPE::INT, IT::IDDATATYPE::INT };
	static const IDDATATYPE EXPONENT_PARAMS[] = { IT::IDDATATYPE::INT, IT::IDDATATYPE::INT };

	struct Entry												// строка таблицы идентификаторов
	{
		int			idxfirstLE;									// индекс первой строки в таблице лексем
		unsigned char id[ID_MAXSIZE];							// индентификатор
		IDDATATYPE	iddatatype = UNKNOWN;						// тип данных
		IDTYPE		idtype;										// тип идентификатора
		unsigned char visibility[ID_MAXSIZE];					// область видимости

		struct
		{
			int vint;											// значение integer
			struct
			{
				int len;										// количество символов в string
				unsigned char str[TI_STR_MAXSIZE - 1];			// символы string
			} vstr;												// значение string

			struct
			{
				int count;										// количество параметров функции
				IDDATATYPE* types;								// типы параметров функции
			} params;
		} value = { LT_TI_NULLIDX };							// значение идентификатора
	};

	struct IdTable												// экземпл€р таблицы идентификаторов
	{
		int maxsize;											// емкость таблицы идентификаторов < TI_MAXSIZE
		int size;												// текущий размер таблицы идентификаторов < maxsize
		Entry* table;											// массив строк таблицы идентификаторов
	};

	IdTable Create(																			// создать таблицу идентификаторов
		int size																			// емкость таблицы идентификаторов < TI_MAXSIZE
		);

	void Add(																				// добавить строку в таблицу идентификаторов
		IdTable& idtable,																	// экземпл€р таблицы идентификаторов
		Entry entry																			// строка таблицы идентификаторов
		);

	Entry GetEntry(																			// получить строку таблицы идентификаторов
		IdTable& idtable,																	// экземпл€р таблицы идентификаторов
		int n																				// номер получаемой строки
		);

	int IsId(																				// возврат: номер строки (если есть), TI_NULLIDX (если нет)
		IdTable& idtable,																	// экземпл€р таблицы идентификаторов
		unsigned char id[ID_MAXSIZE]														// идентификатор
		);

	void Delete(IdTable& idtable);															// удалить таблицу лексем (освободить пам€ть)

	void WriteTable(IdTable& idtable);
};