//
// Created by Cheryle Dionna on 04/05/2022.
//

#include "Server.hpp"

//парсер параметров server до "}"


const std::vector<std::string> &Server::getServerName() const {
    return serverName;
}

const t_listen &Server::getListens() const {
    return _listens;
}

const std::map<std::string, Location *> &Server::getLocation() const {
    return location;
}

void    Server::setAddr()
{
    //memset(_addr.sin_zero, '\0', sizeof _addr.sin_zero);
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(8000/*getListens().port*/);
    _addr.sin_addr.s_addr = INADDR_ANY /*htonl(getListens().host)*/;
}

int     Server::setup()
{
    int fd;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Socket error" << std::endl; /*throw(); must write exceptions*/
        return (-1);
    }

    this->setFd(fd);

    if (fcntl(this->getFd(), F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "FCNTL doesn't work" << std::endl; /*throw(); must write exceptions*/
        return (-1);
    } 

    this->setAddr();

    if (bind(this->getFd(), (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
    {
        std::cerr << "Bind error " << getListens().port << "." << std::endl;
        return (-1);
    }
    if (listen(this->getFd(), 1000) == -1)
    {
        std::cerr << "Listen error " << std::endl;
        return (-1);
    }
    return (0);
}

void    Server::connect()
{   
    int select_ret;
    fd_set fds_reads;
    fd_set fds_err;
    struct timeval timeout;

    if(this->getFd() > this->getMaxFd())
        this->_max_fd = this->_fd;

    FD_ZERO(&fds_reads);
	FD_SET(this->getMaxFd(), &fds_reads);
	FD_ZERO(&fds_err);
	FD_SET(this->getMaxFd(), &fds_err);
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

    
    if (select_ret = select(this->getMaxFd() + 1, &fds_reads, NULL, &fds_err, &timeout) < 0)
    {
        std::cerr << "Select error " << std::endl;
        return ;
    }
    if ((select_ret > 0) && (FD_ISSET(this->getMaxFd(), &fds_reads)) && (!FD_ISSET(this->getMaxFd(), &fds_err)))   
    {
        if ((this->read_fd = accept(this->getMaxFd(), NULL, NULL)) < 0)
        {
            std::cerr << "Accept error " << std::endl;
            return ;
        }
        else
        {
            if (recv(this->read_fd, &this->msg_ret, this->msg_ret.size(), 0) >= 1)
                std::cout << "MESSAGE: " << this->msg_ret << std::endl;
            else
                std::cerr << "Recieve error " << std::endl;
                return ;
        }
    }
    else
    {
        std::cerr << "Fatal Select error " << std::endl;
        return ;
    }

}

void    Server::close_fds()
{
    close(this->_fd);
    close(this->_max_fd);
    close(this->read_fd);
}
