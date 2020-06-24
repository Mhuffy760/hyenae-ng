/*
 * Hyenae NG
 *   Advanced Network Packet Generator (NextGen)
 *
 * Copyright (C) 2020 Robin Richter
 *
 *   Contact  : hyenae.tool@googlemail.com
 *   Homepage : https://github.com/r-richter/hyenae-ng
 *
 * This file is part of Hyenae NG.
 *
 * Hyenae NG is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hyenae NG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hyenae NG.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CONSOLE_IO_H
#define CONSOLE_IO_H

#include "../../../include/os.h"
#include "../../../include/common.h"
#include "../../../include/app_config.h"

#ifdef OS_WINDOWS
    #define CHARSET_ASCII
#else
    #define CHARSET_UNICODE
#endif

namespace hyenae::frontend::console
{
    /*---------------------------------------------------------------------- */

    class console_io
    {
        public:
            /* Lengths */
            static const size_t MENU_WIDTH = 61;

            /* Margins */
            static const size_t BASE_MARGIN = 1;
            static const size_t MENU_ITEM_MARGIN = 2;
            static const size_t MENU_ITEM_INFO_MARGIN = 38;

            /* ANSI Foregrounds */
            static const string_t ANSI_FG_RESET;
            static const string_t ANSI_FG_BLACK;
            static const string_t ANSI_FG_RED;
            static const string_t ANSI_FG_GREEN;
            static const string_t ANSI_FG_YELLOW;
            static const string_t ANSI_FG_BLUE;
            static const string_t ANSI_FG_PURPLE;
            static const string_t ANSI_FG_CYAN;
            static const string_t ANSI_FG_WHITE;

            /* ANSI Backgrounds */
            static const string_t ANSI_BG_RESET;
            static const string_t ANSI_BG_BLACK;
            static const string_t ANSI_BG_RED;
            static const string_t ANSI_BG_GREEN;
            static const string_t ANSI_BG_YELLOW;
            static const string_t ANSI_BG_BLUE;
            static const string_t ANSI_BG_PURPLE;
            static const string_t ANSI_BG_CYAN;
            static const string_t ANSI_BG_WHITE;

        private:
            app_config* _config;

        public:
            console_io(app_config* config);
            void header_out(string_t title);
            void separator_out(bool nl_before, bool nl_after);
            void menu_separator_out(bool nl_before, bool nl_after);

            void menu_item_out(
                const string_t& choice,
                bool selected,
                const string_t& caption,
                const string_t& hint,
                const string_t& info,
                size_t item_count);

            void info_out(string_t message, bool menu_item_margin = false);
            void error_out(string_t message, bool menu_item_margin = false);
            void app_start_error_out(string_t message);
            void task_out(string_t name);
            bool task_out(string_t name, func_t<bool()> task);
            void result_out(string_t name, string_t value);
            void prompt_out(string_t prompt, string_t hint);

            string_t prompt(
                string_t prompt,
                string_t hint = "",
                string_t default_value = "");

            string_t prompt(
                func_t<string_t(string_t input)> parse,
                string_t prompt,
                string_t hint = "",
                string_t default_value = "");

            int64_t prompt(
                int64_t min_value,
                int64_t max_value,
                string_t prompt,
                string_t hint = "",
                size_t default_choice = SIZE_NONE);

            void wait_for_key_press(
                string_t prompt = "Press any key to continue");

            virtual bool was_key_pressed() = 0;

        protected:
            virtual void out(string_t out) = 0;
            virtual string_t in() = 0;
            virtual void clear() = 0;

        private:
            void pad_to_margin(string_t& text, size_t margin);

            void prefixed_out(
                string_t prefix,
                string_t message,
                bool menu_item_margin = false);

            string_t ansi_color(
                string_t text,
                string_t ansi_fg = ANSI_FG_RESET,
                string_t ansi_bg = ANSI_BG_RESET);

    }; /* console_io */

    /*---------------------------------------------------------------------- */

} /* hyenae::frontend::console */

#endif /* CONSOLE_IO_H */
