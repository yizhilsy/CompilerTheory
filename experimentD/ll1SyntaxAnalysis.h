#include <iostream>
#include <string>
#include <deque>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
using namespace std;

bool ll1SyntaxAnalysis(vector<pair<string,string>>& ansVec) {
    //处理分析一行的语法
    //构建剩余串,并且检查是否有非法字符
    deque<char> synataxStr;
    for (const auto& data : ansVec) {
        if (data.first == "number") {
            synataxStr.push_back('n');
        }
        else if (data.first == "ident") {
            synataxStr.push_back('b');
        }
        else if (data.second == "(") {
            synataxStr.push_back('(');
        }
        else if (data.second == ")") {
            synataxStr.push_back(')');
        }
        else if (data.second == "+") {
            synataxStr.push_back('+');
        }
        else if (data.second == "-") {
            synataxStr.push_back('-');
        }
        else if (data.second == "*") {
            synataxStr.push_back('*');
        }
        else if (data.second == "/") {
            synataxStr.push_back('/');
        }
        else {  //此时是非法字符
            cout << "Invaild symbol!" << endl;
            return false;
        }
    }
    deque<char> tempStr = synataxStr;
    synataxStr.push_back('#');

    //构建预测分析表
    unordered_map<char, int> hashmap = {
        {'b',1},{'n',2},{'(',3},{')',4},{'+',5},{'-',6},{'*',7},{'/',8},{'#',9},
        {'E',1},{'R',2},{'I',3},{'O',4},{'F',5},{'A',6},{'M',7}
    };
    //构建终结符集合
    unordered_set<char> hashset = {
        'b','n','(',')','+','-','*','/','#'
    };
    string predictTable[16][16];
    for (int i = 1;i <= 7;i++) {
        for (int j = 1;j <= 9;j++) {
            predictTable[i][j] = "ERROR";
        }
    }
    predictTable[1][1] = "IR";predictTable[1][2] = "IR";predictTable[1][3] = "IR";predictTable[1][5] = "AIR";predictTable[1][6] = "AIR";
    predictTable[2][4] = "ε";predictTable[2][5] = "AIR";predictTable[2][6] = "AIR";predictTable[2][9] = "ε";
    predictTable[3][1] = "FO";predictTable[3][2] = "FO";predictTable[3][3] = "FO";
    predictTable[4][4] = "ε";predictTable[4][5] = "ε";predictTable[4][6] = "ε";predictTable[4][7] = "MFO";predictTable[4][8] = "MFO";predictTable[4][9] = "ε";
    predictTable[5][1] = "b";predictTable[5][2] = "n";predictTable[5][3] = "(E)";
    predictTable[6][5] = "+";predictTable[6][6] = "-";
    predictTable[7][7] = "*";predictTable[7][8] = "/";

    // 进行LL(1)分析
    stack<char> synataxSt;
    // 初始化塞入#和E
    synataxSt.push('#');
    synataxSt.push('E');
    while (synataxSt.top() != '#' || synataxStr.front() != '#') {
        char stCh = synataxSt.top();
        char strCh = synataxStr.front();
        // cout << strCh << "," << stCh << endl;
        if (hashset.find(stCh) != hashset.end() && hashset.find(strCh) != hashset.end() && stCh == strCh) { //同为终结符且相等
            synataxSt.pop();
            synataxStr.pop_front();
        }
        else if (hashset.find(stCh) != hashset.end() && hashset.find(strCh) != hashset.end() && stCh != strCh) {    //同为终结符且不等
            cout << synataxSt.top() << " " << synataxStr.front() << endl;
            cout << "终结符不一致" << endl;
            cout << "Synatax Error, Please Check." << endl;
            //errorSearch(tempStr);
            return false;
        }
        else {  //从分析预测表中取出推理字符串
            string expression = predictTable[hashmap[stCh]][hashmap[strCh]];
            if (expression == "ERROR") {
                cout << synataxSt.top() << " " << synataxStr.front() << endl;
                cout << "空预测表" << endl;
                cout << "Synatax Error, Please Check." << endl;
                //errorSearch(tempStr);
                return false;
            }
            else if (expression == "ε") {   //处理产生式右部是空的情况
                synataxSt.pop();
            }
            else {
                synataxSt.pop();
                for (int i = (int)expression.size() - 1;i >= 0;i--) {
                    synataxSt.push(expression[i]);
                }
            }
        }
        // //cout << synataxSt.size() << "," << synataxStr.size() << endl;
        // if ((synataxSt.top() == '#' && synataxStr.front() != '#') || (synataxSt.top() != '#' && synataxStr.front() == '#')) {
        //     cout << synataxSt.size() << "," << synataxStr.size() << endl;
        //     cout << "Synatax Error, Please Check." << endl;
        //     errorSearch(tempStr);
        //     return;
        // }
        if (synataxSt.top() == '#' && synataxStr.front() != '#') {
            cout << "预测分析栈先空了" << endl;
            cout << "Synatax Error, Please Check." << endl;
            //errorSearch(tempStr);
            return false;
        }
        

    }
    cout << "Synatax is Correct." << endl;
    return true;
}