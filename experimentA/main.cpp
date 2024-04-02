#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <vector>
#include <algorithm>

// ���ַ���ת��ΪСд
std::string toLower(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// �����ʶ�����ִ���
std::map<std::string, int> countIdentifiers(const std::string& sourceCode, const std::vector<std::string>& reservedWords) {
    std::regex pattern("[a-zA-Z][a-zA-Z0-9]*");
    std::map<std::string, int> identifierCounts;
    auto words_begin = std::sregex_iterator(sourceCode.begin(), sourceCode.end(), pattern);
    auto words_end = std::sregex_iterator();
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::string identifier = (*i).str();
        // ����ʶ��ת��ΪСд
        std::string lowerIdentifier = toLower(identifier);
        // ����ʶ���Ƿ��Ǳ�����
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

// д�������ļ�
void writeResultToFile(const std::map<std::string, int>& identifierCounts, const std::string& outputFile) {
    std::ofstream outFile(outputFile);
    if (outFile.is_open()) {
        // ʹ�÷��������������ʶ��������ִ�����������������
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
    // �������б�begin�� call�� const�� do��end�� if�� odd��procedure�� read�� then�� var�� while�� write
    std::vector<std::string> reservedWords = {"begin", "call","const","do", "end", "if", "odd", "procedure", "read", "then", "var", "while","write", "else",  "repeat", "until"};

    // ��ȡ����Դ�����ļ�
    std::ifstream inFile("input.txt");
    if (!inFile.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return 1;
    }

    // ��ȡԴ��������
    std::string sourceCode((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    // �����ʶ�����ִ�����ɸѡ��������
    std::map<std::string, int> identifierCounts = countIdentifiers(sourceCode, reservedWords);

    // д�������ļ������ձ�ʶ�����ֵķ���˳��
    writeResultToFile(identifierCounts, "output.txt");

    return 0;
}
