#pragma once

#include <optional>

#include <boost/asio/ip/address.hpp>

class external_ip_interface {
public:
	virtual std::optional<boost::asio::ip::address> get_address() = 0;
};