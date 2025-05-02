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
        const std::string user_name;
        json::jobject player_variables;
        
    public:
        player( const std::string user_name );
        ~player() {};

        std::string get_user_name() { return this->user_name; }
        int get_mana_count();
};

#endif

//end of file
