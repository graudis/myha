#pragma once

class MServerCmdService : public rnSocketIOHandler
{
public:
	MServerCmdService();
	~MServerCmdService();

    void init(rnSocketIOService* service, const tCHAR* daemonIP);
    void clear();

	void operate(rnSocketIOService* service);

	std::string* getDaemonInfo() { return &daemonIP_; };

	tBOOL deliver(rnPacket* packet);

protected:
	rnSocketIOService* service_;
	std::string daemonIP_;
	TServerInfo server_session_info_;
};

inline tBOOL MServerCmdService::deliver(rnPacket* packet)
{
	if (service_ == NULL)
		return cFALSE;

	service_->deliver(packet);
	return cTRUE;
}
