//Author: Richard Rosenthal, rnr5142, 12-13-2020
//Class: CMPSC 121
//Final Project
//Purpose: To create a program that can pull data from a file, add it to vectors, allow user to search for data or add it if it is missing
// and to be fully comforable with using vectors.


#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;


//Function prototypes
bool get_data(vector <string>& part_number, vector<char>& part_class, vector <int>& part_ohb, vector<double>& part_cost);
int bin_search(string key, const vector<string>& part_number);
string get_target();
void get_more_data(char& class_in, int& part_ohb_in, double& part_cost_in);
void insert_data(vector <string>& part_number, vector<char>& part_class, vector <int>& part_ohb,
                 vector <double>& part_cost, string part_in, char class_in, int part_ohb_in, double part_cost_in);
void display (const vector <string>& part_number, const vector <char>& part_class, const vector <int>& part_ohb,
              const vector <double>& part_cost, int finder);
void sort  (vector <string>& part_number, vector <char>& part_class, vector<int>& part_ohb, vector <double>& part_cost);
void print_stats(int searches, int good, int bad);
void put_data (const vector <string>& part_number, const vector <char>& part_class,
               const vector <int>& part_ohb, const vector <double>& part_cost);


//templated swap function - swaps two items in a vvector of any type
//put this BEFORE main() called from sort function
template <class CType>
void swapper (CType& a, CType & b){
    CType temp;
    temp = a;
    a = b;
    b = temp;
}

vector<string> parts;
vector<char> cls;
vector<int> quantity;
vector<double> price;

string userSearch; // to hold the end user's part number search
int amountOfGoodSearchs; // hold the amount of good searches
int amountOfBadSearchs; // hold the amount of bad searches
char userDecision; // variable to hold user decision to Y or N.
bool continueProgram = true;
string part_number_in;
char class_in;
int part_ohb_in;
double part_cost_in;

int main() {

    bool noErrors;
    noErrors = get_data(parts, cls, quantity, price);

    while (continueProgram){
        if (noErrors){
            sort(parts,cls,quantity,price); //sorts the part list
            userSearch = get_target();
            int returnValue = bin_search(userSearch, parts);
            if (returnValue != -1){

                display(parts, cls, quantity, price, returnValue);
                amountOfGoodSearchs++; //increment the amount of good searches
            }
            else
            {
                amountOfBadSearchs++; //increment the amount of bad searches
                cout << "Would you like to add this part?" << endl;
                cin >> userDecision;
                if (userDecision == 'y' || userDecision == 'Y'){
                    part_number_in = userSearch;
                    get_more_data(class_in, part_ohb_in, part_cost_in);
                    insert_data(parts, cls, quantity, price, part_number_in, class_in, part_ohb_in, part_cost_in);
                }


            }

        }
        else
            cout << "File could not be opened. Aborting Program" << endl;

        cout << "Would you like to do another search? <Y or N>" << endl;
        cin >> userDecision;
        if (userDecision == 'Y' || userDecision == 'y'){
            continueProgram = true;
        }
        else {
            continueProgram = false;
        }
    }


    int numOfSearches = amountOfBadSearchs + amountOfGoodSearchs;
    //print the amount of searches
    print_stats(numOfSearches, amountOfGoodSearchs, amountOfBadSearchs);


    put_data(parts, cls, quantity, price);
    cout << endl;
    cout << "The file is now updated." << endl;
    return 0;
}

//Fills Vectors
bool get_data(vector <string>& part_number, vector<char>& part_class, vector <int>& part_ohb, vector<double>& part_cost){
    bool noErrors = false; //flat to return

    //initiate and open file
    ifstream inputFile;
    inputFile.open("parts.txt");

    string partsVariable;
    char clsVariable;
    double priceVariable;
    int quantityVariable;


    if(inputFile){

        int index = 0;
        while (inputFile >> partsVariable >> clsVariable  >> quantityVariable >> priceVariable){

            part_number.push_back(partsVariable);
            part_class.push_back(clsVariable);
            part_cost.push_back(priceVariable);
            part_ohb.push_back(quantityVariable);
            index++;
        }
        noErrors = true;

    }
    inputFile.close();
    return noErrors;
}

//Does a binary search
int bin_search(string key, const vector<string>& part_number){
 bool found = false;
 int first, mid, last, return_val;
 first = 0;
 last = part_number.size()-1;
 while ((first <= last) && (!found))
 {
     mid = (first + last) / 2;
     if (key == part_number[mid])
         found = true;
     else
         if (key < part_number[mid])
             last = mid -1;
         else
             first = mid + 1;
 }
 if (found)
     return_val = mid;
 else
     return_val = -1;
 return return_val;
}

//Asks user for a part number to search for
string get_target(){
    string target; // holds the target the user is searching for.
    cout << "What part number would you like to look for?" << endl;
    cin >> target;
    return target;
}

// gets remaining info to add a part number
void get_more_data(char& class_in, int& part_ohb_in, double& part_cost_in){
  cout << "What class is this part number in?" << endl;
  cin >> class_in;
  cout << "What is the on hand balance of this part?" << endl;
  cin >> part_ohb_in;
  cout << "What is the cost of this part?" << endl;
  cin >> part_cost_in;
}

//Inserts part number data into vectors
void insert_data(vector <string>& part_number, vector<char>& part_class, vector <int>& part_ohb,
                 vector <double>& part_cost, string part_in, char class_in, int part_ohb_in, double part_cost_in){
    part_number.push_back(part_in);
    part_class.push_back(class_in);
    part_ohb.push_back(part_ohb_in);
    part_cost.push_back(part_cost_in);
    sort(part_number, part_class, part_ohb, part_cost); //resorts the newly added information in the parallel vectors

}

//Displays info on part number
void display (const vector <string>& part_number, const vector <char>& part_class, const vector <int>& part_ohb,
              const vector <double>& part_cost, int finder){
    cout << endl;
    cout << "There are " << quantity[finder] << " of Part Number " << part_number[finder] << " in inventory." << endl;
    cout << "It is a class " << part_class[finder] << " part." << endl;
    cout << "The cost is $" << part_cost[finder] << ". The value of that inventory is $" << (part_ohb[finder] * part_cost[finder])
    << ". " << endl;
}

//sorts vectors (calls swapper)
void sort  (vector <string>& part_number, vector <char>& part_class, vector<int>& part_ohb, vector <double>& part_cost){
  bool flag = true;
  int i, numLength = part_number.size();

  int d = numLength;
  while (flag || (d>1)) //bool flag
  {
      flag = false; //reset flag to 0 to check for future swaps
      d = (d + 1) / 2;
      for (i = 0; i < (numLength - d); i++){
          if (part_number[i + d] < part_number[i]){
              swapper(part_number[i], part_number[i + d]);
              swapper(part_class[i], part_class[i + d]);
              swapper(part_ohb[i], part_ohb[i + d]);
              swapper(part_cost[i], part_cost[i + d]);
              flag = true; //tells swap has occurred
          }
      }
  }

}

//prints search stats
void print_stats(int searches, int good, int bad){
  cout << "There were " << searches << " performed today." << endl;
  cout << good << " of them were successful." << endl;
  cout << bad  << " of them were not in the system." << endl;
}

//writes out file when program ends, so additions are saved to file
void put_data (const vector <string>& part_number, const vector <char>& part_class,
               const vector <int>& part_ohb, const vector <double>& part_cost){
    ofstream fout;
    fout.open("parts.txt");

    for (int index = 0; index < part_number.size(); index++){
        fout << part_number[index] << " " << part_class[index] << " " << part_ohb[index] << " " << part_cost[index] << endl;
    }
    fout.close();
}

/*
 * Execution Sample
 * /Users/richardrosenthal/CLionProjects/Project_3/cmake-build-debug/Project_3
What part number would you like to look for?
P-13682

There are 14 of Part Number P-13682 in inventory.
It is a class D part.
The cost is $25.56. The value of that inventory is $357.84.
Would you like to do another search? <Y or N>
y
What part number would you like to look for?
P-41316

There are 17 of Part Number P-41316 in inventory.
It is a class C part.
The cost is $33.99. The value of that inventory is $577.83.
Would you like to do another search? <Y or N>
y
What part number would you like to look for?
P-16803

There are 44 of Part Number P-16803 in inventory.
It is a class D part.
The cost is $13.61. The value of that inventory is $598.84.
Would you like to do another search? <Y or N>
y
What part number would you like to look for?
P-10022
Would you like to add this part?
y
What class is this part number in?
C
What is the on hand balance of this part?
123
What is the cost of this part?
45.00
Would you like to do another search? <Y or N>
y
What part number would you like to look for?
P-31977
Would you like to add this part?
y
What class is this part number in?
A
What is the on hand balance of this part?
12
What is the cost of this part?
156.00
Would you like to do another search? <Y or N>
y
What part number would you like to look for?
P-32344
Would you like to add this part?
y
What class is this part number in?
D
What is the on hand balance of this part?
88
What is the cost of this part?
12.00
Would you like to do another search? <Y or N>
y
What part number would you like to look for?
P-10022

There are 123 of Part Number P-10022 in inventory.
It is a class C part.
The cost is $45. The value of that inventory is $5535.
Would you like to do another search? <Y or N>
y
What part number would you like to look for?
P-31977

There are 12 of Part Number P-31977 in inventory.
It is a class A part.
The cost is $156. The value of that inventory is $1872.
Would you like to do another search? <Y or N>
y
What part number would you like to look for?
P-32344

There are 88 of Part Number P-32344 in inventory.
It is a class D part.
The cost is $12. The value of that inventory is $1056.
Would you like to do another search? <Y or N>
n
There were 9 performed today.
6 of them were successful.
3 of them were not in the system.

The file is now updated.

Process finished with exit code 0
 */