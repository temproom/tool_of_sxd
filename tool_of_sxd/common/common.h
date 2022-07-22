#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <string>
#include <D:\boost_1_78_0\boost_1_78_0\boost/filesystem.hpp>

class common {
public:
	common(void);
	~common(void);

	// �ַ�����ת��
	static std::string utf2gbk(const char* s);
	static std::string utf2gbk(const std::string& s);
	static std::string gbk2utf(const char* s);
	static std::string gbk2utf(const std::string& s);

	// uri�������
	static std::string uri_decode(const std::string& s);
	static std::string uri_encode(const std::string& s);

	// �ı��ļ���д
	static std::string read_file(const std::string& file_name);
	static void write_file(const std::string& file_name, const std::string& s);

	// �ֽ�˳��ת
	static int16_t reverse_byte_order(int16_t x);
	static int32_t reverse_byte_order(int32_t x);
	static int64_t reverse_byte_order(int64_t x);

	// ��д��
	static int8_t read_int8(std::istream& is);
	static int16_t read_int16(std::istream& is);
	static int32_t read_int32(std::istream& is);
	static int64_t read_int64(std::istream& is);
	static std::string read_string(std::istream& is);
	static std::string read_string(std::istream& is, int16_t length);
	static void write_int8(std::ostream& os, int8_t i);
	static void write_int16(std::ostream& os, int16_t i);
	static void write_int32(std::ostream& os, int32_t i);
	static void write_int64(std::ostream& os, int64_t i);
	static void write_string(std::ostream& os, const std::string& str);

	// ʱ�����ڸ�ʽ��
	static std::string to_string(const std::time_t t, const char* format);

	// �ַ�����ʽ��
	static std::string sprintf(const char* fmt, ...);

	// ��־
	static void log(const std::string& message, int hwnd = -1, bool file = true, bool time = true);

	// ����
	static bool contain(const std::vector<std::string>& v, const std::string& s);
};

#endif /* COMMON_H_ */#pragma once
