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
    {"{", "lbrace"},{"}", "rbrace"},{"[", "lbracket"},{"]", "rbracket"},
    {"(", "lparen"},{"}", "rbrace"},{"{", "lbrace"},{")", "rparen"}

};



vector<pair<string,string>> ansVec;

bool isNumber(char ch){
    return (ch>='0'&&ch<='9');
}

bool isStringNum(string& str){
    for(int i=0;i<str.size();i++){
        if(!isNumber(str[i])){
            return false;
        }
    }
    return true;
}

bool isLetter(char ch){
    return (islower(ch)||isupper(ch));
}

bool isDoubleNum(string& str){
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

void extractingKeywords();

void extractingIdentifiers(string& str);

void compilerWork();

int main()
{
    string compileFileStr = init();
    extractingIdentifiers(compileFileStr);
    std::ofstream outputFile("CompilerTheory\\experimentC\\ans.txt");

    for(const auto& item : ansVec){
        outputFile<<"("<<item.first<<",    "<<item.second<<")"<<endl;
    }
    system("pause");
    return 0;
}

string init(){
    // 打开文件
    std::ifstream file("CompilerTheory\\experimentC\\example01.txt");
    // 读取文件内容到一个 std::string 对象中
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());    
    // 关闭文件
    file.close();
    transform(content.begin(),content.end(),content.begin(),::tolower);
    return content;
}

void extractingIdentifiers(string& str){
    for(int i=0;i<(int)str.length();i++){
        // cout<<i<<endl;
        // 数字开头的标识符
        if(isNumber(str[i])){
            string word = string(1,str[i]);
            int j=i+1;
            while(j<(int)str.size() && (isNumber(str[j]) || isLetter(str[j]) || str[j]=='.' ) ){
                word.push_back(str[j]);
                j++;
            }
            cout<<word<<endl;
            if(isStringNum(word)){
                pair<string,string> ansItem;
                ansItem.first = "number";ansItem.second = word;
                ansVec.push_back(ansItem);
            }else if(isDoubleNum(word)){
                pair<string,string> ansItem;
                ansItem.first = "double";ansItem.second = word;
                ansVec.push_back(ansItem);
            }else{
                cout<<"***********Error:Not a vary****************"<<endl;
            }
            i = j-1;
        }else if(islower(str[i])||isupper(str[i])){
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
        }else if(str[i]=='{'){  //跳过{}内嵌的标识符及关键字
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
        }else if(str[i]=='"'){  //跳过字符串之间的标识符及关键字
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
        }else if( delimiters.find(str[i]) != delimiters.end() ) {
            string word = string(1,str[i]);
            pair<string,string> ansItem;ansItem.first = pl0Symbols[word];ansItem.second = word;
            ansVec.push_back(ansItem);
        }else if( operators.find(string(1,str[i])) != operators.end() ) {
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
        }else if(str[i]!=' '&&str[i]!='\n'&&str[i]!='\t'){
            // 其他非法字符
            string word(1,str[i]);
            pair<string,string> ansItem;ansItem.first = "nul";ansItem.second = word;
            ansVec.push_back(ansItem);
        }
    }
}
