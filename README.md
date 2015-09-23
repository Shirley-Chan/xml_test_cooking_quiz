##　このプログラムは何?

msxmlのラッパーもどきをつくろうとしていた。で昔作ったプログラム

https://gist.github.com/yumetodo/3e1db684f61a73029416

を実験用に書き換えている。

## コンパイル条件

確認しているのは

- Visual Studio 2015 Community
- gcc 4.9.2 (msys2 mingw)
- clang 3.5.2 (msys2 mingw)

static_assert使っているところがあるからそこ消せばVS2013 November CTPでもいけるのか・・・？

gcc/clangのコンパイルには

```
-std=c++14 -lOle32 -lOleAut32 -lmsxml3 -luuid
```

が必要。なお、msxmlを使うので多分windows以外ではコンパイルできない

# Issue
xmlreader_simple.cpp の134行目付近

```cpp
xmlnode_c xmllist_iterator_c::operator*() const{
	if (std::numeric_limits<size_t>::max() == this->m_index) throw std::out_of_range(std::string("存在しないnodeの実体は取得できません"));
	IXMLDOMNode* lpItem = nullptr;
	if (nullptr == this->m_xmllist_c->list) throw xml_runtime_error("nodelistが空です");
	const auto err = this->m_xmllist_c->list->get_item(this->m_index, &lpItem);
	if (nullptr == lpItem) throw xml_runtime_error("nodeを取得できませんでした。原因:" + hresult_to_string(err));
	return xmlnode_c(lpItem);
}
```

get_itemで失敗する。詳細は  
[https://github.com/yumetodo/xml_test_cooking_quiz/issues/2](https://github.com/yumetodo/xml_test_cooking_quiz/issues/2)  
へ


