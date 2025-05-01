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
#include "file_functions/file_names.hpp"
#include "network.hpp"
#include "player.hpp"

class runtime
{
    private:
        typedef enum USER_COMMANDS_RETURN_STATES
        {
            ACTION_STATE,
            LOGIN_STATE,
            SIGNUP_STATE,
            HELP_STATE,
            ERROR_STATE
        } USER_COMMANDS_RETURN_STATES;
        
        file_names *card_files;
        network *benthernet;
        std::map<std::string, player *> all_active_players;

        USER_COMMANDS_RETURN_STATES get_and_handle_user_command();
        void new_signup( std::string user_name_to_add );
        bool user_already_active( std::string user_name_to_check );
    public:
        runtime() {};
        ~runtime();

        void run();
        void init();
};

#endif
