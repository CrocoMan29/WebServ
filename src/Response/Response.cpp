# include "../../includes/Response.hpp"
# include "../../includes/Request.hpp"

// Response::Response() : status(0), socket(0) {}

Response::Response(Request req, int socket) {
	std::cout << "Rspsonse started..... ?" << std::endl;
	this->socket = socket;
	std::cout << "sock/ : " << this->socket << std::endl;
	this->path = req.getRequestInfo()["path"];
	this->method = req.getRequestInfo()["method"];
	// this->method = req._requestInfos["method"];
	if (!req._status)
		// this->status = req._status;
		this->status = 200;
	std::cout << "Path: " << this->path << std::endl;
	std::cout << "Method: " << this->method << std::endl;
	std::cout << "stat: " << this->status << std::endl;
	// exit(10);
}

// Response::Response(const Response& copy) {}

// Response& Response::operator=(const Response& rhs) {
// 	if (this != &rhs) {
// 		this->type = rhs.type;
// 		this->header = rhs.header;
// 	}	
// 	return (*this);
// }

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

bool Response::validPath(std::string& path, std::string& root) {
	return (path.find(root) != std::string::npos);
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
	this->mimetypes[".video/mp4"] = "mp4";
	this->mimetypes[".mpg"] = "video/mpeg";
	this->mimetypes[".mpeg"] = "video/mpeg";
	this->mimetypes[".video/quicktime"] = "mov";
	this->mimetypes[".webm"] = "video/webm";
	this->mimetypes[".flv"] = "video/x-flv";
	this->mimetypes[".m4v"] = "video/x-m4v";
	this->mimetypes[".mng"] = "video/x-mng";
	this->mimetypes[".asx"] = "video/x-ms-asf";
	this->mimetypes[".asf"] = "video/x-ms-asf";
	this->mimetypes[".wmv"] = "video/x-ms-wmv";
	this->mimetypes[".avi"] = "video/x-msvideo";
	
	size_t found = path.rfind('.');
	if (found != std::string::npos) {
		std::string ext = path.substr(found);
		std::map<std::string, std::string>::iterator it = this->mimetypes.find(ext);
		if (it != this->mimetypes.end())
			return (it->second);
	}
	return  ("text/plain");
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

	std::cout << "Status code: " << this->status << std::endl;
	this->header += "HTTP/1.1 " + getStatus(this->status) + "\r\n";
	if (this->status == 301)
		this->header += "Location: " + this->path + "\r\n\r\n";
	else {
		this->header += "Content-Type: " + getContentType(this->path) + "\r\n";
		this->header += "Transfer-Encoding: chunked\r\n";
		// this->header += " Content-Length: 13\n\nHello world!1\r\n";
		
	}
	std::cout << "===========Head=======\n" << this->header << std::endl;
	write(this->socket, this->header.c_str(), this->header.length());
	std::cout << "socket: " << this->socket << std::endl;
}

void	Response::chunk(Request& req) {
	char buf[BUFFERSIZE] = {0};
	std::cout << "content-length----->" << std::endl;
	std::cout << "path: ---->" << this->path << std::endl;
	std::ifstream file(this->path, std::ios::binary); //binary mode
	if (!file.is_open()) {
		this->status = 404;
		setHeader();
		return ;
	}
	// this->status = 200;
	// header();
	file.read(buf, 1023);
	if (file.gcount() > 0) {
		std::stringstream ss;
        ss << std::hex << file.gcount();
		this->chunkSize = ss.str() + "\r\n";
		this->chunkSize.append(buf, file.gcount());
        this->chunkSize.append("\r\n", 2);
		write(this->socket, this->chunkSize.c_str(), this->chunkSize.length());
	}
	else if (file.gcount() == 0) {
        write(this->socket, "0\r\n\r\n", 5);
        file.close();
	}
}

void	Response::checkPath() {
	// if (isDirectory(this->path))
		//need to check auto index;
	if (isRegularFile(this->path)) {
		std::ifstream file(this->path, std::ios::binary);
		if (!file.good()) {
			if (access(this->path.c_str(), F_OK) != -1)
				this->status = 403;
			else
				this->status = 404;
		}
		else {
			getContentType(this->path);
			setHeader();
		}
	}
}


void	Response::setResponse(Request &req, int socket) {
	// setHeader();
	chunk(req);
	exit(1);
}