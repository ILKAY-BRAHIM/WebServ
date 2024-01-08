#pragma once
# include "response.hpp"
# include "dataType.hpp"

class   Message
{
    private :
        std::vector<std::string>	env;
        request						req;
        t_server					server;
        t_location					location;
        int							status;
        std::string					mess;
        size_t						content_length;
        std::string					body;
        bool						transfer_encoding;
    public :
        Message();
        void		setResponse(std::string mess);
        std::string	getResponse();
        void		setRequest(request req);
        request		getRequest();
        void		setServer(t_server server);
        t_server	getServer();
        size_t		getContentLength();
        void		setContentLength(size_t content_length);
        void		setBody(std::string body);
        std::string	getBody();
        void		setLocation(t_location location);
        t_location	getLocation();
        void		setEnv(std::vector<std::string> env);
        std::vector<std::string>	getEnv();
        void		setStatus(int status);
        int			getStatus();
        void		setTransfer_Encoding(bool s);
        bool		getTransfer_Encoding(void);
        ~Message();
};
