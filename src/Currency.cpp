#include "Currency.h"

Currency::Currency(const char *code, std::string name, double rate, double iRate)
{
	strcpy(m_code, code);
	m_name = name;
	m_rate = rate;
	m_inverseRate = iRate;
}

Currency::Currency(Currency* currency)
{
	strcpy(m_code, currency->GetCode());
	m_name = currency->GetName();
	m_rate = currency->GetRate();
	m_inverseRate = currency->GetInverseRate();
}

const char* Currency::GetCode()
{
	return m_code;
}

const std::string& Currency::GetName()
{
	return m_name;
}

double Currency::GetRate()
{
	return m_rate;
}

double Currency::GetInverseRate()
{
	return m_inverseRate;
}