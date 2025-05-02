//-----------------------------------------------
//Source code file for the player_handle header
//-----------------------------------------------

#include "player.hpp"

player::player( const std::string user_name ) : user_name(user_name)
{
    //get all content of the player file
    std::ifstream player_file( "files\\players\\" + user_name + ".json" );
    std::string file_content("");
    while ( player_file.eof() == false )
    {
        std::string line;
        std::getline( player_file, line );
        file_content += line;
    }
    player_file.close();
    
    #if DEBUG
        std::cout << file_content << "\n\r";
    #endif

    //parse player file
    player_variables = json::jobject::parse( file_content );
}

int player::get_mana_count()
{
    return stoi( this->player_variables.get( "mana_count" ) );
}

