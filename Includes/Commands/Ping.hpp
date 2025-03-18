/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:54:00 by jbadaire          #+#    #+#             */
/*   Updated: 2024/04/22 13:54:00 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_PING_HPP
# define IRC_PING_HPP

# include "ICommand.hpp"

class Ping : public ICommand
{
	public :
		Ping();
		void execute(User *user, Channel *channel, std::vector<std::string>args);
};

#endif //IRC_PING_HPP
