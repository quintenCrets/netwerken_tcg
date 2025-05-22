//--------------------------------------------------------------
//runtime functions header
//  contains the runtime class structure and functions
//--------------------------------------------------------------

#ifndef RUNTIME_H
#define RUNTIME_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cstdlib>
#include "file_functions/file_names.hpp"
#include "network.hpp"
#include "player.hpp"

class runtime
{
    private:
        typedef enum USER_COMMANDS_RETURN_STATES
        {
            ACTION_STATE,
            SIGNUP_STATE,
            HELP_STATE,
            ERROR_STATE
        } USER_COMMANDS_RETURN_STATES;
        
        file_names *card_files;
        network *benthernet;
        std::map<std::string, player *> all_active_players;

        USER_COMMANDS_RETURN_STATES get_user_command();
        void check_command_1( USER_COMMANDS_RETURN_STATES *user_commands_return_state );
        void check_command_2( USER_COMMANDS_RETURN_STATES *user_commands_return_state );
        void check_command_3_or_more( USER_COMMANDS_RETURN_STATES *user_commands_return_state );

        void send_help();
        void new_signup( std::string username_to_add );
        bool username_check( std::string username_to_check );
        void do_user_action( std::string username, std::string user_action, std::string extra );
    public:
        runtime() {};
        ~runtime();

        void run();
        void init();
};

#endif
