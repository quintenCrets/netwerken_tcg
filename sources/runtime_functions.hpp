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
#include "file_names.hpp"
#include "network.hpp"
#include "player.hpp"

class runtime
{
    public:
        typedef enum GENERAL_COMMAND_STATES
        {
            NO_COMMAND_STATE,
            LOGIN_STATE,
            SIGNUP_STATE,
            ERROR_STATE
        } GENERAL_COMMAND_STATES;
        
        runtime() {};
        ~runtime();

        void run();
    private:
        file_names *card_files;
        file_names *all_player_files;
        network *benthernet;
        std::vector<player *> all_active_players;

        GENERAL_COMMAND_STATES handle_general_commands();
        void handle_user_commands();
        void login_check( std::string username_to_check );
        void new_signup( std::string user_name_to_add );
        bool user_name_exists( std::string user_name );
        bool user_already_active( std::string user_name_to_check );
};

#endif
