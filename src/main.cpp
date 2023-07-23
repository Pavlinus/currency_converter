#include "main.h"
/*
	Get currency info from the internet
	Show some actual currencies
	Prompt user to choose what to convert to
	Save convertion history to file when quit
*/


int main()
{
	MenuManager mManager;
	MenuManager::Options option;

	do
	{
		mManager.ShowMenu();
		option = MenuManager::Options( mManager.GetMenuInput() );

		switch(option)
		{
			case MenuManager::Options::FIND:
				mManager.Find();
				break;
			case MenuManager::Options::CONVERT:
				mManager.Convert();
				break;
			case MenuManager::Options::SHOW_ALL:
				mManager.ShowAll();
				break;
			case MenuManager::Options::QUIT: break;
			default:
				std::cout << "Incorrect command.\n";
		}
	} while (option != MenuManager::Options::QUIT);

	return 0;
}