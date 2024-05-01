# include "../../includes/Response.hpp"
# include "../../includes/Request.hpp"

Response::Response() : status(0), socket(0) {}

Response::Response(const Response& copy) {}

Response& Response::operator=(const Response& rhs) {return (*this);}

Response::~Response() {}

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
	std::map<std::string, std::string> mimetypes;
	mimetypes[".html"] = "text/html";
	mimetypes[".htm"] = "text/html";
	mimetypes[".shtml"] = "text/html";
	mimetypes[".css"] = "text/css";
	mimetypes[".xml"] = "text/xml";
	mimetypes[".gif"] = "image/gif";
	mimetypes[".jpeg"] = "image/jpeg";
	mimetypes[".jpg"] = "image/jpeg";
	mimetypes[".js"] = "application/javascript";
	mimetypes[".atom"] = "application/atom+xml";
	mimetypes[".rss"] = "application/rss+xml";
	mimetypes[".mml"] = "text/mathml";
	mimetypes[".txt"] = "text/plain";
	mimetypes[".jad"] = "text/vnd.sun.j2me.app-descriptor";
	mimetypes[".wml"] = "text/vnd.wap.wml";
	mimetypes[".htc"] = "text/x-component";
	mimetypes[".avif"] = "image/avif";
	mimetypes[".png"] = "image/png";
	mimetypes[".svg"] = "image/svg+xml";
	mimetypes[".svgz"] = "image/svg+xml";
	mimetypes[".tiff"] = "image/tiff";
	mimetypes[".tif"] = "image/tiff";
	mimetypes[".wbmp"] = "image/vnd.wap.wbmp";
	mimetypes[".webp"] = "image/webp";
	mimetypes[".ico"] = "image/x-icon";
	mimetypes[".jng"] = "image/x-jng";
	mimetypes[".bmp"] = "image/x-ms-bmp";
	mimetypes[".woff"] = "font/woff";
	mimetypes[".woff2"] = "font/woff2";
	mimetypes[".jar"] = "application/java-archive";
	mimetypes[".war"] = "application/java-archive";
	mimetypes[".ear"] = "application/java-archive";
	mimetypes[".json"] = "application/json";
	mimetypes[".hqx"] = "application/mac-binhex40";
	mimetypes[".doc"] = "application/msword";
	mimetypes[".pdf"] = "application/pdf";
	mimetypes[".ps"] = "application/postscript";
	mimetypes[".eps"] = "application/postscript";
	mimetypes[".ai"] = "application/postscript";
	mimetypes[".rtf"] = "application/rtf";
	mimetypes[".m3u8"] = "application/vnd.apple.mpegurl";
	mimetypes[".kml"] = "application/vnd.google-earth.kml+xml";
	mimetypes[".kmz"] = "application/vnd.google-earth.kmz";
	mimetypes[".xls"] = "application/vnd.ms-excel";
	mimetypes[".eot"] = "application/vnd.ms-fontobject";
	mimetypes[".ppt"] = "application/vnd.ms-powerpoint";
	mimetypes[".odg"] = "application/vnd.oasis.opendocument.graphics";
	mimetypes[".odp"] = "application/vnd.oasis.opendocument.presentation";
	mimetypes[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	mimetypes[".odt"] = "application/vnd.oasis.opendocument.text";
	mimetypes[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	mimetypes[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	mimetypes[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	mimetypes[".wmlc"] = "application/vnd.wap.wmlc";
	mimetypes[".wasm"] = "application/wasm";
	mimetypes[".7z"] = "application/x-7z-compressed";
	mimetypes[".cco"] = "application/x-cocoa";
	mimetypes[".jardiff"] = "application/x-java-archive-diff";
	mimetypes[".jnlp"] = "application/x-java-jnlp-file";
	mimetypes[".run"] = "application/x-makeself";
	mimetypes[".pl"] = "application/x-perl";
	mimetypes[".pm"] = "application/x-perl";
	mimetypes[".prc"] = "application/x-pilot";
	mimetypes[".pdb"] = "application/x-pilot";
	mimetypes[".rar"] = "application/x-rar-compressed";
	mimetypes[".rpm"] = "application/x-redhat-package-manager";
	mimetypes[".sea"] = "application/x-sea";
	mimetypes[".swf"] = "application/x-shockwave-flash";
	mimetypes[".sit"] = "application/x-stuffit";
	mimetypes[".tcl"] = "application/x-tcl";
	mimetypes[".tk"] = "application/x-tcl";
	mimetypes[".der"] = "application/x-x509-ca-cert";
	mimetypes[".pem"] = "application/x-x509-ca-cert";
	mimetypes[".crt"] = "application/x-x509-ca-cert";
	mimetypes[".xpi"] = "application/x-xpinstall";
	mimetypes[".xhtml"] = "application/xhtml+xml";
	mimetypes[".xspf"] = "application/xspf+xml";
	mimetypes[".zip"] = "application/zip";
	mimetypes[".bin"] = "application/octet-stream";
	mimetypes[".exe"] = "application/octet-stream";
	mimetypes[".dll"] = "application/octet-stream";
	mimetypes[".deb"] = "application/octet-stream";
	mimetypes[".dmg"] = "application/octet-stream";
	mimetypes[".iso"] = "application/octet-stream";
	mimetypes[".img"] = "application/octet-stream";
	mimetypes[".msi"] = "application/octet-stream";
	mimetypes[".msp"] = "application/octet-stream";
	mimetypes[".msm"] = "application/octet-stream";
	mimetypes[".mid"] = "audio/midi";
	mimetypes[".midi"] = "audio/midi";
	mimetypes[".kar"] = "audio/midi";
	mimetypes[".mp3"] = "audio/mpeg";
	mimetypes[".ogg"] = "audio/ogg";
	mimetypes[".m4a"] = "audio/x-m4a";
	mimetypes[".ra"] = "audio/x-realaudio";
	mimetypes[".3gpp"] = "video/3gpp";
	mimetypes[".3gp"] = "video/3gpp";
	mimetypes[".ts"] = "video/mp2t";
	mimetypes[".video/mp4"] = "mp4";
	mimetypes[".mpg"] = "video/mpeg";
	mimetypes[".mpeg"] = "video/mpeg";
	mimetypes[".video/quicktime"] = "mov";
	mimetypes[".webm"] = "video/webm";
	mimetypes[".flv"] = "video/x-flv";
	mimetypes[".m4v"] = "video/x-m4v";
	mimetypes[".mng"] = "video/x-mng";
	mimetypes[".asx"] = "video/x-ms-asf";
	mimetypes[".asf"] = "video/x-ms-asf";
	mimetypes[".wmv"] = "video/x-ms-wmv";
	mimetypes[".avi"] = "video/x-msvideo";
	
	size_t found = path.rfind('.');
	if (found != std::string::npos) {
		std::string ext = path.substr(found);
		std::map<std::string, std::string>::iterator it = mimetypes.find(ext);
		if (it != mimetypes.end())
			return (it->second);
	}
	return ("text/plain");
}


std::string	Response::getStatus(int status) {

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
	this->code[504	] = "504 Gateway Timeout";

	if (this->code.find(status) != this->code.end())
		return (this->code(status));
	else
		return ("Unknown status code");
}

void    Response::header() {

	std::cout << "Status code: " << this->status << std::endl;
	map<int, std::string>::iterator it = getStatus(this->status);
	this->header = "HTTP/1.1" + it->second + "\r\n";
}