#ifndef __CURRENCY_H__
#define __CURRENCY_H__

#include <string>
#include <cstring>

class Currency
{
	private:
		char m_code[5];
		std::string m_name;
		double m_rate;
		double m_inverseRate;

	public:
		Currency(const char *code, std::string name, double rate, double iRate);
		Currency(Currency* currency);

		const char* GetCode();
		const std::string& GetName();
		double GetRate();
		double GetInverseRate();
};

#endif