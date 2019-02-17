//
//  main.cpp
//  SimplifyEspressoOutput
//
//  Created by Aaron Knoll on 10/30/18.
//  Licensed under the GNU GPLv3
//  (a copy of which is contained along with this application)
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

string promptForRunMode();
int simulateCombinationalLogic(int, int, int, vector<string>, vector<vector<string>>, vector<vector<string>>);

int main(int argc, const char * argv[]) {
      
      int numInputs = 0, numOutputs = 0;
      vector<string> inputLabels;
      vector<vector<string>> outputLabels;
      
      ifstream inputFileStream(argv[1]);
      string inputLine;
      while (getline(inputFileStream, inputLine)) {
            // line of espresso output file
            // create a more flexible structure for storing the espresso output
            stringstream s1(inputLine);
            
            string tempWord, firstWord;
            // get context identifier
            s1 >> firstWord;
            
            // number of inputs
            if (!firstWord.compare(".i")) {
                  s1 >> numInputs;
            }
            // number of outputs
            else if (!firstWord.compare(".o")) {
                  s1 >> numOutputs;
            }
            // input labels
            else if (!firstWord.compare(".ilb")) {
                  while (s1 >> tempWord) {
                        inputLabels.push_back(tempWord);
                  }
            }
            // output labels
            else if (!firstWord.compare(".ob")) {
                  while (s1 >> tempWord) {
                        vector<string> outputLabel;
                        outputLabel.push_back(tempWord);
                        outputLabels.push_back(outputLabel);
                  }
            }
            // products
            else if (!firstWord.compare(".p")) {
                  int numProducts;
                  s1 >> numProducts;
                  
                  vector<vector<string>> products;
                  string dataToBeOutputted;
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
                  
                  string runConfig = promptForRunMode();
                  
                  if (!runConfig.compare("0") || !runConfig.compare("2")) {
                        string inputFile = argv[1];
                        string outputFile = inputFile.substr(0,inputFile.rfind(".txt")) + "_simplified.txt";
                        ofstream outputFileStream(outputFile);
                        outputFileStream << prettyOutputs << "\n" << dataToBeOutputted;
                        outputFileStream.close();
                  }
                  
                  if (!runConfig.compare("1") || !runConfig.compare("2")) {
                        return simulateCombinationalLogic(numInputs, numOutputs, numProducts, inputLabels, products, outputLabels);
                  }
                  
                  return 0;
            }
      }
      
      return 1;
}

string promptForRunMode() {
      cout << "Get ready to simplify your Espresso output!\n\n";
      
      cout << "0) Simplify Espresso output" << endl;
      cout << "1) Simulate combinational logic" << endl;
      cout << "2) Perform functions 0 and 1" << endl;
      cout << "q) Quit\n\n";
      
      cout << "Which function would you like to perform (0,1,2,q)? ";
      string runConfig;
      cin >> runConfig;
      transform(runConfig.begin(), runConfig.end(), runConfig.begin(), ::tolower);
      
      return runConfig;
}

int simulateCombinationalLogic(int numInputs, int numOutputs, int numProducts, vector<string> inputLabels, vector<vector<string>> products, vector<vector<string>> outputLabels) {
      while (1) {
            unordered_map<string, bool> inputsNormalAndInverted;
            cout << "\nEnter boolean values for the following inputs (enter q to quit):" << endl;
            
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
                        
                        if (!testForInputFiltering.compare("q")) {
                              return 0;
                        }
                        
                        cout << "Please try again..." << endl;
                        cin >> testForInputFiltering;
                  }
                  
                  // set boolean values for each input (both standard and inverted)
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
                        if (productsPostSim.at(stoi(outputLabels[i][j]) - 1)) {
                              outputsPostSim[outputLabels[i][0]] = 1;
                              break;
                        }
                  }
                  cout  << outputLabels[i][0] << ": "
                  << outputsPostSim[outputLabels[i][0]] << endl;
            }
      }
}
