//-----------------------------------------------
//Source code file for the network_handle header
//-----------------------------------------------

#include "network.hpp"

void network::tokenize_benthernet_base_string( std::string base_string )
{
    std::string intermediate;
    std::stringstream tokenizer_stream = std::stringstream( base_string );

    this->tokenized_message_list.clear(); //make sure no previous tokenize data is pressent
    while( getline( tokenizer_stream, intermediate, '>') )
    {
        if ( intermediate != "tcg?" ) //cut of the benternet token
        {
            this->tokenized_message_list.push_back( intermediate );
        }
    }
}

network::network()
{
    //initialize a new connection with benternet
    this->pusher.connect( "tcp://benternet.pxl-ea-ict.be:24041" );
    this->subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" );
    this->subscriber.setsockopt( ZMQ_SUBSCRIBE, "tcg?>", 5 );
}

//discription:  send a single string message
//
//arguments:    message, the string to send under the form ">*message*"
void network::send_string( std::string message )
{
    std::string send_data = "tcg!" + message + '>';
    this->pusher.send( zmq::message_t( send_data ) );
}

//discription:  sends a message to the benternet broker
//
//arguments:    message, the tokenized message to be send
//              each token will be seperated by '>'
void network::send_list( std::vector<std::string> message_list )
{
    std::string send_data = ""; //set the base string

    for ( std::string data : message_list )
    {
        send_data += '>' + data; //append all the data
    }

    #if DEBUG
        std::cout << "send data: " << send_data << "\n\r";
    #endif
    
    send_string( send_data ); //actualy send the message
}

//discription:  receives a string from benternet
//
//arguments:    message, a pointer to where to store the recievced message
//
//warnings:     blocking
void network::basic_receive( std::string *message )
{
    zmq::message_t received_data( 1024 ); //set buffer size of 1024

    #if DEBUG
        std::cout << "waiting\n\r";
    #endif


    this->subscriber.recv( received_data );
    
    #if DEBUG
        std::cout << "data received\n\r";
    #endif

    *message = received_data.to_string();
    
    #if DEBUG
        std::cout << "received message: " << *message << "\n\r";
    #endif
}

//discription:  waits for the subscribed message to be received and tokenizes it
//              if the subscribed message isn't conform the expected messages
//              the recv loop will be rerun until a conform message is received
//
//arguments:    message_list, a pointer to the vector that will contain the tokenized data that got received
//
//warnings:     blocking
void network::safe_receive( std::vector<std::string> *message_list )
{
    std::string data_base_str;

    //get and tokenenize received data
    while ( true )
    {
        basic_receive( &data_base_str );
        tokenize_benthernet_base_string( data_base_str );

        //check if the tokenized data is conform the expected message
        if ( this->tokenized_message_list.size() >= 2 )
        {
            message_list->clear();
            *message_list = this->tokenized_message_list;
            break;
        }

        //error scenario
        this->send_string( ">invalide command, use more arguments instead" );
    }
    

    #if DEBUG
        for( int i = 0; i < message_list->size(); ++i )
            std::cout << "tok " << std::dec << i << ": " << message_list->at( i ) << "\n\r";
    #endif
}

//discription:  check wheter or not the service is connected to benternet
//
//arguments:    void
bool network::connected()
{
    return this->subscriber.connected() && this->pusher.connected();
}
