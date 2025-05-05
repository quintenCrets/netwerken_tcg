//--------------------------------------------
//Source code file for the file_handle header
//--------------------------------------------

#include "file_names.hpp"

file_names::file_names( std::string file_extension, std::string location)
{
    this->file_extension = file_extension;
    this->all_files_path = location;
    update_all_file_names(); //set all_file_names for the first time
}

void file_names::update_all_file_names()
{
    //make sure no previous data is present
    this->all_file_names.clear();

    for ( std::filesystem::path p : std::filesystem::directory_iterator( this->all_files_path ) )
    {
        if ( p.extension().string() == this->file_extension )
        {
            std::string file_name_with_extension = p.filename().string();
            std::string file_name = file_name_with_extension.substr( 0, ( file_name_with_extension.length() - file_extension.length() ) ); 
            this->all_file_names.push_back( file_name );
        }
    }

    #if DEBUG_GENERAL
        std::cout << "Found " << this->all_file_names.size() << " " << this->file_extension << " files.\n\r";
    #endif
}

void file_names::print_all_file_names()
{
    for ( std::string name : this->all_file_names )
    {
        std::cout << " -" << name << "\n\r";
    }
}

std::string file_names::get_random_file_name()
{
    return all_file_names.at( rand() % all_file_names.size() );
}

void file_names::get_all_file_names( std::vector<std::string> *all_file_names )
{
    //make sure no pre-existing data is pressent
    all_file_names->clear();

    for ( std::string name : this->all_file_names )
    {
        //copy all names in the new vector
        all_file_names->push_back( name );
    }
}

