//
// Created by Cheryle Dionna on 04/05/2022.
//

#ifndef PARSER_SERVER_HPP
#define PARSER_SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/time.h> 
#include <sys/types.h> 
#include <netinet/in.h>
#include <string>

#include "Location.hpp"
#include "ConfigUtils.hpp"

class Server {
private:
      Server();
//    std::string                         host;
//    std::string                         root;
    
    struct sockaddr_in                  _addr;
    int                                 _fd;
    int                                 _max_fd;
    

    std::vector<std::string>            serverName;
    t_listen                		    _listens;
    std::map<std::string, Location *>   location;
    
public:
  
     int                                 read_fd;
     std::string                         msg_ret;
     ~Server();

    const int       getFd() {return _fd;}
    const int       getMaxFd() {return _max_fd;}
    void            setFd(int fd) {_fd = fd;}
    void            setAddr();
    const std::vector<std::string> &getServerName() const;

    const t_listen &getListens() const;

    const std::map<std::string, Location *> &getLocation() const;

    int             setup();
    void            connect();
    void            close_fds();
};


#endif //PARSER_SERVER_HPP
