#include "server.h"

void t_location::clear()
{
	path.clear();
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
	error_page.clear();
	limite_rate.clear();
	limite_except.clear();
	client_body_buffer_size.clear();
	proxy_set_header.clear();
	redirect.clear();
	autoindex.clear();
	cgi_path.clear();
	cgi_ext.clear();
}

void t_server::clear()
{
	name.clear();
	port.clear();
	host.clear();
	root.clear();
	index.clear();
	allow_methods.clear();
	error_page = std::make_pair("", ""); 
	locations.clear();
}

void t_types::clear()
{
	on = false;
	text.clear();
	image.clear();
	application.clear();
}
