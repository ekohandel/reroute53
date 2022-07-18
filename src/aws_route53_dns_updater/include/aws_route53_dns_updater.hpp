#pragma once

#include <aws/core/Aws.h>
#include <aws/route53/Route53Client.h>

#include "dns_address_record_update_interface.hpp"

class aws_route53_dns_updater : public dns_address_record_update_interface {
public:
	aws_route53_dns_updater();
	~aws_route53_dns_updater();

	virtual bool update_address_record(const std::vector<std::string> &record_names, const boost::asio::ip::address &address, int ttl) override;
private:

	std::string retrieve_hosted_zone_id();

	Aws::SDKOptions options;
	std::shared_ptr<Aws::Route53::Route53Client> client;
	std::string hosted_zone_id;
};
