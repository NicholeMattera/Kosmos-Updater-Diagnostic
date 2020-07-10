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

#include "View.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "zipper/unzipper.h"

using namespace std;
using namespace zipper;

namespace KUDiag {
    View::View(bool secure, bool async, function<void()> backCallback) {
        _secure = secure;
        _async = async;
        _backCallback = backCallback;
        _hasDrawn = false;
        _hasFinished = false;
        _request = NULL;
    }

    View::~View() {
        if (_request) {
            delete _request;
        }
    }

    void View::draw(u64 kDown) {
        if (_hasFinished) {
            if (kDown & KEY_A) {
                _backCallback();
                reset();
            }

            return;
        }

        if (_async && _request) {
            Mutex mutext = _request->getMutex();
            mutexLock(&mutext);

            if (_request->isComplete()) {
                if (_request->hasError()) {
                    cout << "\x1b[3;0HError: " << _request->getErrorMessage() << "\n\n";
                } else {
                    cout << "\x1b[3;14H100";

                    _requestCompletedSuccessfully();
                }

                cout << "\nPress A to continue.";

                _hasFinished = true;
            } else {
                int progress = _request->getProgress();

                if (progress < 10) {
                    cout << "\x1b[3;16H" << progress;
                } else if (progress < 100) {
                    cout << "\x1b[3;15H" << progress;
                } else {
                    cout << "\x1b[3;14H" << progress;
                }
            }

            mutexUnlock(&mutext);
        }

        if (!_hasDrawn) {
            consoleClear();

            cout << "\x1b[0;0H" << _getTitle() << "\n\n";
            if (_async) {
                cout << "Downloading:   0%";
            }

            _request = new WebRequest("GET", ((_secure) ? "https://" : "http://") + _getURL());
            if (_async && !_request->startAsync()) {
                cout << "\x1b[1;12H100";
                cout << "\x1b[3;0HError: Problem starting thread.\n\n";
                cout << "Press A to continue.";

                _hasFinished = true;
            }

            if (!_async) {
                _request->startSync();
                _requestCompletedSuccessfully();

                cout << "\nPress A to continue.";

                _hasFinished = true;
            }

            _hasDrawn = true;
        }
    }

    void View::reset() {
        _hasDrawn = false;
        _hasFinished = false;
        
        if (_request != NULL) {
            delete _request;
            _request = NULL;
        }
    }
    
    string View::_getTitle() {
        return "";
    }

    string View::_getURL() {
        return "";
    }

    void View::_requestCompletedSuccessfully() {}
}
