#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

#include "my_external_ip.hpp"
#include "aws_route53_dns_updater.hpp"

static std::vector<std::string> retrieve_record_names()
{
	std::vector<std::string> record_names;

	auto record_name_env = std::getenv("REROUTE53_RECORD_NAMES");
	if (record_name_env == nullptr)
		return record_names;

	auto record_name_env_str = std::string(record_name_env);

	size_t pos = 0;
	size_t last = 0;
	while (pos = record_name_env_str.find(":", last) != std::string::npos) {
		record_names.emplace_back(record_name_env_str, last, pos - last);
		last = pos + 1;
	}

	record_names.emplace_back(record_name_env_str, last);

	return record_names;
}

static int retrieve_record_ttl()
{
	int ttl = 60;

	auto record_ttl_env = std::getenv("REROUTE53_RECORD_TTL");
	if (record_ttl_env == nullptr)
		return ttl;

	try {
		ttl = std::stoi(record_ttl_env);
	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
	}

	return ttl;
}

static int retrieve_interval()
{
	int interval = 60;

	auto interval_env = std::getenv("REROUTE53_UPDATE_INTERVAL");
	if (interval_env == nullptr)
		return interval;
	
	try {
		interval = std::stoi(interval_env);
	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
	}

	return interval;
}

int main()
{
	std::shared_ptr<external_ip_interface> ip_provider = std::make_shared<my_external_ip>();
	std::shared_ptr<dns_address_record_update_interface> dns_updater = std::make_shared<aws_route53_dns_updater>();

	auto record_names = retrieve_record_names();
	auto ttl = retrieve_record_ttl();
	auto interval = retrieve_interval();

	std::cout << "Update Interval: " << interval << std::endl;
	std::cout << "Recrod TTL: " << ttl << std::endl;
	std::cout << "DNS Record Names:" << std::endl;
	for (auto &record_name : record_names)
		std::cout << "\t" << record_name << std::endl;

	while (true) {
		auto ip = ip_provider->get_address();
		if (ip.has_value()) {
			std::cout << ip.value() << std::endl;
			if (dns_updater->update_address_record(record_names, ip.value(), ttl))
				std::cout << "DNS records updated" << std::endl;
			else
				std::cout << "Could no update DNS records" << std::endl;
		} else
			std::cout << "Could not get external IP address" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds {interval});
	}

	return 0;
}