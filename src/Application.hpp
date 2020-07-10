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

#include "Menu.hpp"
#include "views/AppVersionView.hpp"
#include "views/AppView.hpp"
#include "views/PackageVersionView.hpp"
#include "views/PackageView.hpp"

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
            Menu * _bundleMenu;
            Menu * _channelMenu;
            AppVersionView * _appVersionView;
            AppView * _appView;
            PackageVersionView * _packageVersionView;
            PackageView * _packageView;

            void _menuItemSelected(int itemSelected);
            void _back();
    }; 
}
