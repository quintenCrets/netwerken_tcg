//------------------------------------------------------
//Player header
//  handle player specific variables and function calls
//------------------------------------------------------

#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>

class player
{
    private:
        const std::string user_name;

        int mana_count;
    public:
        player( const std::string user_name );
        ~player() {};

        std::string get_user_name() { return this->user_name; }
};

#endif

//end of file
