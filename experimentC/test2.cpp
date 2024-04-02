#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

// 定义PL/0语言的关键字、运算符和界符
unordered_set<string> keywords = {"const", "var", "begin", "end", "if", "then", "while", "do", "call", "read", "write"};
unordered_set<string> operators = {"+", "-", "*", "/", "=", "<", ">", "<=", ">="};
unordered_set<string> delimiters = {";", ",", "(", ")", "."};


// 判断是否为字母
bool is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// 判断是否为数字
bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

//字符串大写转小写，小写不变
void convertToLowerCase(string& str) {
   
    for (char& c : str) {
        if (isupper(c)) { 
            c = tolower(c); 
        }
    }
}



// 读取PL/0语言源程序文件
string read_source_code(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }
    string source_code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return source_code;
}


vector<pair<string, string>> lexical_analysis(const string& source_code) {
    vector<pair<string, string>> tokens;
    string token;
    for (size_t i=0;i<source_code.size();i++)
    {
        if (source_code[i] == '{')
        {
            i++;
            token="";
            while (i<source_code.size() && source_code[i] != '}') 
            {
                    token+=source_code[i];
                    i++;
            }
            tokens.push_back({"comment", token});
        }
        else if(source_code[i] == '"'){
            int j = i+1;
            token="";token+='"';
            while(j<(int)source_code.size()&&source_code[j]!='"'){
                token+=source_code[j];
                j++;
            }
            token+='"';
            tokens.push_back({"string value",token});
            i = j;
        }
        else if (is_letter(source_code[i]))
        {
            token = source_code[i];
            while (i+1<source_code.size() && (is_letter(source_code[i+1]) || is_digit(source_code[i+1])))
            {
                token += source_code[i+1];
                i++;
            }
            if (token=="const")
            {
                   tokens.push_back({"constsym", token});
            }
            else if(token=="var")
            {
                tokens.push_back({"varsym", token});
            }
            else if(token=="begin")
            {
                tokens.push_back({"beginsym", token});
            }
            else if(token=="read")
            {
                  tokens.push_back({"readsym", token});
            }
            else if(token=="write")
            {
                  tokens.push_back({"writesym", token});
            }
            else if(token=="if")
            {
                  tokens.push_back({"ifsym", token});
            }
            else if(token=="then")
            {
                  tokens.push_back({"thensym", token});
            }
            else if(token=="while")
            {
                  tokens.push_back({"whilesym", token});
            }
            else if(token=="do")
            {
                  tokens.push_back({"dosym", token});
            }
            else if(token=="call")
            {
                  tokens.push_back({"callsym", token});
            }
            else if(token=="end")
            {
                  tokens.push_back({"endsym", token});
            }
            else if(token=="procedure")
            {
                tokens.push_back({"procsym", token});
            }
            else
            {
                  tokens.push_back({"ident", token});
            }
            // if (keywords.find(token) != keywords.end())
            // {
            //     tokens.push_back({"keyword", token});
            // }
            // else
            // {
            //     tokens.push_back({"identifier", token});
            // }
        }
        else if (is_digit(source_code[i]))
        {
            token = source_code[i];
            while (i+1<source_code.size() && is_digit(source_code[i+1]))
            {
                token += source_code[i+1];
                i++;
            }
            tokens.push_back({"number", token});
        }
        else if (source_code[i] == ':' && i+1<source_code.size() && source_code[i+1] == '=')
        {
            tokens.push_back({"becomes", ":="});
            i++;
        }
        else if (source_code[i] == ' ' || source_code[i] == '\t' || source_code[i] == '\n')
        {
            continue;
        }
        else
        {
            token = source_code[i];
        
            if(token=="=")
            {
                tokens.push_back({"eq", token});
            }
            else if(token=="#")
            {
                 tokens.push_back({"neq", token});
            }
            else if(token=="<")
            {
                tokens.push_back({"lt", token});
            }
            else if(token==">")
            {
                tokens.push_back({"gt", token});
            }
            else if(token=="<=")
            {
                tokens.push_back({"le", token});
            }
            else if(token==">=")
            {
                tokens.push_back({"ge", token});
            }
            else if(token=="+")
            {
                tokens.push_back({"plus", token});
            }
            else if(token=="-")
            {
                tokens.push_back({"minus", token});
            }
            else if(token=="*")
            {
                tokens.push_back({"times", token});
            }
            else if(token=="/")
            {
                tokens.push_back({"slash", token});
            }
            else if(token==";")
            {
                tokens.push_back({"semicolon", token});
            }
            else if(token==",")
            {
                tokens.push_back({"comma", token});
            }
            else if(token=="(")
            {
                tokens.push_back({"lparen", token});
            }
            else if(token==")")
            {
                tokens.push_back({"rparen", token});
            }
            else if(token==".")
            {
                tokens.push_back({"period", token});
            }
        }
    
    } 
    return tokens;

}

// 输出词法分析结果到文件
void print_tokens_to_file(const vector<pair<string, string>>& tokens, const string& filename) {
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open output file " << filename << endl;
        exit(1);
    }
    for (const auto& token : tokens) {
        outputFile << "(" << token.first << ", " << token.second << ")" << endl;
    }
    outputFile.close();
}

int main() {
    string source_code = read_source_code("C:/Users/19377/Documents/vscodeworkspace/VsCodecppWorkSpace/CompilerTheory/experimentB/example02.txt");
    convertToLowerCase(source_code);
    vector<pair<string, string>> tokens = lexical_analysis(source_code);
    print_tokens_to_file(tokens, "output.txt");
    return 0;
}
