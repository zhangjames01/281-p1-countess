// Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79

/*

    A utility file containing the member function definitions of the class supermarco.

*/

#include "supermarco.h"
#include <getopt.h>
#include <iostream>

// Read and process command line options.
void supermarco::getOptions(int argc, char** argv) {
    int option_index = 0, option = 0;
    
    // Don't display getopt error messages about options
    opterr = false;

    
    // use getopt to find command line options
    struct option longOpts[] = {{"stack",  no_argument,       nullptr, 's' },
                                {"queue",  no_argument,       nullptr, 'q'},
                                {"output", required_argument, nullptr, 'o'},
                                {"help",   no_argument,       nullptr, 'h' },
                                {nullptr,  0,                 nullptr, '\0' }};
    

    
    while ((option = getopt_long(argc, argv, "sqho:", longOpts, &option_index)) != -1) {
        switch (option) {
            case 's':
                isStack = 1;
                break;

            case 'h':
                cout << "Usage: " << argv[0] << " [-s -o M|L]|-h\n"
                     << "This program helps you rescue the countess in Super Marco\n";
                exit(0);
                
            case 'q':
                isStack = 0;
                break;
                
            case 'o':
                outputFormat = *optarg;
                break;
        }
        
        //HOW DO I CHECK FOR MULTIPLE STACK QUEQ ARGUEMENT ERRORS
        //HOW DO I CHECK FOR NO ARGUMENTS PASSED
        //HOW DO I CHECK FOR UNKNOWN ARGUMENTS
        //3D VECTOR CLARIFICATION
    }
    
    // After all the options have been processed,
    // check to make sure a sorting policy has been selected.
    // If one has not been selected, we will default to
    // sorting by song title (name).
    if (!outputFormat)
        outputFormat = 'M';

    
    // If num_print is still 0, then we are just
    // wasting time sorting, so throw an error.
    if (!isStack)
        cerr << "Stack or queue must be specified\n";
        exit(1);
}

void supermarco::readData() {
    tile tile;
    // Read in the first number to find the number
    // of songs in the file.
    // Also consume the new line after the integer.
    cin >> inputFormat >> ws;
    cin >> numRooms >> ws;
    cin >> numRows >> ws;
    
    //may not need numcols variable
    numCols = numRows;
    
    if (inputFormat == 'M') {
        
    }

    // Reserve the exact amount of memory needed
    // to fit all the songs.
    music.reserve(size_t(num_songs));
    
    string oneLine;

    // Read to the end of the CSV file.
    while (getline(cin, oneLine, ',')) {

        // Skip comments.
        if (oneLine[0] == '/') {
            size_t pos = 0;

            // If there is a newline, we have consumed
            // part of a song, so adjust the string.
            if ((pos = oneLine.find_last_of('\n')) != string::npos)
                // Need a +1 here to move past the \n
                oneLine = oneLine.substr(pos + 1);

            // Otherwise just grab the rest of the line.
            else {
                getline(cin, oneLine);
                continue;
            }
        }
        
        // Get the rest of the line.
        getline(cin, oneLine, ',');
        cin >> song.plays >> ws;
        
        // Put the song into the music list.
        for (size_t i = 0; i < numRows; ++i) {
            tile.symbol = oneLine[static_cast<int>(i)];
            
        }
    }

    // If we didn't read in any data, throw an error.
    if (!music.size())
        throw runtime_error("No data was read in! Refer to the help option to see program usage.");
}
