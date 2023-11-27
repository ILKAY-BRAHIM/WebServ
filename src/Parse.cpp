#include "Parse.hpp"

Parse::ServerError::ServerError(const char *message)
	: msg(message)
{}

const char *Parse::ServerError::what() const throw()
{
	return (msg.c_str());
}

Parse::ServerError::~ServerError() throw()
{}

const char *Parse::FileNameIsNull::what() const throw()
{
	return ("config file name is NULL");
}

const char *Parse::FileError::what() const throw()
{
	return ("could'nt open file for reading");
}

const char *Parse::FileIsEmpty::what() const throw()
{
	return ("the config file is empty");
}

Parse::Parse()
{}

Parse::Parse(const Parse &other)
	: vector(other.vector)
{}

Parse &Parse::operator= (const Parse &other)
{
	this->vector = other.vector;
	return (*this);
}

	template <typename T>
std::string print_vect(std::vector<T> str)
{
	int i = 0;
	int size = str.size();
	while (i < size)
	{
		std::cout << i  << " " << str[i] << "\n";
		i++;
	}
	return ("");
}

bool Parse::acollade_est_symetrique(vectstr_t vect)
{
	int count_open = 0;
	int count_close = 0;
	int i = 0;
	int size = vect.size();
	while (i < size)
	{
		if (vect[i] == "{")
			count_open++;
		else if (vect[i] == "}")
			count_close++;
		i++;
	}
	return (count_open == count_close);
}

bool semi_colone(std::string str)
{
	return (str[str.size() - 1] == ';');
}

void Parse::fill_parts(vectstr_t vector, int &i, std::string &to_fill, std::string msg)
{
	if (!to_fill.empty())
		throw Parse::ServerError(std::string("duplicated " + msg.substr(0, msg.size() - 2)).c_str());
	int size = vector.size ();
	if (++i < size && semi_colone(vector[i]))
	{
		to_fill = vector[i].substr(0, vector[i].size() - 1);
		if (min_det)
			std::cout << msg + to_fill + "\n";
	}
	else
		throw Parse::ServerError("needs a \';\'");
}

void Parse::pass_comment(vectstr_t vector, int &i)
{
	if (min_det)
		std::cout << "//   ";
	int size = vector.size();
	while (i < size && !semi_colone(vector[i]))
	{
		if (min_det)
			std::cout << vector[i] << " ";
		i++;
	}
	if (min_det)
		std::cout << vector[i] << "\n";
}

bool Parse::isNumber(const std::string& str) 
{
	if (str.empty() || (str.size() == 1 && (str[0] == '-' || str[0] == '+')))
		return false;

	for (size_t i = 0; i < str.size(); ++i)
	{
		if (i == 0 && (str[i] == '-' || str[i] == '+')) 
			continue;
		if (!std::isdigit(str[i]))
			return false;
	}
	return true;
}

void Parse::handle_location(vectstr_t vector, int &i, t_location &location)
{
	if (vector[++i].find("/"))
		throw Parse::ServerError("you should set a path after location and before \'{\'");
	else
		location.path = vector[i];
}

bool is_a_method(std::string vector)
{
	return (vector == "GET" || vector == "POST" || vector == "PUT" || vector == "DELETE" || vector == "HEAD");
}

void Parse::fill_methods(vectstr_t vector, int &i, t_location &location)
{
	int size;
	std::string error_msg;

	size = vector.size();
	error_msg = " is not a valid or allowed method";
	while (++i < size && vector[i][vector[i].size() - 1] != ';')
	{
		if (!is_a_method(vector[i]))
			throw Parse::ServerError((vector[i] + error_msg).c_str());
		else
			location.allow_methods.push_back(vector[i]);
	}
	if (!is_a_method(vector[i].substr(0, vector[i].size() - 1)))
		throw Parse::ServerError((vector[i] + error_msg).c_str());
	else
		location.allow_methods.push_back(vector[i].substr(0, vector[i].size() - 1));
	if (min_det)
	{
		std::cout << "allowed methods : ";
		size_t j = 0;
		while (j < location.allow_methods.size())
			std::cout << location.allow_methods[j++] << " ";
		std::cout << std::endl;
	}
}

void Parse::fill_cgi(vectstr_t vector, int &i, vectstr_t &in, std::string msg)
{
	int size = vector.size();

	while (++i < size && vector[i][vector[i].size() - 1] != ';')
		in.push_back(vector[i]);
	if (vector[i][vector[i].size() - 1] == ';')
	{
		in.push_back(vector[i].substr(0, vector[i].size() - 1));
	}
	if (min_det)
	{
		int j = 0;
		size = in.size();
		std::cout << msg;
		while (j < size)
		{
			std::cout << in[j] << " ";
			j++;
		}
		std::cout << std::endl;
	}
}

void Parse::fill_locations(vectstr_t vector, int &i, t_location &location)
{
	if (vector[i] == "{")
		i++;
	if (vector[i] == "root")
		fill_parts(vector, i, location.root, "root : ");
	else if (vector[i] == "alias")
		fill_parts(vector, i, location.alias, "alias : ");
	else if (vector[i] == "index")
		fill_parts(vector, i, location.index, "index : ");
	else if (vector[i] == "proxy_pass")
		fill_parts(vector, i, location.proxy_pass, "proxy_pass : ");
	else if (vector[i] == "rewrite")
		fill_parts(vector, i, location.rewrite, "rewrite : ");
	else if (vector[i] == "allow_methods")
		fill_methods(vector, i, location);
	else if (vector[i] == "denny")
		fill_parts(vector, i, location.denny, "denny : ");
	else if (vector[i] == "try_files")
		fill_parts(vector, i, location.try_files, "try_files : ");
	else if (vector[i] == "expires")
		fill_parts(vector, i, location.expires, "expires : ");
	else if (vector[i] == "access_log")
		fill_parts(vector, i, location.access_log, "access_log : ");
	else if (vector[i] == "error_page")
		fill_parts(vector, i, location.error_page, "error_page : ");
	else if (vector[i] == "limite_rate")
		fill_parts(vector, i, location.limite_rate, "limite_rate : ");
	else if (vector[i] == "limite_except")
		fill_parts(vector, i, location.limite_except, "limite_except : ");
	else if (vector[i] == "client_body_buffer_size")
		fill_parts(vector, i, location.client_body_buffer_size, "client_body_buffer_size : ");
	else if (vector[i] == "proxy_set_header")
		fill_parts(vector, i, location.proxy_set_header, "proxy_set_header : ");
	else if (vector[i] == "return")
		fill_parts(vector, i, location.redirect, "return : ");
	else if (vector[i] == "autoindex")
		fill_parts(vector, i, location.autoindex, "autoindex : ");
	else if (vector[i] == "cgi_path")
		fill_cgi(vector, i, location.cgi_path, "cgi_path : ");
	else if (vector[i] == "cgi_ext")
		fill_cgi(vector, i, location.cgi_ext, "cgi_ext : ");
	else
		throw Parse::ServerError(std::string("not allowed or defined keyword -> " + vector[i]).c_str());
}

void Parse::fill_server(vectstr_t vector)
{
	t_server	server;
	t_location	location;
	int			i = 0;
	int			lock = 0;
	int			size = vector.size();

	if (!acollade_est_symetrique (vector))
		throw Parse::ServerError("unmatched \'{\' \'}\'");
	while (i < size)
	{
		server.clear();
		if (vector[i++] != "server")
		{
			std::cout << vector[i] << "\n";
			throw Parse::ServerError("server name is needed");
		}
		if (i < size && vector[i] != "{")
			throw Parse::ServerError("set begin point, should have a \'{\' after server keyword");
		lock = 1;
		while (++i < size && lock != 0)
		{
			if (vector[i] == "{")
				lock++;
			else if (vector[i] == "}")
				lock--;
			if (lock == 1)
			{
				if (vector[i] == "server_name")
					fill_parts(vector, i, server.name, "name : ");
				else if (vector[i] == "listen")
					fill_parts(vector, i, server.port, "port : ");
				else if (vector[i] == "host")
					fill_parts(vector, i, server.host, "host : ");
				else if (vector[i] == "root")
					fill_parts(vector, i, server.root, "root : ");
				else if (vector[i] == "index")
				{
					fill_parts(vector, i, server.index, "index : ");
				}
				else if (vector[i] == "error_page" && i + 2 < size)
				{
					if (Parse::isNumber(vector[++i]) && server.error_page.first.empty())
					{
						server.error_page = std::make_pair(vector[i], vector[i + 1].substr(0, vector[i+1].size() - 1));
						if (min_det)
							std:: cout << "error_page : " << server.error_page.first + " " + server.error_page.second << "\n";
						i++;
					}
					else 
						throw Parse::ServerError("check error_page");
				}
				else if (vector[i] == "location")
				{
					location.clear();
					handle_location (vector, i, location);
					if (min_det)
						std::cout << "_____" << "inside location with " << location.path << std::endl;
				}
			}
			else if (lock == 2)
			{
				if (!location.path.empty())
				{
					fill_locations(vector, i, location);
					if (vector[i+1] == "}")
					server.locations.push_back (location);
				}
			}
		}
		servers.push_back(server);
		if (min_det)
			std::cout << "-------------------\n";
	}
}

bool is_comment(std::string &line) //needs an upgrade
{
	size_t i = 0;
	size_t size = line.size();
	while (i < size && Parse::is_space(line[i]))
		i++;
	return ((line[i] == '#'));
}

Parse::Parse(char *file_name)
{
	std::string file_cont;
	std::string line;

	if (file_name == NULL)
		throw  Parse::FileNameIsNull();
	std::ifstream file_in(file_name);
	if (!file_in.is_open())
		throw Parse::FileError();
	while (std::getline(file_in, line))
	{
		if (is_comment(line))
			continue ;
		file_cont += line + ' ';
	}
	if (min_det)
		std::cout << file_cont + '\n' + "data is good\n-------\n";
	int i = 0;
	int size = file_cont.size();
	if (size == 0)
		throw Parse::FileIsEmpty();
	int j = 0;
	bool in = false;
	int begin;
	while (i < size)
	{
		while (is_space(file_cont[i]))
			i++;
		begin = i;
		while (i < size && !is_space(file_cont[i]))
		{
			in = true;
			i++;
		}
		if (in)
		{
			vector.push_back(file_cont.substr (begin , i - begin));
			in = false;
			j++;
		}
		i++;
	}
	fill_server(vector);
}

bool Parse::is_space(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

std::vector<t_server> const &Parse::get_servers() const
{
	return (servers);
}

std::vector<t_server> Parse::read_parse(int ac, char **av)
{
	(void)ac;
	std::vector<t_server> servers;
	try
	{
		Parse a(av[1]);
		servers = a.get_servers();
	}
	catch (std::exception &e)
	{
		std::cerr << RED << "Error : " << e.what() << WHITE << std::endl;
	}
	return (servers);
}

Parse::~Parse()
{}


