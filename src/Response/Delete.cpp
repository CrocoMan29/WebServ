# include "../../includes/Request.hpp"

void Response::deleteContent(const std::string& path) {
    if (!access(path.c_str(), W_OK | X_OK)) {
        std::string entryName;
        DIR* dir = opendir(path.c_str());

        if (dir != NULL) {
            dirent* entry;
            while ((entry = readdir(dir)) != NULL) {
                entryName = entry->d_name;
                if (entryName != "." && entryName != "..") {
                    std::string full_path = path + "/" + entryName;
                    if (entry->d_type == DT_DIR) {
                        deleteContent(full_path);
                    } else {
                        if (access(full_path.c_str(), F_OK | W_OK) != 0)
                            throw (-1);
                        remove(full_path.c_str());
                    }
                }
            }
            closedir(dir);
        }
        if (rmdir(path.c_str()) == -1)
            throw (-1);
    } else {
        throw -1;
    }
}

void Response::delete_directory(const std::string& path, const Request& request) {

    if (request.getRequestInfo().at("path")[request.getRequestInfo().at("path").length() - 1] != '/') {
        this->setStatus(409);
    } else {
        this->setStatus(204);
        try {
            deleteContent(path);
        } catch(const int& e) {
            (!access(path.c_str(), W_OK | X_OK)) ? this->setStatus(500) : this->setStatus(403);
        }
    }
}

void Response::delete_file(const std::string& path) {
    if (access(path.c_str(), X_OK | W_OK) != 0) {
        this->setStatus(403);
        std::cout << "delete file with path:" << path << std::endl;
    } else {
        this->setStatus(204);
        remove(path.c_str());
    }
}

void Response::del(const Request& request) {    
    char cd[50];

    std::cout << "////////////////////DELETE\\\\\\\\\\\\\\\\\\\\" << std::endl;
    if (!getcwd(cd, sizeof(cd)))
        throw INTERNALSERVERERROR;
    std::string path(cd);
    path = path +"/"+request.getPath();
    
    if (this->_isDeleted == 0) {
        std::cout << "delete path : " << path << std::endl;
        if (isDirectory(path.c_str())) {
            std::cout << "delete directory" << std::endl;
            delete_directory(path, request);
        } else if (isFile(path.c_str())) {
            std::cout << "delete file" << std::endl;
            delete_file(path);
        } else {
            this->setStatus(404);
        }
        this->_isDeleted = 1;
    }

    // if (this->_status != 204) {
    // }
}

bool isFile(const char* path) {
    struct stat s;
    if (stat(path, &s) == 0) {
        return S_ISREG(s.st_mode);
    }
    return false;
}