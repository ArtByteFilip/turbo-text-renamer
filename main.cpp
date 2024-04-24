#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <cstring>

void replaceEmailInFile(const std::string &filePath, const std::string &oldEmail, const std::string &newEmail);
void processFilesInDirectory(const std::string &directoryPath, const std::string &oldEmail, const std::string &newEmail);

int main(int argc, char *argv[]) {

    std::string oldEmail, newEmail;
    if (argc >= 3) {
        oldEmail = argv[1];
        newEmail = argv[2];

        std::string currentDirectory = ".";
        processFilesInDirectory(currentDirectory, oldEmail, newEmail);
    } else {
        std::cerr << "Error: Bad arguments." << std::endl;
        std::cerr << "Info: Program ignores .exe and .cpp files." << std::endl;
        std::cerr << "Example: program.exe example1@mail.com example2@mail.com" << std::endl;
        return 1;
    }

    return 0;
}

void replaceEmailInFile(const std::string &filePath, const std::string &oldEmail, const std::string &newEmail) {
    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Cannot open file: " << filePath << std::endl;
        return;
    }

    std::string fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    size_t pos = fileContent.find(oldEmail);
    while (pos != std::string::npos) {
        fileContent.replace(pos, oldEmail.length(), newEmail);
        pos = fileContent.find(oldEmail, pos + newEmail.length());
    }

    inputFile.close();

    std::ofstream outputFile(filePath);
    if (!outputFile) {
        std::cerr << "Cannot open file for writing: " << filePath << std::endl;
        return;
    }

    outputFile << fileContent;
}

void processFilesInDirectory(const std::string &directoryPath, const std::string &oldEmail, const std::string &newEmail) {
    DIR *directory = opendir(directoryPath.c_str());
    if (directory == nullptr) {
        std::cerr << "Cannot open directory: " << directoryPath << std::endl;
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(directory)) != nullptr) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            std::string fileName = entry->d_name;
            if (fileName.substr(fileName.find_last_of(".") + 1) != "cpp") {
                if (fileName.substr(fileName.find_last_of(".") + 1) != "exe") {
                    std::string filePath = directoryPath + "/" + fileName;
                if (entry->d_type == DT_DIR) {
                    processFilesInDirectory(filePath, oldEmail, newEmail);
                }
                else if (entry->d_type == DT_REG) {
                    replaceEmailInFile(filePath, oldEmail, newEmail);
                }
                }
                
            }
        }
    }

    closedir(directory);
}
