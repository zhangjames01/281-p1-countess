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
    
    void readData();
    
    void routing();
    
    void backtracing();
    
    void firstOutputMode();
    
    void secondOutputMode();
    
    
private:

    bool isStack;
    char outputFormat;
    char inputFormat;
    
    struct tile {
        char symbol = '.';
        bool isDiscovered;
        char predecessor;
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
    
    bool isSolution;
    
    stack<location> backtrace;
    
    
};
