# include "../../includes/Request.hpp"
# include "../../includes/Response.hpp"

std::string randomFileGenerator() {
    std::string randomFile;
    std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    for (size_t i = 0; i < 10; i++) {
        randomFile += charset[rand() % charset.length()];
    }
    return (randomFile);
}

void Response::postResponse(Request &request, Location &location) {
    std::string file = randomFileGenerator();
    if(location.upload_enable == false) {
        std::cout << "Upload is not enabled" << std::endl;
        return;
    }
    
    std::string path = location.upload_store + "/" + file;
    std::ofstream ofs(path);
    if (ofs.is_open()) {
        ofs << request._requestInfos["body"];
        ofs.close();
        std::cout << "File uploaded successfully" << std::endl;
    } else {
        std::cout << "Error opening file" << std::endl;
    }
}