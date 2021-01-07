#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "../config/config.cpp"

#include <string>
#include <curl/curl.h>
#include <iostream>
#include <regex>
#include <vector>
#include <map>
#include "json.hpp"

using namespace nlohmann;

struct Entry
{
	std::string time;
	std::vector<std::pair<std::string, double>> readings;
};

std::vector<Entry> get_entries();
std::stringstream fetch_data();
size_t buf_to_stream(void *ptr, size_t size, size_t nmemb, void *stream);
std::vector<Entry> parse_data(std::stringstream& sstream);
int insert_data(std::unique_ptr<sql::Connection>& con, std::vector<Entry> entries);
void print_entries(std::vector<Entry>& readings);

int main()
{
	try
	{
		//Get parsed data from API
		std::vector<Entry> entries = get_entries();
		if(entries.empty())
		{
			std::cout << "No data after parsing\n";
			return -1;
		}
		//Connect to database
		sql::Driver* driver = get_driver_instance();
		std::unique_ptr<sql::Connection> con(driver->connect("tcp://" + sourceDbHostNameV6 + ":" + sourceDbPort, sourceDbUsername, sourceDbPassword));
		con->setSchema("weather");

		int res = insert_data(con, entries);
		std::cout << res << " rows inserted\nClean exit\n";
		return 0;
	}
	catch(sql::SQLException& e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		std::cout << "Exit on error\n\n\n";
		return -1;
	}
}

int insert_data(std::unique_ptr<sql::Connection>& con, std::vector<Entry> entries)
{
	int total = 0;

	//Running separate insert for each hour.
	//Not the best solution in terms of time complexity, but as the key from the api is used to specify which fields to insert into, we dont want to mess things up on the offchance that the values appear in a different order for another hour.
	//However, time complexity does not really matter, as there is only 24 rows a day being inserted.
	for(auto& entry : entries)
	{
		std::string query =
			"INSERT IGNORE INTO `readings` "
			"(`time`, ";
		//Insert rest of field names into query
		for(auto& reading : entry.readings)
		{
			query += "`" + reading.first + "`";
			query += (reading != entry.readings.back())? ", " : ") ";
		}

		query += "VALUES (str_to_date('" + entry.time + "', '%Y-%m-%dT%TZ'), ";
		//Insert rest of values into query
		for(auto& reading : entry.readings)
		{
			query += std::to_string(reading.second);
			query += (&reading != &entry.readings.back())? ", " : ")";
		}
		std::cout << query << "\n\n\n";

		std::unique_ptr<sql::Statement> stmt(con->createStatement());
		//Perform query
		int res = stmt->execute(query);
		total += res;
	}
	return total;
}

std::vector<Entry> get_entries()
{
	std::stringstream sstream = fetch_data();
	std::vector<Entry> entries = parse_data(sstream);

	return entries;
}

//Get data from api
std::stringstream fetch_data()
{
	std::string url = "https://api.met.no/weatherapi/locationforecast/2.0/complete.json?lat=59.668&lon=9.642";

	//Fetch data with libCurl
	CURL *curl;
	std::stringstream out;
	CURLcode res;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, buf_to_stream);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
		{
			std::cout << "Curl error: \n" << curl_easy_strerror(res);
		}

		curl_easy_cleanup(curl);
	}

	return out;
}

//Helper for fetch data
size_t buf_to_stream(void *ptr, size_t size, size_t nmemb, void *stream)
{
	std::string data((const char*) ptr, (size_t) size * nmemb);
	*((std::stringstream*) stream) << data;
	return size * nmemb;
}

std::vector<Entry> parse_data(std::stringstream& sstream)
{
	json j;
	sstream >> j;
	std::vector<Entry> entries;

	auto o = j.at("properties").at("timeseries");
	int i = 0;
	for (json::iterator it = o.begin(); it != o.end() && i < 24; ++it, ++i)
	{
		Entry entry;
		entry.time = it->at("time");
		json readings = it->at("data").at("instant").at("details");

		for (auto& reading : readings.items()) {
			entry.readings.push_back(std::make_pair(reading.key(), reading.value()));
		}

		entries.push_back(entry);
	}

	return entries;
}

void print_entries(std::vector<Entry>& entries)
{
	for(auto& entry : entries)
	{
		std::cout << entry.time << "\n";
		for(auto& reading : entry.readings)
		{
			std::cout << "\t" << reading.first << ": " << reading.second << "\n";
		}
		std::cout << "\n";
	}
	std::cout << "Num of entries: " << entries.size() << "\n";
}

