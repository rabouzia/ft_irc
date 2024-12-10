
#ifndef NUMERIC_RPL_HPP
# define  NUMERIC_RPL_HPP

# define user_id(nickname, username, cmd) (":" + nickname + "!~" + username + "@localhost " + cmd + " ") // used

            /* = = =    TO CONNECT    = = = */
// 001
# define RPL_WELCOME(nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "\r\n") // used

            /* = = =    MODE    = = = */
# define SET_CHANEL_MODE(nickname, username, cmd, chanel, mode) (user_id(nickname, username, cmd) + "#" + chanel + " +" + mode + "\r\n") //used

# define UNSET_CHANEL_MODE(nickname, username, cmd, chanel, mode) (user_id(nickname, username, cmd) + "#" +chanel + " -" + mode + "\r\n") // used

# define SET_NEWOPER(nickname, username, cmd, chanel, mode, concerned_client_nickname) ((user_id(nickname, username, cmd) + "#" + chanel + " +" + mode  + " " + concerned_client_nickname + "\r\n")) //used

# define UNSET_OPER(nickname, username, cmd, chanel, mode, concerned_client_nickname) ((user_id(nickname, username, cmd) + "#" + chanel + " -" + mode  + " " + concerned_client_nickname + "\r\n")) // used
// 324
# define RPL_CHANNELMODEIS(nickname, chanel, chanel_mods) (":localhost 324 " + nickname + " #" + chanel + " +" + chanel_mods + "\r\n") // used
// 481
# define ERR_NOPRIVILEGES(nickname, channel) (":localhost 482 " + nickname + " #" + channel + " :You cannot destitute the channel funder\r\n") // used

# define ERR_INVALIDLIMIT(nickname, channel) (":localhost 482 " + nickname + " #" + channel + " : The limit you tried to set in unvalid\r\n") // used

# define ERR_INVALIDKEY(nickname, channel) (":localhost 482 " + nickname + " #" + channel + " : The Key you tried to set in unvalid\r\n") // used
// 482
# define ERR_CHANOPRIVSNEED(nickname, channel) (":localhost 482 " + nickname + " #" + channel + " :You're not channel operator\r\n") // used
// 501
# define ERR_UMODEUNKNOWNFLAG(nickname) (":localhost 501 " + nickname + " :Unknown MODE flag\r\n") //used
            /* = = =    PASS    = = = */
// 464
# define ERR_PASSWDMISMATCH(nickname) (":localhost 464 " + nickname + " :Password incorrect.\r\n")//used

            /* = = =    WHO     = = = */
// 311
# define RPL_WHOISUSER(nickname, host_name, username) (":localhost 311 " + nickname + " " + host_name + " ~" + username + " localhost * :" + username + "\r\n") // used
// 318
# define RPL_ENDOFWHOIS(nickname) (":localhost 318: " + nickname + " :End of /WHOIS\r\n")//used
            /* = = =    NICK    = = = */
// 401
# define ERR_NOSUCHNICK(nickname) (":localhost 401: " + nickname + " There is no such nick :" + nickname + "\r\n") //USED
// 433
# define ERR_NICKNAMEINUSE(nickname) (":localhost 433 * " + nickname + " :Nickname is already in use\r\n")//used

            /* = = =    PING / PONG     = = = */

# define RPL_PONG ":localhost PONG localhost :localhost\r\n" // used

            /* = = =    JOIN     = = = */
// # define RPL_JOIN(nickname, chanel) (':' + nickname + " JOIN #" + chanel + "\r\n") // used
#define RPL_JOIN(nickname, username, channel) (CLIENT_ID(nickname, username, "JOIN") + ":#" + channel + "\r\n")

# define PART_CHANEL(nickname, username, cmd, chanel) (user_id(nickname, username, cmd) + chanel + "\r\n") // used

# define RPL_PART(nickname, channel, reason) (":" + nickname + " PART #" + channel + " :" + reason + "\r\n")
// 353
# define RPL_NAMREPLY(nickname, chanel, list_client) (":localhost 353 " + nickname + " = #" + chanel + " :" + list_client +"\r\n") // used
// 366
# define RPL_ENDOFNAMES(nickname, chanel) (":localhost 366 " + nickname + " " + chanel + " :End of /NAMES list.\r\n")//used
// 471
# define ERR_CHANNELISFULL(nickname, chanel) (":localhost 471 " + nickname + " " + chanel + " :Cannot join channel (+l)\r\n") // used
// 475
# define ERR_BADCHANNELKEY(nickname, channel) (":localhost 475 " + nickname + " " + channel + " :\r\n") // used
// 403
# define ERR_NOSUCHCHANNEL(nickname, chanel) (":localhost 403 " + nickname + " " + chanel + " :No such channel\r\n") // used
// 404
# define ERR_CANNOTSENDTOCHAN(nickname, chanel) (":localhost 404 " + nickname + " " + chanel + " :Cannot send to channel\r\n")//used
// 441
# define ERR_USERNOTINCHANNEL(nickname, concerned_client_nickname, chanel) ((":localhost 441 " + nickname + " " + concerned_client_nickname + " " + chanel + " :They aren't on that channel\r\n")) // used
// 442
# define ERR_NOTONCHANNEL(nickname, chanel) (":localhost 442 " + nickname + " #" + chanel + " :You're not on that channel\r\n") // used
// 443
# define ERR_USERONCHANNEL(nickname, channel) (":localhost 443 " + channel + " " + nickname + " :is already on channel\r\n") // used

# define ERR_ERRONEUSNICKNAME(nickname) (":localhost 432 * " + nickname + " :Erroneous nickname\r\n")

            /* = = =    PRV MESSAGES     = = = */

# define RPL_PRIVMSG_CHANNEL(nickname, chanel, msg) (":" + nickname + " PRIVMSG " + chanel + " " + msg + "\r\n") // used

# define CLIENT_ID(nickname, username, command) (":" + nickname + "!~" + username + "@" + "localhost" + " " + command + " ")
# define RPL_PRIVMSG_CLIENT(nickname, username, dest, msg) (CLIENT_ID(nickname, username, "PRIVMSG") + dest + " :" + msg + "\r\n") // used

# define ERR_NOTEXTTOSEND(nickname) (nickname + " :No text to send\r\n") // used

            /* = = =    INVITE     = = = */
// 341
# define RPL_INVITING(nickname, target, channel) (":localhost 341 " + nickname + " " + target + " :" + channel + "\r\n") // used

# define INVITE_CLIENT(nickname, username, cmd, concerned_client_nickname, chanel) (user_id(nickname, username, cmd) + concerned_client_nickname + " :" + chanel + "\r\n") // used

# define NOCTICE_CLIENT_INVITE(nickname, chanel) (nickname + " invites you to " + chanel + "\r\n") // used
// 473
# define ERR_INVITEONLYCHAN(nickname, chanel) (":localhost 473 " +  nickname + " " + chanel + " :Cannot join channel (+i)\r\n") // used

            /* = = =    KICK     = = = */
#define RPL_KICK(nickname, channel, cible, reason) (':' + nickname + " KICK #" + channel + " " + cible + " :" + reason + "\r\n")
#define KICK_CLIENT(nickname, username, cmd, chanel, concerned_client_nickname) ((user_id(nickname, username, cmd)) + chanel + " " + concerned_client_nickname + " :\r\n") // used
            /* = = =    TOPIC     = = = */
// 331

#define SERVER_NAME "localhost" 
// #define RPL_NOTOPIC(nickname, chanel) (":localhost 331 " + nickname + " #" + chanel + " :No topic is set\r\n") // used

// 332
#define SET_TOPIC(nickname, username, cmd, chanel, topic) ((user_id(nickname, username, cmd)) + chanel + " " + topic + "\r\n")
// 331
// #define RPL_NOTOPIC(nickname, chanel) (":localhost 331 " + nickname + " " + chanel + " :No topic is set\r\n")
// 332
// #define RPL_TOPIC(nickname, chanel, topic) (":localhost 332 " + nickname + " " + chanel + " " + topic + "\r\n")
// 333
// #define ROL_TOPICWHOTIME(nickname, chanel, concerned_client_nickname, time ) (":localhost 333 " + nickname +  + " " + chanel + " " concerned_client_nickname + " " + time + "\r\n")
// 471

#define RPL_NOTOPIC(nickname, channel) (std::string(":") + SERVER_NAME + " 331 " + nickname + " #" + channel + " :No topic is set" + "\r\n")

#define RPL_TOPIC(nickname, channel, topic) (std::string(":") + SERVER_NAME + " 332 " + nickname + " #" + channel + " :" + topic + "\r\n")

#define PART(nickname, username, channel, message) (CLIENT_ID(nickname, username, "PART") + "#" + channel + " :" + message + "\r\n")
#define ERR_UNKNOWNCOMMAND(nickname, command) (std::string(":") + SERVER_NAME + " 421 " + nickname + " " + command + " :Unknown command"+ "\r\n")

#define ERR_NEEDMOREPARAMS(nickname, command) (std::string(":") + SERVER_NAME + " 461 " + nickname + " " + command + " :Not enough parameters\r\n")

#define ROL_TOPICWHOTIME(nickname, channel, concerned_client_nickname, time) (std::string(":") + SERVER_NAME + " 333 " + nickname + " " + channel + " " + concerned_client_nickname + " " + time + "\r\n")
// #define CLIENT_ID(nickname, username, command) (":" + nickname + "!~" + username + "@" + chanel + " " + command + " ")
 #define  NICK_RPL(nickname, username, new_nickname) (CLIENT_ID(nickname, username, "NICK") + ":" + new_nickname + "\r\n")

#endif