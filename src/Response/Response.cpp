// # include "../../includes/Response.hpp"
#include "../../includes/Request.hpp"

Response::Response() : status(0), socket(0), readed(false), isCGI(false), isError(false), finish(false) , isNot(false)
{
	// path = "";
	// method = "";
	this->_isDeleted = 0;
	this->count = 0;
	this->env = NULL;
	this->start = 0;
	this->end = 0;
}

void Response::sendResp(Request req, int socket)
{
	if (!this->readed)
	{
		// std::cout << "===========	===============Rspsonse started..... ?=================================" << std::endl;
		this->req = req;
		this->socket = socket;
		this->chunkSize = "";
		this->path = req.getRequestInfo()["path"];
		this->reqType = req.getRequestInfo()["content-type"];
		this->method = req.getRequestInfo()["method"];
		this->querry = req.getRequestInfo()["query"];
		this->cookies = req.getRequestInfo()["cookie"];
		this->conLength = req.getRequestInfo()["content-length"];
		this->status = req.getStatus();
		this->valueOfAutoIndex = req.getLocation().autoIndex;
		this->indexFile = req.getIndexes();
		this->scriptfile = this->path;
		this->postpath = req.getAbspath();
	}
	bool f = false;
	if (req.isBadRequest() && !f)
	{
		f = true;
		if (this->status == 400) {
			this->path = "./error/400.html";
			file.open(this->path.c_str(), std::ios::binary);
		}
		else if (this->status == 501) {
			this->path = "./error/501html";
			file.open(this->path.c_str(), std::ios::binary);
		}
		else if (this->status == 413) {
			this->path = "./error/413.html";
			file.open(this->path.c_str(), std::ios::binary);
		}
		else if (this->status == 404) {
			this->path = "./error/error.html";
			file.open(this->path.c_str(), std::ios::binary);
		}
		else if (this->status == 403) {
			this->path = "./error/403.html";
			file.open(this->path.c_str(), std::ios::binary);
		}
		else if (this->status == 405) {
			this->path = "./error/405.html";
			file.open(this->path.c_str(), std::ios::binary);
		}
		if (!this->readed) {
			setHeader(); 
			this->readed = true;
		}
		chunk(req);
		write(this->socket, "0\r\n\r\n", 5);
		file.close();
		this->finish = true;
	}
	if (this->method == "GET" && !req.isBadRequest())
	{
		if (checkPath(req))
		{
			if (this->readed && !this->isError)
			{
				setHeader();
				this->isError = true;
			}
			else if (this->isError && !this->readed)
			{
				this->readed = true;
				if (this->readed)
				{
					setHeader();
				}
			}
			chunk(req);
		}
	}
	else if (this->method == "POST" && !req.isBadRequest())
	{
		if (checkPath(req))
		{
			if (this->readed && !this->isError)
			{
				setHeader();
				this->isError = true;
			}
			else if (this->isError && !this->readed)
			{
				this->readed = true;
				if (this->readed)
				{
					setHeader();
				}
			}
			chunk(req);
		}
	}
	else if (this->method == "DELETE" && !req.isBadRequest())
	{
		this->del(req);
		if (checkPath(req)) 
		{
			if (!this->readed) {
				setHeader(); 
				this->readed = true;
			}
			file.open(this->path.c_str(), std::ios::binary);
			chunk(req);
			write(this->socket, "0\r\n\r\n", 5);
			file.close();
			this->finish = true;
		}
	}
}

Response::Response(const Response &copy)
{
	this->type = copy.type;
	this->header = copy.header;
	this->path = copy.path;
	this->chunkSize = copy.chunkSize;
	this->method = copy.method;
	this->body = copy.body;
	this->status = copy.status;
	this->generatedtPath = copy.generatedtPath;
	this->scriptfile = copy.scriptfile;
	this->status = copy.status;
	this->pathCgi = copy.pathCgi;
	this->readed = copy.readed;
	this->_isDeleted = copy._isDeleted;
}

Response &Response::operator=(const Response &rhs)
{
	if (this != &rhs)
	{
		this->type = rhs.type;
		this->header = rhs.header;
		this->path = rhs.path;
		this->chunkSize = rhs.chunkSize;
		this->method = rhs.method;
		this->status = rhs.status;
		this->socket = rhs.socket;
		this->readed = rhs.readed;
		this->_isDeleted = rhs._isDeleted;
	}
	return (*this);
}

Response::~Response() {}

bool Response::isDirectory(const std::string &path)
{
	struct stat statbuf;
	if (stat(path.c_str(), &statbuf) != 0)
	{
		return false; // Cannot access path
	}
	return S_ISDIR(statbuf.st_mode);
}

bool Response::isRegularFile(const std::string &path)
{
	struct stat save;
	if ((stat(path.c_str(), &save) == 0) && S_ISREG(save.st_mode))
		return (true);
	return (false);
}

std::string Response::getContentType(std::string &path)
{

	this->mimetypes[".html"] = "text/html";
	this->mimetypes[".htm"] = "text/html";
	this->mimetypes[".shtml"] = "text/html";
	this->mimetypes[".css"] = "text/css";
	this->mimetypes[".xml"] = "text/xml";
	this->mimetypes[".gif"] = "image/gif";
	this->mimetypes[".jpeg"] = "image/jpeg";
	this->mimetypes[".jpg"] = "image/jpeg";
	this->mimetypes[".js"] = "application/javascript";
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
	this->mimetypes[".jnlp"] = "application/x-java-jnlp-file";
	this->mimetypes[".run"] = "application/x-makeself";
	this->mimetypes[".pl"] = "application/x-perl";
	this->mimetypes[".pm"] = "application/x-perl";
	this->mimetypes[".prc"] = "application/x-pilot";
	this->mimetypes[".pdb"] = "application/x-pilot";
	this->mimetypes[".rar"] = "application/x-rar-compressed";
	this->mimetypes[".rpm"] = "application/x-	-package-manager";
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
	if (found != std::string::npos)
	{
		std::string ext = path.substr(found);
		std::map<std::string, std::string>::iterator it = this->mimetypes.find(ext);
		if (it != this->mimetypes.end())
			return (it->second);
	}
	return ("text/html");
}

std::string Response::getStatus(int stat)
{
	this->code[200] = "200 OK";
	this->code[201] = "201 Created";
	this->code[202] = "202 Accepted";
	this->code[204] = "204 No Content";
	this->code[301] = "301 Moved Permanently";
	this->code[400] = "400 Bad Request";
	this->code[401] = "401 Unauthorized";
	this->code[403] = "403 Forbidden";
	this->code[404] = "404 Not Found";
	this->code[405] = "405 Method Not Allowed";
	this->code[409] = "409 Conflict";
	this->code[411] = "411 Length Required";
	this->code[413] = "413 Content Too Large";
	this->code[415] = "415 Unsupported Media Type";
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

void Response::setHeader()
{
	this->header += "HTTP/1.1 " + getStatus(this->status) + "\r\n";
	if (this->status == 301)
	{
		this->path = this->path.substr(4);
		this->header += "Location: /" + this->path + "\r\n\r\n";
	}
	else
	{
		this->header += "Content-Type: " + getContentType(this->path) + "\r\n";
		this->header += "Transfer-Encoding: chunked\r\n";
	}
	this->header += "connection: close\r\n";
	if ((this->scriptfile.find(".php") == std::string::npos))
	{
		this->header += "\r\n";
		write(this->socket, this->header.c_str(), this->header.length());
	}
	else {
		write(this->socket, this->header.c_str(), this->header.length());
	}
}

void Response::chunk(Request &req)
{
	unsigned long pos;
	std::string str;
	std::string sizereaded;
	std::stringstream ss;
	if (this->isCGI)
	{
		if (this->scriptfile.find(".php") != std::string::npos)
		{
			char buffer[BUFFERSIZE] = {0};
			file.read(buffer, 1023);
			ss << buffer;
			str = ss.str();
			if ((pos = str.find("\r\n\r\n")) != std::string::npos)
			{
				int i = (str.length() - (pos + 4)) * -1;
				this->cgiHeader = str.substr(0, pos + 4);
				this->header += this->cgiHeader;
				write(this->socket, this->header.c_str(), this->header.length());
				file.clear();
				file.seekg(i, std::ios_base::cur);
				std::string bodyy = buffer + (pos + 4);
				char buf[80000] = {0};
				file.read(buf, 80000);
				if (file.gcount() > 0 && this->readed)
				{
					std::stringstream sd;
					sd << std::hex << file.gcount();
					sizereaded = sd.str() + "\r\n";
					sizereaded.append(buf, file.gcount());
					sizereaded.append("\r\n", 2);
					write(this->socket, sizereaded.c_str(), sizereaded.length());
					this->isCGI = false;
				}
				else if (file.gcount() == 0 && this->readed)
				{
					write(this->socket, "0\r\n\r\n", 5);
					file.close();	
					this->finish = true;
					ft_free(this->env);
				}
			}
		}
		else
		{
			char buf[BUFFERSIZE] = {0};
			file.read(buf, 1023);
			if (file.gcount() > 0 && this->readed)
			{
				std::stringstream ss;
				ss << std::hex << file.gcount();
				this->chunkSize = ss.str() + "\r\n";
				this->chunkSize.append(buf, file.gcount());
				this->chunkSize.append("\r\n", 2);
				write(this->socket, this->chunkSize.c_str(), this->chunkSize.length());
			}
			else if (file.gcount() == 0 && this->readed)
			{
				write(this->socket, "0\r\n\r\n", 5);
				file.close();
				this->finish = true;
				this->isCGI = false;
				if (!this->env)
					ft_free(this->env);
			}
		}
	}
	else
	{
		char buf[BUFFERSIZE] = {0};
		file.read(buf, 1023);
		if (file.gcount() > 0 && this->readed)
		{
			std::stringstream ss;
			ss << std::hex << file.gcount();
			this->chunkSize = ss.str() + "\r\n";
			this->chunkSize.append(buf, file.gcount());
			this->chunkSize.append("\r\n", 2);
			write(this->socket, this->chunkSize.c_str(), this->chunkSize.length());

		}
		else if (file.gcount() == 0 && this->readed)
		{
			write(this->socket, "0\r\n\r\n", 5);
			file.close();
			this->finish = true;
		}
	}
}

int Response::checkPath(Request req)
{
		if (this->method == "POST" && (!((this->path.rfind(".py") != std::string::npos) || (this->path.rfind(".php") != std::string::npos))))
		{
			if (this->status == 201)
				this->path = "./error/201.html";
			else if (this->status == 403)
				this->path = "./error/403.html";
			else if (this->status == 404)
				this->path = "./error/error.html";
		}
		else if (this->method == "DELETE") {
			if (this->status == 204)
				this->path = "./error/204.html";
			else if (this->status == 403)
				this->path = "./error/403.html";
			else if (this->status == 404)
				this->path = "./error/error.html";
			else if (this->status == 409)
				this->path = "./error/409.html";
			else if (this->status == 500)
				this->path = "./error/500.html";
			return 1;
		}
		if (isDirectory(this->path))
		{
			if (this->path[this->path.size() - 1] != '/')
			{
				this->status = 301;
				this->path += '/';
				setHeader();
				this->finish = true;
			}
			else if (this->path[this->path.size() - 1] == '/')
			{
				if (directoryHasFiles(this->path))
				{
					if (directoryHasIndexFile(this->path))
					{
						checkIndexFiles();
						if (!valueOfAutoIndex)
						{
							this->path = "./error/403.html";
							this->isError = true;
							file.open(this->path.c_str(), std::ios::binary);
						}
						else if (valueOfAutoIndex)
						{
							listDir();
							this->isError = true;
						}
					}
				}
			}
		}
		else if (((this->path.rfind(".py") != std::string::npos) || (this->path.rfind(".php") != std::string::npos)))
		{
			if (this->path.rfind(".php") != std::string::npos)
				this->pathCgi = "/usr/bin/php-cgi";
			else
				this->pathCgi = "/usr/bin/python3";
			int d = executeCgi(req);
			if(d == 1) {
				this->path = this->generatedtPath;
				std::ifstream _file(this->generatedtPath.c_str(), std::ios::binary);
				if (_file.is_open() && (this->status == 200 || this->status == 201))
				{
					this->isError = true;
					file.open(this->path.c_str(), std::ios::binary);
					this->isCGI = true;
				}
				else if (this->status == 504)
				{
					this->path = "./error/504.html";
					this->isError = true;
					file.open(this->path.c_str(), std::ios::binary);
					this->isCGI = true;
				}
				else
				{
					this->status = 404;
					this->path = "./error/error.html";
					this->isError = true;
					file.open(this->path.c_str(), std::ios::binary);
					this->isCGI = true;
				}
			}
			else
				return 0;
			
		}
		else if (isRegularFile(this->path))
		{
			if (!this->readed)
			{
				file.open(this->path.c_str(), std::ios::binary);
				if (!file.good())
				{
					if (access(this->path.c_str(), F_OK) != -1)
					{
						this->status = 403;
						this->path = "./error/403.html";
					}
					else
					{
						this->status = 404;
						this->path = "./error/error.html";
					}
					file.open(this->path.c_str(), std::ios::binary);
					this->isError = true;
				}
				else
				{
					this->readed = true;
				}
			}
		}
		else
		{
			this->status = 404;
			this->path = "./error/error.html";
			this->isError = true;
			file.open(this->path.c_str(), std::ios::binary);
		}
		return (1);
}

bool Response::getExt()
{
	if ((this->path.rfind(".py") != std::string::npos) || (this->path.rfind(".php") != std::string::npos))
		return (true);
	return (false);
}
bool Response::directoryHasFiles(const std::string &directoryPath)
{
	DIR *dir = opendir(directoryPath.c_str());
	if (dir == NULL)
		return false;
	std::vector<std::string> files;
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		files.push_back(entry->d_name);
	}
	closedir(dir);
	return (!files.empty());
}

bool Response::directoryHasIndexFile(const std::string &directoryPath)
{
	DIR *dir = opendir(directoryPath.c_str());
	if (dir == NULL)
		return (false);
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		return (true);
	}
	closedir(dir);
	return (false);
}

void Response::listDir()
{
	DIR *dir = opendir(this->path.c_str());
	if (dir)
	{
		struct dirent *entry;
		std::string name;
		std::string body = "<html><head></head><body><ul>";
		while ((entry = readdir(dir)) != NULL)
		{
			name = entry->d_name;
			body += "<li><a href='" + name + "'>" + name + "</a></li>";
		}
		closedir(dir);
		std::stringstream ss;
		setHeader();
		ss << std::hex << body.length();
		this->body += ss.str() + "\r\n";
		this->body += body + "\r\n";
		this->body += "0\r\n\r\n";
		write(this->socket, this->body.c_str(), this->body.length());
	}
}

void Response::checkIndexFiles()
{
	for (size_t i = 0; i < this->indexFile.size(); i++)
	{
		std::string newPath = this->path + this->indexFile[i];
		this->file.open(newPath.c_str(), std::ios::binary);
		if (file.is_open())
		{
			this->path = newPath;
			std::cout << this->path << std::endl;
			this->status = 200;
			this->isError = true;
			return;
		}
	}
}
long long Response::fileSize(std::string path)
{
    struct stat metadata;
    stat(path.c_str(), &metadata);
    return metadata.st_size;
}
int Response::fillEnv()
{
	this->env = new char *[9];
	env[0] = strdup(("REQUEST_METHOD=" + this->method).c_str());
	env[1] = strdup(("QUERY_STRING=" + this->querry).c_str());
	env[2] = strdup("REDIRECT_STATUS=200");
	env[3] = strdup(("PATH_INFO=" +  this->path).c_str());
	env[4] = strdup(("SCRIPT_FILENAME=" + this->scriptfile).c_str());
	if (this->method == "GET")
		env[5] = strdup(("CONTENT_TYPE=" + getContentType(this->path)).c_str());	
	else
		env[5] = strdup(("CONTENT_TYPE=" + this->reqType).c_str());	
	if (this->method == "GET")
		env[6] = strdup("CONTENT_LENGTH=0");
	else {
		env[6] = strdup(("CONTENT_LENGTH=" + conLength).c_str());
	}
	env[7] = strdup(("HTTP_COOKIE=" + this->cookies).c_str());	
	env[8] = NULL;
	return (1);
}

void Response::ft_free(char **env)
{
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

int Response::executeCgi(Request req)
{
	if (!this->isCGI)
	{
		this->isCGI = true;
		this->start = clock();
		srand(time(NULL));
		this->generatedtPath = "WWW/CGI/" + toString(rand());
		fillEnv();
		int fd[2];
		pipe(fd);
		this->pid = fork();
		if (this->pid == -1) {
			perror("fork");
		}
		if (this->pid == 0)
		{
			const char *av[] = {this->pathCgi.c_str(), this->path.c_str(), NULL};
			(freopen(this->generatedtPath.c_str(), "w", stdout));
			if (this->method == "POST") {
				freopen(this->postpath.c_str(),"r", stdin);
			}
			// for (int i = 0; this->env[i] != NULL; i++) {
			// 	std::cerr << "env[" << i << "]: " << this->env[i] << std::endl;
			// }
			execve(av[0], (char *const *)av, this->env);
			perror("execve");
		}
	}
	pid_t res = waitpid(this->pid, &this->cgistat, WNOHANG);
	if (res  == 0 ) 
	{
		this->end = clock();
   		float processTime = static_cast<float>(this->end - this->start ) / CLOCKS_PER_SEC;
		if ( processTime > 4) {
			this->status = 504;
			if (this->pid > 0) {
				kill(this->pid, SIGKILL);
				waitpid(this->pid, &this->cgistat, 0);
			}
			return 1;
		}
	}
	else if(res) {
		return 1;
	}
	return 0;
}