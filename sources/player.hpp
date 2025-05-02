//------------------------------------------------------
//Player header
//  handle player specific variables and function calls
//------------------------------------------------------

#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "file_functions/json.h"

class player
{
    private:
        const std::string username;
        json::jobject player_variables;
        
    public:
        player( const std::string username );
        ~player() {};

        std::string get_user_name() { return this->username; }
        void write_user_variables();

        //user command functions
        int get_mana_count();
        int get_max_mana();
        int gather_mana();
};

#endif

//end of file
