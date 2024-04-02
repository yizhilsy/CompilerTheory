#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
#define MAXN 1024

string str;

std::unordered_set<std::string> keywords = {
    "const", "var", "procedure", "call", "begin", "end", "if", "then",
    "else", "while", "do", "read", "write", "odd"
};

std::unordered_set<std::string> operators = {
    "+", "-", "*", "/", ":=", "=", "<>", "<", ">", "<=", ">="
};

std::unordered_set<char> delimiters = {
    ',', '.', ';', ':', '(', ')','{','}'
};

// 存储答案的map
map<string,int> ansm;

vector<string> v2;

void init();
unordered_map<string,int> m;
int main()
{
    init();
    cout<<str<<endl;
    // cout<<str.size()<<endl;
    // for(int i=0;i<(int)str.length();i++){
    //     cout<<str[i]<<endl;
    // }

    for(int i=0;i<(int)str.length();i++){
        // cout<<i<<endl;
        // 数字开头的标识符
        if(str[i]>='0'&&str[i]<='9'){
            int j=i+1;
            while(delimiters.find(str[j])==delimiters.end()&&
            operators.find(string(1,str[j]))==operators.end()){
                // cout<<j<<endl;
                j++;
            }
            i = j-1;
        }else if(islower(str[i])||isupper(str[i])){
            int j=i;string word;
            while(j<(int)str.length()
                &&delimiters.find(str[j])==delimiters.end()
                &&operators.find(string(1,str[j]))==operators.end()&&str[j]!='\n'&&str[j]!=' '){
                    word.push_back(str[j]);
                    j++;
            }
            // cout<<word<<endl;
            // 非关键字
            if(keywords.find(word)==keywords.end()){
                // cout<<"找到了！"<<word<<endl;
                if(ansm[word]==0){
                    v2.push_back(word);
                }
                ansm[word]++;
            }
            i = j;
        }else if(str[i]=='{'){
            int j=i+1;
            while(j<(int)str.length()&&str[j]!='}'){
                j++;
            }
            i = j;
        }else if(str[i]=='"'){
            // cout<<"***"<<i<<endl;
            int j=i+1;
            while(j<(int)str.length()&&str[j]!='"'){
                j++;
            }
            i = j;
        }
    }
    // 输出结果
    // for(map<string,int>::iterator it = ansm.begin();it!=ansm.end();it++){
    //     cout<<it->first<<","<<it->second<<endl;
    // }
    std::ofstream outputFile("ans.txt");
    for(int i=0;i<(int)v2.size();i++){
        outputFile<<"("<<v2[i]<<", "<<ansm[v2[i]]<<")"<<endl;
    }
    system("pause");
    return 0;
}

void init(){
    // 打开文件
    std::ifstream file("example2.txt");
    // 读取文件内容到一个 std::string 对象中
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    // 关闭文件
    file.close();
    str=content;
    transform(str.begin(),str.end(),str.begin(),::tolower);
}