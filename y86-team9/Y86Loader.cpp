#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <map>
#include "Sim.h"
#include "Y86.h"
#include "Tools.h"
#include "Memory.h"
using namespace std;


//Function Definitions/Prototypes
//helper functions
bool load(char *filename);
static bool isValidFileName(string input);
static bool readFile(string fileName);
static bool checkLine(string inputLine);
static bool hasValidAddress(string inputLine);
static bool isCommentLine(string inputLine);
static bool isBlankLine(string InputLine);
static bool hasData(string inputLine);
static int hasValidData(string inputLine);
static int getAddress(string inputLine);
static bool hasSpaces(string inputLine, int start, int end);
static bool checkHex(string inputLine);
void storeData(string inputLine, int dataBytes);
void storeByte(string inputLine, int byteAddress);
static string getData(string inputLine);

//Load function: reads in file and checks line by line for errors.
bool Y86::load(char *filename)
{
    if(readFile(filename)) return true;
    else return false;
}

// - takes as input a string representing the name of a file and returns TRUE if the file name is valid (ends with a .yo extension)
static bool isValidFileName(string inputLine)
{
    string result = "";
    int counter = 0;
    for(int i =0;i<=inputLine.length(); i++)
    {
      if(inputLine[i] != '.')
      {
        counter++;
      }
      else
      {
        result = inputLine.substr(counter);
      }
    }
    if(result[0] == '.' && result[1] == 'y' && result[2] == 'o')
    {
      return true;
    }
    else
    {
      return false;
    }
}

// - given a valid file, reads and processes file line by line (this is a "top level" function that calls others to process the file)
bool Y86::readFile(string fileName)
{
  bool boolo = true;
  if(isValidFileName(fileName))
  {
    ifstream inf{fileName};
    int counter = 1;

    bool flag = false;

      string inputLine;
      string prevLine;
      int dataBytes = 0;
    int prevDataBytes = 0;

      //getline(inf, nextLine);

    while (inf && (flag != true))
    {

      getline(inf, inputLine);

      if(checkLine(inputLine))
      {

        if(isBlankLine(inputLine) != true)
        {
          dataBytes = hasValidData(inputLine);

          if(dataBytes)
          {


              if(isBlankLine(prevLine) != true && isCommentLine(prevLine) != true)
              {
                if((getAddress(prevLine) + prevDataBytes) > getAddress(inputLine))
                {

                  cout << "Error on line " << counter << '\n';
                  cout << inputLine << '\n';
                  boolo = false;
                  return boolo;
                }

              }

              storeData(inputLine, dataBytes);

          }


        }
        prevLine = inputLine;
        prevDataBytes = dataBytes;
        if(!inf) flag = true;
        counter++;
      }
      else
      {
        cout << "Error on line " << counter << '\n';
        cout << inputLine << '\n';
        flag = true;
        boolo = false;
      }

      }
  }
  else
  {
    cerr << "File open failed: " << fileName << '\n' << "Usage: yess " << fileName << ".yo" << '\n';
    boolo = false;

  }
  return boolo;
}

//- returns true if line passes all formatting tests (this would call other functions to check parts of the input line...see the following functions)
static bool checkLine(string iL)
{
    bool result = true;
    if(hasValidAddress(iL))
    {
        if(hasValidData(iL) && iL[7] != ' ')
        {
            string hexLine = getData(iL);
            if(checkHex(hexLine))
            {
                if(iL[5] == ':' && iL[28] == '|')
                {
                  result = true;
                }
                else if(iL[6] == ':' && iL[29] == '|') result = true;
                else result = false;
            }
            else result = false;
        }
        else
        {
            if(iL[7] == ' ' && iL[8] == ' ') result = true;
            else result = false;
            //cout << "has valid address but no data\n";
        }
    }
    else if(isCommentLine(iL))
    {
      result = true;
    }
    else if(isBlankLine(iL))
    {
        result = true;
    }
    else result = false;
    return result;
}

//- returns true if line has a valid address field
static bool hasValidAddress(string inputLine)
{
    bool result = true;
    string test = "";
    if(inputLine[0] != '0' && inputLine[1] != 'x')
    {
        result = false;
        return result;
    }
    if(inputLine[5] != ':' && inputLine[6] != ':')
    {
        result = false;
        return result;
    }
    if (inputLine[5] == ':') test = inputLine.substr(2,3);
    if (inputLine[6] == ':') test = inputLine.substr(2,4);

    for(int i = 0; i< test.length(); i++)
    {

      if(((test[i] >= '0' && test[i] <= '9') || (test[i] == 'a' || test[i] == 'b' || test[i] == 'c' || test[i] == 'd' || test[i] == 'e' || test[i] == 'f')) && (result != false))
      {
          result = true;
      }
      else result = false;
    }
    return result;

}

//- returns true if line only has comment
static bool isCommentLine(string inputLine)
{
    for(int i = 0; i < 28; i++)
    {
        if(inputLine[i] != ' ')
        {
            return false;
        }
    }
    if(inputLine[28] == '|')
    {
        return true;
    }
    else
    {
        return false;
    }
}

// - returns true if line is blank
static bool isBlankLine(string inputLine)
{
    bool allSpaces = true;
    if(inputLine != "")
    {
      for(int i = 0; i<inputLine.length(); i++)
      {
        if(inputLine[i] != ' ' && inputLine[i] != '|')
        {
          allSpaces = false;
        }
        else
        {
          if(allSpaces != false)
          {
            allSpaces = true;
          }

        }
      }
    }
    return allSpaces;
}

//- true if data is present
static bool hasData(string inputLine)
{
    bool result = false;
  for(int i = 0; i<inputLine.length(); i++)
  {
    if(inputLine[7] != ' ')
    {
      result = true;
    }
    else result = false;
  }
  return result;
}

//- returns zero if data is invalid, returns the number of data bytes if valid
static int hasValidData(string inputLine)
{
    int counter = 0;
    int j = 1;
    int k = 0;
    string test = "";
    test = inputLine.substr(7,20);
    if(hasData(inputLine) != true)
    {
        return 0;
    }
    else
    {
      for(int i = 0; i < (test.length()/2); i++)
      {

        if(test[k] != ' ' || test[j] != ' ')
        {
            counter++;
        }
        k++;
        k++;
        j++;
        j++;
      }
    }
    return counter;
}
//- takes as input a data record and returns the address in that line
static int getAddress(string inputLine)
{
  int result = 0;
  string test = "";
  if(inputLine[0] == '0' && inputLine[1] == 'x')
  {
    if (inputLine[5] == ':')
    {
        test = inputLine.substr(2,3);
        result = stoi(test, nullptr, 16);
    }

    if(inputLine[6] == ':')
    {
        test = inputLine.substr(2,4);
        result = stoi(test, nullptr, 16);
    }
  }
return result;
}

//- takes as input a record and starting and ending indices into that record and returns true if
//- there are spaces beginning at the starting position through to the ending position
static bool hasSpaces(string inputLine, int start, int end)
{
    bool hasSpace = true;
    for(int i =start; i<=end;i++)
    {
      if(inputLine[i] != ' ') hasSpace = false;
    }
    return hasSpace;
}

//- takes as input a record and starting and ending indices into that record and
//- returns true if there are hex characters beginning at the starting position through to the ending position
static bool checkHex(string inputLine)
{
  bool hasHex = true;
  string test = inputLine;
  for(int i = 0; i < test.length(); i++)
  {
    if((test[i] != 'A' && test[i] != 'B' && test[i] != 'C' && test[i] != 'D' && test[i] != 'E' && test[i] != 'F' && test[i] != 'a' && test[i] != 'b' && test[i] != 'c' && test[i] != 'd' && test[i] != 'e' && test[i] != 'f' && test[i] != '0' && test[i] != '1' && test[i] != '2' && test[i] != '3' && test[i] != '4' && test[i] != '5' && test[i] != '6' && test[i] != '7' && test[i] != '8' && test[i] != '9') && hasHex != false) hasHex = false;
  }
  return hasHex;
}

//- takes as input a record and number of data bytes, extracts data and stores in YESS memory.
void Y86::storeData(string inputLine, int dataBytes)
{
  string test = inputLine;
  string sub = "";
  int digits = dataBytes * 2;
  string data = inputLine.substr(7,digits);
  int address = getAddress(inputLine);
  int j = 0;
  for(int i = 0; i< dataBytes; i++)
  {
    sub = data.substr(j, 2);
    storeByte(sub, address);
    j++;
    j++;
    address++;

  }
}

//- takes a string representation of two hex characters and a byte address, converts
//- the string to a byte and stores the byte at the specified address using Memory methods
//- you have written (putByte, etc.).
void Y86::storeByte(string inputByte, int byteAddress)
{
  uint8_t byteVal = 0;
  byteVal = stoi(inputByte, nullptr, 16);
  memory.putByte(byteAddress, byteVal);
}

static string getData(string inputLine)
{
    int test = hasValidData(inputLine);
    return inputLine.substr(7,(test*2));
}
