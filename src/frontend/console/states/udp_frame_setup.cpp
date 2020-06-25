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

#include "../../../../include/frontend/console/states/udp_frame_setup.h"

namespace hyenae::frontend::console::states
{
    /*---------------------------------------------------------------------- */

    udp_frame_setup::udp_frame_setup(
        uint8_t protocol,
        console_app_state_context* context,
        console_app_config* config,
        console_io* console_io,
        console_app_state* parent,
        ip_frame_setup* ip_frame_setup) :
            ip_based_frame_setup(
                context,
                config,
                console_io,
                parent,
                ip_frame_setup)
    {
        _protocol = protocol;

        _menu = new console_menu(
            console_io, get_generator_name() + " Setup", this, parent);

        _payload = new generator_selector(
            "Payload Setup", context, config, console_io, this);

        // Default values
        _src_port_pattern = "*****";
        _dst_port_pattern = "*****";

        // Source Port
        _src_port_pattern_item = new console_menu::item("Source Port");
        _menu->add_item(_src_port_pattern_item);

        // Destination Port
        _dst_port_pattern_item = new console_menu::item("Destination Port");
        _menu->add_item(_dst_port_pattern_item);

        // Payload
        _payload_item =
            new console_menu::item("Payload");
        _menu->add_item(_payload_item);

        update_generator();

    } /* udp_frame_setup */

    /*---------------------------------------------------------------------- */

    udp_frame_setup::~udp_frame_setup()
    {
        safe_delete(_menu);
        safe_delete(_src_port_pattern_item);
        safe_delete(_dst_port_pattern_item);
        safe_delete(_payload_item);
        safe_delete(_generator);
        safe_delete(_payload);

    } /* ~udp_frame_setup */

    /*---------------------------------------------------------------------- */

    bool udp_frame_setup::run()
    {
        update_generator();
        update_menu_items();

        _menu->set_start_state(get_start_state());
        _payload->set_start_state(get_start_state());

        console_menu::item* choice = _menu->prompt();

        if (choice == _src_port_pattern_item)
        {
            prompt_src_port_pattern();
        }
        else if (choice == _dst_port_pattern_item)
        {
            prompt_dst_port_pattern();
        }
        else if (choice == _payload_item)
        {
            _payload->enter();
        }

        return true;

    } /* run */

    /*---------------------------------------------------------------------- */

    void udp_frame_setup::set_src_port(uint16_t port)
    {
        _src_port_pattern = std::to_string(port);

    } /* set_src_port */

    /*---------------------------------------------------------------------- */

    void udp_frame_setup::set_dst_port(uint16_t port)
    {
        _dst_port_pattern = std::to_string(port);

    } /* set_dst_port */

    /*---------------------------------------------------------------------- */

    string_t udp_frame_setup::get_generator_name() const
    {
        return "UDP-Frame";

    } /* get_generator_name */

    /*---------------------------------------------------------------------- */

    udp_frame_setup::data_generator_t*
        udp_frame_setup::get_generator() const
    {
        return _generator;

    } /* get_generator */

    /*---------------------------------------------------------------------- */

    void udp_frame_setup::update_generator()
    {
        update_generator(
            _src_port_pattern,
            _dst_port_pattern);

    } /* update_generator */

    /*---------------------------------------------------------------------- */

    void udp_frame_setup::on_select()
    {
        get_ip_frame_setup()->set_protocol(_protocol);

    } /* on_select */

    /*---------------------------------------------------------------------- */

    void udp_frame_setup::update_menu_items()
    {
        _src_port_pattern_item->set_info(_src_port_pattern);
        _dst_port_pattern_item->set_info(_dst_port_pattern);
        _payload_item->set_info(_payload->get_generator_name());

    } /* update_menu_items */

    /*---------------------------------------------------------------------- */

    void udp_frame_setup::prompt_src_port_pattern()
    {
        _src_port_pattern = get_console()->prompt([this](string_t input)
            {
                update_generator(
                    input,
                    _dst_port_pattern);

                return input;

            },
            "Enter Source Port Pattern",
                _src_port_pattern,
                _src_port_pattern);

    } /* prompt_src_port_pattern */

/*---------------------------------------------------------------------- */

    void udp_frame_setup::prompt_dst_port_pattern()
    {
        _dst_port_pattern = get_console()->prompt([this](string_t input)
            {
                update_generator(
                    _src_port_pattern,
                    input);

                return input;

            },
            "Enter Destination Port Pattern",
                _dst_port_pattern,
                _dst_port_pattern);

    } /* prompt_src_port_pattern */

    /*---------------------------------------------------------------------- */

    void udp_frame_setup::update_generator(
        string_t src_port_pattern,
        string_t dst_port_pattern)
    {
        safe_delete(_generator);
        
        _generator = new udp_frame_generator_t(
            get_ip_frame_setup()->get_pseudo_header(),
            src_port_pattern,
            10,
            dst_port_pattern,
            10);

        _payload->update_generator();

        if (_payload->get_generator() != NULL)
        {
            ((udp_frame_generator_t*)_generator)->
                get_payload()->add_generator(_payload->get_generator());
        }

    } /* update_generator */

    /*---------------------------------------------------------------------- */

} /* hyenae::frontend::console::states */
