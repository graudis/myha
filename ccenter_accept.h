#pragma once

class rnSocketIOService;

class CCenterAccept : public rnSocketIOHandler
{
public:
	CCenterAccept();
	~CCenterAccept();

	void operate(rnSocketIOService* service);
};
