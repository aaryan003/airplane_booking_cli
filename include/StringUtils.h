#pragma once
#ifndef STRINGUTILS_H
#define STRINGUTILS_H

// String utility functions to avoid using STL
class StringUtils {
public:
    static int stringLength(const char* str);
    static bool stringCompare(const char* str1, const char* str2);
    static bool stringCompareNoCase(const char* str1, const char* str2);
    static void copyString(char* dest, const char* src, int maxLen);
    static void toLowerCase(char* str);
    static void toUpperCase(char* str);
    static bool contains(const char* str, const char* substr);
    static void trimWhitespace(char* str);
};

// Global helper functions for convenience
int stringLength(const char* str);
bool stringCompare(const char* str1, const char* str2);
bool stringCompareNoCase(const char* str1, const char* str2);
void copyString(char* dest, const char* src, int maxLen);
void toLowerCase(char* str);
void toUpperCase(char* str);
bool contains(const char* str, const char* substr);
void trimWhitespace(char* str);

#endif // STRINGUTILS_H