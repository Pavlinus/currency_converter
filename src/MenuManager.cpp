#include "MenuManager.h"

void MenuManager::ShowMenu()
{
	std::cout << "\n== Enter option's number ==\n";
	std::cout << "[1] Find currencies\n";
	std::cout << "[2] Convert currency\n";
	std::cout << "[3] Show all\n";
	std::cout << "[4] Quit\n\n";
}

int MenuManager::GetMenuInput()
{
	const char *prompt = "";
	const char *errMsg = "Incorrect input. Enter single number.";
	int *input = Utils::GetCharacters<int>(prompt, errMsg, 1);
	int option = input[0];

	delete [] input;

	return option;
}

void MenuManager::ShowAll()
{
	m_cManager.DisplayCurrencyList();
}

void MenuManager::Convert()
{
	const char *promptFrom = "Enter currency code to convert from (ex. 'RUB'): ";
	const char *promptTo = "Enter currency code to convert to (ex. 'EUR'): ";
	
	m_cManager.ClearSearchResults();

	AddConversionArgument(promptFrom);
	AddConversionArgument(promptTo);

	std::vector<Currency*> currencies = m_cManager.GetConversionCurrencies();

	char promptVal[50];
	sprintf(promptVal, "Enter number of %s to convert to %s: ", currencies[0]->GetCode(),
																currencies[1]->GetCode());

	double conversionValue = Utils::GetNumericInput<double>(promptVal);
	double result = conversionValue * currencies[0]->GetInverseRate() 
									* currencies[1]->GetRate();
	
	std::cout << conversionValue << " " << currencies[0]->GetCode() << " = ";
	std::cout << result << " " << currencies[1]->GetCode() << std::endl;
}

void MenuManager::AddConversionArgument(const char* prompt)
{
	const char *errMsg = "Incorrect input.";

	while (true)
	{
		char *code = Utils::GetCharacters<char>(prompt, errMsg, 3);
		if (!m_cManager.FindCurrencies(code, CurrencyManager::SearchFields::CODE))
		{
			std::cout << code << " currency code not found.\n\n";
			continue;
		}
		break;
	}
}

void MenuManager::Find()
{
	const char *prompt = "Enter currency's code or part of it's name (ex. 'RUB' or 'Rouble'): ";
	const char *errMsg = "Incorrect input.";
	Utils::InputOpt iopt = Utils::InputOpt::LESS_OR_EQ_CHAR_NUM;
	char *input = Utils::GetCharacters<char>(prompt, errMsg, UCHAR_MAX - 1, iopt);
	
	m_cManager.ClearSearchResults();
	m_cManager.FindCurrencies(input);
	m_cManager.DisplaySearchResults();

	delete [] input;
}

void MenuManager::SetOption(Options option)
{
	m_option = option;
}

MenuManager::Options MenuManager::GetOption()
{
	return m_option;
}