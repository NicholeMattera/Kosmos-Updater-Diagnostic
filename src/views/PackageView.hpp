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
#include <functional>
#include "../View.hpp"

#pragma once

namespace KUDiag {
    class PackageView : public View {
        public:
            PackageView(std::function<void()> backCallback);

            int channel;
            int bundle;
        private:
            std::string _getChannel();
            std::string _getBundle();
            std::string _getTitle();
            std::string _getURL();
            void _requestCompletedSuccessfully();
    }; 
}
