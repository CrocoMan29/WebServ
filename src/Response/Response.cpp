# include "../../includes/Request.hpp"
// # include "../../includes/Response.hpp"

Response::Response() : status(0), socket(0), readed(false), isCGI(false), isError(false),finish(false) {
    // path = "";
    // method = "";
	this->count = 0;
	this->env = NULL;
// 	this->start = 0;
// 	this->end = 0;
}


void Response::sendResp(Request req, int socket)
{
	if (!this->readed) {
		// exit(1);
		std::cout << "==========================2Rspsonse started2..... ?=================================" << std::endl;
		this->req = req;
		this->socket = socket;
		this->chunkSize = "";
		std::cout << "sock/ : " << this->socket << std::endl;
		this->path = req.getRequestInfo()["path"];
		// this->absolutPath = req.getRequestInfo()["path"];
		this->method = req.getRequestInfo()["method"];
		this->querry = req.getRequestInfo()["query"];
		// this->cookies = req.getRequestInfo()["cookies"];
		if (!req.getStatus())
			// this->status = req._status;
			this->status = 200;
		// this->readed = false;
		this->valueOfAutoIndex = req.getLocation().autoIndex;
		this->indexFile = req.getIndexes();
		for (std::vector<std::string>::iterator it = indexFile.begin(); it != indexFile.end(); it++ ) {
			std::cout << "index File:  " << *it << std::endl;
		}
		// vars = true;
		std::cout << "Path: " << this->path << std::endl;
		std::cout << "Method: " << this->method << std::endl;
		std::cout << "stat: " << this->status << std::endl;
		std::cout << "AUTOindex: " << this->valueOfAutoIndex << std::endl;
		std::cout << "QUERRY: " << this->querry << std::endl;
		// exit(2);
		// exit(1);
		// setHeader();
	}
	if (checkPath(req)) {
		std::cout << "checkPath---------========>" << std::endl;
		std::cout << "Error Flag====>" << this->isError << std::endl;
		if (this->readed && !this->isError) {
			setHeader();
			this->isError = true;
		}
		else if (this->isError && !this->readed) {
			this->readed = true;
			if (this->readed) {
				setHeader();
			}
		}
		chunk(req);
	}
}

Response::Response(const Response& copy) {
    this->type = copy.type;
    this->header = copy.header;
    this->path = copy.path;
    this->chunkSize = copy.chunkSize;
    this->method = copy.method;
    this->status = copy.status;
    this->socket = copy.socket;
    this->readed = copy.readed;
}

Response& Response::operator=(const Response& rhs) {
    if (this != &rhs) {
        this->type = rhs.type;
        this->header = rhs.header;
        this->path = rhs.path;
        this->chunkSize = rhs.chunkSize;
        this->method = rhs.method;
        this->status = rhs.status;
        this->socket = rhs.socket;
        this->readed = rhs.readed;
    }   
    return (*this);
}


Response::~Response() {
	std::cout << "===================response end============================" << std::endl;
}

bool Response::isDirectory(const std::string& path) {
	struct stat save;
	if ((stat(path.c_str(), &save) == 0) && S_ISDIR(save.st_mode))
		return (true);
	return (false);
}

bool Response::isRegularFile(const std::string& path) {
	struct stat save;
	if ((stat(path.c_str(), &save) == 0) && S_ISREG(save.st_mode))
		return (true);
	return (false);
}

std::string Response::getContentType(std::string& path) {

	this->mimetypes[".html"] = "text/html";
	this->mimetypes[".htm"] = "text/html";
	this->mimetypes[".shtml"] = "text/html";
	this->mimetypes[".css"] = "text/css";
	this->mimetypes[".xml"] = "text/xml";
	this->mimetypes[".gif"] = "image/gif";
	this->mimetypes[".jpeg"] = "image/jpeg";
	this->mimetypes[".jpg"] = "image/jpeg";
	this->mimetypes[".js"] = "application/javascript";
	this->mimetypes[".atom"] = "application/atom+xml";
	this->mimetypes[".rss"] = "application/rss+xml";
	this->mimetypes[".mml"] = "text/mathml";
	this->mimetypes[".txt"] = "text/plain";
	this->mimetypes[".jad"] = "text/vnd.sun.j2me.app-descriptor";
	this->mimetypes[".wml"] = "text/vnd.wap.wml";
	this->mimetypes[".htc"] = "text/x-component";
	this->mimetypes[".avif"] = "image/avif";
	this->mimetypes[".png"] = "image/png";
	this->mimetypes[".svg"] = "image/svg+xml";
	this->mimetypes[".svgz"] = "image/svg+xml";
	this->mimetypes[".tiff"] = "image/tiff";
	this->mimetypes[".tif"] = "image/tiff";
	this->mimetypes[".wbmp"] = "image/vnd.wap.wbmp";
	this->mimetypes[".webp"] = "image/webp";
	this->mimetypes[".ico"] = "image/x-icon";
	this->mimetypes[".jng"] = "image/x-jng";
	this->mimetypes[".bmp"] = "image/x-ms-bmp";
	this->mimetypes[".woff"] = "font/woff";
	this->mimetypes[".woff2"] = "font/woff2";
	this->mimetypes[".jar"] = "application/java-archive";
	this->mimetypes[".war"] = "application/java-archive";
	this->mimetypes[".ear"] = "application/java-archive";
	this->mimetypes[".json"] = "application/json";
	this->mimetypes[".hqx"] = "application/mac-binhex40";
	this->mimetypes[".doc"] = "application/msword";
	this->mimetypes[".pdf"] = "application/pdf";
	this->mimetypes[".ps"] = "application/postscript";
	this->mimetypes[".eps"] = "application/postscript";
	this->mimetypes[".ai"] = "application/postscript";
	this->mimetypes[".rtf"] = "application/rtf";
	this->mimetypes[".m3u8"] = "application/vnd.apple.mpegurl";
	this->mimetypes[".kml"] = "application/vnd.google-earth.kml+xml";
	this->mimetypes[".kmz"] = "application/vnd.google-earth.kmz";
	this->mimetypes[".xls"] = "application/vnd.ms-excel";
	this->mimetypes[".eot"] = "application/vnd.ms-fontobject";
	this->mimetypes[".ppt"] = "application/vnd.ms-powerpoint";
	this->mimetypes[".odg"] = "application/vnd.oasis.opendocument.graphics";
	this->mimetypes[".odp"] = "application/vnd.oasis.opendocument.presentation";
	this->mimetypes[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	this->mimetypes[".odt"] = "application/vnd.oasis.opendocument.text";
	this->mimetypes[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	this->mimetypes[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	this->mimetypes[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	this->mimetypes[".wmlc"] = "application/vnd.wap.wmlc";
	this->mimetypes[".wasm"] = "application/wasm";
	this->mimetypes[".7z"] = "application/x-7z-compressed";
	this->mimetypes[".cco"] = "application/x-cocoa";
	this->mimetypes[".jardiff"] = "application/x-java-archive-diff";
	this->mimetypes[".jnlp"] = "application/x-java-jnlp-file";
	this->mimetypes[".run"] = "application/x-makeself";
	this->mimetypes[".pl"] = "application/x-perl";
	this->mimetypes[".pm"] = "application/x-perl";
	this->mimetypes[".prc"] = "application/x-pilot";
	this->mimetypes[".pdb"] = "application/x-pilot";
	this->mimetypes[".rar"] = "application/x-rar-compressed";
	this->mimetypes[".rpm"] = "application/x-redhat-package-manager";
	this->mimetypes[".sea"] = "application/x-sea";
	this->mimetypes[".swf"] = "application/x-shockwave-flash";
	this->mimetypes[".sit"] = "application/x-stuffit";
	this->mimetypes[".tcl"] = "application/x-tcl";
	this->mimetypes[".tk"] = "application/x-tcl";
	this->mimetypes[".der"] = "application/x-x509-ca-cert";
	this->mimetypes[".pem"] = "application/x-x509-ca-cert";
	this->mimetypes[".crt"] = "application/x-x509-ca-cert";
	this->mimetypes[".xpi"] = "application/x-xpinstall";
	this->mimetypes[".xhtml"] = "application/xhtml+xml";
	this->mimetypes[".xspf"] = "application/xspf+xml";
	this->mimetypes[".zip"] = "application/zip";
	this->mimetypes[".bin"] = "application/octet-stream";
	this->mimetypes[".exe"] = "application/octet-stream";
	this->mimetypes[".dll"] = "application/octet-stream";
	this->mimetypes[".deb"] = "application/octet-stream";
	this->mimetypes[".dmg"] = "application/octet-stream";
	this->mimetypes[".iso"] = "application/octet-stream";
	this->mimetypes[".img"] = "application/octet-stream";
	this->mimetypes[".msi"] = "application/octet-stream";
	this->mimetypes[".msp"] = "application/octet-stream";
	this->mimetypes[".msm"] = "application/octet-stream";
	this->mimetypes[".mid"] = "audio/midi";
	this->mimetypes[".midi"] = "audio/midi";
	this->mimetypes[".kar"] = "audio/midi";
	this->mimetypes[".mp3"] = "audio/mpeg";
	this->mimetypes[".ogg"] = "audio/ogg";
	this->mimetypes[".m4a"] = "audio/x-m4a";
	this->mimetypes[".ra"] = "audio/x-realaudio";
	this->mimetypes[".3gpp"] = "video/3gpp";
	this->mimetypes[".3gp"] = "video/3gpp";
	this->mimetypes[".ts"] = "video/mp2t";
	this->mimetypes[".mp4"] = "video/mp4";
	this->mimetypes[".mpg"] = "video/mpeg";
	this->mimetypes[".mpeg"] = "video/mpeg";
	this->mimetypes[".video/quicktime"] = "mov";
	this->mimetypes[".webm"] = "video/webm";
	this->mimetypes[".flv"] = "video/x-flv";
	this->mimetypes[".m4v"] = "video/x-m4v";
	this->mimetypes[".mng"] = "video/x-mng";
	this->mimetypes[".asx"] = "video/x-ms-asf";
	this->mimetypes[".asf"] = "video/x-ms-asf";
	this->mimetypes[".avi"] = "video/x-msvideo";
	this->mimetypes[".wmv"] = "video/x-ms-wmv";
	
	size_t found = path.rfind('.');
	if (found != std::string::npos) {
		std::string ext = path.substr(found);
		std::map<std::string, std::string>::iterator it = this->mimetypes.find(ext);
		if (it != this->mimetypes.end())
			return (it->second);
	}
	return  ("text/html");
}


std::string	Response::getStatus(int stat) {
	//SUCCESS
	this->code[200] = "200 OK";
	this->code[201] = "201 Created";
	this->code[202] = "202 Accepted";
	//redir
	this->code[301] = "301 Moved Permanently";
	//CLient Error
	this->code[400] = "400 Bad Request";
	this->code[401] = "401 Unauthorized";
	this->code[403] = "403 Forbidden";
	this->code[404] = "404 Not Found";
	this->code[405] = "405 Method Not Allowed";
	this->code[411] = "411 Length Required";
	this->code[415] = "415 Unsupported Media Type";
	//Server Error
	this->code[500] = "500 Internal Server Error";
	this->code[501] = "501 Not Implemented";
	this->code[502] = "502 Bad Gateway";
	this->code[503] = "503 Service Unavailable";
	this->code[504] = "504 Gateway Timeout";

	if (this->code.find(stat) != this->code.end())
		return (this->code[stat]);
	else
		return (" Unknown status code");
}

void    Response::setHeader() {
	std::cout << "=================================HEADER================================" << std::endl;
	std::cout << "Status code: " << this->status << std::endl;
	this->header += "HTTP/1.1 " + getStatus(this->status) + "\r\n";
	if (this->status == 301)
		this->header += "Location: " + this->path + "\r\n\r\n";
	else {
		this->header += "Content-Type: " + getContentType(this->path) + "\r\n";
		this->header += "Transfer-Encoding: chunked\r\n";
		// this->header += " Content-Length: 13\n\nHello world!1\r\n";
	}
	this->header += "connection: close\r\n\r\n";
	std::cout << "===========Head=======\n" << this->header << std::endl;
	write(this->socket, this->header.c_str(), this->header.length());
	std::cout << "socket: " << this->socket << std::endl;
	std::cout << "=================================HEADER END================================" << std::endl;
}

void	Response::chunk(Request& req) {
	char buf[BUFFERSIZE] = {0};
	// std::cout << "content-length----->" << std::endl;
	std::cout << "path: ---->" << this->path << std::endl;
	file.read(buf, 1023);
	this->count += file.gcount();
	std::cout << "size file: " << this->count << std::endl; 
	if (file.gcount() > 0 && this->readed) {
		// std::cout << "statuscode: " << this->status << std::endl;
		std::stringstream ss;
        ss << std::hex << file.gcount();
		std::cout << "buffer size: " << ss.str() << std::endl;
		this->chunkSize = ss.str() + "\r\n";
		this->chunkSize.append(buf, file.gcount());
        this->chunkSize.append("\r\n", 2);
		std::cout << "Sockeeeeeeet-->: " << this->socket << std::endl;
		std::cout << "File: ----->" << this->chunkSize.c_str() << std::endl;
		write(this->socket, this->chunkSize.c_str(), this->chunkSize.length());
		std::cout << "Sockeeeeeeet--<: " << this->socket << std::endl;
	}
	else if (file.gcount() == 0 && this->readed) {
		std::cout << "Gcout========" << file.gcount() << std::endl;
		// std::cout << "path= " << this->path << std::endl;
        write(this->socket, "0\r\n\r\n", 5);
        file.close();
		this->finish = true;
	}
}

int	Response::checkPath(Request req) {
	if (isDirectory(this->path)) {
		std::cout << "is Directory -------->" << std::endl;
		if (this->path.back() != '/') {
			std::cout << "search for '/' -------->" << std::endl;	
			this->status = 301;
			this->path += '/';
			setHeader();
			exit(11);
		}
		else if (this->path.back() == '/') {
			std::cout << "find: ====  '/' -------->" << std::endl;
			if (directoryHasFiles(this->path)) {
				std::cout << "DIr has files:  -------->" << std::endl;
				// exit(2);
				if (directoryHasIndexFile(this->path)) {
					// std::cout << "has files:  -------->" << std::endl;
					std::cout << "has files:  -------->"<< this->path << std::endl;
					checkIndexFiles();
					if (!valueOfAutoIndex) {
						std::cout << " FOrbidden:  -------->" << std::endl;
						this->path = "./error/403.html";
						this->isError = true;
						file.open(this->path, std::ios::binary);
					}
					else if (valueOfAutoIndex) {
						std::cout << " Read from the file:  -------->" << std::endl;
						listDir();
						this->isError = true;
					}
				}
			}
		}
	}
	// else if (((this->path.rfind(".py") != std::string::npos) || (this->path.rfind(".php")  != std::string::npos))) {
	else if (getExt() ) {
		std::cout << "CGI----</ " << std::endl;
		// exit(2);
		// std::ifstream file;
		file.open(this->path.c_str(), std::ios::binary);
		std::cout << "CGI PATH = " << this->path << std::endl;
		this->scriptfile = this->path.substr(4);
		std::cout << "Script file name: " << this->scriptfile << std::endl;
		if (file.is_open() && !this->isCGI) { 
			std::cout << "cgi file opened->: " << std::endl;
			file.close();
            if (this->path.rfind(".php") != std::string::npos)
                this->pathCgi = "/usr/bin/php-cgi";
            else
                this->pathCgi = "/usr/bin/python3";
			std::cout << "CGI path:  " << this->pathCgi << std::endl;
			executeCgi(req);
			this->isError = true;
			std::cout << "PATH OPENED BY CGI: " << this->path << std::endl;
			// exit(2);
			file.open(this->path, std::ios::binary);
			this->isCGI = true;
			// exit(23);
		}
		else if (!file.is_open() && !this->isCGI){
			std::cout << "cgi file Not found-------<" << std::endl;
			// exit(2);
			this->status = 404;
			this->path = "./error/error.html";
			this->isError = true;
			file.open(this->path, std::ios::binary);
			// this->isCGI = true;
			std::cout << "CGI path= " << this->path << std::endl;
		}
	}
	else if (isRegularFile(this->path)) {
		std::cout << "is Regular file: " << std::endl;
		// exit(15);
		if (!this->readed) {
			std::cout << "Here-------------->check if open <----------Here" << std::endl;
			file.open(this->path, std::ios::binary);
			if (!file.good()) {
				std::cout << "Here--------------> <----------Here" << std::endl;
				// if (access(this->path.c_str(), R_OK) != -1) {
				if (access(this->path.c_str(), F_OK) != -1) {
					std::cout << "don't have access" << std::endl;
					this->status = 403;
					this->path = "./error/403.html";
					// file.open(this->path, std::ios::binary);
					// exit(12);
				}
				else {
					std::cout << "file not found" << std::endl;
					this->status = 404;
					this->path = "./error/error.html";
				}
				file.open(this->path, std::ios::binary);
				this->isError = true;
			}
			else {
				// std::cout << "Here--------------> <----------Here" << std::endl;
				// exit(15);
				this->readed = true;
			}

		}
	}
	else {
		std::cout << "Not found-------<" << std::endl;
		this->status = 404;
		this->path = "./error/error.html";
		this->isError = true;
		file.open(this->path, std::ios::binary);
	}
	return (1);
}
bool	Response::getExt() {
	if ((this->path.rfind(".py") != std::string::npos) || (this->path.rfind(".php") != std::string::npos))
		return (true);
	return (false);
}
bool Response::directoryHasFiles(const std::string& directoryPath) {
    DIR* dir = opendir(directoryPath.c_str());
    if (dir == NULL) 
        return false;
    std::vector<std::string> files;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            files.push_back(entry->d_name);
        // }
    }
    closedir(dir);
    return (!files.empty());
}

bool Response::directoryHasIndexFile(const std::string& directoryPath) {
    DIR* dir = opendir(directoryPath.c_str());
    if (dir == NULL)
        return (false);
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // if (strcmp(entry->d_name, "index.html") == 0 || strcmp(entry->d_name, "index.htm") == 0) {
            // closedir(dir);
            return (true);
        // }
    }
    closedir(dir);
    return (false);
}

void	Response::listDir() {
	DIR	*dir = opendir(this->path.c_str());
	if (dir) {
		struct dirent* entry;
		std::string name;
		std::string body = "<html><head></head><body><ul>";
		while ((entry = readdir(dir)) != NULL) {
			name = entry->d_name;
			std::cout << "Name---->: " << name << std::endl;
			// exit(21);
			body += "<li><a href='"+ name  +"'>"  + name +"</a></li>";
		}
		closedir(dir);
		std::stringstream ss;
        setHeader();
        ss << std::hex << body.length();
        this->body += ss.str() + "\r\n";
        this->body += body + "\r\n";
        this->body += "0\r\n\r\n";
        write(this->socket, this->body.c_str(),  this->body.length());
	}
}

void	Response::checkIndexFiles() {
	for (size_t i = 0; i < this->indexFile.size(); i++) {
		std::string newPath = this->path + this->indexFile[i];
		std::cout << "new path1/: " << this->path << std::endl;
		this->file.open(newPath.c_str(), std::ios::binary);
		if (file.is_open()) {
			this->path = newPath;
			std::cout << "new path/->>>>>>: " << this->path << std::endl;
			if (getExt()) {
				file.close();
				if (this->path.rfind(".php") != std::string::npos)
					this->pathCgi = "/usr/bin/php-cgi";
				else
					this->pathCgi = "/usr/bin/python3";
			}
			else {
				this->status = 200;
				this->isError = true;
				return ;
			}
		}
	}
}

int		Response::fillEnv(Request req) {
	this->env = new char* [9];
	env[0] = strdup(("REQUEST_METHOD=" + this->method).c_str());
	// env[1] = strdup(("QUERY_STRING=" + this->querry));
	env[2] = strdup("REDIRECT_STATUS=200");
	env[3] = strdup(("PATH_INFO=" + this->path).c_str());
	env[4] = strdup(("SCRIPT_FILENAME=" + this->scriptfile).c_str());
	env[5] = strdup(("CONTENT_TYPE=" + getContentType(this->path)).c_str());
	if (this->method == "GET") 
		env[6] = strdup("CONTENT_LENGTH=0");
	// else {

	// }
	// env[7] = strdup(("HTTP_COOKIE=" + this->cookies));
	env[8] = NULL;
	return(1);
}

void	Response::ft_free(char **env) {
	for (int i = 0; env[i]; i++) {
		free(env[i]);
	}
	delete[] env;
}

std::string Response::toString(long long nb) {
	std::stringstream ss;
	ss << nb;
	return (ss.str());
}

void Response::executeCgi(Request req) {
	this->isCGI = true;
	this->start = clock();
	srand(time(NULL));
	this->generatedtPath = "./WWW/CGI/" + toString(rand());
	fillEnv(req);
	int fd[2]; 	
	if (pipe(fd) == -1) {
		perror("pipe");
		return ;
	}
	this->pid = fork();
	if (this->pid == -1) {
		perror("fork");
		return ;
	}
	std::cout << "PID " << this->pid << std::endl;
	if (this->pid == 0) {
		// exit(55);
		const char *av[] = {this->pathCgi.c_str(), this->path.c_str(), NULL};
		std::cout << "THis CGI path : " << this->pathCgi << std::endl;
		std::cout << "THis path : " << this->path << std::endl;
		close(fd[0]);
		std::cout << "THis generated path : " << this->generatedtPath << std::endl;
		std::cout << "THis file name1 : " << this->scriptfile << std::endl;
		std::cout << "av[0]: " << av[0] << std::endl;
		// exit(21);
		freopen(this->generatedtPath.c_str(), "w", stdout);	
		fclose(stdout);
		std::cout << "Execve: " << std::endl;
		if (this->method == "GET")
			close(STDIN_FILENO);	
		else {
			freopen(this->scriptfile.c_str(),"r", stdin);
			std::cout << "THis file name : " << this->scriptfile << std::endl;
		}
		if (execve(av[0], (char* const*)av, this->env) < 0) {
			perror("execve");
			exit(127);
		}
	}
	else {
		std::cout << "Parent process: " << std::endl;
		std::cout << "ENV: " << this->env << std::endl;
		// exit(20);
		int stat;
		waitpid(this->pid, &stat, WNOHANG);
		// waitpid(this->pid, &status, WEXITED);

	}
	// pid_t pidWait = waitpid(this->pid, &status, WNOHANG);
	// this->end = clock();
	// double processTime = (this->end - this->start) / CLOCKS_PER_SEC;
	// if (pidWait == -1 || pidWait > 0 || processTime > 5) {
	// 	char buf[1024];
	// 	std::string str;
	// 	std::stringstream ss;
	// 	this->path = this->generatedtPath;
	// 	this->file.open(this->path.c_str(), ios::in | ios::binary);
	// 	int status;
	// 	if (status != 0 || processTime > 5)
    //     {
    //         if (status != 0)
    //             this->status = 500;
    //         else {
    //             kill(this->pid, SIGKILL);
    //             waitpid(pid, 0, 0);
    //             this->status = 504;
    //         }
    //         file.close();
	// 		ft_free(this->env);
	// 		remove(this->path.c_str());
	// 		remove(scriptfile.c_str());
    //     }
	// }
}

void Response::update(const Request &request){
	this->req = request;
}