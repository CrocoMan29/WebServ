#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# define BUFFERSIZE 1024
#include "Request.hpp"


class Request;
class Response {
	private:
		std::map<int, std::string> code;
		std::map<std::string, std::string> mimetypes;
		std::vector<std::string> indexFile;
		std::string type;
		std::string header;
		std::string path;
		std::string chunkSize;
		std::string method;
		std::string querry;
		std::string cookies;
		std::ifstream file;
		std::string body;
		std::string generatedtPath;
		std::string scriptfile;
		std::string pathCgi;
		char **env;
		int status;
		int socket;
		bool readed;
		bool isError;
		bool isCGI;
		bool valueOfAutoIndex;
		Request req;
		int count;
		double start;
		double end;
		pid_t	pid;
	public:
		bool finish;
		Response();
		Response(Request req, int socket);
		void sendResp(Request req, int socket);
		Response(const Response& copy);
		Response& operator=(const Response& rhs);
		~Response();
		std::string	getStatus(int code);
		std::string getContentType(std::string& path);
		bool isRegularFile(const std::string& path);
		bool isDirectory(const std::string& path);
		void postResponse(Request &request, Location &location);
		void setHeader();
		void chunk(Request& req);
		int checkPath(Request req);
		bool directoryHasFiles(const std::string& directoryPath);
		bool directoryHasIndexFile(const std::string& directoryPath);
		void listDir();
		void checkIndexFiles();
		std::string toString(long long nb);
		int	fillEnv(Request req);
		void	ft_free(char **env);
		void executeCgi(Request req);
		bool	getExt();
		void Delete(Request &req, const std::string &path);
		void update(const Request &request);
};
#endif