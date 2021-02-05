// B99292359FFD910ED13A7E6C7F9705B8742F0D79

/*

    A utility file containing the declaration of the class Castle.

*/

#include <string>
#include <vector>
#include <stack>
#include <deque>
#include <iostream>

using namespace std;


class Castle {
public:
    
    
    // ----------------------------------------------------------------------------
    //                           Command Line Options
    // ----------------------------------------------------------------------------
    void getOptions(int argc, char** argv);
    
    
    // ----------------------------------------------------------------------------
    //                               Reading Input
    // ----------------------------------------------------------------------------
    // Checks for illegal map characters when reading in data.
    void illegalMapCharacter(const char tileSymbol) {
        if ((tileSymbol != '.' && tileSymbol != '#' && tileSymbol != '!' && tileSymbol != 'S' && tileSymbol != 'C') &&
            (tileSymbol < '0' || tileSymbol > '9')) {
            // Print error message and exit 1.
            cerr << "Unknown map character\n";
            exit(1);
        }
    }
    // Checks for invalid coordinate positions when reading in 'list' input mode.
    void invalidCoordinate(const u_int32_t room, const u_int32_t row, const u_int32_t col) {
        u_int32_t zero = 0;
        // Print error message and exit 1.
        if (room < zero || room >= numRooms) {
            cerr << "Invalid room number\n";
            exit(1);
        }
        if (row < zero || row >= numRows) {
            cerr << "Invalid row number\n";
            exit(1);
        }
        if (col < zero || col >= numRows) {
            cerr << "Invalid column number\n";
            exit(1);
        }
    }
    void readData();
    
    
    // ----------------------------------------------------------------------------
    //                             Searching Scheme
    // ----------------------------------------------------------------------------
    void routing();
    
    
    // ----------------------------------------------------------------------------
    //                            Is there a solution?
    // ----------------------------------------------------------------------------
    // Checks if there is a solution to the castle. If not, return with message.
    bool checkSolution() {
        if (!isSolution) {
            cout << "No solution, " << tilesDiscovered << " tiles discovered.\n";
            return 0;
        }
        else {
            return 1;
        }
    }
    
    
    // ----------------------------------------------------------------------------
    //                                Backtracing
    // ----------------------------------------------------------------------------
    void backtracing();
    
    
    // ----------------------------------------------------------------------------
    //                              Printing Output
    // ----------------------------------------------------------------------------
    // Determines which output mode to print in.
    void printOutput() {
        if (outputFormat == 'M') {
            mapOutput();
        }
        else {
            listOutput();
        }
    }
    void mapOutput();
    void listOutput();
    
    
private:
    
    char outputFormat = ' ';
    char inputFormat = ' ';
    bool isStack = 0;
    bool isSolution = 0;
    
    uint16_t numRooms = 0;
    u_int32_t numRows = 0;
    u_int32_t tilesDiscovered = 0;
    
    struct location {
        uint16_t room = 0;
        u_int32_t row = 0;
        u_int32_t col = 0;
        char directionTravelled = ' ';
    };
    
    struct tile {
        char symbol = '.';
        bool isDiscovered = 0;
        char predecessor = ' ';
        char preceedingRoom = ' '; 
    };
    
    location startLocation;
    
    vector<vector<vector<tile>>> castleMap;
    stack<location> backtrace;
};
