#include <iostream>
#include <cstring> // 包含 cstring 头文件
#include <direct.h> // 包含 unistd.h 头文件

int main() {
    char currentPath[FILENAME_MAX];
    
    // 获取当前工作目录
    if (getcwd(currentPath, sizeof(currentPath)) != nullptr) {
        std::cout << "Current working directory: " << currentPath << std::endl;
    } else {
        std::cerr << "Error: Unable to get current working directory" << std::endl;
    }
    system("pause");
    return 0;
}
