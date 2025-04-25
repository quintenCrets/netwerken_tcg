//-----------------------------------------------
//Source code file for the player_handle header
//-----------------------------------------------

#include "player.hpp"

player::player( const std::string user_name ) : user_name(user_name)
{
}

int player::get_mana_count()
{
    std::string temp_player_file_content = "{\"mana_count\":7,\"max_mana\":10,\"cards\":{}}";
    json::jobject result = json::jobject::parse( temp_player_file_content );

    return stoi( result.get( "mana_count" ) );
}

