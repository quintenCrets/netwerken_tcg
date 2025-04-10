//------------------------------------------------------------------------
// Trading Card Game service
//  a game that is based around collecting and fighting other cards
//------------------------------------------------------------------------
//  received messages will automatically be tokenized,
//  therefore the message will be constructed as
//  one of the two following ways:
//
//      -benternet token    -benternet token
//      -user_name token    -general_command token
//      -action token       -extra token's
//      -extra token's      ...
//      ...
//
//  the service however will cut or add the "benternet token" accordingly
//------------------------------------------------------------------------

#include "runtime_functions.hpp"

int main( void )
{
    runtime service_application;
    service_application.run();

    return 0;
}
