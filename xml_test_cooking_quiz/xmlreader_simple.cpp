#include "xmlreader_simple.h"
#include "hresult_to_string.h"
#include <comdef.h>
#include <limits>
#ifdef max
#undef max
#endif

static inline void xml_read(IXMLDOMDocument* lpXMLDoc, const wchar_t *xml_name, bool& xml_readed) {
	VARIANT_BOOL result;
	lpXMLDoc->put_async(VARIANT_FALSE);
	const auto err = lpXMLDoc->load(_variant_t(xml_name), &result);
	if (VARIANT_FALSE == result) {
		xml_readed = false;
		throw xml_runtime_error("xmlのopenに失敗しました。原因:" + hresult_to_string(err));
	}
	xml_readed = true;
}
IXMLDOMDocument* init_xmldoc_c(const wchar_t * xml_name,  bool& xml_readed) {
	CoInitialize(nullptr);
	IXMLDOMDocument* lpXMLDoc;//CoInitializeより後に
	const auto err = CoCreateInstance(CLSID_DOMDocument, nullptr, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&lpXMLDoc);
	if(nullptr == lpXMLDoc) throw xml_runtime_error("xmlのopenに失敗しました。原因:" + hresult_to_string(err));
	xml_read(lpXMLDoc, xml_name, xml_readed);
	return lpXMLDoc;
}
xmldoc_c::xmldoc_c(const wchar_t * xml_name){
	this->lpXMLDoc = init_xmldoc_c(xml_name, this->xml_readed);
}
xmldoc_c::xmldoc_c(const std::wstring & xml_name){
	this->lpXMLDoc = init_xmldoc_c(xml_name.c_str(), this->xml_readed);
}
xmldoc_c::~xmldoc_c(){
	if(this->xml_readed) this->lpXMLDoc->Release();
	CoUninitialize();
}
xmllist_c xmldoc_c::get(const wchar_t * node_path) const
{
	const auto path = olestring(node_path);
	IXMLDOMNodeList* list;
	if(E_INVALIDARG == this->lpXMLDoc->selectNodes(path.c_str(), &list)) throw xml_runtime_error(std::string("nodeが見つかりませんでした"));
	return xmllist_c(list);
}
xmllist_c::xmllist_c(IXMLDOMNodeList * list){
	this->list = list;
	long len;
	this->list->get_length(&len);
	this->len = static_cast<size_t>(len);
}

xmllist_c::xmllist_c(const xmllist_c & other){
	this->list = other.list;
	this->len = other.len;
}
xmllist_c::xmllist_c(xmllist_c && other) {
	this->list = other.list;
	this->len = other.len;
	other.list = nullptr;
}

xmllist_c::~xmllist_c(){
	this->list->Release();
}

xmllist_c::iterator xmllist_c::begin() noexcept{
	return xmllist_c::iterator(this, 0);
}

xmllist_c::iterator xmllist_c::end() noexcept {
	return xmllist_c::iterator();
}

xmlnode_c xmllist_c::at(size_t index) const {
	auto it = xmllist_c::iterator(this, index);
	return *it;
}

xmlnode_c xmllist_c::get(size_t index) const {
	return this->at(index);
}

xmlnode_c xmllist_c::operator[](size_t index) const {
	return this->at(index);
}

olestring::olestring(const wchar_t * str) {
	this->str = SysAllocString(str);
	if (NULL == str || 0 == SysStringLen(this->str)) throw std::bad_alloc();
}
olestring::~olestring() {
	SysFreeString(this->str);
}
size_t olestring::length() const noexcept {
	return SysStringLen(this->str);
}

xmlnode_c::xmlnode_c(IXMLDOMNode * node){
	this->lpItem = node;
}

xmlnode_c::~xmlnode_c(){
	this->lpItem->Release();
}

std::wstring xmlnode_c::get_text() const
{
	BSTR str;
	this->lpItem->get_text(&str);
	auto&& re = std::wstring(str);//BSTR is UTF-16 encoded, and null-terminated. In windows, this is equal with wchar_t*.
	//http://stackoverflow.com/questions/71980/how-do-you-efficiently-copy-bstr-to-wchar-t
	SysFreeString(str);
	return re;
}

xmllist_iterator_c::xmllist_iterator_c(const xmllist_iterator_c & iterator){
	this->m_index = iterator.m_index;
	this->m_xmllist_c = iterator.m_xmllist_c;
}

xmllist_iterator_c::xmllist_iterator_c(){
	this->m_xmllist_c = nullptr;
	this->m_index = std::numeric_limits<size_t>::max();
}

xmllist_iterator_c::xmllist_iterator_c(const xmllist_c * list_c, size_t index){
	this->m_xmllist_c = list_c;
	this->m_index = (index < list_c->len)? index : std::numeric_limits<size_t>::max();
}

xmlnode_c xmllist_iterator_c::operator*() const{
	if (std::numeric_limits<size_t>::max() == this->m_index) throw std::out_of_range(std::string("存在しないnodeの実体は取得できません"));
	IXMLDOMNode* lpItem = nullptr;
	if (nullptr == this->m_xmllist_c->list) throw xml_runtime_error("nodelistが空です");
	const auto err = this->m_xmllist_c->list->get_item(this->m_index, &lpItem);
	if (nullptr == lpItem) throw xml_runtime_error("nodeを取得できませんでした。原因:" + hresult_to_string(err));
	return xmlnode_c(lpItem);
}

bool xmllist_iterator_c::operator==(const xmllist_iterator_c & iterator) const noexcept {
	return !(*this != iterator);
}

bool xmllist_iterator_c::operator!=(const xmllist_iterator_c & it) const noexcept {
	return (this->m_xmllist_c == it.m_xmllist_c && this->m_index == it.m_index) ? true : false;
}

xmllist_iterator_c & xmllist_iterator_c::operator++() noexcept {
	if (this->m_index + 1 < this->m_xmllist_c->len)
		++this->m_index;
	else
		this->m_index = std::numeric_limits<size_t>::max();
	return *this;
}

xmllist_iterator_c xmllist_iterator_c::operator++(int) noexcept {
	const auto re = *this;
	if (this->m_index + 1 < this->m_xmllist_c->len)
		++this->m_index;
	else
		this->m_index = std::numeric_limits<size_t>::max();
	return re;
}

