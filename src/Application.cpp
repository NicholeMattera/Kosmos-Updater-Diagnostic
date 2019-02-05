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
        mainMenuItems.push_back("Get latest app version");
        mainMenuItems.push_back("Get latest app");
        mainMenuItems.push_back("Get latest package version");
        mainMenuItems.push_back("Get latest package");
        mainMenuItems.push_back("Exit");

        _mainMenu = new Menu(
            string("Kosmos Updater Diagnostic ") + VERSION + " - Main Menu",
            mainMenuItems,
            bind(&Application::_menuItemSelected, this, _1),
            bind(&Application::_back, this)
        );

        vector<string> bundleItems;
        bundleItems.push_back("Kosmos");
        bundleItems.push_back("Atmosphere");
        bundleItems.push_back("Back");

        _bundleMenu = new Menu(
            string("Kosmos Updater Diagnostic ") + VERSION + " - Select a bundle",
            bundleItems,
            bind(&Application::_menuItemSelected, this, _1),
            bind(&Application::_back, this)
        );

        vector<string> channelItems;
        channelItems.push_back("Stable");
        channelItems.push_back("Bleeding-Edge");
        channelItems.push_back("Back");

        _channelMenu = new Menu(
            string("Kosmos Updater Diagnostic ") + VERSION + " - Select a channel",
            channelItems,
            bind(&Application::_menuItemSelected, this, _1),
            bind(&Application::_back, this)
        );

        _appVersionView = new AppVersionView(bind(&Application::_back, this));
        _appView = new AppView(bind(&Application::_back, this));
        _packageVersionView = new PackageVersionView(bind(&Application::_back, this));
        _packageView = new PackageView(bind(&Application::_back, this));

        _state = 0;
    }

    Application::~Application() {
        if (_mainMenu != NULL) {
            delete _mainMenu;
        }

        if (_bundleMenu != NULL) {
            delete _bundleMenu;
        }

        if (_channelMenu != NULL) {
            delete _channelMenu;
        }

        if (_appVersionView != NULL) {
            delete _appVersionView;
        }

        if (_appView != NULL) {
            delete _appView;
        }

        if (_packageVersionView != NULL) {
            delete _packageVersionView;
        }

        if (_packageView != NULL) {
            delete _packageView;
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
                    _appVersionView->draw(kDown);
                    break;

                case 2:
                    _appView->draw(kDown);
                    break;

                case 3:
                case 5:
                    _channelMenu->draw(kDown);
                    break;

                case 4:
                    _packageVersionView->draw(kDown);
                    break;

                case 6:
                    _bundleMenu->draw(kDown);
                    break;

                case 7:
                    _packageView->draw(kDown);
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
                    _appVersionView->reset();
                    _state = 1;
                } else if (itemSelected == 1) {
                    _appView->reset();
                    _state = 2;
                } else if (itemSelected == 2) {
                    _channelMenu->reset();
                    _state = 3;
                } else if (itemSelected == 3) {
                    _channelMenu->reset();
                    _state = 5;
                } else {
                    _state = -1;
                }
                break;

            case 3:
                if (itemSelected == 0) {
                    _packageVersionView->channel = 1;
                    _packageVersionView->reset();
                    _state = 4;
                } else if (itemSelected == 1) {
                    _packageVersionView->channel = 2;
                    _packageVersionView->reset();
                    _state = 4;
                } else {
                    _mainMenu->reset();
                    _state = 0;
                }
                break;

            case 5:
                if (itemSelected == 0) {
                    _packageView->channel = 1;
                    _bundleMenu->reset();
                    _state = 6;
                } else if (itemSelected == 1) {
                    _packageView->channel = 2;
                    _bundleMenu->reset();
                    _state = 6;
                } else {
                    _mainMenu->reset();
                    _state = 0;
                }
                break;

            case 6:
                if (itemSelected == 0) {
                    _packageView->bundle = 1;
                    _packageView->reset();
                    _state = 7;
                } else if (itemSelected == 1) {
                    _packageView->bundle = 2;
                    _packageView->reset();
                    _state = 7;
                } else {
                    _channelMenu->reset();
                    _state = 5;
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
            case 5:
            case 7:
                _mainMenu->reset();
                _state = 0;
                break;

            case 6:
                _channelMenu->reset();
                _state = 5;
                break;
        }
    }
}