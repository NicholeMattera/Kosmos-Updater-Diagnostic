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

#include "AppView.hpp"
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

namespace KUDiag {
    AppView::AppView(function<void()> backCallback) {
        _backCallback = backCallback;
        _hasDrawn = false;
        _hasFinished = false;
        _request = NULL;
    }

    void AppView::draw(u64 kDown) {
        if (_hasFinished) {
            if (kDown & KEY_A) {
                _backCallback();
                reset();
            }

            return;
        }

        if (_request) {
            if (_request->isComplete()) {
                if (_request->hasError()) {
                    cout << "\x1b[3;0HError: " << _request->getErrorMessage() << "\n\n";
                } else {
                    cout << "\x1b[3;14H100";

                    NacpStruct nacp = _getNACP(_request->getRawData());
                    NacpLanguageEntry * langEntry = NULL;
                    Result rc = nacpGetLanguageEntry(&nacp, &langEntry);

                    if (R_SUCCEEDED(rc) && langEntry != NULL) {
                       cout << "\x1b[5;0HApp Name: " << langEntry->name << "\n";
                       cout << "App Author: "  << langEntry->author << "\n";
                       cout << "App Version: "  << nacp.version << "\n\n";
                    } else {
                        cout << "\x1b[5;0HApp Version: " << nacp.version << "\n\n";
                    }
                }

                cout << "Press A to continue.";

                _hasFinished = true;
            } else {
                Mutex mutext = _request->getMutex();
                mutexLock(&mutext);
                int progress = _request->getProgress();
                mutexUnlock(&mutext);

                if (progress < 10) {
                    cout << "\x1b[3;16H" << progress;
                } else if (progress < 100) {
                    cout << "\x1b[3;15H" << progress;
                } else {
                    cout << "\x1b[3;14H" << progress;
                }
            }
        }

        if (!_hasDrawn) {
            consoleClear();

            cout << "\x1b[0;0HKosmos Updater Diagnostic " << VERSION << " - Get latest app\n\n";
            cout << "Downloading:   0%";

            _request = new WebRequest("GET", string("http://kosmos-updater.teamatlasnx.com/") + API_VERSION + "/app");
            if (!_request->start()) {
                cout << "\x1b[1;12H100";
                cout << "\x1b[3;0HError: Problem starting thread.\n\n";
                cout << "Press A to continue.";

                _hasFinished = true;
            }

            _hasDrawn = true;
        }
    }

    void AppView::reset() {
        _hasDrawn = false;
        _hasFinished = false;
        
        if (_request != NULL) {
            delete _request;
            _request = NULL;
        }
    }

    NacpStruct AppView::_getNACP(char * data) {
        NroHeader header;
        memcpy(&header, &(data[sizeof(NroStart)]), sizeof(NroHeader));

        NroAssetHeader assetHeader;
        memcpy(&assetHeader, &(data[header.size]), sizeof(NroAssetHeader));

        NacpStruct nacp;
        memcpy(&nacp, &(data[header.size + assetHeader.nacp.offset]), sizeof(NacpStruct));

        return nacp;
    }
}
