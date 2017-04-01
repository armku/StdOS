#ifndef _HttpClient_H_
#define _HttpClient_H_

#include "Core\SString.h"

class Socket;

// Http¿Í»§¶Ë
class HttpClient
{
public:

	HttpClient();

	String Get(const String& uri);

private:
	Socket*	_Socket;
};

#endif
