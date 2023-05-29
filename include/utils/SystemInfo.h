#pragma once

#include <string>
#include "windows.h"
#include <unistd.h>
#include <limits.h>


namespace Utils
{
	class SystemInfo
	{
	public:
		static std::string getHostName()
		{
#ifdef WIN32
			const unsigned int bufferSize = 32767;
			TCHAR  infoBuf[bufferSize];
			DWORD  bufCharCount = bufferSize;
			if (!GetComputerName(infoBuf, &bufCharCount))
				return "";
			return infoBuf;
#else
			const unsigned int bufferSize = 32767;
			char hostname[bufferSize];
			gethostname(hostname, bufferSize);
			return hostname;
#endif
		}
	};
}

