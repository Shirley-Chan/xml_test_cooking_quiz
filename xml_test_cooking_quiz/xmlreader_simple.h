#pragma once
#include <cstddef>
#include <string>
#include <msxml.h>
#include <exception>
#include <iterator>

class xml_runtime_error : std::runtime_error{
public:
	xml_runtime_error(const std::string& what_arg) : std::runtime_error("xml_runtime_error : " + what_arg) {}
	xml_runtime_error(const char* what_arg) : std::runtime_error(std::string("xml_runtime_error : ") + what_arg) {}
};
class olestring
{
public:
	olestring(const wchar_t* str);
	~olestring();
	BSTR c_str() const noexcept{
		return this->str;
	}
	size_t length() const noexcept;
	size_t size() const noexcept {
		return this->length();
	}
private:
	BSTR str;
};

class xmllist_c;
class xmlnode_c
{
public:
	xmlnode_c(IXMLDOMNode* node);
	~xmlnode_c();
	std::wstring get_text() const;
private:
	IXMLDOMNode* lpItem;
};
class xmllist_iterator_c : public std::iterator<std::input_iterator_tag, xmlnode_c>
{
public:
	xmllist_iterator_c(const xmllist_iterator_c& iterator);
	xmlnode_c operator*() const;
	bool operator==(const xmllist_iterator_c& iterator) const noexcept;
	bool operator!=(const xmllist_iterator_c& it) const noexcept;
	xmllist_iterator_c& xmllist_iterator_c::operator++() noexcept;
	xmllist_iterator_c xmllist_iterator_c::operator++(int) noexcept;
private:
	friend xmllist_c;
	xmllist_iterator_c();
	xmllist_iterator_c(const xmllist_c* list_c, size_t index);
	const xmllist_c* m_xmllist_c;
	size_t m_index;
};
class xmllist_c
{
public:
	xmllist_c(IXMLDOMNodeList* list);
	xmllist_c(const xmllist_c& other);
	xmllist_c(xmllist_c && other);
	~xmllist_c();
	size_t size() const noexcept {
		return this->len;
	}
	size_t length() const noexcept {
		return this->len;
	}
	typedef xmllist_iterator_c iterator;
	xmllist_c::iterator begin() noexcept;
	xmllist_c::iterator end() noexcept;
	xmlnode_c at(size_t index) const;
	xmlnode_c get(size_t index) const;
	xmlnode_c operator[](size_t index) const;
private:
	friend xmllist_iterator_c;
	IXMLDOMNodeList* list;
	size_t len;
};
class xmldoc_c
{
public:
	xmldoc_c(const wchar_t* xml_name);
	xmldoc_c(const std::wstring& xml_name);
	~xmldoc_c();
	xmllist_c get(const wchar_t* node_path) const;
	xmllist_c get(const std::wstring& node_path) const{ return this->get(node_path.c_str()); }
	xmllist_c operator[](const wchar_t* node_path) const{ return this->get(node_path); }
	xmllist_c operator[](const std::wstring& node_path) const{ return this->get(node_path); }
private:
	IXMLDOMDocument* lpXMLDoc;
	bool xml_readed;
};
