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

#include "AppView.hpp"
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

namespace KUDiag {
    AppView::AppView(std::function<void()> backCallback) : View(backCallback) {}

    string AppView::_getTitle() {
        return string("Kosmos Updater Diagnostic ") + VERSION + " - Get latest app";
    }

    string AppView::_getURL() {
        return string("http://kosmos-updater.teamatlasnx.com/") + API_VERSION + "/app";
    }

    void AppView::_requestCompletedSuccessfully() {
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
