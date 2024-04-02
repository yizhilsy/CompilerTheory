#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <vector>
#include <algorithm>

// 将字符串转换为小写
std::string toLower(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// 计算标识符出现次数
std::map<std::string, int> countIdentifiers(const std::string& sourceCode, const std::vector<std::string>& reservedWords) {
    std::regex pattern("[a-zA-Z][a-zA-Z0-9]*");
    std::map<std::string, int> identifierCounts;
    auto words_begin = std::sregex_iterator(sourceCode.begin(), sourceCode.end(), pattern);
    auto words_end = std::sregex_iterator();
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::string identifier = (*i).str();
        // 将标识符转换为小写
        std::string lowerIdentifier = toLower(identifier);
        // 检查标识符是否是保留字
        if (std::find(reservedWords.begin(), reservedWords.end(), lowerIdentifier) == reservedWords.end()) {
            if (identifierCounts.find(lowerIdentifier) != identifierCounts.end()) {
                identifierCounts[lowerIdentifier]++;
            } else {
                identifierCounts[lowerIdentifier] = 1;
            }
        }
    }
    return identifierCounts;
}

// 写入结果到文件
void writeResultToFile(const std::map<std::string, int>& identifierCounts, const std::string& outputFile) {
    std::ofstream outFile(outputFile);
    if (outFile.is_open()) {
        // 使用反向迭代器遍历标识符及其出现次数，并反向输出结果
        for (auto iter = identifierCounts.rbegin(); iter != identifierCounts.rend(); ++iter) {
            outFile << "(" << iter->first << ", " << iter->second << ")" << std::endl;
        }
        outFile.close();
        std::cout << "Results have been written to " << outputFile << std::endl;
    } else {
        std::cerr << "Error: Unable to open file " << outputFile << " for writing." << std::endl;
    }
}

int main() {
    // 保留字列表begin、 call、 const、 do、end、 if、 odd、procedure、 read、 then、 var、 while、 write
    std::vector<std::string> reservedWords = {"begin", "call","const","do", "end", "if", "odd", "procedure", "read", "then", "var", "while","write", "else",  "repeat", "until"};

    // 读取输入源程序文件
    std::ifstream inFile("input.txt");
    if (!inFile.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return 1;
    }

    // 读取源程序内容
    std::string sourceCode((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    // 计算标识符出现次数，筛选掉保留字
    std::map<std::string, int> identifierCounts = countIdentifiers(sourceCode, reservedWords);

    // 写入结果到文件，按照标识符出现的反向顺序
    writeResultToFile(identifierCounts, "output.txt");

    return 0;
}
