// B99292359FFD910ED13A7E6C7F9705B8742F0D79

/*

    A utility file containing the member function definitions of the class supermarco.

*/

#include "supermarco.h"
#include <getopt.h>
#include <iostream>

// Read and process command line options.
void supermarco::getOptions(int argc, char** argv) {
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
                
            default:
                // Checks for invalid command line option.
                cerr << "Unknown command line option\n";
                exit(1);
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
    if (!outputFormat) {
        outputFormat = 'M';
    }
}

// Checks for illegal map characters when reading in data.
void supermarco::illegalMapCharacter(const char tileSymbol) {
    if ((tileSymbol != '.' && tileSymbol != '#' && tileSymbol != '!' && tileSymbol != 'S' && tileSymbol != 'C') &&
        (tileSymbol < '0' || tileSymbol > '9')) {
        // Print error message and exit 1.
        cerr << "Unknown map character\n";
        exit(1);
    }
}

// Checks for invalid coordinate positions when reading in 'list' input mode.
void supermarco::invalidCoordinate(const u_int32_t room, const u_int32_t row, const u_int32_t col) {
    // Print error message and exit 1.
    if (room < 0 || room >= numRooms) {
        cerr << "Invalid room number\n";
        exit(1);
    }
    if (row < 0 || row >= numRows) {
        cerr << "Invalid row number\n";
        exit(1);
    }
    if (col < 0 || col >= numRows) {
        cerr << "Invalid column number\n";
        exit(1);
    }
}

// Reads in input data with either 'map' mode or 'list' mode.
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
    
    if (inputFormat == 'M') {
        string oneLine; // Variable to store in a line from the input file.
        

        for (size_t i = 0; i < numRooms; ++i) {
            for (int j = 0; j < numRows; ++j) { //HAVE TO CHECK THIS UNSIGNED INTEGER COMPARISON
               
                // For each row, read in one line from the input file.
                if (getline(cin, oneLine)) {
                    
                    if (oneLine[0] == '/') { // Skip comments.
                        --j; // Account for comments and move j index back so rows dont go +1.
                    }
                    else { // Adds in tile with respect to [room][row][col] into 3D vector.
                        for (size_t k = 0; k < numRows; ++k) {
                            illegalMapCharacter(oneLine[k]); // Check error.
                            temp.symbol = oneLine[k]; // Set tile symbol.
                            
                            // If the tile is the starting location.
                            if (temp.symbol == 'S') {
                                // Add it into the deque.
                                location startLocation = {static_cast<u_int32_t>(i), static_cast<u_int32_t>(j), static_cast<u_int32_t>(k)};
                                routingScheme.push_back(startLocation);
                                ++tilesDiscovered; // Start counts as a discovered tile.
                                temp.isDiscovered = 1; // Start location has been discovered.
                            }
                            
                            castleMap[i][j][k] = temp;
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
        u_int32_t roomNumber;
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
                    location startLocation = {roomNumber, rowNumber, colNumber};
                    routingScheme.push_back(startLocation);
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
        
        char room = currentLocation.room;
        char row = currentLocation.row;
        char col = currentLocation.col;
        
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

bool supermarco::checkSolution() {
    if (!isSolution) {
        cout << "No solution, " << tilesDiscovered << " tiles discovered.\n";
        return 0;
    }
    else {
        return 1;
    }
}

void supermarco::printOutput() {
    if (outputFormat == 'M') {
        mapOutput();
    }
    else {
        listOutput();
    }
}

void supermarco::mapOutput() {
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

void supermarco::listOutput() {
    cout << "Path taken:\n";
    while (backtrace.size() != 1) {
        cout << "(" << backtrace.top().room << "," << backtrace.top().row << "," << backtrace.top().col << ","
        << castleMap[backtrace.top().room][backtrace.top().row][backtrace.top().col].directionTravelled << ")\n";
        
        backtrace.pop();
    }
}
