// Kosmos Updater Diagnostic
// Copyright (C) 2019 Nichole Mattera
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <switch.h>
#include <curl/curl.h>
#include <string>

#pragma once

namespace KUDiag {
    class WebRequest {
        public:
            WebRequest(std::string method, std::string url);
            ~WebRequest();

            std::string getMethod();
            std::string getURL();

            Mutex getMutex();
            u8 getProgress();
            bool isComplete();

            size_t getSize();
            std::string getData();
            bool saveData(std::string filename);
            
            bool hasError();
            std::string getErrorMessage();

            std::string startSync();
            bool startAsync();

            size_t appendData(const char* in, std::size_t size, std::size_t num);
            int setProgress(curl_off_t dltotal, curl_off_t dlnow);
            void setCompleted();
            void setErrored(std::string message);

        private:
            static void _createRequest(void * ptr);
            static size_t _writeFunction(const char* in, std::size_t size, std::size_t num, WebRequest * request);
            static size_t _progressFunction(WebRequest * request, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

            std::string _method;
            std::string _url;
            std::string _data;

            Thread _thread;
            bool _threadCreated;
            Mutex _mutex;
            u8 _progress;
            bool _completed;
            bool _errored;
            std::string _errorMessage;
    }; 
}
