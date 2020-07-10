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

#include "Application.hpp"
#include <curl/curl.h>

using namespace std;
using namespace std::placeholders;

namespace KUDiag {
    Application::Application() {
        socketInitializeDefault();
        consoleInit(NULL);
        curl_global_init(CURL_GLOBAL_ALL);

        vector<string> mainMenuItems;
        mainMenuItems.push_back("Get latest app version (http - sync)");
        mainMenuItems.push_back("Get latest app version (http - async)");
        mainMenuItems.push_back("Get latest app version (https - sync)");
        mainMenuItems.push_back("Get latest app version (https - async)");
        mainMenuItems.push_back("Exit");

        _mainMenu = new Menu(
            string("Kosmos Updater Diagnostic SSL Debug - Main Menu"),
            mainMenuItems,
            bind(&Application::_menuItemSelected, this, _1),
            bind(&Application::_back, this)
        );

        _httpSyncAppVersionView = new AppVersionView(false, false, bind(&Application::_back, this));
        _httpAsyncAppVersionView = new AppVersionView(false, true, bind(&Application::_back, this));
        _httpsSyncAppVersionView = new AppVersionView(true, false, bind(&Application::_back, this));
        _httpsAsyncAppVersionView = new AppVersionView(true, true, bind(&Application::_back, this));

        _state = 0;
    }

    Application::~Application() {
        if (_mainMenu != NULL) {
            delete _mainMenu;
        }

        if (_httpSyncAppVersionView != NULL) {
            delete _httpSyncAppVersionView;
        }

        if (_httpAsyncAppVersionView != NULL) {
            delete _httpAsyncAppVersionView;
        }

        if (_httpsSyncAppVersionView != NULL) {
            delete _httpsSyncAppVersionView;
        }

        if (_httpsAsyncAppVersionView != NULL) {
            delete _httpsAsyncAppVersionView;
        }

        curl_global_cleanup();
        consoleExit(NULL);
        socketExit();
    }

    void Application::start() {
        
        while(appletMainLoop())
        {
            hidScanInput();
            u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

            switch (_state) {
                case 0:
                    _mainMenu->draw(kDown);
                    break;

                case 1:
                    _httpSyncAppVersionView->draw(kDown);
                    break;

                case 2:
                    _httpAsyncAppVersionView->draw(kDown);
                    break;

                case 3:
                    _httpsSyncAppVersionView->draw(kDown);
                    break;

                case 4:
                    _httpsAsyncAppVersionView->draw(kDown);
                    break;

                default:
                    return;
            }

            consoleUpdate(NULL);
        }
    }

    void Application::_menuItemSelected(int itemSelected) {
        switch (_state) {
            case 0:
                if (itemSelected == 0) {
                    _httpSyncAppVersionView->reset();
                    _state = 1;
                } else if (itemSelected == 1) {
                    _httpAsyncAppVersionView->reset();
                    _state = 2;
                } else if (itemSelected == 2) {
                    _httpsSyncAppVersionView->reset();
                    _state = 3;
                } else if (itemSelected == 3) {
                    _httpsAsyncAppVersionView->reset();
                    _state = 4;
                } else {
                    _state = -1;
                }
                break;
        }
    }

    void Application::_back() {
        switch (_state) {
            case 0:
                _state = -1;
                break;

            case 1:
            case 2:
            case 3:
            case 4:
                _mainMenu->reset();
                _state = 0;
                break;
        }
    }
}