#ifndef __UTILS_H__
#define __UTILS_H__

#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <regex>
#include <vector>
#include <typeinfo>
#include <limits>
#include "utf8.h"

class Utils
{
    private:
        static const int INPUT_BUFFER_SIZE = 255;
        static const int UNICODE_SEQ_LEN = 6;

        static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

    public:
        enum InputOpt {
            STRICT_CHAR_NUM = 1,
            LESS_OR_EQ_CHAR_NUM
        };

        static int DownloadFile(const char *url, const char *fileName);
        static int U_Strlen(const std::string &str);
        static void DeleteSpecialChars(std::string &str);
        static std::string ReadFile(const char *fileName);
        static std::string U_Substr(const std::string &str, int start = 0, int length = -1);

        template<typename T>
        static  T* GetCharacters(const char* prompt, const char* errorMsg, int chars, 
                                 InputOpt iopt = InputOpt::STRICT_CHAR_NUM)
        {
            const int INPUT_BUFFER_SIZE = UCHAR_MAX;
            bool isValidInput = false;
            char input[ INPUT_BUFFER_SIZE ];
            T *out = new T[ chars + 1 ];

            while (!isValidInput)
            {
                std::cout << prompt;
                std::cin.clear();

                std::cin.getline(input, INPUT_BUFFER_SIZE - 1, '\n');
                if (std::cin.gcount() - 1 > chars) 
                {
                    printf("%s (More than %d letters entered)\n\n", errorMsg, chars);
                    continue;
                }

                if (std::cin.fail())
                {
                    std::cout << errorMsg << std::endl;
                }
                else
                {
                    int chrCount = 0;
                    for (int i = 0; i < std::cin.gcount() && chrCount < chars; i++)
                    {
                        if (typeid(*out) == typeid(int) && isdigit(input[i]))
                        {
                            out[ chrCount++ ] = atoi(&input[i]);
                        }
                        else
                        {
                            out[ chrCount++ ] = input[i];
                        }
                    }

                    out[ chrCount ] = '\0';

                    if (chrCount != chars && iopt == InputOpt::STRICT_CHAR_NUM) 
                    {
                        std::cout << errorMsg << std::endl;
                    }
                    else
                    {
                        isValidInput = true;
                    }
                }
            }

            return out;
        }

        template<typename T> 
        static T GetNumericInput(const char* prompt)
        {
            T inNumber;
            bool isValidInput = false;

            do
            {
                std::cout << prompt;
                std::cin >> inNumber;

                if (inNumber <= 0)
                {
                    std::cout << "Entered value is incorrect. (Should be numeric)\n";
                    std::cin.clear();
                    
                }
                else
                {
                    isValidInput = true;
                }

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            } while (!isValidInput);

            return inNumber;
        }
};

#endif