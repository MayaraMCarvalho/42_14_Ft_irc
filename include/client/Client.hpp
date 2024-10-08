#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "MsgHandler.hpp"
#include <string>
#include <set>

class Channel;
class ChannelList;

class Client {
	public:
		typedef enum {
			NO_MODE = 0,
			AWAY = 1,
			WALLOPS = 2,
			INVISIBLE = 4,
			RESTRICTED = 8,
			OPERATOR = 16,
			LOCAL_OP = 32,
			RECV_NOTICES = 64
		} t_mode;

		typedef enum {
			UNKNOWN = 0,
			CONNECTED,
			AUTHENTICATED,
			GOT_USER,
			GOT_NICK,
			REGISTERED,
			DISCONNECTED
		} t_status;

		// Constructors
		Client(int fd, MsgHandler &MsgHandler);
		Client(const Client &src);

		~Client(void);

		Client &operator=(const Client &src);

		// Getters
		const std::string &getNick(void);
		const std::string &getUser(void);
		const std::string &getHost(void);
		const std::string &getRealName(void);
		const std::string getFullId(void);
		int getFD(void);
		std::set<std::string> &getChannelList(void);
		t_status getStatus(void);
		bool getMode( t_mode mode) ;
		int getModeFlags();

		// Setters
		void setNick(const std::string &nick);
		void setHost(const std::string &host);
		void setUser(const std::string &user);
		void setRealName(const std::string &realName);
		void setModeFlags(int modeFlags);
		void setMode(const std::string &modeStr);
		void setStatus(t_status status);

		// Channel functions
		bool isInChannel(const std::string &channelStr) ;
		void addChannel(const std::string &channelStr);
		void removeChannel(const std::string &channelStr);
		void sendMessage(const std::string &from, const std::string &msg);
		void sendMessage(const std::string &msg);

	private:
		MsgHandler &_msgHandler;
		std::string _nick;
		std::string _user;
		std::string _host;
		std::string _realName;
		int _fd;
		std::set<std::string> _channels;
		int _modeFlags;
		t_status _status;
};

#endif
