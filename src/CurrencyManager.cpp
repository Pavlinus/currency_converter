#include "CurrencyManager.h"

CurrencyManager::CurrencyManager()
{
	InitCurrencyVector();
}

bool CurrencyManager::IsActualRatesFile()
{
	struct stat fileInfo;

	if (stat(FILENAME, &fileInfo) != 0)
	{
		std::cout << "[-] Unable to access file " << FILENAME;
		std::cout << ": " << strerror(errno) << std::endl;

		return false;
	}

	if (time(nullptr) - fileInfo.st_mtime >= 3600)
	{
		return false;
	}

	return true;
}

void CurrencyManager::InitCurrencyVector()
{
	if (!IsActualRatesFile())
	{
		Utils::DownloadFile(URL, FILENAME);
	}

	std::string jsonRaw = Utils::ReadFile(FILENAME);
	
	return ParseData(jsonRaw.data());
}

void CurrencyManager::ParseData(const char *jsonRaw)
{
	rapidjson::Document doc;
	std::vector<std::string> vDataChunks;
	ExtractJsonChunks(jsonRaw, &vDataChunks);

	for (int i = 0; i < vDataChunks.size(); i++)
	{
		doc.Parse(vDataChunks[i].data());

		Currency *currencyObj = new Currency(
			doc["code"].GetString(),
			doc["name"].GetString(), 
			doc["rate"].GetFloat(), 
			doc["inverseRate"].GetFloat()
		);

		m_currencyVector.push_back(*currencyObj);
	}
}

void CurrencyManager::ExtractJsonChunks(const char *jsonRaw, std::vector<std::string> *matchesArr)
{
	std::smatch matches;
	std::regex reg("\\{\"code[\"\\\a-zA-Z0-9\\-:,.[:space:]]+\\}");
	std::string strJson(jsonRaw);
	int nMatch = 0;

	while (std::regex_search(strJson, matches, reg))
	{
		matchesArr->push_back(matches[0]);
		strJson = matches.suffix().str();
	}
}

void CurrencyManager::DisplayCurrencyList()
{
	std::string currencyName;

	for (int i = 0; i < m_currencyVector.size(); i++)
	{
		DisplayCurrency(&m_currencyVector[i]);
	}
}

void CurrencyManager::DisplayCurrency(Currency* currency)
{
	std::string currencyName = currency->GetName();
	Utils::DeleteSpecialChars(currencyName);

	if (Utils::U_Strlen(currencyName) > CUR_NAME_MAX_LEN)
	{
		currencyName = Utils::U_Substr(currencyName, 0, CUR_NAME_MAX_LEN - 3);
		currencyName.append("...");
	}

	std::cout << currency->GetCode() << "\t";
	printf("%-25s\t", currencyName.data());
	printf("%10.4f\t", currency->GetRate());
	std::cout << currency->GetInverseRate() << "\n";
}

void CurrencyManager::DisplaySearchResults()
{
	for (int i = 0; i < m_currencyFound.size(); i++)
	{
		DisplayCurrency(m_currencyFound[i]);
	}
}

int CurrencyManager::FindCurrencies(const char* str, CurrencyManager::SearchFields searchBy)
{
	char findStr[ UCHAR_MAX ];
	sprintf(findStr, ".*%s.*", str);
	std::regex reg(findStr);
	std::cmatch cm;
	int matchesFound = 0;

	for (Currency& item : m_currencyVector)
	{
		switch(searchBy)
		{
			case SearchFields::CODE | SearchFields::NAME:
				if (std::regex_search(item.GetCode(), cm, reg) ||
					std::regex_search(item.GetName().data(), cm, reg))
				{
					m_currencyFound.push_back(&item);
					matchesFound++;
				}
				break;

			case SearchFields::CODE:
				if (std::regex_search(item.GetCode(), cm, reg))
				{
					m_currencyFound.push_back(&item);
					matchesFound++;
				}
				break;

			case SearchFields::NAME:
				if (std::regex_search(item.GetName().data(), cm, reg))
				{
					m_currencyFound.push_back(&item);
					matchesFound++;
				}
				break;
			default:
				return 0;
		}
	}

	return matchesFound;
}

void CurrencyManager::ClearSearchResults()
{
	m_currencyFound.clear();
}

std::vector<Currency*> CurrencyManager::GetConversionCurrencies()
{
	std::vector<Currency*> result;
	for (Currency* item : m_currencyFound)
	{
		Currency *copy = new Currency(item);
		result.push_back(copy);
	}

	return result;
}