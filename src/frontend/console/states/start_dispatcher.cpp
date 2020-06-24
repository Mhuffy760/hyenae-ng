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

#include "../../../../include/assert.h"
#include "../../../../include/stop_watch.h"
#include "../../../../include/frontend/console/states/start_dispatcher.h"

namespace hyenae::frontend::console::states
{
    /*---------------------------------------------------------------------- */

    start_dispatcher::start_dispatcher(
        console_app_state_context* context,
        console_app_config* config,
        console_io* console_io,
        output_setup* output_setup,
        generator_setup* generator_setup,
        dispatcher_setup* dispatcher_setup) :
            console_app_state(context, config, console_io)
    {
        assert::argument_not_null(output_setup, "output_setup");
        assert::argument_not_null(generator_setup, "generator_setup");
        assert::argument_not_null(dispatcher_setup, "dispatcher_setup");

        _output_setup = output_setup;
        _generator_setup = generator_setup;
        _dispatcher_setup = dispatcher_setup;
    
    } /* start_dispatcher */

    /*---------------------------------------------------------------------- */

    start_dispatcher::~start_dispatcher()
    {
        safe_delete(_dispatcher);
        safe_delete(_thread_exception);

    } /* ~start_dispatcher */

    /*---------------------------------------------------------------------- */

    bool start_dispatcher::run()
    {
        if (_dispatcher == NULL)
        {
            if (!start_screen())
            {
                get_parent()->enter();
            }
        }
        else
        {
            result_screen();
        }

        return true;

    } /* run */

    /*---------------------------------------------------------------------- */

    void start_dispatcher::on_thread_exception(const exception_t& exception)
    {
        _thread_exception = new exception_t(exception);

    } /* on_thread_exception */

    /*---------------------------------------------------------------------- */

    bool start_dispatcher::start_screen()
    {
        bool task_result = false;

        safe_delete(_dispatcher);
        safe_delete(_thread_exception);
        
        get_console()->header_out("Start");

        // Init
        task_result = get_console()->task_out("Init", [this]()
        {
            _generator_setup->update_generator();

            _dispatcher = new data_dispatcher_t(
                _output_setup->get_output(),
                _generator_setup->get_generator(),
                _dispatcher_setup->get_delay(),
                _dispatcher_setup->get_limits());

            _dispatcher->add_listener(this);

            return true;
        });

        // Start
        if (task_result)
        {
            task_result = get_console()->task_out("Start", [this]()
            {
                _dispatcher->start(true);

                return true;
            });
        }

        // Warm-Up
        if (task_result)
        {
            task_result = get_console()->task_out("Warm-Up", [this]()
            {
                assert::in_time([this]()
                {
                    return _dispatcher->get_stats()->get_packet_count() > 0;

                }, WARMUP_TIMEOUT);

                return true;
            });
        }

        // Dispatching...
        if (task_result)
        {
            get_console()->task_out("Dispatching...");

            get_console()->separator_out(true, false);

            get_console()->prompt_out("", "Press any key to stop");

            while (_dispatcher->is_running())
            {
                if (get_console()->was_key_pressed())
                {
                    _dispatcher->stop();
                }
            }

            return true;
        }
        else
        {
            if (_thread_exception != NULL)
            {
                get_console()->error_out(_thread_exception->what(), true);
            }

            get_console()->separator_out(true, false);

            get_console()->wait_for_key_press();
        }

        return false;

    } /* start_screen */

    /*---------------------------------------------------------------------- */

    void start_dispatcher::result_screen()
    {
        get_console()->header_out("Results");

        // Bytes
        get_console()->result_out(
            "Bytes",
            std::to_string(_dispatcher->get_stats()->get_byte_count()));

        // Pckets
        get_console()->result_out(
            "Packets",
            std::to_string(_dispatcher->get_stats()->get_packet_count()));

        // Duration
        get_console()->result_out(
            "Duration",
            to_ms_string(_dispatcher->get_stats()->get_duration()));

        // Error
        if (_thread_exception != NULL)
        {
            get_console()->error_out(_thread_exception->what(), true);
        }

        get_console()->separator_out(true, false);

        safe_delete(_dispatcher);

        if (get_console()->prompt(
            0, 1, "Restart?", "0 = Main Menu, 1 = Restart] [1", 1) == 0)
        {
            get_parent()->enter();
        }

    } /* result_screen */

    /*---------------------------------------------------------------------- */

} /* hyenae::frontend::console::states */
