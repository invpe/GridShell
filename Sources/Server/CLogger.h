// ------------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2022-2023-2024 https://github.com/invpe/GridShell
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//
// -----------------------------------------------------------------------------
#ifndef __CLOGGER__
#define __CLOGGER__
#include <fstream>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <iostream>
#define SERVER_LOG_INFO         0               // Info log level
#define SERVER_LOG_WARRNING     1               // Warrning log level
#define SERVER_LOG_ERROR        2               // Error log level
class CLogger {
public:
    static CLogger& GetInstance() {
        static CLogger instance;
        return instance;
    }
    void SetPath(const std::string& rstrPath)
    {
    	m_strPath = rstrPath;
    }
    void SetPrint(const bool& rbFlag)
    {
    	m_bPrint = rbFlag;
    }
    void Log(const int& riType, const std::string& rstrText) {
         const std::array<std::string, 4> logTypes = { "[INF]", "[WARN]", "[ERR]", "[???]" };
	    std::string strLogType = logTypes[std::clamp(riType, 0, static_cast<int>(logTypes.size()) - 1)];

	    auto now = std::chrono::system_clock::now();
	    auto time_t_now = std::chrono::system_clock::to_time_t(now);
	    std::tm tm_now = *std::localtime(&time_t_now);
	    std::ostringstream time_stream;
	    time_stream << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
	    std::string strTime = time_stream.str();

	    std::string strOutput = strTime + " " + strLogType + " " + rstrText + "\n";
	     
	    std::ofstream file(m_strPath+std::to_string(tm_now.tm_year + 1900) + "_" +
	                    std::to_string(tm_now.tm_mon + 1) + "_" +
	                    std::to_string(tm_now.tm_mday) + ".txt", std::ios::app);
	    if (file) {
	        file << strOutput;
	        if (m_bPrint) {
	            std::cout << strOutput;
	        }
	    } else {  
	    	// TODO: Handle the error
	    }
    }

private:
    CLogger() {


    	m_strPath = "logs/";
    	m_bPrint = true;
    	
    }  // Private constructor
 
    CLogger(CLogger const&) = delete;              
    CLogger(CLogger&&) = delete;                  
    CLogger& operator=(CLogger const&) = delete;  
    CLogger& operator=(CLogger &&) = delete;     
    bool m_bPrint;
    std::string m_strPath;
}; 

#endif