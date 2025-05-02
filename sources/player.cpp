//-----------------------------------------------
//Source code file for the player_handle header
//-----------------------------------------------

#include "player.hpp"

player::player( const std::string username ) : username(username)
{
    //get all content of the player file
    std::ifstream player_file( "files\\players\\" + username + ".json" );
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

int player::get_max_mana()
{
    return stoi( this->player_variables.get( "max_mana" ) );
}

int player::gather_mana()
{
    int new_mana_count = get_mana_count() + 1;
    
    if ( new_mana_count > get_max_mana() ) return get_mana_count();

    this->player_variables["mana_count"] = new_mana_count;
    write_user_variables();
    
    return new_mana_count;
}
void player::write_user_variables()
{
    std::ofstream user_file( "files\\players\\" + this->username + ".json" );
    user_file.clear();
    user_file << (std::string)this->player_variables.pretty( 1 );
    user_file.close();
}

