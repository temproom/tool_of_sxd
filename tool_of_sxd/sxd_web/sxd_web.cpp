#include "stdafx.h"
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "sxd_web.h"

sxd_web::sxd_web() :
	//连接socket和io_service
	sock(ios) {
}

sxd_web::~sxd_web() {
	sock.close();
}

void sxd_web::connect(const std::string& host, const std::string& port) {
	this->host = host;
	this->port = port;
	//解析服务器信息,创建reslover对象
	boost::asio::ip::tcp::resolver resolver(ios);
	//把host和port转化为socket内部格式
	boost::asio::ip::tcp::resolver::query query(host, port);
	//auto endpoints=resolver.resolve(query)	查询dns服务器,结果可能有多个,ipv6,ipv4
	//把socket和服务器连接
	//boost::asio::connect会挨个尝试，逐一调用socket.connect直到成功
	boost::asio::connect(sock, resolver.resolve(query));
} //sxd_web::connect

void sxd_web::service(const std::string& method, const std::string& request_uri, const std::string& request_http_version, const std::string& request_header, const std::string& content, std::string& response_http_version, unsigned& status_code, std::string& status_message, std::string& response_header, std::string& response_body) {
	// -------------------------------------------------------------------------------------------
	// 0. prepare
	// -------------------------------------------------------------------------------------------
	// method
	if (method != "GET" && method != "POST")
		throw std::runtime_error("未知的 [method]: " + method);
	// stream
	//流类型缓冲区，动态申请内存，请求
	boost::asio::streambuf request;
	//输出流
	std::ostream request_stream(&request);
	//流类型缓冲区，动态申请内存，接受
	boost::asio::streambuf response;
	//输入流
	std::istream response_stream(&response);
	// Transfer-Encoding
	std::string transfer_encoding("");
	// Content-Length
	int content_length(-1);

	// -------------------------------------------------------------------------------------------
	// 1. write
	// -------------------------------------------------------------------------------------------
	request_stream << method << " " << "/" << " " << request_http_version << "\r\n";
	request_stream << "Host: " << host << "\r\n";
	request_stream << "User-Agent: " << "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.43 Safari/537.31" << "\r\n";
	if (method == "GET") {
		request_stream << request_header << "\r\n";
	}
	else if (method == "POST") {
		request_stream << "Content-Type: " << "application/x-www-form-urlencoded" << "\r\n";
		request_stream << "Content-Length: " << content.size() << "\r\n";
		request_stream << request_header << "\r\n";
		request_stream << content;
	}
	//阻塞，发完才返回
	boost::asio::write(sock, request);

	// -------------------------------------------------------------------------------------------
	// 2. read status line
	// -------------------------------------------------------------------------------------------
	//同步读方式，读到"\r\n"后结束
	//HTTP/1.1 200 OK\r\n
	boost::asio::read_until(sock, response, "\r\n");
	response_stream >> response_http_version;
	response_stream >> status_code;
	//从标准输入流中获取行
	//getline(input, str, input.widen(’\n’)), 默认的分隔符是’\n’字符.
	//input	流中获取数据 
	//str	把数据转换成字符串
	std::getline(response_stream, status_message);
	//去除左右空格
	boost::algorithm::trim(status_message);
	// -------------------------------------------------------------------------------------------
	// 3. read response header
	// -------------------------------------------------------------------------------------------
	std::ostringstream osstream;
	boost::asio::read_until(sock, response, "\r\n\r\n");
	//regex正则表达式
	//Transfer-Encoding: chunked\r\n
	boost::regex transfer_encoding_regex("Transfer-Encoding: (.*)");
	boost::regex content_length_regex("Content-Length: (.*)");
	std::string header;
	while (std::getline(response_stream, header) && header != "\r") {
		boost::algorithm::trim(header);
		//match保存结果
		boost::smatch match;
		osstream << header << "\r\n";
		if (regex_search(header, match, transfer_encoding_regex))
			transfer_encoding = boost::lexical_cast<std::string>(match[1]);
		if (regex_search(header, match, content_length_regex))
			content_length = boost::lexical_cast<int>(match[1]);
	}
	response_header = osstream.str();
	// -------------------------------------------------------------------------------------------
	// 4. read response body
	// -------------------------------------------------------------------------------------------
	osstream.str("");
	if (transfer_encoding == "chunked") {
		content_length = 0;
		size_t chunk_size;
		do {
			boost::asio::read_until(sock, response, "\r\n");
			std::string chunk_size_string;
			std::getline(response_stream, chunk_size_string);
			chunk_size = std::stoi(chunk_size_string, nullptr, 16);
			content_length += chunk_size;

			if (chunk_size + 2 > response.size())
				boost::asio::read(sock, response, boost::asio::transfer_exactly(chunk_size + 2 - response.size()));
			std::unique_ptr<char[]> buff(new char[chunk_size + 2]);
			response_stream.read(buff.get(), chunk_size + 2);
			buff[chunk_size] = '\0';
			osstream << buff.get();
		} while (chunk_size > 0);
		response_body = osstream.str();
	}
	else if (content_length >= 0) {
		if ((size_t)content_length > response.size())
			boost::asio::read(sock, response, boost::asio::transfer_exactly(content_length - response.size()));
		std::unique_ptr<char[]> buff(new char[content_length + 1]);
		response_stream.read(buff.get(), content_length);
		buff[content_length] = '\0';
		osstream << buff.get();
		response_body = osstream.str();
	}
	else
		throw std::runtime_error("未知的[Content-Length]");

	// -------------------------------------------------------------------------------------------
	// E. response body verification
	// -------------------------------------------------------------------------------------------
	if (response_body.length() != (size_t)content_length)
		throw std::runtime_error("[Content-Length]与实际[Response-Body]长度不一致");

} //sxd_web::service

std::string sxd_web::get(const std::string& request_uri, const std::string& request_header) {
	std::string response_http_version;
	unsigned int status_code;
	std::string status_message;
	std::string response_header;
	std::string response_body;

	this->service("GET", request_uri, "HTTP/1.1", request_header, "", response_http_version, status_code, status_message, response_header, response_body);
	return response_body;
}

std::string sxd_web::post(const std::string& request_uri, const std::string& request_header, const std::string& content) {
	std::string response_http_version;
	unsigned int status_code;
	std::string status_message;
	std::string response_header;
	std::string response_body;

	this->service("POST", request_uri, "HTTP/1.1", request_header, content, response_http_version, status_code, status_message, response_header, response_body);
	return response_body;
}