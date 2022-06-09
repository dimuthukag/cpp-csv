#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>

using namespace std;

class csvFile
{
private:
    string __fileName__;
    int __r__;
    int __c__;
    char __seperator__;
    vector<string> __headerVector__;
    map<int, map<string, string>> __csvDataMap__;
    vector<int> __colStrMaxLen__;

    void __getSize__()
    {
        string row;
        int rowLen = 0;
        int colLen = 0;
        int colCount = 0;
        fstream csvFile;
        csvFile.open(__fileName__, ios::in);
        if (csvFile.is_open())
        {
            while (getline(csvFile, row))
            {
                rowLen++;
                if (rowLen == 1)
                {
                    if (row.length() != 0)
                    {
                        char rowCharArray[row.length() + 1];
                        strcpy(rowCharArray, row.c_str());
                        for (int index = 0; index < row.length(); index++)
                        {
                            if (rowCharArray[index] == ',')
                            {
                                colCount++;
                            }
                        }
                        colCount++;
                        colLen = colCount;
                        colCount = 0;
                    }
                }
            }
            __r__ = rowLen;
            __c__ = colLen;
        }
        csvFile.close();
    }

    string __rowAsString__(int rowNumber)
    {
        string rowString;
        fstream csvFile;
        int currentRowNumber = 1;
        csvFile.open(__fileName__, ios::in);
        while (csvFile.is_open())
        {
            getline(csvFile, rowString);
            if (currentRowNumber == rowNumber)
            {
                break;
            }
            currentRowNumber++;
        }
        return rowString;
    }

    vector<string> __stringToVector__(string str)
    {
        vector<string> vec;
        int strLen = str.length();
        int startPos = 0;
        int index = 0;
        string element;
        std::string::size_type endPos;
        while (true)
        {
            if (endPos == -1)
            {
                break;
            }
            endPos = str.find(__seperator__, startPos);
            element = str.substr(startPos, endPos - startPos);
            vec.emplace(vec.begin() + index, element);
            index++;
            startPos = endPos + 1;
        }
        return vec;
    }

    map<int, map<string, string>> __getcsvData__()
    {
        map<int, map<string, string>> csvDataMap;
        map<string, string> element;
        vector<string> dataRow;
        for (int rowIndex = 0; rowIndex < __r__; rowIndex++)
        {
            dataRow = __stringToVector__(__rowAsString__(rowIndex + 1));
            for (int colIndex = 0; colIndex < __c__; colIndex++)
            {
                if (rowIndex == 0)
                {
                    element.emplace(__headerVector__[colIndex], __headerVector__[colIndex]);
                    __colStrMaxLen__.emplace(__colStrMaxLen__.begin() + colIndex, __headerVector__[colIndex].length());
                }
                else
                {
                    if (colIndex >= dataRow.size())
                    {
                        element.emplace(__headerVector__[colIndex], " ");
                    }
                    else
                    {
                        element.emplace(__headerVector__[colIndex], dataRow[colIndex]);
                        if (__colStrMaxLen__[colIndex] < dataRow[colIndex].length())
                        {
                            __colStrMaxLen__[colIndex] = dataRow[colIndex].length();
                        }
                    }
                }
            }
            csvDataMap.emplace(rowIndex, element);
            element.clear();
        }

        return csvDataMap;
    }

public:
    csvFile(string fileName, char seperator = ',')
    {
        __fileName__ = fileName;
        __getSize__();
        __seperator__ = seperator;
        __headerVector__ = __stringToVector__(__rowAsString__(1));
        __csvDataMap__ = __getcsvData__();
    }

    vector<string> getcsvHeaders()
    {
        return __headerVector__;
    }

    map<int, map<string, string>> getcsvData()
    {
        return __csvDataMap__;
    }

    vector<int> getcsvDataSize(){
        vector<int> size[2];
        size[0] = __r__;
        size[1] = __c__;
        return size;
    }

    void print_csvData()
    {
        map<string, string> dataRow;
        int str__r__Len = to_string(__r__).length();
        int maxRowLen;
        for (int rowIndex = 0; rowIndex < __r__; rowIndex++)
        {
            if (rowIndex == 0)
            {
                for (int i = 0; i < __colStrMaxLen__.size(); i++)
                {
                    maxRowLen += __colStrMaxLen__[i];
                }
                string headerUpLine(maxRowLen + 12, '-');
                cout << " " << headerUpLine << endl;
            }
            string rowNumDisplay(str__r__Len - to_string(rowIndex).length(), ' ');
            cout << "| " << rowNumDisplay << rowIndex << " | ";
            dataRow = __csvDataMap__[rowIndex];
            for (int colIndex = 0; colIndex < __c__; colIndex++)
            {
                string rowValueDisplay(__colStrMaxLen__[colIndex] - dataRow[__headerVector__[colIndex]].length(), ' ');
                cout << dataRow[__headerVector__[colIndex]] << rowValueDisplay << " | ";
            }
            cout << endl;

            if (rowIndex == 0)
            {
                string headerUpLine(maxRowLen + 12, '-');
                cout << " " << headerUpLine << endl;
            }
        }
        string headerUpLine(maxRowLen + 12, '-');
        cout << " " << headerUpLine << endl;
    }
};
