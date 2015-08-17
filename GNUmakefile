# Makefile for xml_test_cooking_quiz.exe

TARGET 	= xml_test_cooking_quiz.exe
SRCS 	= ./xml_test_cooking_quiz/Environment-dependent.cpp ./xml_test_cooking_quiz/hlesult_to_std_string.cpp ./xml_test_cooking_quiz/hresult_to_std_wstring.cpp ./xml_test_cooking_quiz/prelude.cpp ./xml_test_cooking_quiz/Source.cpp ./xml_test_cooking_quiz/xml_checker.cpp ./xml_test_cooking_quiz/xmlreader_simple.cpp

# 基本コマンド
RM 		:= rm
CXX 	:= g++
CC 		:= gcc

# デバッグ時とリリース時の微調整
CXX_DEBUG_FLAGS		=	-g -O0 -Wall
CXX_RELEASE_FLAGS	=	-O2

# 基本オプション
ifeq ($(CXX),gcc)
CPPFLAGS = --input-charset=utf-8 -fexec-charset=CP932 -std=c++14
else
CPPFLAGS = -std=c++14
endif

# make
# header echo
.PHONY: header
header: CXXFLAGS+=-H
header: all

# debug
.PHONY: Debug
Debug: CXXFLAGS+=$(CXX_DEBUG_FLAGS)
Debug: all
# release
.PHONY: Release
Release: CXXFLAGS+=$(CXX_RELEASE_FLAGS)
Release: all

all: $(TARGET)
$(TARGET): $(SRCS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(CPPFLAGS)

# make clean
.PHONY: clean
clean:
	$(RM) -f *.exe