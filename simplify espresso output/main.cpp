//
//  main.cpp
//  simplify espresso output
//
//  Created by Aaron Knoll on 10/30/18.
//  Copyright © 2018 Aaron Knoll. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, const char * argv[]) {
      
      string inputFileName = argv[1];
//      cout << inputFileName << endl;
      ifstream inputFileStream(inputFileName.c_str());
      string inputLine;
      string dataToBeOutputted;
      int numInputs, numOutputs, numProducts;
      vector<string> inputLabels;
      vector<vector<string>> outputLabels;
      while (getline(inputFileStream, inputLine)) {
            // line of espresso output file
            // create a more flexible structure for storing the espresso output
            stringstream s1(inputLine);
            
            string tempWord, firstWord;
            s1 >> firstWord;
            
            // test first word to determine context
            if (!firstWord.compare(".i")) {
                  s1 >> numInputs;
            }
            else if (!firstWord.compare(".o")) {
                  s1 >> numOutputs;
            }
            else if (!firstWord.compare(".ilb")) {
                  while (s1 >> tempWord) {
                        inputLabels.push_back(tempWord);
                  }
            }
            else if (!firstWord.compare(".ob")) {
                  while (s1 >> tempWord) {
                        vector<string> outputLabel;
                        outputLabel.push_back(tempWord);
//                        cout << outputLabel.at(0) << endl;
                        outputLabels.push_back(outputLabel);
//                        cout << outputLabel.size() << endl;
//                        cout << outputLabels.size() << endl;
                  }
            }
            else if (!firstWord.compare(".p")) {
                  s1 >> numProducts;
                  
                  for (int i = 0; i < numProducts; i++) {
                        getline(inputFileStream, inputLine);
                        stringstream s2(inputLine);
                        string inputs, outputs;
                        s2 >> inputs >> outputs;
                        string productForCompilation, prettyProduct;
                        for (int j = 0; j < numInputs; j++) {
                              if (inputs.at(j) == '1') {
                                    productForCompilation += inputLabels.at(j) + " * ";
                              }
                              else if (inputs.at(j) == '0') {
                                    productForCompilation += inputLabels.at(j) + "' * ";
                              }
                        }
                        dataToBeOutputted += to_string(i + 1) + ") " + productForCompilation.substr(0, productForCompilation.size() - 3) + "\n";
                        for (int j = 0; j < numOutputs; j++) {
                              if (outputs.at(j) == '1') {
                                    outputLabels[j].push_back(to_string(i + 1));
                              }
                        }
                  }
                  string prettyOutputs;
                  for (int i = 0; i < numOutputs; i++) {
                        prettyOutputs += outputLabels[i][0] + ":";
                        for (int j = 1; j < outputLabels[i].size(); j++) {
                              prettyOutputs += " " + outputLabels[i][j];
                        }
                        prettyOutputs += "\n";
                  }
                  dataToBeOutputted = prettyOutputs + "\n" + dataToBeOutputted;
                  cout << dataToBeOutputted;
                  
                  string outputFileName = argv[2];
//                  cout << outputFileName << endl;
                  ofstream outputFileStream(outputFileName.c_str());
                  outputFileStream << dataToBeOutputted;
                  return 0;
            }
      }
      
      return 1;
}
