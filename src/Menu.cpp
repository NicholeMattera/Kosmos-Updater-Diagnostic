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
#include <iostream>
#include <string>

using namespace std;

namespace KUDiag {
    Menu::Menu(string title, vector<string> items, function<void(u64)> itemSelectedCallback, function<void()> backCallback) {
        _title = title;
        _items = items;
        _itemSelectedCallback = itemSelectedCallback;
        _backCallback = backCallback;
        _hasDrawn = false;
        _itemSelected = 0;
    }

    void Menu::draw(u64 kDown) {
        u64 itemSelectedChanged = 0;

        if (kDown & KEY_A) {
            _itemSelectedCallback(_itemSelected);
        } else if (kDown & KEY_B) {
            _backCallback();
        } else if (kDown & KEY_UP && _itemSelected > 0) {
            itemSelectedChanged = -1;
        } else if (kDown & KEY_DOWN && _itemSelected < _items.size() - 1) {
            itemSelectedChanged = 1;
        }

        if (!_hasDrawn) {
            _itemSelected += itemSelectedChanged;

            consoleClear();

            cout << "\x1b[0;0H" << _title << "\n\n";

            u64 index = 0;
            for (auto const& item : _items) {
                if (index == _itemSelected) {
                    cout << " > " << item << "\n";
                } else {
                    cout << "   " << item << "\n";
                }

                index++;
            }

            _hasDrawn = true;
        } else if (itemSelectedChanged != 0) {
            u64 previousItemIndex = _itemSelected;
            _itemSelected += itemSelectedChanged;

            printf("\x1b[%lu;0H   ", previousItemIndex + 3);
            printf("\x1b[%lu;0H > ", _itemSelected + 3);
        }
    }

    void Menu::reset() {
        _hasDrawn = false;
        _itemSelected = 0;
    }
}
