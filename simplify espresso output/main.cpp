//
//  main.cpp
//  simplify espresso output
//
//  Created by Aaron Knoll on 10/30/18.
//  Copyright © 2018 Aaron Knoll. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, const char * argv[]) {
      // line of espresso output file
      string spaceDelimitedString = ".ilb D_3 D_2 D_1 D_0 N D Q N1 N2 N3 N4 D1 D2";
      // create a more flexible structure for storing the espresso output
      stringstream s(spaceDelimitedString);
      
      vector<string> lineItems;
      string tempWord;
      while (s >> tempWord) {
            lineItems.push_back(tempWord);
      }
      
      // test first word to determine context
      if(!lineItems.at(0).compare(".ilb")){
            cout << lineItems.at(0) << " ";
            lineItems.erase(lineItems.begin());
      } else if (!lineItems.at(0).compare(".ob")){
            
      }
      
      
      cout << lineItems.size() << endl;
      cout << spaceDelimitedString;
      return 0;
}
