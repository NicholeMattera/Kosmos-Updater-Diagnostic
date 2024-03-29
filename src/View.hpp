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

#include <switch.h>
#include <functional>
#include "WebRequest.hpp"

#pragma once

namespace KUDiag {
    class View {
        public:
            View(std::function<void()> backCallback);
            virtual ~View();

            void draw(u64 kDown);
            void reset();

        protected:
            WebRequest * _request;
        
        private:
            virtual std::string _getTitle();
            virtual std::string _getURL();
            virtual void _requestCompletedSuccessfully();

            std::function<void()> _backCallback;
            bool _hasDrawn;
            bool _hasFinished;
    }; 
}
