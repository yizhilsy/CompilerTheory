#include <bits/stdc++.h>
using namespace std;
std::unordered_set<std::string> keywords = {
    "const", "var", "procedure", "call", "begin", "end", "if", "then",
    "else", "while", "do", "read", "write", "odd"
};

std::unordered_set<std::string> operators = {
    "+", "-", "*", "/", ":=", "=", "<", ">", "<=", ">=",":"
};

std::unordered_set<char> delimiters = {
    ',', '.', ';', '(', ')','{','}'
};

std::map<std::string, std::string> pl0Symbols = {
    // 基本字
    {"begin", "beginsym"},{"end", "endsym"},{"if", "ifsym"},
    {"then", "thensym"},{"else", "elsesym"},{"const", "constsym"},
    {"var", "varsym"},{"procedure", "procsym"},{"while", "whilesym"},
    {"do", "dosym"},{"call", "callsym"},{"read", "readsym"},
    {"write", "writesym"},{"repeat", "repeatsym"},{"until", "untilsym"},
    {"odd", "oddsym"},{"return", "returnsym"},
    // 运算符
    {"+", "plus"},{"-", "minus"},{"*", "times"},
    {"/", "slash"},{"=", "eql"},{"!=", "neq"},
    {"<", "lss"},{"<=", "leq"},{">", "gtr"},
    {">=", "geq"},{":=", "becomes"},
    // 界符
    {",", "comma"},{".", "period"},{";", "semicolon"},
    {"(", "lparen"},{")", "rparen"}
};

// 判断字符是否是数字
bool isNumber(char ch) {
    return (ch>='0'&&ch<='9');
}

// 判断字符串是否是数字
bool isStringNum(string& str) {
    for(int i=0;i<str.size();i++){
        if(!isNumber(str[i])){
            return false;
        }
    }
    return true;
}

// 判断是否是大小写字符
bool isLetter(char ch) {
    return (islower(ch)||isupper(ch));
}

// 判断是否是浮点数
bool isDoubleNum(string& str) {
    int pointNum = 0;
    for(int i=0;i<str.size();i++){
        if(str[i]=='.'){
            pointNum++;
        }
        if(isLetter(str[i])){
            return false;
        }
    }
    if(pointNum>1){
        return false;
    }
    return true;
}

// 编译程序初始化
string init();

// 词法分析
vector<pair<string, string>> ansVec;    //存储词法分析结果
void extractingKeywords();
void compilerWork();
void extractingIdentifiers(string& str);

// 语法分析
bool ll1SyntaxAnalysis(string& str);    // LL(1) expression syntax analysis
void errorSearch(const deque<char>& synataxStr);
bool ll1ConditionSyntaxAnalysis(string& str);

int main()
{
    string compileFileStr = init();
    std::ofstream outputFile("CompilerTheory\\experimentD\\SyntaxCompilationResult.txt");
    string checkstr;
    int lineCt = 1;
    for (int i = 0;i < (int)compileFileStr.size();i++) {
        if (compileFileStr[i] == '\n') {
            cout << lineCt++ << ":" << endl;
            cout << checkstr << endl;
            extractingIdentifiers(checkstr);
            ll1SyntaxAnalysis(checkstr);
            checkstr.clear();
        }
        else if (i == (int)compileFileStr.size() - 1) {
            checkstr.push_back(compileFileStr[i]);
            cout << lineCt++ << ":" << endl;
            cout << checkstr << endl;
            extractingIdentifiers(checkstr);
            ll1SyntaxAnalysis(checkstr);
            checkstr.clear();
        }
        else {
            checkstr.push_back(compileFileStr[i]);
        }
    }
    
    // for(const auto& item : ansVec){
    //     outputFile<<"("<<item.first<<",    "<<item.second<<")"<<endl;
    // }
    // ll1SyntaxAnalysis(compileFileStr);

    cout << "*****Condition Syntax Test*****" << endl;
    // 打开文件
    std::ifstream file("CompilerTheory\\experimentD\\synataxAnalysis02.txt");
    // 读取文件内容到一个 std::string 对象中
    std::string content02((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());    
    // 关闭文件
    file.close();
    // 将待编译文件全部转为小写字符
    transform(content02.begin(), content02.end(), content02.begin(), ::tolower);
    cout << content02 << endl;
    extractingIdentifiers(content02);
    ll1ConditionSyntaxAnalysis(content02);

    return 0;
}

string init(){
    // 打开文件
    std::ifstream file("CompilerTheory\\experimentD\\synataxAnalysis01.txt");
    // 读取文件内容到一个 std::string 对象中
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());    
    // 关闭文件
    file.close();
    // 将待编译文件全部转为小写字符
    transform(content.begin(), content.end(), content.begin(), ::tolower);
    return content;
}

void extractingIdentifiers(string& str) {
    ansVec.clear();
    for (int i = 0;i < (int)str.length();i++) {
        // cout<<i<<endl;
        // 数字开头的标识符
        if (isNumber(str[i])) {
            string word = string(1, str[i]);
            int j=i+1;
            while(j<(int)str.size() && (isNumber(str[j]) || isLetter(str[j]) || str[j]=='.' ) ) {
                word.push_back(str[j]);
                j++;
            }
            if(isStringNum(word)){
                pair<string,string> ansItem;
                ansItem.first = "number";ansItem.second = word;
                ansVec.push_back(ansItem);
            }else if(isDoubleNum(word)){
                pair<string,string> ansItem;
                ansItem.first = "double";ansItem.second = word;
                ansVec.push_back(ansItem);
            }else{
                cout<<"***********Error:Not a ident****************"<<endl;
                cout<<word<<endl;
            }
            i = j-1;
        }
        else if (islower(str[i]) || isupper(str[i])) {
            int j=i;string word;
            while(j<(int)str.length() && delimiters.find(str[j])==delimiters.end()
                && operators.find(string(1,str[j]))==operators.end() && str[j]!='\n' && str[j]!=' '){
                    word.push_back(str[j]);
                    j++;
            }
            // 非关键字
            if(word=="true"||word=="false"){
                pair<string,string> ansItem;
                ansItem.first = "bool";ansItem.second = word;
                ansVec.push_back(ansItem);
            }else if(keywords.find(word)==keywords.end()){
                pair<string,string> ansItem;
                ansItem.first = "ident";ansItem.second = word;
                ansVec.push_back(ansItem);
            }else{
                pair<string,string> ansItem;
                ansItem.first = pl0Symbols[word];ansItem.second = word;
                ansVec.push_back(ansItem);
            }
            i = j-1;
        }
        else if (str[i] == '{') {  //跳过{}内嵌的标识符及关键字
            string word;word.push_back('{');
            int j=i+1;
            while(j<(int)str.length()&&str[j]!='}'){
                word.push_back(str[j]);
                j++;
            }
            word.push_back('}');
            pair<string,string> ansItem;ansItem.first = "comment";ansItem.second = word;
            ansVec.push_back(ansItem);
            i = j;
        }
        else if (str[i] == '"') {  //跳过字符串之间的标识符及关键字
            string word = string(1,str[i]);
            int j=i+1;
            while(j<(int)str.length()&&str[j]!='"'){
                word.push_back(str[j]);
                j++;
            }
            word.push_back('"');
            pair<string,string> ansItem;ansItem.first = "string value";ansItem.second = word;
            ansVec.push_back(ansItem);
            i = j;
        }
        else if (delimiters.find(str[i]) != delimiters.end()) { //寻找到了界符
            string word = string(1, str[i]);
            pair<string,string> ansItem;ansItem.first = pl0Symbols[word];ansItem.second = word;
            ansVec.push_back(ansItem);
        }
        else if (operators.find(string(1, str[i])) != operators.end()) {    //寻找到了运算符
            if(str[i] == ':'||str[i] == '<'||str[i] == '>'){
                // cout<<"qwe"<<endl;
                if( (i+1)<str.size() && str[i+1]=='=' ){
                    string word = string(1,str[i]);word.push_back('=');
                    pair<string,string> ansItem;ansItem.first = pl0Symbols[word];ansItem.second = word;
                    ansVec.push_back(ansItem);
                    
                    i = i+1;
                }else{
                    string word = string(1,str[i]);
                    pair<string,string> ansItem;ansItem.first = pl0Symbols[word];ansItem.second = word;
                    ansVec.push_back(ansItem);
                }
            }else{
                string word = string(1,str[i]);
                pair<string,string> ansItem;ansItem.first = pl0Symbols[word];ansItem.second = word;
                ansVec.push_back(ansItem);
            }
        }
        else if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t') {
            // 其他非法字符
            string word(1,str[i]);
            pair<string,string> ansItem;ansItem.first = "nul";ansItem.second = word;
            ansVec.push_back(ansItem);
        }
    }
}

bool ll1SyntaxAnalysis(string& str) {
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
    predictTable[1][1] = "I";predictTable[1][2] = "I";predictTable[1][3] = "I";predictTable[1][5] = "AIR";predictTable[1][6] = "AIR";
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
            errorSearch(tempStr);
            return false;
        }
        else {  //从分析预测表中取出推理字符串
            string expression = predictTable[hashmap[stCh]][hashmap[strCh]];
            if (expression == "ERROR") {
                cout << synataxSt.top() << " " << synataxStr.front() << endl;
                cout << "空预测表" << endl;
                cout << "Synatax Error, Please Check." << endl;
                errorSearch(tempStr);
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
            errorSearch(tempStr);
            return false;
        }
        

    }
    cout << "Synatax is Correct." << endl;
    return true;
}

// 错误检查
void errorSearch(const deque<char>& synataxStr) {
    int lparenCt = 0;int rparenCt = 0;
    if (synataxStr.front() == '(') {
        lparenCt++;
    }
    else if (synataxStr.front() == ')') {
        rparenCt++;
    }
    char priorCh = synataxStr.front();
    if (synataxStr.front() == '*' || synataxStr.front() == '/') {
        cout << "Error Reason: nothing before * or /!" << endl;
    }

    if (synataxStr.back() == '*' || synataxStr.back() == '/') {
        cout << "Error Reason: nothing after * or /!" << endl;
    }

    for (int i = 1;i < (int)synataxStr.size();i++) {
        if (synataxStr[i] == priorCh && ((synataxStr[i] == '+' && priorCh == '+') || (synataxStr[i] == '-' && priorCh == '-')
            || (synataxStr[i] == '*' && priorCh == '*') || (synataxStr[i] == '/' && priorCh == '/') )) {
            cout << "Error Reason: Multile times " << synataxStr[i] << "!" << endl;
        }
        if (synataxStr[i] == priorCh && ((synataxStr[i] == 'b' && priorCh == 'b') || (synataxStr[i] == 'n' && priorCh == 'n'))) {
            cout << "Error Reason: Multile times idents or number!"<< endl;
        }
        if (synataxStr[i] == ')' && (priorCh == '+' || priorCh == '-')) {
            cout << "Error Reason: Unexpected + or - before )!" << endl;
        }
        if (synataxStr[i] == '(') {
            lparenCt++;
            if (lparenCt < rparenCt) {
                cout << "Error Reason: rparenCt before lparenCt!" << endl;
            }
        }
        else if (synataxStr[i] == ')') {
            rparenCt++;
            if (lparenCt < rparenCt) {
                cout << "Error Reason: rparenCt before lparenCt!" << endl;
            }
        }
        priorCh = synataxStr[i];
    }

    if (lparenCt != rparenCt) {
        cout << "Error Reason: lparenNum doesn't equal to rparenNum!" << endl;
    }

}

bool ll1ConditionSyntaxAnalysis(string& str) {
    //处理分析一行的语法
    //构建剩余串,并且检查是否有非法字符
    deque<string> synataxStr;
    for (const auto& data : ansVec) {
        if (data.first == "number") {
            synataxStr.push_back("n");
        }
        else if (data.first == "ident") {
            synataxStr.push_back("b");
        }
        else if (data.second == "(") {
            synataxStr.push_back("(");
        }
        else if (data.second == ")") {
            synataxStr.push_back(")");
        }
        else if (data.second == "+") {
            synataxStr.push_back("+");
        }
        else if (data.second == "-") {
            synataxStr.push_back("-");
        }
        else if (data.second == "*") {
            synataxStr.push_back("*");
        }
        else if (data.second == "/") {
            synataxStr.push_back("/");
        }
        else if (data.second == "=") {
            synataxStr.push_back("=");
        }
        else if (data.second == "!=") {
            synataxStr.push_back("!=");
        }
        else if (data.second == "<") {
            synataxStr.push_back("<");
        }
        else if (data.second == "<=") {
            synataxStr.push_back("<=");
        }
        else if (data.second == ">") {
            synataxStr.push_back(">");
        }
        else if (data.second == ">=") {
            synataxStr.push_back(">=");
        }
        else if (data.second == "if") {
            synataxStr.push_back("if");
        }
        else if (data.second == "then") {
            synataxStr.push_back("then");
        }
        else {  //此时是非法字符
            cout << "Invaild symbol!" << endl;
            return false;
        }
    }
    // 条件表达式文法暂时不做错误检测了/(ㄒoㄒ)/~~
    // deque<string> tempStr = synataxStr;
    synataxStr.push_back("#");

    // for (int i = 0;i < (int)synataxStr.size();i++) {
    //     cout << synataxStr[i] << " ";
    // }
    // cout << endl;
    //构建预测分析表
    unordered_map<string, int> hashmap = {
        {"b",1},{"n",2},{"(",3},{")",4},{"+",5},{"-",6},{"*",7},{"/",8},
        {"=",9},{"!=",10},{"<",11},{"<=",12},{">",13},{">=",14},{"odd",15},{"if",16},{"then",17},{"#",18},
        {"S",1},{"T",2},{"E",3},{"R",4},{"I",5},{"O",6},{"F",7},{"A",8},{"M",9},{"P",10}
    };
    //构建终结符集合
    unordered_set<string> hashset = {
        "b","n","(",")","+","-","*","/","=","!=","<","<=",">",">=","odd","if","then","#"
    };
    vector<string> predictTable[32][32];
    for (int i = 1;i <= 10;i++) {
        for (int j = 1;j <= 18;j++) {
            predictTable[i][j].push_back("ERROR");
        }
    }

    predictTable[1][16].pop_back();predictTable[1][16].push_back("if");predictTable[1][16].push_back("T");predictTable[1][16].push_back("then");
    predictTable[2][1].pop_back();predictTable[2][1].push_back("E");predictTable[2][1].push_back("P");predictTable[2][1].push_back("E");
    predictTable[2][2].pop_back();predictTable[2][2].push_back("E");predictTable[2][2].push_back("P");predictTable[2][2].push_back("E");
    predictTable[2][3].pop_back();predictTable[2][3].push_back("E");predictTable[2][3].push_back("P");predictTable[2][3].push_back("E");
    predictTable[2][5].pop_back();predictTable[2][5].push_back("E");predictTable[2][5].push_back("P");predictTable[2][5].push_back("E");
    predictTable[2][6].pop_back();predictTable[2][6].push_back("E");predictTable[2][6].push_back("P");predictTable[2][6].push_back("E");
    predictTable[2][15].pop_back();predictTable[2][15].push_back("odd");predictTable[2][15].push_back("E");
    predictTable[3][1].pop_back();predictTable[3][1].push_back("I");
    predictTable[3][2].pop_back();predictTable[3][2].push_back("I");
    predictTable[3][3].pop_back();predictTable[3][3].push_back("I");
    predictTable[3][5].pop_back();predictTable[3][5].push_back("A");predictTable[3][5].push_back("I");predictTable[3][5].push_back("R");
    predictTable[3][6].pop_back();predictTable[3][6].push_back("A");predictTable[3][6].push_back("I");predictTable[3][6].push_back("R");
    predictTable[4][4].pop_back();predictTable[4][4].push_back("ε");
    predictTable[4][5].pop_back();predictTable[4][5].push_back("A");predictTable[4][5].push_back("I");predictTable[4][5].push_back("R");
    predictTable[4][6].pop_back();predictTable[4][6].push_back("A");predictTable[4][6].push_back("I");predictTable[4][6].push_back("R");
    predictTable[4][9].pop_back();predictTable[4][9].push_back("ε");
    predictTable[4][10].pop_back();predictTable[4][10].push_back("ε");
    predictTable[4][11].pop_back();predictTable[4][11].push_back("ε");
    predictTable[4][12].pop_back();predictTable[4][12].push_back("ε");
    predictTable[4][13].pop_back();predictTable[4][13].push_back("ε");
    predictTable[4][14].pop_back();predictTable[4][14].push_back("ε");
    predictTable[4][17].pop_back();predictTable[4][17].push_back("ε");
    predictTable[5][1].pop_back();predictTable[5][1].push_back("F");predictTable[5][1].push_back("O");
    predictTable[5][2].pop_back();predictTable[5][2].push_back("F");predictTable[5][2].push_back("O");
    predictTable[5][3].pop_back();predictTable[5][3].push_back("F");predictTable[5][3].push_back("O");
    predictTable[6][4].pop_back();predictTable[6][4].push_back("ε");
    predictTable[6][5].pop_back();predictTable[6][5].push_back("ε");
    predictTable[6][6].pop_back();predictTable[6][6].push_back("ε");
    predictTable[6][7].pop_back();predictTable[6][7].push_back("M");predictTable[6][7].push_back("F");predictTable[6][7].push_back("O");
    predictTable[6][8].pop_back();predictTable[6][8].push_back("M");predictTable[6][8].push_back("F");predictTable[6][8].push_back("O");
    predictTable[6][9].pop_back();predictTable[6][9].push_back("ε");
    predictTable[6][10].pop_back();predictTable[6][10].push_back("ε");
    predictTable[6][11].pop_back();predictTable[6][11].push_back("ε");
    predictTable[6][12].pop_back();predictTable[6][12].push_back("ε");
    predictTable[6][13].pop_back();predictTable[6][13].push_back("ε");
    predictTable[6][14].pop_back();predictTable[6][14].push_back("ε");
    predictTable[6][17].pop_back();predictTable[6][17].push_back("ε");
    predictTable[7][1].pop_back();predictTable[7][1].push_back("b");
    predictTable[7][2].pop_back();predictTable[7][2].push_back("n");
    predictTable[7][3].pop_back();predictTable[7][3].push_back("(");predictTable[7][3].push_back("E");predictTable[7][3].push_back(")");
    predictTable[8][5].pop_back();predictTable[8][5].push_back("+");
    predictTable[8][6].pop_back();predictTable[8][6].push_back("-");
    predictTable[9][7].pop_back();predictTable[9][7].push_back("*");
    predictTable[9][8].pop_back();predictTable[9][8].push_back("/");
    predictTable[10][9].pop_back();predictTable[10][9].push_back("=");
    predictTable[10][10].pop_back();predictTable[10][10].push_back("!=");
    predictTable[10][11].pop_back();predictTable[10][11].push_back("<");
    predictTable[10][12].pop_back();predictTable[10][12].push_back("<=");
    predictTable[10][13].pop_back();predictTable[10][13].push_back(">");
    predictTable[10][14].pop_back();predictTable[10][14].push_back(">=");

    // 进行LL(1)分析
    stack<string> synataxSt;
    // 初始化塞入#和E
    synataxSt.push("#");synataxSt.push("S");
    while (synataxSt.top() != "#" || synataxStr.front() != "#") {
        string stCh = synataxSt.top();
        string strCh = synataxStr.front();
        // cout << strCh << "," << stCh << endl;
        if (hashset.find(stCh) != hashset.end() && hashset.find(strCh) != hashset.end() && stCh == strCh) { //同为终结符且相等
            synataxSt.pop();
            synataxStr.pop_front();
        }
        else if (hashset.find(stCh) != hashset.end() && hashset.find(strCh) != hashset.end() && stCh != strCh) {    //同为终结符且不等
            cout << synataxSt.top() << " " << synataxStr.front() << endl;
            cout << "终结符不一致" << endl;
            cout << "Synatax Error, Please Check." << endl;
            return false;
        }
        else {  //从分析预测表中取出推理字符串
            vector<string> expression = predictTable[hashmap[stCh]][hashmap[strCh]];
            if (expression.front() == "ERROR") {    //踩到预测表中空的项
                cout << synataxSt.top() << " " << synataxStr.front() << endl;
                cout << "空预测表" << endl;
                cout << "Synatax Error, Please Check." << endl;
                return false;
            }
            else if (expression.front() == "ε") {   //处理产生式右部是空的情况
                synataxSt.pop();
            }
            else {  //正常推理
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
        if (synataxSt.top() == "#" && synataxStr.front() != "#") {
            cout << "预测分析栈先空了" << endl;
            cout << "Synatax Error, Please Check." << endl;
            return false;
        }
    }
    cout << "Synatax is Correct." << endl;
    return true;
}



// predictTable[1][5] = "E->AIR";
// predictTable[1][6] = "E->AIR";
// predictTable[2][5] = "R->AIR";
// predictTable[2][6] = "R->AIR";
// predictTable[2][9] = "R->ε";
// predictTable[3][1] = "I->FO";
// predictTable[3][2] = "I->FO";
// predictTable[3][3] = "I->FO";
// predictTable[4][7] = "O->MFO";
// predictTable[4][8] = "O->MFO";
// predictTable[4][9] = "O->ε";
// predictTable[5][1] = "F->b";
// predictTable[5][2] = "F->n";
// predictTable[5][3] = "F->(";
// predictTable[6][5] = "A->+";
// predictTable[6][6] = "A->-";
// predictTable[7][7] = "M->*";
// predictTable[7][8] = "M->/";