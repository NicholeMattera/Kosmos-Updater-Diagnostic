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

#include <switch.h>
#include <string>
#include <vector>
#include <functional>

#pragma once

namespace KUDiag {
    class Menu {
        public:
            Menu(std::string title, std::vector<std::string> items, std::function<void(u64)> itemSelectedCallback, std::function<void()> backCallback);

            void draw(u64 kDown);
            void reset();
        private:
            std::string _title;
            std::vector<std::string> _items;
            std::function<void(u64)> _itemSelectedCallback;
            std::function<void()> _backCallback;

            bool _hasDrawn;
            u64 _itemSelected;
    }; 
}
