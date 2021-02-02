// B99292359FFD910ED13A7E6C7F9705B8742F0D79

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
                                {"queue",  no_argument,       nullptr, 'q' },
                                {"output", required_argument, nullptr, 'o' },
                                {"help",   no_argument,       nullptr, 'h' },
                                {nullptr,  0,                 nullptr, '\0'}};
    

    
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
    // check to make sure an output format has been selected.
    // If one has not been selected, we will default to
    // map output.
    if (!outputFormat) {
        outputFormat = 'M';
    }

    
    // If isStack is not specified, then we exit
    // while printing an error message
    if (isStack != 0 && isStack != 1)  {//FIXXX THISSSSSSSS CUZ ITS TRUE OR FALSEEEE
        cerr << "Stack or queue must be specified\n";
        exit(1);
    }
}

void supermarco::readData() {
    
    // Read in the first three rows to find the
    // input format, number of rooms, and size of each room.
    // Also consume the new line after the integer.
    cin >> inputFormat >> ws;
    cin >> numRooms >> ws;
    cin >> numRows >> ws;
    
    // Reserve the exact amount of memory needed
    // to fit the number of rooms and their size
    castleMap.resize(numRooms, vector<vector<tile>>(numRows, vector<tile>(numRows)));
    
    // ----------------------------------------------------------------------------
    //                               MAP INPUT MODE
    // ----------------------------------------------------------------------------
    
    if (inputFormat == 'M') { // Map input mode
        
        for (size_t i = 0; i < numRooms; ++i) { // for i
            for (size_t j = 0; j < numRows; ++j) { // for j
               
                string oneLine;
                // For each row, read in one line from the input file
                while (getline(cin, oneLine)) {
                    

                    if (oneLine[0] == '/') { // Skip comments.
                    }
                    else { // Adds in tile with respect to [room][row][col] into 3D vector
                        for (size_t k = 0; k < numRows; ++k) { //for k
                            tile temp = {oneLine[static_cast<u_int32_t>(k)], static_cast<u_int32_t>(i), static_cast<u_int32_t>(j), static_cast<u_int32_t>(k)};
                            
                            castleMap[static_cast<int>(i)][static_cast<int>(j)][static_cast<int>(k)] = temp;
                        } // for k
                    }
                } // while loop
            } // for j
        } // for i
    }
    
    // ----------------------------------------------------------------------------
    //                               LIST INPUT MODE
    // ----------------------------------------------------------------------------
    
    else {
        
        // Variables to store the coordinates
        u_int32_t roomNumber;
        u_int32_t rowNumber;
        u_int32_t colNumber;
        char tileSymbol;
        
        // Variables to skip over comments and other unneeded characters ['(', ',', etc.]
        string comments;
        char junk;
        
        // Read in first char of line to determine if it is a comment
        while (cin >> junk) {
            if (junk != '/') { // If it is not a comment, proceed to read the rest of the line
                cin >> roomNumber >> junk >> rowNumber >> junk >> colNumber >> junk >> tileSymbol >> junk;
                
                // Adds in tile with respect to [room][row][col] into 3D vector
                tile temp = {tileSymbol, roomNumber, rowNumber, colNumber};
                castleMap[roomNumber][rowNumber][colNumber] = temp;
            }
            else { // Reads line to skip over comments or consumes remaining '\n\' at the end of each line
                getline(cin, comments);
            }
        } // while loop
    }



    // If we didn't read in any data, throw an error.
    if (!numRooms)
        throw runtime_error("No data was read in! Refer to the help option to see program usage.");
}

void supermarco::routing() {
    
}
