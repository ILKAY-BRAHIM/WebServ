#include "server.h"

/*server methods*/

s_server::s_server()
{
	this->clear();
}

void t_server::clear()
{
	name.clear();
	port.clear();
	autoindex = false;
	autoindex_exact_size = false;
	autoindex_localtime = false;
	autoindex_format = false;
	host.clear();
	root.clear();
	index.clear();
	timeout.clear();
	allow_methods.clear();
	error_page = std::make_pair(allow_methods, ""); 
	client_body_size.clear();
	locations.clear();
}

std::pair<bool, std::string> t_server::full()
{
	if (name.empty() || port.empty() || host.empty() || root.empty())
		return (std::make_pair(0, "name"));
	if (error_page.first.empty() || error_page.second.empty())
		return (std::make_pair(0, "error_page"));
	return (std::make_pair(1, ""));
}

/*location methods*/

s_location::s_location()
{
	this->clear();
}

void t_location::clear()
{
	path.clear();
	autoindex = false;
	autoindex_exact_size = false;
	autoindex_localtime = false;
	autoindex_format = false;
	root.clear();
	alias.clear();
	index.clear();
	proxy_pass.clear();
	rewrite.clear();
	allow_methods.clear();
	denny.clear();
	try_files.clear();
	expires.clear();
	access_log.clear();
	limite_rate.clear();
	limite_except.clear();
	client_body_size.clear();
	proxy_set_header.clear();
	redirect.clear();
	cgi_path.clear();
	cgi_ext.clear();
	cgi_index.clear();
	cgi_script.clear();
	internal = false;
}

/*types methods*/

s_types::s_types()
{
	this->clear();
}

void t_types::clear()
{
	on = false;
	text.clear();
	image.clear();
	video.clear();
	audio.clear();
	application.clear();
}
