// Kosmos Updater Diagnostic
// Copyright (C) 2019 Steven Mattera
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
            char * getRawData();
            bool saveData(std::string filename);
            
            bool hasError();
            std::string getErrorMessage();

            bool start();

            size_t appendData(void *contents, size_t size, size_t nmemb);
            int setProgress(curl_off_t dltotal, curl_off_t dlnow);
            void setCompleted();
            void setErrored(std::string message);

        private:
            static void _createRequest(void * ptr);
            static size_t _writeFunction(void *contents, size_t size, size_t nmemb, void * ptr);
            static size_t _progressFunction(void *ptr, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

            std::string _method;
            std::string _url;

            size_t _size;
            char * _data;

            Thread _thread;
            Mutex _mutex;
            u8 _progress;
            bool _completed;
            bool _errored;
            std::string _errorMessage;
    }; 
}
