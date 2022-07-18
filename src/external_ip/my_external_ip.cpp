#include "external_ip_interface.hpp"
#include "my_external_ip.hpp"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <iterator>
#include <sstream>

std::optional<boost::asio::ip::address> my_external_ip::get_address()
{
	std::stringstream stream;

	try {
		stream << curlpp::options::Url("http://myexternalip.com/raw");
	}
	catch(std::exception &e) {
		std::cout << e.what();
		return std::optional<boost::asio::ip::address>();
	}

	return boost::asio::ip::make_address(stream.str());
}