// B99292359FFD910ED13A7E6C7F9705B8742F0D79

/*

    A utility file containing the member function definitions of the class Castle.

*/

#include "Castle.h"
#include <getopt.h>


// Read and process command line options.
void Castle::getOptions(int argc, char** argv) {
    // Use to check whether 'stack' or 'queue' argument
    // is entered more or less than one time.
    u_int8_t sqOnce = 0;
    
    int option_index = 0, option = 0;
    
    // Don't display getopt error messages about options.
    opterr = false;

    // Use getopt to find command line options.
    struct option longOpts[] = {{"stack",  no_argument,       nullptr, 's' },
                                {"queue",  no_argument,       nullptr, 'q' },
                                {"output", required_argument, nullptr, 'o' },
                                {"help",   no_argument,       nullptr, 'h' },
                                {nullptr,  0,                 nullptr, '\0'}};
    

    
    while ((option = getopt_long(argc, argv, "sqho:", longOpts, &option_index)) != -1) {
        switch (option) {
            case 's':
                // Set routing scheme to 'stack' mode.
                isStack = 1;
                ++sqOnce;
                break;

            case 'h':
                // Prints a help message describing what this program does.
                cout << "Usage: " << argv[0] << " [-s -o M|L]|-h\n"
                     << "This program helps you rescue the countess in Super Marco\n";
                exit(0);
                
            case 'q':
                // Set routing scheme to 'queue' mode.
                isStack = 0;
                ++sqOnce;
                break;
                
            case 'o':
                // Sets the output format to either 'map' or 'list'.
                outputFormat = *optarg;
                break;
        }
    }
        
    // If routing scheme is not specifed, then we exit 1
    // while printing an error message.
    if (sqOnce == 0)  {
        cerr << "Stack or queue must be specified\n";
        exit(1);
    }
    
    // If more than one routing scheme is specifed, then we exit 1
    // while printing an error message.
    if (sqOnce > 1) {
        cerr << "Stack or queue can only be specified once\n";
        exit(1);
    }
    
    // If  and output mode is not specified, default to 'map' output.
    if (outputFormat == ' ') {
        outputFormat = 'M';
    }
}

// Reads in input data with either 'map' mode or 'list' mode.
void Castle::readData() {
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
    
    if (inputFormat == 'M') {
        string oneLine; // Variable to store in a line from the input file.
        

        for (uint16_t i = 0; i < numRooms; ++i) {
            for (int j = 0; j < static_cast<int>(numRows); ++j) { //HAVE TO CHECK THIS UNSIGNED INTEGER COMPARISON
               
                // For each row, read in one line from the input file.
                if (getline(cin, oneLine)) {
                    
                    if (oneLine[0] == '/') { // Skip comments.
                        --j; // Account for comments and move j index back so rows dont go +1.
                    }
                    else { // Adds in tile with respect to [room][row][col] into 3D vector.
                        for (u_int32_t k = 0; k < numRows; ++k) {
                            illegalMapCharacter(oneLine[k]); // Check error.
                            temp.symbol = oneLine[k]; // Set tile symbol.
                            
                            // If the tile is the starting location.
                            if (temp.symbol == 'S') {
                                // Add it into the deque.
                                
                                startLocation = {static_cast<char>('0' + i), static_cast<u_int32_t>(j), k};
                                ++tilesDiscovered; // Start counts as a discovered tile.
                                temp.isDiscovered = 1; // Start location has been discovered.
                            }
                            
                            castleMap[i][static_cast<u_int32_t>(j)][k] = temp;
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
        // Variables to store the coordinates and the tile symbol.
        uint16_t roomNumber;
        u_int32_t rowNumber;
        u_int32_t colNumber;
        char tileSymbol;
        // Variables to skip over comments and other unneeded characters ['(', ',', etc.].
        string comments;
        char junk;
        
        // Read in first char of line to determine if it is a comment.
        while (cin >> junk) {
            // If it is not a comment, proceed to read the rest of the line.
            if (junk != '/') {
                // Read in the coordinate in the form of '(#,#,#,symbol)'.
                cin >> roomNumber >> junk >> rowNumber >> junk >> colNumber >> junk >> tileSymbol >> junk;
                
                illegalMapCharacter(tileSymbol); // Check error.
                invalidCoordinate(roomNumber, rowNumber, colNumber); // Check error.
                
                temp.symbol = tileSymbol; // Set tile symbol.
                
                if (tileSymbol == 'S') { // If the tile is the start, add it into the deque.
                    startLocation = {static_cast<char>('0' + roomNumber), rowNumber, colNumber};
                    ++tilesDiscovered; // Start counts as a discovered tile.
                    temp.isDiscovered = 1; // Start location has been discovered.
                }
                
                // Adds in tile with respect to [room][row][col] into 3D vector.
                castleMap[roomNumber][rowNumber][colNumber] = temp;
                temp.isDiscovered = 0; // Resets isDiscovered for next coordinate.
            }
            // Reads line to skip over comments or consumes
            // remaining '\n\' at the end of each line.
            else {
                getline(cin, comments);
            }
        } // while loop
    }
}

// Searches the castle based on a 'stack' or 'queue' routing scheme.
void Castle::routing() {
    deque<location> routingScheme;
    //Insert the start where Marco is into the routing search container.
    routingScheme.push_back(startLocation);
    location currentLocation;
    
    while(!routingScheme.empty()) { // Routing Scheme Loop
        
        if (isStack) { // Remove position in back of container if stack.
            currentLocation = routingScheme.back();
            routingScheme.pop_back();
        }
        else { // Remove position in front of container if queue.
            currentLocation = routingScheme.front();
            routingScheme.pop_front();
        }
        
        // Variables to set the current location's coordinates for code readabality.
        uint16_t room = static_cast<uint16_t>(currentLocation.room - '0');
        u_int32_t row = currentLocation.row;
        u_int32_t col = currentLocation.col;
        
        // If currentlocation at [room][row][col] is a pipe.
        if (isdigit(castleMap[room][row][col].symbol)) {
            if (castleMap[room][row][col].symbol - '0' >= 0 && castleMap[room][row][col].symbol - '0' < static_cast<int8_t>(numRooms)) { // Checks if pipe leads to an existing room.
                // Sets the room that pipe leads to using ASCII operation.
                uint16_t pipeRoom = static_cast<uint16_t>(castleMap[room][row][col].symbol - '0');
                
                // Check if room that pipe leads to is walkable.
                if (castleMap[pipeRoom][row][col].symbol != '#' &&
                    castleMap[pipeRoom][row][col].symbol != '!') {
                    // Check if room that pipe leads to has not been discovered.
                    if (!castleMap[pipeRoom][row][col].isDiscovered) {
                        
                        // Set the location to discovered.
                        castleMap[pipeRoom][row][col].isDiscovered = 1;
                        
                        //Set the location's predecessor.
                        castleMap[pipeRoom][row][col].predecessor = static_cast<char>('0' + room);
                        
                        // Add it to the deque.
                        location pipe = {static_cast<char>('0' + pipeRoom), row, col};
                        routingScheme.push_back(pipe);
                        ++tilesDiscovered;
                        
                        // If we added the Countess' location,
                        // stop searching and add to the backtrace stack.
                        if (castleMap[static_cast<uint16_t>(pipe.room - '0')][pipe.row][pipe.col].symbol == 'C') {
                            isSolution = 1;
                            backtrace.push(pipe);
                            break;
                        }
                    }
                }
            }
        }
        
        else { // Else current location is not a pipe.
            if (row - 1 != static_cast<u_int>(-1)) { // Check if North exists (subtract row: < 0).
                if (castleMap[room][row - 1][col].symbol != '#' &&
                    castleMap[room][row - 1][col].symbol != '!') { // Check if North is walkable.
                    if (!castleMap[room][row - 1][col].isDiscovered) { // Check if North has not been discovered.
                        
                        // Set the location to discovered.
                        castleMap[room][row - 1][col].isDiscovered = 1;
                        
                        //Set the location's predecessor.
                        castleMap[room][row - 1][col].predecessor = 'n';
                        
                        // Add it to the deque.
                        location north = {static_cast<char>('0' + room), row - 1, col};
                        routingScheme.push_back(north);
                        ++tilesDiscovered;
                        
                        // If we added the Countess' location,
                        // stop searching and add to the backtrace stack
                        if (castleMap[static_cast<uint16_t>(north.room - '0')][north.row][north.col].symbol == 'C') {
                            isSolution = 1;
                            backtrace.push(north);
                            break;
                        }
                    }
                }
            }
            if (col + 1 != numRows) { // Check if East exists (add col: >= numCols).
                if (castleMap[room][row][col + 1].symbol != '#' &&
                    castleMap[room][row][col + 1].symbol != '!') { // Check if East is walkable.
                    if (!castleMap[room][row][col + 1].isDiscovered) { // Check if East has not been discovered.
                        
                        // Set the location to discovered.
                        castleMap[room][row][col + 1].isDiscovered = 1;
                        
                        //Set the location's predecessor.
                        castleMap[room][row][col + 1].predecessor = 'e';
                        
                        // Add it to the deque.
                        location east = {static_cast<char>('0' + room), row, col + 1};
                        routingScheme.push_back(east);
                        ++tilesDiscovered;
                        
                        // If we added the Countess' location,
                        // stop searching and add to the backtrace stack.
                        if (castleMap[static_cast<uint16_t>(east.room - '0')][east.row][east.col].symbol == 'C') {
                            isSolution = 1;
                            backtrace.push(east);
                            break;
                        }
                    }
                }
            }
            if (row + 1 != numRows) { // Check if South exists (add row: >= numRows).
                if (castleMap[room][row + 1][col].symbol != '#' &&
                    castleMap[room][row + 1][col].symbol != '!') { // Check if South is walkable.
                    if (!castleMap[room][row + 1][col].isDiscovered) { // Check if South has not been discovered.
                        
                        // Set the location to discovered.
                        castleMap[room][row + 1][col].isDiscovered = 1;
                        
                        //Set the location's predecessor.
                        castleMap[room][row + 1][col].predecessor = 's';
                        
                        // Add it to the deque.
                        location south = {static_cast<char>('0' + room), row + 1, col};
                        routingScheme.push_back(south);
                        ++tilesDiscovered;
                        
                        // If we added the Countess' location,
                        // stop searching and add to the backtrace stack.
                        if (castleMap[static_cast<uint16_t>(south.room - '0')][south.row][south.col].symbol == 'C') {
                            isSolution = 1;
                            backtrace.push(south);
                            break;
                        }
                    }
                }
            }
            if (col - 1 != static_cast<u_int>(-1)) { // Check if West exists (subtract col: < 0).
                if (castleMap[room][row][col - 1].symbol != '#' &&
                    castleMap[room][row][col - 1].symbol != '!') { // Check if West is walkable.
                    if (!castleMap[room][row][col - 1].isDiscovered) { // Check if West has not been discovered.
                        
                        // Set the location to discovered.
                        castleMap[room][row][col - 1].isDiscovered = 1;
                        
                        //Set the location's predecessor.
                        castleMap[room][row][col - 1].predecessor = 'w';
                        
                        // Add it to the deque.
                        location west = {static_cast<char>('0' + room), row, col - 1};
                        routingScheme.push_back(west);
                        ++tilesDiscovered;
                        
                        // If we added the Countess' location,
                        // stop searching and add to the backtrace stack.
                        if (castleMap[static_cast<uint16_t>(west.room - '0')][west.row][west.col].symbol == 'C') {
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

// Records the path that Marco took to get to the Countess.
void Castle::backtracing() {
    location currentLocation = backtrace.top(); // Sets current location to Countess.
    

    // While loops until starting position has been added into the backtrace stack.
    while (castleMap[static_cast<uint16_t>(currentLocation.room - '0')][currentLocation.row][currentLocation.col].symbol != 'S') {
        char thePredecessor = castleMap[static_cast<uint16_t>(currentLocation.room - '0')][currentLocation.row][currentLocation.col].predecessor;
        if (thePredecessor == 'n') { // North.
            // Change currentLocation to the room south.
            currentLocation = {currentLocation.room, currentLocation.row + 1, currentLocation.col, 'n'};
            // Set that room's direction travelled to north.
            backtrace.push(currentLocation);
        }
        else if (thePredecessor == 'e') { // East.
            // Change currentLocation to the room west.
            currentLocation = {currentLocation.room, currentLocation.row, currentLocation.col - 1, 'e'};
            // Set that room's direction travelled to east.
            backtrace.push(currentLocation);
        }
        else if (thePredecessor == 's') { // South.
            // Change currentLocation to the room north.
            currentLocation = {currentLocation.room, currentLocation.row - 1, currentLocation.col, 's'};
            // Set that room's direction travelled to south.
            backtrace.push(currentLocation);
        }
        else if (thePredecessor == 'w') { // West.
            // Change currentLocation to the room east.
            currentLocation = {currentLocation.room, currentLocation.row, currentLocation.col + 1, 'w'};
            // Set that room's direction travelled to west.
            backtrace.push(currentLocation);
        }
        else { // Pipe.
            // Change currentLocation to the same tile in the room from pipe.
            currentLocation = {castleMap[static_cast<uint16_t>(currentLocation.room - '0')][currentLocation.row][currentLocation.col].predecessor, currentLocation.row, currentLocation.col, 'p'};
            // Set that room's direction travelled to pipe.
            backtrace.push(currentLocation);
        }
    } // while loop
}

// Prints path using map output.
void Castle::mapOutput() {
    cout << "Start in room " << backtrace.top().room << ", row " << backtrace.top().row << ", column " << backtrace.top().col << "\n";
    
    while (backtrace.size() != 1) {
            castleMap[static_cast<uint16_t>(backtrace.top().room - '0')][backtrace.top().row][backtrace.top().col].symbol = backtrace.top().directionTravelled;
            
            backtrace.pop();
        }

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

// Print path using list output.
void Castle::listOutput() {
    cout << "Path taken:\n";
    while (backtrace.size() != 1) {
        cout << "(" << backtrace.top().room << "," << backtrace.top().row << "," << backtrace.top().col << ","
        << backtrace.top().directionTravelled << ")\n";
        
        backtrace.pop();
    }
}
