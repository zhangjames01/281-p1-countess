// B99292359FFD910ED13A7E6C7F9705B8742F0D79

/*

    A utility file containing the declaration of the class supermarco.

*/

#include <stdio.h>
#include <string>
#include <vector>
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
        //add more stuff later
        
        u_int32_t room = 0;
        u_int32_t row = 0;
        u_int32_t col = 0;
        
        char predecessor;
        
        bool isDiscovered;
    }; //make a different struct for the deque?
    
    vector<vector<vector<tile>>> castleMap;
    
    
    
    deque<tile> routingScheme;
    
    u_int32_t numRooms;
    u_int32_t numRows;
    
    bool isSolution;
    
    
};
