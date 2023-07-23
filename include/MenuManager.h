#ifndef __MENU_MANAGER__
#define __MENU_MANAGER__

#include "Utils.h"
#include "CurrencyManager.h"

class MenuManager
{
	public:
		enum Options
		{
			FIND = 1, 
			CONVERT, 
			SHOW_ALL, 
			QUIT
		};

		int GetMenuInput();
		void ShowMenu();
		void ShowAll();
		void Convert();
		void AddConversionArgument(const char* prompt);
		void Find();
		void SetOption(Options option);
		Options GetOption();

	private:
		Options m_option;
		CurrencyManager m_cManager;
};

#endif