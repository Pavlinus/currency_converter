#include "Utils.h"


size_t Utils::write_callback(char *ptr, size_t size, size_t nmemb, void *stream)
{
	return fwrite(ptr, size, nmemb, (FILE*)stream);
}

int Utils::DownloadFile(const char *url, const char *fileName)
{
	CURLcode res = CURLE_OK;
	CURL *curl_handle;
	FILE *fdPage;
	char errorBuffer[ CURL_ERROR_SIZE ];

	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();

	curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER, errorBuffer);

	fdPage = fopen(fileName, "w");
	if (fdPage != nullptr)
	{
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fdPage);
		res = curl_easy_perform(curl_handle);
		fclose(fdPage);

		if (res != CURLE_OK)
			fprintf(stderr, "[-] Unable to download file %s: %s\n", fileName, 
					curl_easy_strerror(res));
	}
	else
		printf("[-] Unable to open file %s: %s\n", fileName, strerror(errno));

	curl_easy_cleanup(curl_handle);
	curl_global_cleanup();

	return res | errno;
}

std::string Utils::ReadFile(const char *fileName)
{
	std::ifstream ifs(fileName);
	if (ifs.fail())
	{
		std::cout << "[-] Unable to open file " << fileName << std::endl;
		return "";
	}

	std::string content = "";
	char buf[ INPUT_BUFFER_SIZE ];
	int bytesRead = 0;
	while (!ifs.eof())
	{
		bytesRead += ifs.get(buf, INPUT_BUFFER_SIZE).gcount();
		content.append(buf);
	}

	if (bytesRead <= 0)
	{
		std::cout << "[-] Content of " << fileName << " file is 0 length.\n";
		return "";
	}

	return content;
}

void Utils::DeleteSpecialChars(std::string &str)
{
	std::basic_regex reg("[\\t\\n\\r]");

	str = std::regex_replace(str, reg, "");
}

int Utils::U_Strlen(const std::string &str)
{
	std::vector<int> vecUtf32;
	utf8::utf8to32(str.begin(), str.end(), back_inserter(vecUtf32));
	return vecUtf32.size();
}

std::string Utils::U_Substr(const std::string &str, int start, int length)
{
	std::vector<int> resultUtf32;
	std::vector<int> vecUtf32;
	utf8::utf8to32(str.begin(), str.end(), back_inserter(vecUtf32));

	if (start < 0 || start >= vecUtf32.size())
	{
		return "";
	}

	if (length == std::string::npos) length = INT_MAX;

	for (int i = start; i < vecUtf32.size() && i < length; i++)
	{
		resultUtf32.push_back( vecUtf32[i] );
	}

	std::vector<char> vecUtf8;
	utf8::utf32to8(resultUtf32.begin(), resultUtf32.end(), back_inserter(vecUtf8));
	vecUtf8.push_back('\0');

	return std::string(vecUtf8.data());
}