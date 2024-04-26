#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <fstream>
# include <iostream>
# include <map>
# include <vector>
#include <cstring>
# include <algorithm>
# include <sstream>
#include <sys/stat.h>   

class Response {
    public:
        Response();
        Response(const Response& copy);
        Response& operator=(const Response& rhs);
        ~Response();
};


#endif