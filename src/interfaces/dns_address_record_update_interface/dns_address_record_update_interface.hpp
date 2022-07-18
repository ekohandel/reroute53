#pragma once

#include <string>

#include <boost/asio/ip/address.hpp>

class dns_address_record_update_interface {
public:
	virtual bool update_address_record(const std::vector<std::string> &record_names, const boost::asio::ip::address &address, int ttl) = 0;
};