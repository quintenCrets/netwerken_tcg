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
    
    #if DEBUG_GENERAL
        std::cout << file_content << "\"\n\r";
    #endif

    //parse player file
    std::cout << "test\n\r";
    player_variables = json::jobject::parse( file_content );
    std::cout << "test\n\r";
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
    write_player_variables();
    
    return new_mana_count;
}

int player::search_card( file_names *card_files, std::string *new_card, int *new_card_count )
{
    int new_mana_count = get_mana_count() - 3;
    if ( new_mana_count < 0 ) return get_mana_count();
    this->player_variables["mana_count"] = new_mana_count;

    //write data to the nested "cards" object
    std::vector<json::jobject> temp;
    temp.push_back( this->player_variables["cards"].array(0).as_object() );
    *new_card = card_files->get_random_file_name();
    *new_card_count = std::stoi( temp.at( 0 )[ *new_card ] ) + 1;
    temp.at( 0 )[ *new_card ] = *new_card_count;
    this->player_variables["cards"] = temp;

    #if DEBUG_GENERAL
        std::cout << "got new card: \"" << *new_card << "\" new count " << std::to_string( *new_card_count ) << "\n\r";
    #endif
    
    write_player_variables();
    return new_mana_count;
}

void player::write_player_variables()
{
    #if DEBUG_GENERAL
        std::cout << (std::string)this->player_variables.pretty( 0 ) << "\n\r";
    #endif

    std::ofstream user_file( "files\\players\\" + this->username + ".json" );
    user_file.clear();
    user_file << ( std::string)this->player_variables.pretty( 0 );
    user_file.close();
}

