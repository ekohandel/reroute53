#include <memory>
#include <string.h>

#include <aws/route53/model/Change.h>
#include <aws/route53/model/ChangeBatch.h>
#include <aws/route53/model/ChangeResourceRecordSetsRequest.h>

#include "aws_route53_dns_updater.hpp"

aws_route53_dns_updater::aws_route53_dns_updater()
{
	Aws::InitAPI(options);

	client = std::make_shared<Aws::Route53::Route53Client>(Aws::Client::ClientConfiguration());

	hosted_zone_id = retrieve_hosted_zone_id();
}

aws_route53_dns_updater::~aws_route53_dns_updater()
{
	Aws::ShutdownAPI(options);
}

bool aws_route53_dns_updater::update_address_record(const std::vector<std::string> &record_names, const boost::asio::ip::address &address, int ttl)
{
	if (record_names.size() < 1)
		return false;

	Aws::Route53::Model::RRType rr_type = address.is_v4() ? Aws::Route53::Model::RRType::A : Aws::Route53::Model::RRType::AAAA;

	auto resource_record = Aws::Route53::Model::ResourceRecord().WithValue(address.to_string());

	auto change_batch = Aws::Route53::Model::ChangeBatch();

	for (auto &record_name : record_names) {
		auto resource_record_set = Aws::Route53::Model::ResourceRecordSet()
			.WithName(record_name)
			.WithType(rr_type)
			.WithTTL(ttl)
			.WithResourceRecords({resource_record});

		auto change = Aws::Route53::Model::Change()
			.WithAction(Aws::Route53::Model::ChangeAction::UPSERT)
			.WithResourceRecordSet(resource_record_set);
		
		change_batch.AddChanges(change);
	}

	auto request = Aws::Route53::Model::ChangeResourceRecordSetsRequest()
		.WithHostedZoneId(hosted_zone_id)
		.WithChangeBatch(change_batch);

	auto outcome = client->ChangeResourceRecordSets(request);

	if (!outcome.IsSuccess()) {
		std::cout << outcome.GetError().GetMessage() << std::endl;
		return false;
	}

	return true;
}



std::string aws_route53_dns_updater::retrieve_hosted_zone_id()
{
	auto hosted_zone_id = std::getenv("AWS_HOSTED_ZONE_ID");
	if (hosted_zone_id == nullptr)
		return std::string();
	
	return std::string(hosted_zone_id);
}