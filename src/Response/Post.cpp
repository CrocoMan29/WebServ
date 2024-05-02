# include "../../includes/Request.hpp"
# include "../../includes/Response.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

std::string randomFileGenerator() {
    std::string randomFile;
    std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    for (size_t i = 0; i < 10; i++) {
        randomFile += charset[rand() % charset.length()];
    }
    return (randomFile);
}

void Response::postResponse(Request &request, Location &location) {
    std::string file = randomFileGenerator() + request.getExtension();
    // std::string extension = request.getExtension();
    if(location.upload_enable == false) {
        std::cout << "Upload is not enabled" << std::endl;
        return;
    }
    
    std::string path = "/home/yassinelr/Desktop/WebServ"+location.upload_store + "/" + file;
    std::cout << "Path: " << path << std::endl;
    // struct stat st = {0};
    // if (stat(location.upload_store.c_str(), &st) == -1) {
    //     if (mkdir(location.upload_store.c_str(), 0700) == -1) {
    //         std::cerr << "Failed to create directory: " << strerror(errno) << std::endl;
    //         return;
    //     }
    // }
    std::ofstream ofs(path);
    if (ofs.is_open()) {
        ofs << request.getBody();
        ofs.close();
        std::cout << "File uploaded successfully" << std::endl;
    } else {
        std::cout << "Error opening file" << std::endl;
         perror("Error");
    }
}