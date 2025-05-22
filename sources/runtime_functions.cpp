//seperate source file containing multiple functions to handle login and signup events

#include "runtime_functions.hpp"

void runtime::init()
{
    #if DEBUG_GENERAL
        std::cout << "start init\n\r";
    #endif

    //general init
    card_files = new file_names( ".json", "files\\cards\\" );
    benthernet = new network();

    //get all player names
    file_names *all_player_file_names = new file_names( ".json", "files\\players\\" );
    std::vector<std::string> all_player_file_names_string_vector;
    all_player_file_names->get_all_file_names( &all_player_file_names_string_vector );

    //set all pleayers active
    for ( std::string name : all_player_file_names_string_vector )
    {
        player *p = new player( name );
        all_active_players.insert( std::pair<std::string, player *>( name, p ) );
    }

    srand( time( nullptr ) );

    #if DEBUG_GENERAL
        std::cout << "init done\n\r";
    #endif
}

void runtime::run()
{
    while ( benthernet->connected() )
    {
        //get data
        benthernet->wait_for_tokenized_receive_data();

        #if DEBUG_GENERAL
            for ( std::string s : benthernet->tokenized_receive_data )
                std::cout << "received tokens: " << s << "\n\r";
        #endif

        //handle received data
        USER_COMMANDS_RETURN_STATES user_commands_return_state = get_user_command();

        //handle user command
        switch ( benthernet->tokenized_receive_data.size() )
        {
            case 1:
                check_command_1( &user_commands_return_state );
                break;
            case 2:
                check_command_2( &user_commands_return_state );
                break;
            case 3:
            case 4:
                check_command_3_or_more( &user_commands_return_state );
                break;
            default:
                user_commands_return_state = ERROR_STATE;
                break;
        }

        //if any error got detected
        if ( user_commands_return_state == ERROR_STATE )
        {
            benthernet->tokenized_send_data.push_back( "error state" );
        }

        //send data and clear buffers
        benthernet->push_tokenized_send_data();
        benthernet->tokenized_receive_data.clear();
        benthernet->tokenized_send_data.clear();
    }

    std::cout << "disconnect from benthernet\n\r";
}

void runtime::check_command_1( runtime::USER_COMMANDS_RETURN_STATES *user_commands_return_state )
{
    switch ( *user_commands_return_state )
    {
        case HELP_STATE:
            send_help();
            break;
        default:
            *user_commands_return_state = ERROR_STATE;
            break;
    }
}

void runtime::check_command_2( runtime::USER_COMMANDS_RETURN_STATES *user_commands_return_state )
{
    std::string username = benthernet->tokenized_receive_data.at(1);

    switch ( *user_commands_return_state )
    {
        case SIGNUP_STATE:
            new_signup( username );
            break;
        default:
            *user_commands_return_state = ERROR_STATE;
            break;
    }
}

void runtime::check_command_3_or_more( runtime::USER_COMMANDS_RETURN_STATES *user_commands_return_state )
{
    std::string username = benthernet->tokenized_receive_data.at(1);
    std::string action = benthernet->tokenized_receive_data.at(2);
    std::string extra = benthernet->tokenized_receive_data.size() >= 4 ? benthernet->tokenized_receive_data.at(3) : "";

    benthernet->tokenized_send_data.push_back( username );

    switch ( *user_commands_return_state )
    {
        case ACTION_STATE:
            if ( username_check( username ) ) do_user_action( username, action, extra );
            break;
        default:
            *user_commands_return_state = ERROR_STATE;
            break;
    }
}

void runtime::send_help()
{
    benthernet->tokenized_send_data.push_back( "\n\n   general message construction:\n\n   \"benthernet token\">\"general command token\">\"username token\">\"action token\">\n\n   \"benthernet token\" = \"tcg?\"\n   examples:\n    \"tcg?>action>quinten>get max mana>\"\n    \"tcg!>signup>quinten>succesfully signed up>\"\n    \"tcg?>help>\"\n\n   list of general commands:\n\n   list  of all general command tokens:\n   - signup>   used to register a new account\n   - action>   used to perform a player specific action\n   - help>     prints out a helpfull quick start in game\n\n   list of action tokens:\n\n   - action>(username)>(extra)>\n\n   list of all possible extra tokens:\n   - get mana count        get a players mana count\n   - get max mana          get the maximum amount of mana a player can have\n   - gather mana>(count)   gather more mana if possible\n   - search card           use mana to search for a new card\n\n   examples:\n    \"tcg?>action>quinten>get max mana>\"\n    \"tcg?>action>quinten>search card>\"\n");
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

runtime::USER_COMMANDS_RETURN_STATES runtime::get_user_command()
{
    //init empty strings for later checks but assign them if possible
    std::string general_command("");

    switch ( benthernet->tokenized_receive_data.size() )
    {
        case 0:
            return HELP_STATE;
        case 4:
        case 3:    
        case 2:
        case 1:
            general_command = benthernet->tokenized_receive_data.at( 0 );
            break;
        default:    
            return ERROR_STATE;
    }        

    benthernet->tokenized_send_data.push_back( general_command );

    //check general_command
    if ( general_command == "help" )
    {
        return HELP_STATE;
    }    
    else if ( general_command == "signup" )
    {
        return SIGNUP_STATE;
    }    
    else if ( general_command == "action" )
    {
        return ACTION_STATE;
    }    
    else
    {
        return ERROR_STATE;
    }    
}    

bool runtime::username_check( std::string username_to_check )
{
    return ( all_active_players.find( username_to_check ) == all_active_players.end() ) ? false : true;
}    

void runtime::new_signup( std::string username_to_add )
{
    benthernet->tokenized_send_data.push_back( username_to_add );

    //check if username exists
    if ( username_check( username_to_add ) )
    {
        benthernet->tokenized_send_data.push_back( "username already exists" );
        return;
    }    

    //copy base file into the new user file
    std::ofstream new_user_file( "files\\players\\" + username_to_add + ".json" );
    std::ifstream base_user_file( "files\\base_user_file.json" );
    std::string red_line;

    while ( std::getline( base_user_file, red_line ) )
    {
        new_user_file << red_line << "\n";
    }    

    //cleanup
    new_user_file.close();
    base_user_file.close();
    
    player *p = new player( username_to_add );
    all_active_players.insert( std::pair<std::string, player *>( username_to_add, p ) );

    if ( username_check( username_to_add ) )
    {
        benthernet->tokenized_send_data.push_back( "succesfull signup" );
    }    
    else
    {
        benthernet->tokenized_send_data.push_back( "unexpected error" );
    }    
}    

void runtime::do_user_action( std::string username, std::string user_action, std::string extra )
{
    //check for valid username
    benthernet->tokenized_receive_data.push_back( username );

    //check for valid action and do it
    if ( user_action == "get mana count" )
    {
        benthernet->tokenized_send_data.push_back( "you currentrly have " + std::to_string( all_active_players.at( username )->get_mana_count() ) + " mana" );
    }    
    else if ( user_action == "get max mana" )
    {
        benthernet->tokenized_send_data.push_back( "you can hold a maximum of " + std::to_string( all_active_players.at( username )->get_max_mana() ) + " mana" );
    }    
    else if ( user_action == "gather mana" )
    {
        int amount_of_mana_to_add = 1;
        if ( extra.at( 0 ) >= '1' && extra.at( 0 ) <= '9' );
        {
            amount_of_mana_to_add = extra.at( 0 ) - '0';
        }
        
        int max_mana = all_active_players.at( username )->get_max_mana();
        int new_mana_count = all_active_players.at( username )->gather_mana( amount_of_mana_to_add );
        if ( max_mana == new_mana_count )
        {
            benthernet->tokenized_send_data.push_back( "you are at your max mana count of " + std::to_string( max_mana ) + " mana" );
        }    
        else
        {
            benthernet->tokenized_send_data.push_back( "you now have a new mana count of " + std::to_string( new_mana_count ) + " mana" );
        }    
    }
    else if ( user_action == "search card" )
    {
        std::string new_card;
        int new_card_count;
        int mana_count = all_active_players.at( username )->get_mana_count();
        
        if ( mana_count < 3 )
        {
            benthernet->tokenized_send_data.push_back( "you only have " + std::to_string( mana_count ) + " mana, you should have at least 3 . . ." );
        }
        else
        {
            mana_count = all_active_players.at( username )->search_card( card_files, &new_card, &new_card_count );
            benthernet->tokenized_send_data.push_back( "you found a " + new_card + " card! you now have " + std::to_string( new_card_count ) + " of these cards . . ." );
        }
    }
    else
    {
        benthernet->tokenized_send_data.push_back( "invalid action" );
    }    
}    

