// Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79

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
        char symbol;
        //add more stuff late
    };
    
    vector<vector<vector<tile>>> castleMap;
    
    u_int32_t numRooms;
    u_int32_t numRows;
    u_int32_t numCols;
    
    bool isSolution;
    
    
};
