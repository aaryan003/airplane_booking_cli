#include "StringUtils.h"
#include <cctype>

int StringUtils::stringLength(const char* str) {
    if (!str) return 0;
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

bool StringUtils::stringCompare(const char* str1, const char* str2) {
    if (!str1 || !str2) return false;
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return false;
        }
        i++;
    }
    return str1[i] == str2[i];
}

bool StringUtils::stringCompareNoCase(const char* str1, const char* str2) {
    if (!str1 || !str2) return false;
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (std::tolower(static_cast<unsigned char>(str1[i])) != std::tolower(static_cast<unsigned char>(str2[i]))) {
            return false;
        }
        i++;
    }
    return str1[i] == str2[i];
}

void StringUtils::copyString(char* dest, const char* src, int maxLen) {
    if (!dest || !src || maxLen <= 0) return;
    int i = 0;
    while (i < maxLen - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void StringUtils::toLowerCase(char* str) {
    if (!str) return;
    int i = 0;
    while (str[i] != '\0') {
        str[i] = std::tolower(static_cast<unsigned char>(str[i]));
        i++;
    }
}

void StringUtils::toUpperCase(char* str) {
    if (!str) return;
    int i = 0;
    while (str[i] != '\0') {
        str[i] = std::toupper(static_cast<unsigned char>(str[i]));
        i++;
    }
}

bool StringUtils::contains(const char* str, const char* substr) {
    if (!str || !substr) return false;
    int strLen = stringLength(str);
    int subLen = stringLength(substr);
    if (subLen > strLen) return false;
    if (subLen == 0) return true;
    for (int i = 0; i <= strLen - subLen; i++) {
        bool match = true;
        for (int j = 0; j < subLen; j++) {
            if (str[i + j] != substr[j]) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

void StringUtils::trimWhitespace(char* str) {
    if (!str) return;
    int len = stringLength(str);
    if (len == 0) return;
    while (len > 0 && std::isspace(static_cast<unsigned char>(str[len - 1]))) {
        len--;
    }
    str[len] = '\0';
    int start = 0;
    while (str[start] != '\0' && std::isspace(static_cast<unsigned char>(str[start]))) {
        start++;
    }
    if (start > 0) {
        int i = 0;
        while (str[start + i] != '\0') {
            str[i] = str[start + i];
            i++;
        }
        str[i] = '\0';
    }
}

int stringLength(const char* str) { return StringUtils::stringLength(str); }
bool stringCompare(const char* str1, const char* str2) { return StringUtils::stringCompare(str1, str2); }
bool stringCompareNoCase(const char* str1, const char* str2) { return StringUtils::stringCompareNoCase(str1, str2); }
void copyString(char* dest, const char* src, int maxLen) { StringUtils::copyString(dest, src, maxLen); }
void toLowerCase(char* str) { StringUtils::toLowerCase(str); }
void toUpperCase(char* str) { StringUtils::toUpperCase(str); }
bool contains(const char* str, const char* substr) { return StringUtils::contains(str, substr); }
void trimWhitespace(char* str) { StringUtils::trimWhitespace(str); }