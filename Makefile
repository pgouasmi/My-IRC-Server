# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/05 13:40:10 by chonorat          #+#    #+#              #
#    Updated: 2024/05/27 13:47:42 by chonorat         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#COLOR
_GREEN = \033[92m
_YELLOW = \033[33m
_RED = \033[31m

#POLICE
_END = \033[0m
_BOLD = \033[1m

NAME = ircserv
BONUS_NAME = ircserv_bot
CC = @c++
INCLUDES =	-I ./Includes/				\
			-I ./Includes/Server		\
			-I ./Includes/Builders		\
			-I ./Includes/CacheManager	\
			-I ./Includes/Channel		\
			-I ./Includes/Commands		\
			-I ./Includes/Exceptions	\
			-I ./Includes/User			\
			-I ./Includes/Configuration \
			-I ./Includes/Replies

C++FLAGS = -Wall -Wextra -Werror $(INCLUDES) -std=c++98 -MD -g3
RM = @rm -rf
DIR = @mkdir -p
PRINT = @echo
FILES =	main										\
		Server/Server								\
		Configuration/Configuration					\
		Configuration/ConfigurationSection			\
		Exceptions/ChannelCacheException			\
		Exceptions/ChannelGetException				\
		Exceptions/ChannelBuildException			\
		Exceptions/UserBuildException				\
		Exceptions/UserCacheException				\
		Exceptions/UserCacheExceptionString			\
		Exceptions/UserConnectionException			\
		Exceptions/ConfigurationIOException			\
		Exceptions/ServerStartingException			\
		Exceptions/ServerInitializationException	\
		Utils/PrimitivePredicate					\
		Utils/IRCPredicate							\
		Utils/StringUtils							\
		Utils/IrcLogger								\
		Utils/Colors								\
		Utils/FileUtils								\
		Utils/TimeUtils								\
		Models/User/User							\
		Models/User/UserProperties					\
		Models/Channel/ChannelProperties			\
		Models/Channel/Channel						\
		CacheManager/ChannelCacheManager			\
		CacheManager/UsersCacheManager				\
		CacheManager/CommandManager					\
		Builders/UserBuilder						\
		Builders/ChannelBuilder						\
		Helpers/UserListHelper						\
		Replies/NumericReplies						\
		Commands/ICommand							\
		Commands/Message							\
		Commands/Join								\
		Commands/Part								\
		Commands/Quit								\
		Commands/List								\
		Commands/Who								\
		Commands/WhoIs								\
		Commands/Invite								\
		Commands/Ping								\
		Commands/Pong								\
		Commands/Mode								\
		Commands/Nick								\
		Commands/Topic								\
		Commands/Kick

BONUS_FILES =	main							\
				Bot								\
				Exceptions/BotBuildException	\
				Exceptions/BotRunException

OBJS = $(addsuffix .o, $(addprefix Objects/, $(FILES)))
DPDS = $(addsuffix .d, $(addprefix Objects/, $(FILES)))
BONUS_OBJS = $(addsuffix .o, $(addprefix Bonus/Objects/, $(BONUS_FILES)))
BONUS_DPDS = $(addsuffix .d, $(addprefix Bonus/Objects/, $(BONUS_FILES)))

$(NAME): $(OBJS)
	$(PRINT) "\n${_YELLOW}Making $(NAME)...${_END}"
	$(CC) $(OBJS) -o $(NAME)
	$(PRINT) "${_BOLD}${_GREEN}$(NAME) done.\a${_END}"

$(BONUS_NAME): $(BONUS_OBJS)
	$(PRINT) "\n${_YELLOW}Making $(BONUS_NAME)...${_END}"
	$(CC) $(BONUS_OBJS) -o $(BONUS_NAME)
	$(PRINT) "${_BOLD}${_GREEN}$(BONUS_NAME) done.\a${_END}"

Objects/%.o: Sources/%.cpp Makefile
	$(DIR) Objects/Builders Objects/Exceptions Objects/Server Objects/Configuration \
	Objects/Models/Channel Objects/Models/User Objects/CacheManager Objects/Utils \
	Objects/Helpers Objects/Replies Objects/Commands
	$(PRINT) "Compiling ${_BOLD}$<$(_END)..."
	$(CC) -c $(C++FLAGS) $< -o $@

Bonus/Objects/%.o: Bonus/Sources/%.cpp Makefile
	$(DIR) Bonus/Objects Bonus/Objects/Exceptions
	$(PRINT) "Compiling ${_BOLD}$<$(_END)..."
	$(CC) -c $(C++FLAGS) $< -o $@

all: $(NAME) $(BONUS_NAME)

bonus: $(BONUS_NAME)

clean:
ifneq ($(strip $(wildcard $(OBJS))),)
	$(PRINT) "\n${_RED}Cleaning Objects...${_END}"
	$(RM) Objects
	$(PRINT) "${_GREEN}Objects cleaned.${_END}"
endif
ifneq ($(strip $(wildcard $(BONUS_OBJS))),)
	$(PRINT) "${_RED}Cleaning Bonus/Objects...${_END}"
	$(RM) Bonus/Objects
	$(PRINT) "${_GREEN}Bonus/Objects cleaned.${_END}"
endif

fclean: clean
ifeq ($(wildcard $(NAME)), $(NAME))
	$(PRINT) "${_RED}Deleting $(NAME)...${_END}"
	$(RM) $(NAME)
	$(PRINT) "${_GREEN}$(NAME) deleted.\a${_END}"
endif
ifeq ($(wildcard $(BONUS_NAME)), $(BONUS_NAME))
	$(PRINT) "${_RED}Deleting $(BONUS_NAME)...${_END}"
	$(RM) $(BONUS_NAME)
	$(PRINT) "${_GREEN}$(BONUS_NAME) deleted.\a${_END}"
endif

re: fclean all

exec : all
		./ircserv 7777 434

val : all
		valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./ircserv 7777 434

.PHONY: all clean fclean re exec val bonus

-include $(DPDS)
-include $(BONUS_DPDS)