// B99292359FFD910ED13A7E6C7F9705B8742F0D79

/*

    A utility file containing the declaration of the class supermarco.

*/

#include <string>
#include <vector>
#include <stack>
#include <deque>

using namespace std;


class supermarco {
public:
    
    void getOptions(int argc, char** argv);
    
    void illegalMapCharacter(const char tileSymbol);
    
    void invalidCoordinate(const u_int32_t room, const u_int32_t row, const u_int32_t col);
    
    void readData();
    
    void routing();
    
    bool checkSolution(); // REMEMBER THIS. SHOULD BE DONE BEFORE EVEN BACKTRACING
    
    void backtracing();
    
    void printOutput();
    
    void mapOutput();
    
    void listOutput();
    
    
private:

    bool isStack;
    char outputFormat;
    char inputFormat;
    
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
    
    u_int32_t numRooms;
    u_int32_t numRows;
    
    u_int32_t tilesDiscovered = 0;
    
    bool isSolution = 0;
    
    stack<location> backtrace;
    
    
};
