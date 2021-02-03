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
    tile temp; // Temporary tile struct to add into 3D vector
    
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
        string oneLine;
        for (size_t i = 0; i < numRooms; ++i) { // for i
            for (int j = 0; j < numRows; ++j) { // for j
               
                // For each row, read in one line from the input file
                if (getline(cin, oneLine)) {
                    
//BECAUSE '.' TILES ARE INITIALIZED, I CAN CHECK THAT USING AN IF STATEMENT AND DO NOTHING, SAVING TIME
                    if (oneLine[0] == '/') { // Skip comments.
                        --j; // Account for comments and move j index back so rows dont go +1
                    }
                    else { // Adds in tile with respect to [room][row][col] into 3D vector
                        for (size_t k = 0; k < numRows; ++k) { //for k
                            temp.symbol = oneLine[static_cast<int>(k)];
                            
                            if (temp.symbol == 'S') { // If the tile is the start, add it into the deque
                                location startLocation = {static_cast<u_int32_t>(i), static_cast<u_int32_t>(j), static_cast<u_int32_t>(k)};
                                routingScheme.push_back(startLocation);
                                ++tilesDiscovered;
                                
                                temp.isDiscovered = 1; // Start location has been discovered
                            }
                            
                            castleMap[static_cast<int>(i)][static_cast<int>(j)][static_cast<int>(k)] = temp;
                            temp.isDiscovered = 0; // THIS RESETS THE VARIABLE, FIGURE OUT IF TEMP SHOULD BE OUTSIDE OR INSIDE WHILE LOOP
                            
                        } // for k
                    }
                } // if for getline
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
                temp.symbol = tileSymbol;
                
                if (tileSymbol == 'S') { // If the tile is the start, add it into the deque
                    location startLocation = {roomNumber, rowNumber, colNumber};
                    routingScheme.push_back(startLocation);
                    ++tilesDiscovered;
                    temp.isDiscovered = 1; // Start location has been discovered
                }
                
                // Adds in tile with respect to [room][row][col] into 3D vector
                castleMap[roomNumber][rowNumber][colNumber] = temp;
                temp.isDiscovered = 0; // THIS RESETS THE VARIABLE, FIGURE OUT IF TEMP SHOULD BE OUTSIDE OR INSIDE WHILE LOOP
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
    location currentLocation;
    while(!routingScheme.empty()) { // Routing Scheme Loop
        
        if (isStack) { // Remove position in back of container if stack
            currentLocation = routingScheme.back();
            routingScheme.pop_back();
        }
        else { // Remove position in front of container if queue
            currentLocation = routingScheme.front();
            routingScheme.pop_front();
        }
        
        // If currentlocation at [room][row][col] is a pipe
        if (isdigit(castleMap[currentLocation.room][currentLocation.row][currentLocation.col].symbol)) {
            u_int32_t pipeRoom = static_cast<u_int32_t>(castleMap[currentLocation.room][currentLocation.row][currentLocation.col].symbol - '0');
            if (castleMap[pipeRoom][currentLocation.row][currentLocation.col].symbol != '#' &&
                castleMap[pipeRoom][currentLocation.row][currentLocation.col].symbol != '!') { // Check if room that pipe leads to is walkable
                if (!castleMap[pipeRoom][currentLocation.row][currentLocation.col].isDiscovered) { // Check if room that pipe leads to has not been discovered
                    
                    // Set the location to discovered
                    castleMap[pipeRoom][currentLocation.row][currentLocation.col].isDiscovered = 1;
                    
                    //Set the location's predecessor
                    castleMap[pipeRoom][currentLocation.row][currentLocation.col].predecessor = 'p';
                    
                    //Set the location's preceeding room
                    castleMap[pipeRoom][currentLocation.row][currentLocation.col].preceedingRoom = currentLocation.room;
                    
                    // Add it to the deque
                    location pipe = {pipeRoom, currentLocation.row, currentLocation.col};
                    routingScheme.push_back(pipe);
                    ++tilesDiscovered;
                    
                    // If we added the Countess' location, stop searching and add to the backtrace stack
                    if (castleMap[pipe.room][pipe.row][pipe.col].symbol == 'C') {
                        isSolution = 1;
                        backtrace.push(pipe);
                        break;
                    }
                }
            }
            
        }
        // need to know if it exists, then if its walkable, then if its discovered
        else { // Else current location is not a pipe
            if (currentLocation.row - 1 != -1) { // Check if North exists (subtract row: < 0)
                if (castleMap[currentLocation.room][currentLocation.row - 1][currentLocation.col].symbol != '#' &&
                    castleMap[currentLocation.room][currentLocation.row - 1][currentLocation.col].symbol != '!') { // Check if North is walkable
                    if (!castleMap[currentLocation.room][currentLocation.row - 1][currentLocation.col].isDiscovered) { // Check if North has not been discovered
                        
                        // Set the location to discovered
                        castleMap[currentLocation.room][currentLocation.row - 1][currentLocation.col].isDiscovered = 1;
                        
                        //Set the location's predecessor
                        castleMap[currentLocation.room][currentLocation.row - 1][currentLocation.col].predecessor = 'n';
                        
                        // Add it to the deque
                        location north = {currentLocation.room, currentLocation.row - 1, currentLocation.col};
                        routingScheme.push_back(north);
                        ++tilesDiscovered;
                        
                        // If we added the Countess' location, stop searching and add to the backtrace stack
                        if (castleMap[north.room][north.row][north.col].symbol == 'C') {
                            isSolution = 1;
                            backtrace.push(north);
                            break;
                        }
                    }
                }
            }
            if (currentLocation.col + 1 != numRows) { // Check if East exists (add col: >= numCols)
                if (castleMap[currentLocation.room][currentLocation.row][currentLocation.col + 1].symbol != '#' &&
                    castleMap[currentLocation.room][currentLocation.row][currentLocation.col + 1].symbol != '!') { // Check if East is walkable
                    if (!castleMap[currentLocation.room][currentLocation.row][currentLocation.col + 1].isDiscovered) { // Check if East has not been discovered
                        
                        // Set the location to discovered
                        castleMap[currentLocation.room][currentLocation.row][currentLocation.col + 1].isDiscovered = 1;
                        
                        //Set the location's predecessor
                        castleMap[currentLocation.room][currentLocation.row][currentLocation.col + 1].predecessor = 'e';
                        
                        // Add it to the deque
                        location east = {currentLocation.room, currentLocation.row, currentLocation.col + 1};
                        routingScheme.push_back(east);
                        ++tilesDiscovered;
                        
                        // If we added the Countess' location, stop searching and add to the backtrace stack
                        if (castleMap[east.room][east.row][east.col].symbol == 'C') {
                            isSolution = 1;
                            backtrace.push(east);
                            break;
                        }
                    }
                }
            }
            if (currentLocation.row + 1 != numRows) { // Check if South exists (add row: >= numRows)
                if (castleMap[currentLocation.room][currentLocation.row + 1][currentLocation.col].symbol != '#' &&
                    castleMap[currentLocation.room][currentLocation.row + 1][currentLocation.col].symbol != '!') { // Check if South is walkable
                    if (!castleMap[currentLocation.room][currentLocation.row + 1][currentLocation.col].isDiscovered) { // Check if South has not been discovered
                        
                        // Set the location to discovered
                        castleMap[currentLocation.room][currentLocation.row + 1][currentLocation.col].isDiscovered = 1;
                        
                        //Set the location's predecessor
                        castleMap[currentLocation.room][currentLocation.row + 1][currentLocation.col].predecessor = 's';
                        
                        // Add it to the deque
                        location south = {currentLocation.room, currentLocation.row + 1, currentLocation.col};
                        routingScheme.push_back(south);
                        ++tilesDiscovered;
                        
                        // If we added the Countess' location, stop searching and add to the backtrace stack
                        if (castleMap[south.room][south.row][south.col].symbol == 'C') {
                            isSolution = 1;
                            backtrace.push(south);
                            break;
                        }
                    }
                }
            }
            if (currentLocation.col - 1 != -1) { // Check if West exists (subtract col: < 0)
                if (castleMap[currentLocation.room][currentLocation.row][currentLocation.col - 1].symbol != '#' &&
                    castleMap[currentLocation.room][currentLocation.row][currentLocation.col - 1].symbol != '!') { // Check if West is walkable
                    if (!castleMap[currentLocation.room][currentLocation.row][currentLocation.col - 1].isDiscovered) { // Check if West has not been discovered
                        
                        // Set the location to discovered
                        castleMap[currentLocation.room][currentLocation.row][currentLocation.col - 1].isDiscovered = 1;
                        
                        //Set the location's predecessor
                        castleMap[currentLocation.room][currentLocation.row][currentLocation.col - 1].predecessor = 'w';
                        
                        // Add it to the deque
                        location west = {currentLocation.room, currentLocation.row, currentLocation.col - 1};
                        routingScheme.push_back(west);
                        ++tilesDiscovered;
                        
                        // If we added the Countess' location, stop searching and add to the backtrace stack
                        if (castleMap[west.room][west.row][west.col].symbol == 'C') {
                            isSolution = 1;
                            backtrace.push(west);
                            break;
                        }
                    }
                }
            }
            
        }
    } // while loop
}

void supermarco::backtracing() {
    location currentLocation = backtrace.top(); // Sets current location to Countess

    // While loops until starting position has been added into the backtrace stack
    while (castleMap[currentLocation.room][currentLocation.row][currentLocation.col].symbol != 'S') {
        char thePredecessor = castleMap[currentLocation.room][currentLocation.row][currentLocation.col].predecessor;
        if (thePredecessor == 'n') {
            // can i change a struct's variables after declaration
            currentLocation = {currentLocation.room, currentLocation.row + 1, currentLocation.col};
            //if I came from north, then past location went
            castleMap[currentLocation.room][currentLocation.row][currentLocation.col].directionTravelled = 'n';
            backtrace.push(currentLocation);
        }
        else if (thePredecessor == 'e') {
            currentLocation = {currentLocation.room, currentLocation.row, currentLocation.col - 1};
            
            castleMap[currentLocation.room][currentLocation.row][currentLocation.col].directionTravelled = 'e';
            backtrace.push(currentLocation);
        }
        else if (thePredecessor == 's') {
            currentLocation = {currentLocation.room, currentLocation.row - 1, currentLocation.col};
            
            castleMap[currentLocation.room][currentLocation.row][currentLocation.col].directionTravelled = 's';
            backtrace.push(currentLocation);
        }
        else if (thePredecessor == 'w') {
            currentLocation = {currentLocation.room, currentLocation.row, currentLocation.col + 1};
            
            castleMap[currentLocation.room][currentLocation.row][currentLocation.col].directionTravelled = 'w';
            backtrace.push(currentLocation);
        }
        else { // PIPE
            currentLocation = {castleMap[currentLocation.room][currentLocation.row][currentLocation.col].preceedingRoom, currentLocation.row, currentLocation.col};
            
            castleMap[currentLocation.room][currentLocation.row][currentLocation.col].directionTravelled = 'p';
            backtrace.push(currentLocation);
        }
    } // while loop
            

}

void supermarco::firstOutputMode() {
    cout << "Path taken:\n";
    while (backtrace.size() != 1) {
        cout << "(" << backtrace.top().room << "," << backtrace.top().row << "," << backtrace.top().col << ","
        << castleMap[backtrace.top().room][backtrace.top().row][backtrace.top().col].directionTravelled << ")\n";
        
        backtrace.pop();
    }
}

void supermarco::secondOutputMode() {
    cout << "Start in room " << backtrace.top().room << ", row " << backtrace.top().row << ", column " << backtrace.top().col << "\n";
    
    while (backtrace.size() != 1) {
        castleMap[backtrace.top().room][backtrace.top().row][backtrace.top().col].symbol = castleMap[backtrace.top().room][backtrace.top().row][backtrace.top().col].directionTravelled;
        
        backtrace.pop();
    }
    //print out path traveled in map form
    
    for (size_t i = 0; i < numRooms; ++ i) {
        cout << "//castle room " << i << "\n";
        for (size_t j = 0; j < numRows; ++ j) {
            for (size_t k = 0; k < numRows; ++ k) {
                cout << castleMap[i][j][k].symbol;
            }
            cout << '\n';
        }
    }
}
