//----------------------------------------------------------------------
// Network header file
//  contains class prototype for the network handle object used in main
//----------------------------------------------------------------------

#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <Windows.h>
#include <zmq.hpp>

class network
{
    private:
        zmq::context_t ctx = zmq::context_t( 1 );
        zmq::socket_t pusher = zmq::socket_t( ctx, ZMQ_PUSH );
        zmq::socket_t subscriber = zmq::socket_t( ctx, ZMQ_SUB );
        std::vector<std::string> tokenized_message_list;
        
        void tokenize_benthernet_base_string( std::string base_string );
        void send_string( std::string message );
        void send_list( std::vector<std::string> message_list );
        void basic_receive( std::string *message );
        void safe_receive( std::vector<std::string> *message_list );
    public:
        std::vector<std::string> tokenized_receive_data;
        std::vector<std::string> tokenized_send_data;
        
        network();
        ~network() {};

        void push_tokenized_send_data() { send_list( tokenized_send_data ); };
        void wait_for_tokenized_receive_data() { safe_receive( &tokenized_receive_data ); };
        bool connected();
};

#endif
