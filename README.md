## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Commands](#commands)
- [Bonus: IRC Bot](#bonus-irc-bot)
- [Logging System](#logging-system)
- [Project Structure](#project-structure)
- [Authors](#authors)

## Overview

My-IRC-Server is a C++ implementation of an IRC (Internet Relay Chat) server following the RFC standards. It provides a platform for real-time text messaging, with support for multiple clients, channels, and advanced IRC functionalities. Built with a focus on reliability, performance, and extensibility, My-IRC-Server offers a complete IRC server solution with additional features like a monitoring bot and comprehensive logging capabilities.

## Features

- **Standards-Compliant**: Implements the core IRC protocol functionality as per RFC specifications
- **Multi-Client Support**: Handles multiple client connections simultaneously
- **Channel Management**: Full support for creating, joining, and managing channels
- **User Authentication**: Secure user registration and authentication
- **Command System**: Comprehensive set of IRC commands (JOIN, PART, QUIT, PRIVMSG, etc.)
- **Configuration System**: Customizable server settings through a configuration file
- **Logging System**: Detailed logging with multiple severity levels (TRACE, INFO, WARN, ERROR)
- **Word Censoring**: Optional content filtering for inappropriate words
- **Timeout Management**: Automatic handling of user timeouts and disconnections
- **Signal Handling**: Graceful shutdown on system signals (SIGINT, SIGQUIT, SIGTERM)
- **Bot Integration**: Optional bot module that can monitor and interact with the server (bonus feature)

## Requirements

- C++ Compiler with C++98 support
- GNU Make
- Linux/Unix operating system

## Installation

1. Clone the repository:
   ```
   git clone <repository-url>
   cd My-IRC-Server
   ```

2. Compile the server:
   ```
   make
   ```

3. Compile the bot (optional):
   ```
   make bonus
   ```

## Usage

### Starting the Server

```
./ircserv <port> <password>
```

Where:
- `<port>`: The port number for the server (must be between 1024 and 49151)
- `<password>`: The server password for authentication

Example:
```
./ircserv 7777 yourpassword
```

### Starting the Bot (Bonus Feature)

```
./ircserv_bot <options>
```

See the Bot section for more details on bot configuration and options.

## Configuration

The server uses a configuration file (`config.conf`) to customize its behavior. Here's an example configuration:

```
[SERVER]
servername = "My-IRC-Server"
version = "3"
user_timeout = 78000
max_channels_per_user = 100
default_channels = "general, admin:MONSUPERMMOTDEPASSE"
log_level = "TRACE"
print_config = false
censored = "CUSRE WORD, ADMIN"

[ON_JOIN]
message_of_the_day = "Hello {user_nickname}, welcome to the server | I hope the karma is not with you today | {server_name} v.{server_version} team"
channels = "general:12345"
```

### Configuration Sections

#### SERVER Section

- `servername`: The name of the IRC server
- `version`: Server version number
- `user_timeout`: Timeout period for inactive users (in milliseconds)
- `max_channels_per_user`: Maximum number of channels a user can join
- `default_channels`: Default channels to create on server startup (format: "channel1, channel2:password")
- `log_level`: Logging level (TRACE, INFO, WARN, ERROR)
- `print_config`: Whether to print configuration at startup (true/false)
- `censored`: List of words to censor in messages

#### ON_JOIN Section

- `message_of_the_day`: Welcome message shown to users when joining (supports variables: {user_nickname}, {user_username}, {server_name}, {server_version})
- `channels`: Channels to join users to upon connection (format: "channel:password")

## Commands

My-IRC-Server supports standard IRC commands including:

- `/JOIN #channel [password]` - Join a channel
- `/PART #channel` - Leave a channel
- `/QUIT [message]` - Disconnect from the server
- `/PRIVMSG target message` - Send a message to a user or channel
- `/NICK nickname` - Change nickname
- `/LIST` - List available channels
- `/WHO [mask]` - List users
- `/WHOIS nickname` - Display information about a user
- `/INVITE nickname #channel` - Invite a user to a channel
- `/KICK #channel nickname [reason]` - Kick a user from a channel
- `/MODE #channel/nickname [+/-][modes]` - Set channel or user modes
- `/TOPIC #channel [topic]` - View or change channel topic
- `/PING [server]` - Ping the server to check connectivity

## Bonus: IRC Bot

The My-IRC-Server Bot is an autonomous client that connects to the IRC server and provides automated responses and functionality. The bot can:

- Monitor channel activities
- Respond to specific commands
- Provide information and assistance to users
- Execute predefined actions based on triggers

To run the bot:

```
./ircserv_bot
```

## Logging System

My-IRC-Server includes a robust logging system that helps with server monitoring and debugging. The logger writes to a file (`irc_server.log`) and supports multiple severity levels:

- `TRACE`: Detailed information, typically useful for debugging
- `INFO`: General operational information
- `WARN`: Warning messages that don't affect core functionality
- `ERROR`: Error conditions that affect operation but don't cause shutdown

The log level can be configured in the `config.conf` file.

## Project Structure

The project is organized into several modules:

- **Server**: Core server functionality, socket handling, and client management
- **Configuration**: Configuration file parsing and management
- **Commands**: IRC command implementations
- **Channel**: Channel management and properties
- **User**: User management and properties
- **CacheManager**: Caching mechanisms for users and channels
- **Exceptions**: Custom exception classes
- **Utils**: Utility functions and helpers
- **Replies**: Server response formatting according to IRC protocol
- **Bot**: IRC bot implementation (bonus feature)

## Authors

- pgouasmi
- chonorat
- jbadaire