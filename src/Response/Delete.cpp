# include "../../includes/Request.hpp"

void Response::Delete(Request &req, const std::string &path) {
    struct stat pathStat;
    if (stat(path.c_str(), &pathStat) != 0) {
        req.setStatusCode(404);
        return;
    }

    int deleted = -1;

    if (S_ISDIR(pathStat.st_mode)) {
        if (path.back() != '/') {
            req.setStatusCode(409);
            return;
        }

        DIR *dir = opendir(path.c_str());
        if (!dir) {
            req.setStatusCode(500);
            return;
        }

        struct dirent *ent;
        while ((ent = readdir(dir)) != nullptr) {
            std::string entryName = ent->d_name;
            if (entryName == "." || entryName == "..") continue;

            std::string entryPath = path + entryName;
            if (ent->d_type == DT_DIR) {
                entryPath += "/";
                Delete(req, entryPath);
                if (req.getStatus() != 204) {
                    closedir(dir);
                    return;
                }
            } else {
                deleted = remove(entryPath.c_str());
                if (deleted == -1) {
                    req.setStatusCode(500);
                    closedir(dir);
                    return;
                }
            }
        }

        closedir(dir);
        deleted = rmdir(path.c_str());
        if (deleted == -1) {
            req.setStatusCode(500);
            return;
        }
    } else if (S_ISREG(pathStat.st_mode)) {
        deleted = remove(path.c_str());
        if (deleted == -1) {
            req.setStatusCode(500);
            return;
        }
    } else {
        req.setStatusCode(400);
        return;
    }

    req.setStatusCode(204);
    // std::string response = req.getVersion() + " " + std::to_string(req.getStatusCode()) + " " + getStatusCodeMsg(req.getStatusCode()) + "\r\n\n";

    // std::ifstream file("error_pages/deleted.html");
    // if (!file.is_open()) {
    //     std::cerr << "Failed to open file\n";
    // } else {
        // std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        // send(req.clientSocket, response.c_str(), response.size(), 0);
        // send(req.clientSocket, body.c_str(), body.size(), 0);
        // send(req.clientSocket, "", 1, 0);
    // }

    // close(req.clientSocket);
}