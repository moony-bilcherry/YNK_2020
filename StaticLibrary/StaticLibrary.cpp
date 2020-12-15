#include <iostream>

extern "C"
{
	int EXPONENT(int value, int exponent) {
		return pow(value, exponent);
	}

	int MEASURE(char* str)
	{
		return strlen(str);
	}

	char* COMBINE(char* buffer, char* str1, char* str2)
	{
		int i = NULL, len1 = NULL, len2 = NULL;

		for (; str1[len1] != '\0'; len1++);
		for (; str2[len2] != '\0'; len2++);

		for (int j = 0; str1[j] != '\0'; j++)
		{
			if (i == 255) break;
			buffer[i++] = str1[j];
		}
		for (int j = 0; str2[j] != '\0'; j++)
		{
			if (i == 255) break;
			buffer[i++] = str2[j];
		}
		buffer[i] = '\0';
		return buffer;
	}

	void BREAKL()
	{
		std::cout << std::endl;
	}

	void OutputInt(int a)
	{
		std::cout << a;
	}

	void OutputStr(char* ptr)
	{
		if (ptr == nullptr)
		{
			std::cout << std::endl;
			return;
		}
		for (int i = 0; ptr[i] != '\0'; i++)
		{
			std::cout << ptr[i];
		}
	}

	int RANDOMIZE(int min, int max)
	{
		return rand() % (max - min) + min;
	}

}