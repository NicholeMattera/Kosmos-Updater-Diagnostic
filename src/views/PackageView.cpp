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

#include "PackageView.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "../zipper/unzipper.h"

using namespace std;
using namespace zipper;

namespace KUDiag {
    PackageView::PackageView(std::function<void()> backCallback) : View(backCallback) {}

    string PackageView::_getTitle() {
        return string("Kosmos Updater Diagnostic ") + VERSION + " - Get latest package";
    }

    string PackageView::_getURL() {
        return string("http://kosmos-updater.teamatlasnx.com/") + API_VERSION + "/package?channel=" + _getChannel() + "&bundle=" + _getBundle();
    }

    void PackageView::_requestCompletedSuccessfully() {
        unsigned char * data = (unsigned char *) _request->getRawData();

        vector<unsigned char> vectorData(data, data + _request->getSize());
        Unzipper unzipper(vectorData);
        vector<ZipEntry> entries = unzipper.entries();

        cout << "\x1b[5;0HChannel: "  << _getChannel() << "\n";
        cout << "Bundle: "  << _getBundle() << "\n";
        cout << "Number of entries: "  << entries.size() << "\n\n";
        cout << "First Ten Entries\n";

        int i = 0;
        for (auto const& entry : entries) {
            if (!entry.name.empty()) {
                cout << "Entry #" << i << ": " << entry.name << "\n";
                i++;
            }

            if (i == 10) {
                break;
            }
        }
    }

    string PackageView::_getChannel() {
        switch (channel) {
            case 2:
                return "bleeding-edge";

            default:
                return "stable";
        }
    }

    string PackageView::_getBundle() {
        switch (bundle) {
            case 2:
                return "atmosphere";

            default:
                return "kosmos";
        }
    }
}
