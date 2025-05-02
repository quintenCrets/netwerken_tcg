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
            SIGNUP_STATE,
            HELP_STATE,
            ERROR_STATE
        } USER_COMMANDS_RETURN_STATES;
        
        file_names *card_files;
        network *benthernet;
        std::map<std::string, player *> all_active_players;

        USER_COMMANDS_RETURN_STATES get_user_command();
        void new_signup( std::string username_to_add );
        bool username_check( std::string username_to_check );
        void do_user_action( std::string username, std::string user_action );
    public:
        runtime() {};
        ~runtime();

        void run();
        void init();
};

#endif
