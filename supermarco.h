// B99292359FFD910ED13A7E6C7F9705B8742F0D79

/*

    A utility file containing the declaration of the class supermarco.

*/

#include <string>
#include <vector>
#include <stack>
#include <deque>
#include <iostream>

using namespace std;


class supermarco {
public:
    
    void getOptions(int argc, char** argv);
    
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
    
    void readData();
    
    void routing();
    
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
    
    void backtracing();
    
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
    //Reorder this to optimize memory.
    struct tile {
        char symbol = '.';
        bool isDiscovered = 0;
        char predecessor;
        char directionTravelled;
        u_int32_t preceedingRoom;
    };
    
    struct location {
        u_int32_t room = 0;
        u_int32_t row = 0;
        u_int32_t col = 0;
    };
    
    vector<vector<vector<tile>>> castleMap;
    deque<location> routingScheme;
    stack<location> backtrace;
    
    
    u_int32_t numRooms;
    u_int32_t numRows;
    u_int32_t tilesDiscovered = 0;
    
    
    char outputFormat;
    char inputFormat;
    bool isStack;
    bool isSolution = 0;
};
