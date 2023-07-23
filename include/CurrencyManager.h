#ifndef __CURRENCY_MANAGER_H__
#define __CURRENCY_MANAGER_H__

#include <string>
#include <iostream>
#include <rapidjson/document.h>
#include <regex>
#include <vector>
#include <sys/stat.h>
#include "Currency.h"
#include "Utils.h"

class CurrencyManager
{
	private:
		const char* URL = "http://floatrates.com/daily/usd.json";
		const char* FILENAME = "rates.json";
		const int CUR_NAME_MAX_LEN = 25;

		std::vector<Currency> m_currencyVector;
		std::vector<Currency*> m_currencyFound;

		bool IsActualRatesFile();
		void InitCurrencyVector();
		void ParseData(const char *jsonRaw);
		void ExtractJsonChunks(const char *jsonRaw, std::vector<std::string> *matchesArr);

	public:
		enum SearchFields {
			CODE = 1,
			NAME
		};

		CurrencyManager();

		void DisplayCurrencyList();
		void DisplayCurrency(Currency* currency);
		void DisplaySearchResults();
		void ClearSearchResults();
		int FindCurrencies(const char* str, 
							SearchFields by = SearchFields(SearchFields::CODE | SearchFields::NAME));
		std::vector<Currency*> GetConversionCurrencies();
};

#endif