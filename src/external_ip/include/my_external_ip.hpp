#pragma once

#include "external_ip_interface.hpp"

class my_external_ip : public external_ip_interface {
public:
	virtual std::optional<boost::asio::ip::address> get_address() override;
};