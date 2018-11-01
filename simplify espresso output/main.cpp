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
#include <unordered_map>
#include <algorithm>

using namespace std;

int main(int argc, const char * argv[]) {
      
      ifstream inputFileStream(argv[1]);
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
                        outputLabels.push_back(outputLabel);
                  }
            }
            else if (!firstWord.compare(".p")) {
                  s1 >> numProducts;
                  
                  vector<vector<string>> products;
                  for (int i = 0; i < numProducts; i++) {
                        getline(inputFileStream, inputLine);
                        string inputs, outputs;
                        stringstream s2(inputLine);
                        s2 >> inputs >> outputs;
                        vector<string> temp;
                        string prettyProduct;
                        for (int j = 0; j < numInputs; j++) {
                              if (inputs.at(j) == '1') {
                                    temp.push_back(inputLabels.at(j));
                                    prettyProduct += inputLabels.at(j) + " * ";
                              }
                              else if (inputs.at(j) == '0') {
                                    temp.push_back(inputLabels.at(j) + "'");
                                    prettyProduct += inputLabels.at(j) + "' * ";
                              }
                        }
                        products.push_back(temp);
                        dataToBeOutputted += to_string(i + 1) + ") " + prettyProduct.substr(0, prettyProduct.size() - 3) + "\n";
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
                  cout << dataToBeOutputted << endl;
                  
                  ofstream outputFileStream(argv[2]);
                  outputFileStream << dataToBeOutputted;
                  
                  while (1) {
                        unordered_map<string, bool> inputsNormalAndInverted;
                        cout << "Enter boolean values for the following inputs:" << endl;
                        cout << inputLabels.at(0);
                        for (int i = 1; i < numInputs; i++) {
                              cout << " " << inputLabels.at(i);
                        }
                        cout << endl;
                        
                        for (int i = 0; i < numInputs; i++) {
                              string testForInputFiltering;
                              cin >> testForInputFiltering;
                              
                              while (testForInputFiltering.compare("1") && testForInputFiltering.compare("0")) {
                                    
                                    transform(testForInputFiltering.begin(), testForInputFiltering.end(), testForInputFiltering.begin(), ::tolower);
                                    
                                    if (!testForInputFiltering.compare(":q")) {
                                          return 0;
                                    }
                                    
                                    cout << "Please try again..." << endl;
                                    cin >> testForInputFiltering;
                              }
                              
                              if (!testForInputFiltering.compare("1")) {
                                    inputsNormalAndInverted[inputLabels.at(i)] = 1;
                                    inputsNormalAndInverted[inputLabels.at(i) + "'"] = 0;
                              }
                              else if (!testForInputFiltering.compare("0")) {
                                    inputsNormalAndInverted[inputLabels.at(i)] = 0;
                                    inputsNormalAndInverted[inputLabels.at(i) + "'"] = 1;
                              }
                        }
                        
                        vector<bool> productsPostSim;
                        for (int i = 0; i < numProducts; i++) {
                              productsPostSim.push_back(1);
                              for (int j = 0; j < products[i].size(); j++) {
                                    if(!inputsNormalAndInverted[products[i][j]]){
                                          productsPostSim.at(i) = 0;
                                          break;
                                    }
                              }
                        }
                        
                        unordered_map<string, bool> outputsPostSim;
                        for (int i = 0; i < numOutputs; i++) {
                              outputsPostSim[outputLabels[i][0]] = 0;
                              for (int j = 1; j < outputLabels[i].size(); j++) {
                                    if(productsPostSim.at(stoi(outputLabels[i][j]) - 1)){
                                          outputsPostSim[outputLabels[i][0]] = 1;
                                          break;
                                    }
                              }
                              cout  << outputLabels[i][0] << ": "
                              << outputsPostSim[outputLabels[i][0]] << endl;
                        }
                        
                  }
                  return 0;
            }
      }
      
      return 1;
}
