//-------------------------------------------------------------------------------------
//File_handle header
//  this file contains class defenition for the file handles used in the main function
//  it is mainly used to have access to all the file names
//-------------------------------------------------------------------------------------

#ifndef FILE_HANDLE_H
#define FILE_HANDLE_H

#include <iostream>
#include <filesystem>
#include <vector>
#include <cstdlib>

class file_names
{
    private:
        std::string all_files_path;
        std::string file_extension;
        std::vector<std::string> all_file_names;

    public:
        file_names( std::string file_extension, std::string location );
        ~file_names() {};

        void update_all_file_names();
        void print_all_file_names();
        std::string get_random_file_name();
        void get_all_file_names( std::vector<std::string> *all_file_names );
};

#endif

//end of file
