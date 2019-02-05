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

#include "PackageVersionView.hpp"
#include <iostream>
#include <string>

using namespace std;

namespace KUDiag {
    PackageVersionView::PackageVersionView(std::function<void()> backCallback) : View(backCallback) {}

    string PackageVersionView::_getTitle() {
        return string("Kosmos Updater Diagnostic ") + VERSION + " - Get latest package version";
    }

    string PackageVersionView::_getURL() {
        return string("http://kosmos-updater.teamatlasnx.com/") + API_VERSION + "/package/version-number?channel=" + _getChannel();
    }

    void PackageVersionView::_requestCompletedSuccessfully() {
        cout << "\x1b[5;0HChannel: "  << _getChannel() << "\n";
        cout << "Latest Package Version: " << _request->getData() << "\n\n";
    }

    string PackageVersionView::_getChannel() {
        switch (channel) {
            case 2:
                return "bleeding-edge";

            default:
                return "stable";
        }
    }
}
