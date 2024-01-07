#ifndef PARSE_HPP
#define PARSE_HPP

#include <iostream>
#include <vector>
#include <map>
#include "colors.h"
#include <fstream>
#include <exception>
#include <cstdlib>
#include <sstream>
#include "server.h"

class Parse
{
	private :

		std::vector<std::string>	vector;
		std::vector<t_server>		servers;

		Parse();
		Parse(char *file_name);
		Parse(const Parse &other);
		Parse& operator= (const Parse &other);

		typedef std::vector<std::string> 							vectstr_t;
		typedef std::map<std::string, std::vector<std::string> >	map_vectstr_t;

void fill_autoindexs(vectstr_t &vector, int &i, bool &autoindex, std::string msg);
		void						fill_error_page(vectstr_t &vector, int &i, t_server &server, int size);
		void						fill_parts(vectstr_t vector, int &i, std::string &to_fill, std::string msg);
		void						fill_cbz(vectstr_t vector, int &i, std::string &to_fill, std::string msg);
		void						fill_server();
		void						last_check();
		void						handle_location(vectstr_t vector, int &i, t_location &location);
		void						fill_indexs(vectstr_t vector, int &i, vectstr_t &to_fill, std::string msg);
		void						fill_locations(vectstr_t vector, int &i, t_location &location);
		void						fill_cgi(vectstr_t vector, int &i, vectstr_t &in, std::string msg);
		void						fill_methods(vectstr_t vector, int &i, t_location &location);
		void 						pass_comment(vectstr_t vector, int &i);
		void						pass_comment(std::ifstream &file_in, std::string &file_cont, std::string &line);
		void						fast_check(std::ifstream &file_in, char **file_name);
		void						fill_data_in_vector(std::string &file_cont);
		bool						acollade_est_symetrique(vectstr_t vect);
		void						fill_types(vectstr_t vector, int &i, t_types &type);
		void						collect_in_type(vectstr_t vector, int &i, map_vectstr_t &text, std::string name);

		std::vector<t_server> const	&get_servers() const;

		class	FileNameIsNull;
		class	FileError;
		class	FileIsEmpty;

	public :

		static	bool is_space(char c);
		static	bool isNumber(const std::string& str);
		static	std::vector<t_server> read_parse(int ac, char **av);
		class	ServerError;
		virtual ~Parse();

};

class Parse::FileNameIsNull : public std::exception
{
	const char *what() const throw();
};

class Parse::FileError : public std::exception
{
	const char *what() const throw();
};

class Parse::FileIsEmpty : public std::exception
{
	const char *what() const throw();
};

class Parse::ServerError : public std::exception
{
	private :
		std::string msg;
		const char *what() const throw();
	public :
		ServerError(const char *message);
		~ServerError() throw();
};

#endif
