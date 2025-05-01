//seperate source file containing multiple functions to handle login and signup events

#include "runtime_functions.hpp"

void runtime::init()
{
    std::cout << "start init\n\r";
    card_files = new file_names( ".json", "files\\cards\\" );
    benthernet = new network();

    file_names *all_player_file_names = new file_names( ".json", "files\\players\\" );
    std::vector<std::string> all_player_file_names_string_vector;
    all_player_file_names->get_all_file_names( &all_player_file_names_string_vector );

    for ( std::string name : all_player_file_names_string_vector )
    {
        player *p = new player( name );
        all_active_players.insert( std::pair<std::string, player *>( name, p ) );
    }

    std::cout << "init done\n\r";
}

void runtime::run()
{
    while ( benthernet->connected() )
    {
        //get data
        benthernet->wait_for_tokenized_receive_data();

        #if DEBUG
            for ( std::string s : benthernet->tokenized_receive_data )
                std::cout << "received tokens: " << s << "\n\r";
        #endif

        //handle received data
        USER_COMMANDS_RETURN_STATES user_commands_return_state = get_and_handle_user_command();

        //send data and clear buffers
        benthernet->push_tokenized_send_data();
        benthernet->tokenized_receive_data.clear();
        benthernet->tokenized_send_data.clear();
    }

    std::cout << "disconnect from benthernet\n\r";
}

runtime::~runtime()
{
    for ( std::pair<std::string, player *> p : all_active_players )
    {
        delete p.second;
    }

    delete card_files;
    delete benthernet;
}

runtime::USER_COMMANDS_RETURN_STATES runtime::get_and_handle_user_command()
{
    //init empty strings for later checks but assign them if possible
    std::string general_command("");
    std::string username_or_extra("");
    std::string action("");

    switch ( benthernet->tokenized_receive_data.size() )
    {
        case 0:
            return HELP_STATE;
        case 3:
            action = benthernet->tokenized_receive_data.at( 2 );
        case 2:
            username_or_extra = benthernet->tokenized_receive_data.at( 1 );
        case 1:
            general_command = benthernet->tokenized_receive_data.at( 0 );
            break;
        default:
            return ERROR_STATE;
    }

    //check general_command
    if ( general_command == "help" )
    {
        //TODO: write custom help functionality ( use extra token )
        benthernet->tokenized_send_data.push_back( "help" );
        benthernet->tokenized_send_data.push_back( "List of all the possible general command's:\n -login>(username)\n -signup>(username)\n -help\n" );
        return HELP_STATE;
    }
    else if ( general_command == "signup" )
    {
        new_signup( username_or_extra );
        return SIGNUP_STATE;
    }
    else if ( general_command == "action" )
    {
        benthernet->tokenized_receive_data.push_back( "action" );
        return ACTION_STATE;
    }
    //invalid argument case
    else
    {
        benthernet->tokenized_send_data.push_back( "invalid command" );
        return ERROR_STATE;
    }
}

bool runtime::user_already_active( std::string user_name_to_check )
{
    if ( all_active_players.at( user_name_to_check ) == nullptr )
    {
        return false;
    }

    return true;
}

void runtime::new_signup( std::string user_name_to_add )
{
    benthernet->tokenized_send_data.push_back( user_name_to_add );

    if ( all_active_players.at( user_name_to_add ) != nullptr )
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
    
    if ( all_active_players.at( user_name_to_add ) != nullptr )
    {
        benthernet->tokenized_send_data.push_back( "succesfull signup" );
    }
    else
    {
        benthernet->tokenized_send_data.push_back( "unexpected error" );
    }
}
