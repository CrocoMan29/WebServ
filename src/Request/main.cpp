# include "Request.hpp"

int main(){
    Request request;
    std::ifstream infile("tst.txt");
    
    request.readingRequest(infile);
    for(auto a : request.getRequestInfo()){
        std::cout << a.first << ":=> " << a.second << std::endl;
    }
}