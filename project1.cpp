//  B99292359FFD910ED13A7E6C7F9705B8742F0D79
//  project1.cpp
//  p1-countess
//
//  Created by James Zhang on 1/31/21.
//

#include "supermarco.h"
#include "xcode_redirect.hpp"

// ----------------------------------------------------------------------------
//                               Driver
// ----------------------------------------------------------------------------




int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false); // Speeds up I/O
    xcode_redirect(argc, argv); // Xcode redirect
    
    supermarco play; // Instantiate an instant of the supermarco game
    
    try {
        play.getOptions(argc, argv);
        play.readData();
        play.routing();
        play.backtracing();
        if (!play.checkSolution()) {
            return 0;
        }
        else {
            play.printOutput();
            return 0;
        }
        
    }

    // Catch runtime_errors, print the message, and exit the
    // program with a non-zero status.
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // All done!
    return 0;
}
