#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <vector>
#include <map>

# define min_det	0
# ifndef COLOR
# define COLOR
#  define RED	"\033[0;31m"
#  define WHITE	"\033[0;37m"
# endif

typedef struct s_types
{
	bool												on;
	std::map<std::string, std::vector<std::string> >	text;
	std::map<std::string, std::vector<std::string> >	application;
	std::map<std::string, std::vector<std::string> >	image;
	void	clear();
}				t_types;

typedef struct s_location
{
	std::string					path;					//path after location key
	std::string 				root;					//root dir location
	std::string 				alias;					//define remplacement 
	//for specefied url
	std::vector<std::string>	index;					//set default file
	std::string 				proxy_pass;				//forwards requests to a 
	//specified backend server 
	std::string 				rewrite;				//modifies the requested 
	//uri before processing it further
	std::vector<std::string>	allow_methods;			//restricts the http methods 
	//allowed for this location
	std::string					denny;					//specifies the ip addresses
	//	or ranges to deny access
	std::string 				try_files;				//attempts to serve specific 
	//files in a specified order
	std::string 				expires;				//sets expiration times for 
	//cached content
	std::string 				access_log;				//specifies the log file and 
	//logging format for this location's requests
	std::string 				error_page;				//defines custom error pages 
	//for specific http status codes
	std::string 				limite_rate;			//sets the maximum transfer 
	//rate for responses
	std::string 				limite_except;			//defines the set of http 
	//methods that are allowed in a given location
	std::string 				client_body_buffer_size;//sets the buffer size for 
	//the client request body
	std::string 				proxy_set_header;		//sets header fields to be 
	//passed to the proxied server
	std::string 				redirect;				//performs url redirection 
	//(return or rewrite)
	std::string					autoindex;
	std::vector<std::string>	cgi_path;
	std::vector<std::string>	cgi_ext;
	//method
	void		clear();
}	t_location;

typedef struct s_server
{
	std::string 						name;
	std::vector<int>					port;
	std::string 						host;
	std::string 						root;
	std::vector<std::string>			allow_methods;
	std::string							redirect;
	std::vector<std::string> 			index;
	std::pair<std::string, std::string> error_page;
	std::vector<t_location>				locations;
	std::vector<t_types>				types;
	//method
	void		clear();
}	t_server;

#endif
