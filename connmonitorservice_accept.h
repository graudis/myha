#pragma once

class MConnMonitorServiceAccept : public rnSocketIOHandler
{
	MConnMonitorServiceAccept();
	~MConnMonitorServiceAccept();

public:
	static MConnMonitorServiceAccept* instance();

	void operate(rnSocketIOService* service);
};
