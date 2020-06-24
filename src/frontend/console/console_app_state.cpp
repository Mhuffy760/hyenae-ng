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

#include "../../../include/assert.h"
#include "../../../include/frontend/console/console_app_state.h"

namespace hyenae::frontend::console
{
    /*---------------------------------------------------------------------- */

    console_app_state::console_app_state(
        console_app_state_context* context,
        app_config* config,
        console_io* console_io,
        console_app_state* parent)
    {
        assert::argument_not_null(context, "context");
        assert::argument_not_null(config, "config");
        assert::argument_not_null(console_io, "console_io");

        _context = context;
        _config = config;
        _console_io = console_io;
        _parent = parent;

    } /* console_app_state */

    /*---------------------------------------------------------------------- */

    console_app_state_context* console_app_state::get_context() const
    {
        return _context;

    } /* get_context */

    /*---------------------------------------------------------------------- */

    app_config* console_app_state::get_config() const
    {
        return _config;

    } /* get_config */

    /*---------------------------------------------------------------------- */

    console_io* console_app_state::get_console() const
    {
        return _console_io;

    } /* get_console */

    /*---------------------------------------------------------------------- */

    console_app_state* console_app_state::get_parent() const
    {
        return _parent;

    } /* get_parent */

    /*---------------------------------------------------------------------- */

    void console_app_state::enter()
    {
        get_context()->set_state(this);

    } /* enter */

    /*---------------------------------------------------------------------- */

    void console_app_state::enter(console_app_state* parent)
    {
        _parent = parent;

        enter();

    } /* enter */

    /*---------------------------------------------------------------------- */

} /* hyenae::frontend::console */
