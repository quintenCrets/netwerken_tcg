//seperate source file containing multiple functions to handle login and signup events

#include "runtime_functions.hpp"

void runtime::run()
{
    card_files = new file_names( ".json", "files\\cards\\" );
    all_player_files = new file_names( ".json", "files\\players\\" );
    benthernet = new network();

    //execution block
    while ( benthernet->connected() )
    {
        //get data
        benthernet->wait_for_tokenized_receive_data();

        #if DEBUG
            for ( std::string s : benthernet->tokenized_receive_data )
                std::cout << "received tokens: " << s << "\n\r";
        #endif

        //handle received data
        GENERAL_COMMAND_STATES general_command_state = handle_general_commands();

        if ( general_command_state == NO_COMMAND_STATE )
            handle_user_commands();

        //send data and clear buffers
        benthernet->push_tokenized_send_data();
        benthernet->tokenized_receive_data.clear();
        benthernet->tokenized_send_data.clear();
    }
}

runtime::~runtime()
{
    for ( player *p : all_active_players )
        delete p;

    delete card_files;
    delete benthernet;
}

runtime::GENERAL_COMMAND_STATES runtime::handle_general_commands()
{
    //tokenized_receive_data.at( 0 ) is stored in supposed_general_command since it isn't know yet to be a command or username
    std::string supposed_general_command = benthernet->tokenized_receive_data.at( 0 );

    if ( supposed_general_command == "login" )
    {
        //tokenized_receive_data.at( 1 ) should contain a username
        login_check( benthernet->tokenized_receive_data.at( 1 ) );
        return LOGIN_STATE;
    }
    else if ( supposed_general_command == "signup" )
    {
        //tokenized_receive_data.at( 1 ) should contain a username
        new_signup( benthernet->tokenized_receive_data.at( 1 ) );
        all_player_files->update_all_file_names();
        return SIGNUP_STATE;
    }
    else if ( supposed_general_command == "help" )
    {
        benthernet->tokenized_send_data.push_back( "help" );
        benthernet->tokenized_send_data.push_back( "List of all the possible general command's:\n -login>(username)\n -signup>(username)\n -help\n" );
        return HELP_STATE;
    }
    //if supposed_general_command isn't a general command used in the previous checks, continue to use it as a username
    else
    {
        benthernet->tokenized_send_data.push_back( supposed_general_command );

        if ( user_name_exists( supposed_general_command ) == false )
        {
            benthernet->tokenized_send_data.push_back( "non existing username" );
            return ERROR_STATE;
        }
        
        for ( player * player : all_active_players )
        {
            if ( supposed_general_command == player->get_user_name() )
            {
                return NO_COMMAND_STATE; // the supposed_general_command is the username of an active player
            }
        }

        benthernet->tokenized_send_data.push_back( "not yet logged in, use \"login>\" first" );
        return ERROR_STATE;
    }

    //something dire went wrong
    return ERROR_STATE;
}

void runtime::handle_user_commands()
{
    std::string username = benthernet->tokenized_receive_data.at( 0 );
    std::string user_command = benthernet->tokenized_receive_data.at( 1 );

    if ( user_command == "help" )
    {
        benthernet->tokenized_send_data.push_back( "message received" );
    }
    else if ( user_command == "gather mana" )
    {
        benthernet->tokenized_send_data.push_back( "mana + 1" );
    }
    else if ( user_command == "get mana count" )
    {
        for ( player * player : all_active_players )
        {
            if ( player->get_user_name() == username )
            {
                
                benthernet->tokenized_send_data.push_back( "you have " + std::to_string( player->get_mana_count() ) + " mana!" );
            }
        }
    }
    //invalid argument case
    else
    {
        benthernet->tokenized_send_data.push_back( "invalid command" );
    }
}

void runtime::login_check( std::string user_name_to_check )
{
    if ( user_name_exists( user_name_to_check ) )
    {
        if ( user_already_active( user_name_to_check ) )
        {
            benthernet->tokenized_send_data.push_back( user_name_to_check );
            benthernet->tokenized_send_data.push_back( "you are already logged in" );
            return;
        }

        //add new player to active players

        player *new_login_player = new player( user_name_to_check );
        all_active_players.push_back( new_login_player );
        benthernet->tokenized_send_data.push_back( user_name_to_check );
        benthernet->tokenized_send_data.push_back( "succesfull login" );
    }
    else 
    {
        benthernet->tokenized_send_data.push_back( "login" );
        benthernet->tokenized_send_data.push_back( "not a valid username" );
    }
}

bool runtime::user_name_exists( std::string user_name_to_check )
{
    std::vector<std::string> all_player_names;
    all_player_files->update_all_file_names(); //make sure to update all_player_files first
    all_player_files->get_all_file_names( &all_player_names);

    for ( std::string name : all_player_names )
    {
        if ( user_name_to_check == name )
        {
            return true;
        }
    }

    return false;
}

bool runtime::user_already_active( std::string user_name_to_check )
{
    for ( player * player : all_active_players )
    {
        if ( user_name_to_check == player->get_user_name() )
        {
            return true;
        }
    }

    return false;
}

void runtime::new_signup( std::string user_name_to_add )
{
    benthernet->tokenized_send_data.push_back( user_name_to_add );
    if ( user_name_exists( user_name_to_add ) )
    {
        benthernet->tokenized_send_data.push_back( "user name already used" );
        return;
    }

    //copy base file into the new user file
    std::ofstream new_user_file( "files\\players\\" + user_name_to_add + ".json" );
    std::ifstream base_user_file( "files\\base_user_file.json" );
    std::string red_line;

    while ( std::getline( base_user_file, red_line ) )
    {
        new_user_file << red_line << "\n";
    }

    //cleanup
    new_user_file.close();
    base_user_file.close();
    
    if ( user_name_exists( user_name_to_add ) )
    {
        benthernet->tokenized_send_data.push_back( "succesfull signup" );
    }
    else
    {
        benthernet->tokenized_send_data.push_back( "unexpected error" );
    }
}
