#ifndef PARSE_HPP
#define PARSE_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <exception>
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

		typedef std::vector<std::string> vectstr_t;

		void		fill_parts(vectstr_t vector, int &i, std::string &to_fill, std::string msg);
		void		fill_server(vectstr_t vector);
		void		handle_location(vectstr_t vector, int &i, t_location &location);
		void		fill_locations(vectstr_t vector, int &i, t_location &location);
		void		fill_cgi(vectstr_t vector, int &i, vectstr_t &in, std::string msg);
		void		fill_methods(vectstr_t vector, int &i, t_location &location);
		void 		pass_comment(vectstr_t vector, int &i);
		bool		acollade_est_symetrique(vectstr_t vect);

		std::vector<t_server> const &get_servers() const;

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
