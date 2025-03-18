/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 19:11:07 by jbadaire          #+#    #+#             */
/*   Updated: 2024/04/23 19:11:07 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <cerrno>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <csignal>

#include "UserBuilder.hpp"
#include "Configuration.hpp"
#include "StringUtils.hpp"
#include "IrcLogger.hpp"
#include "UsersCacheManager.hpp"
#include "ServerExceptions.hpp"
#include "NumericReplies.hpp"

class Server
{
	private:
		static Server *_instance;
		Server(const Server &obj);
		Server &operator=(const Server &obj);

		int _socketfd;
		std::string version;
		sockaddr_in _serverSocket;
		std::string _password;
		std::vector<pollfd> _fds;

		std::vector<std::string> _censoredWords;
		std::map<int, UserBuilder> _danglingUsers;

	public:
		static bool servUp;
		Server() throw(ServerInitializationException);
		~Server();

		void serverUp() throw(ServerStartingException);
		bool handleNewClient();
		void handleIncomingRequest(int incomingFD);
		void getNewClientInfos(int incomingFD);
		void handleKnownClient(int incomingFD, std::string buffer);
		void removeTimeoutDanglingUsers();
		void closeOpenedSockets();
		void sigHandler();
		bool serverIsRunning();
		void removePollFd(int fd);
};


#endif
