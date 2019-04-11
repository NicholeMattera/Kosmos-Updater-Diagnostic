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

#include "Menu.hpp"
#include "views/AppVersionView.hpp"

#pragma once

namespace KUDiag {
    class Application {
        public:
            Application();
            ~Application();

            void start();
        private:
            int _state;
            Menu * _mainMenu;
            AppVersionView * _httpSyncAppVersionView;
            AppVersionView * _httpAsyncAppVersionView;
            AppVersionView * _httpsSyncAppVersionView;
            AppVersionView * _httpsAsyncAppVersionView;

            void _menuItemSelected(int itemSelected);
            void _back();
    }; 
}
