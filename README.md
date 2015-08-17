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

