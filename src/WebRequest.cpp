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

#include "WebRequest.hpp"
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

namespace KUDiag {
    WebRequest::WebRequest(string method, string url) {
        _method = method;
        _url = url;

        _size = 0;
        _data = (char *) malloc(1);

        mutexInit(&_mutex);
        _progress = 0;
        _completed = false;
        _errored = false;
        _errorMessage = "";
    }

    WebRequest::~WebRequest() {
        if (_data != NULL) {
            free(_data);
        }

        threadWaitForExit(&_thread);
        threadClose(&_thread);
    }

    string WebRequest::getMethod() {
        return _method;
    }

    string WebRequest::getURL() {
        return _url;
    }

    Mutex WebRequest::getMutex() {
        return _mutex;
    }

    u8 WebRequest::getProgress() {
        return _progress;
    }

    bool WebRequest::isComplete() {
        return _completed;
    }

    size_t WebRequest::getSize() {
        return _size;
    }

    string WebRequest::getData() {
        return string(_data);
    }

    char * WebRequest::getRawData() {
        return _data;
    }

    bool WebRequest::saveData(string filename) {
        ifstream fileExists(filename);
        if (fileExists.good()) {
            remove(filename.c_str());
        }
        fileExists.close();

        ofstream file;
        file.open(filename, ios::out | ios::binary);
        file.write(_data, _size);
        file.flush();
        file.close();

        return file.bad();
    }
    
    bool WebRequest::hasError() {
        return _errored;
    }

    string WebRequest::getErrorMessage() {
        return _errorMessage;
    }

    bool WebRequest::start() {
        Result res;

        if (R_FAILED( res = threadCreate(&_thread, _createRequest, (void *) this, 0x2000, 0x2B, -2)))
            return false;

        if (R_FAILED( res = threadStart(&_thread)))
            return false;

        return true;
    }

    size_t WebRequest::appendData(void *contents, size_t size, size_t nmemb) {
        size_t realsize = size * nmemb;
        mutexLock(&_mutex);

        _data = (char *) realloc(_data, _size + realsize + 1);
        if (_data == NULL) {
            setErrored("Not enough memory.");
            return 0;
        }

        memcpy(&(_data[_size]), contents, realsize);
        _size += realsize;
        _data[_size] = 0;

        mutexUnlock(&_mutex);

        return realsize;
    }

    int WebRequest::setProgress(curl_off_t dltotal, curl_off_t dlnow) {
        mutexLock(&_mutex);

        double progress = ((double) dlnow / (double) dltotal) * 100;
        _progress = progress;

        mutexUnlock(&_mutex);

        return 0;
    }

    void WebRequest::setCompleted() {
        mutexLock(&_mutex);

        _completed = true;

        mutexUnlock(&_mutex);
    }

    void WebRequest::setErrored(std::string message) {
        mutexLock(&_mutex);

        _completed = true;
        _errored = true;
        _errorMessage = message;

        mutexUnlock(&_mutex);
    }

    void WebRequest::_createRequest(void * ptr) {
        WebRequest * request = (WebRequest *) ptr;
        CURL * curl;
        CURLcode res;
        string userAgent = string("kosmos-updater/diag-") + VERSION;

        curl = curl_easy_init();

        if (curl) {
            struct curl_slist * headers = NULL;
            headers = curl_slist_append(headers, "Cache-Control: no-cache");

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_URL, request->getURL().c_str());
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request->getMethod().c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _writeFunction);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) request);
            curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, _progressFunction);
            curl_easy_setopt(curl, CURLOPT_XFERINFODATA, (void *) request);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str());

            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                request->setErrored(string(curl_easy_strerror(res)));
                curl_easy_cleanup(curl);
                curl_slist_free_all(headers);
                return;
            }

            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);

            if (http_code != 200) {
                request->setErrored("There was an error on the server, please try again later.");
                return;
            }
        } else {
            request->setErrored("Unable to initialize CURL.");
            return;
        }

        request->setCompleted();
    }

    size_t WebRequest::_writeFunction(void *contents, size_t size, size_t nmemb, void * ptr) {
        WebRequest * request = (WebRequest *) ptr;
        return request->appendData(contents, size, nmemb);
    }

    size_t WebRequest::_progressFunction(void *ptr, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
        WebRequest * request = (WebRequest *) ptr;
        return request->setProgress(dltotal, dlnow);
    }
}
