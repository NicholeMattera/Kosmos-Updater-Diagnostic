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

#include "AppVersionView.hpp"
#include <iostream>
#include <string>

using namespace std;

namespace KUDiag {
    AppVersionView::AppVersionView(bool secure, bool async, std::function<void()> backCallback) : View(secure, async, backCallback) {}

    string AppVersionView::_getTitle() {
        return string("Kosmos Updater Diagnostic ") + VERSION + " - Get latest app version";
    }

    string AppVersionView::_getURL() {
        return string("kosmos-updater.teamatlasnx.com/") + API_VERSION + "/app/version-number";
    }

    void AppVersionView::_requestCompletedSuccessfully() {
        cout << "\x1b[5;0HLatest App Version: "  << _request->getData() << "\n\n";
    }
}
