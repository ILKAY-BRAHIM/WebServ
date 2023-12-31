#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <vector>
#include <map>
#include "colors.h"

# define min_det	1

typedef struct s_types
{
	typedef std::map<std::string, std::vector<std::string> >	mapper_t;
	bool		on;
	mapper_t	text;
	mapper_t	application;
	mapper_t	image;
	mapper_t	video;
	mapper_t	audio;
	void		clear();
}				t_types;

typedef struct s_location
{
	std::string					path;					//path after location key
	std::string 				root;					//root dir location
	std::string 				alias;					//define remplacement for specefied url
	std::vector<std::string>	index;					//set default file
	std::string 				proxy_pass;				//forwards requests to a specified backend server 
	std::string 				rewrite;				//modifies the requested uri before processing it further
	std::vector<std::string>	allow_methods;			//restricts the http methods allowed for this location
	std::string					denny;					//specifies the ip addresses or ranges to deny access
	std::string 				try_files;				//attempts to serve specific files in a specified order
	std::string 				expires;				//sets expiration times for cached content
	std::string 				access_log;				//specifies the log file and logging format for this location's requests
	std::string 				limite_rate;			//sets the maximum transfer rate for responses
	std::string 				limite_except;			//defines the set of http methods that are allowed in a given location
	std::string 				client_body_size;		//sets the buffer size for the client request body
	std::string 				proxy_set_header;		//sets header fields to be passed to the proxied server
	std::string 				redirect;				//performs url redirection (return or rewrite)
	std::vector<std::string>	cgi_path;
	std::vector<std::string>	cgi_ext;
	std::vector<std::string>	cgi_index;
	std::vector<std::string>	cgi_script;
	bool						internal;
	bool						autoindex;
	bool						autoindex_exact_size;
	bool						autoindex_format;
	bool						autoindex_localtime;
	//method
	void		clear();
}	t_location;

typedef struct s_server
{
	std::string 										name;
	std::vector<int>									port;
	std::string 										host;
	std::string 										root;
	std::vector<std::string>							allow_methods;
	std::string											redirect;
	std::vector<std::string> 							index;
	std::pair<std::vector<std::string>, std::string>	error_page;
	std::vector<t_location>								locations;
	std::vector<t_types>								types;
	std::string											timeout;
	bool												autoindex;
	bool												autoindex_exact_size;
	bool												autoindex_format;
	bool												autoindex_localtime;
	std::string 										client_body_size;

	//method
	void								clear();
	std::pair<bool, std::string>		full();
}	t_server;

void    printServerData(t_server serv);

#endif
