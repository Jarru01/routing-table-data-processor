#pragma once

#include <string>
#include <libds/amt/implicit_sequence.h>
//#include <algorithm>
#include "node.h"

class Comparators
{

public:
    Comparators();
    ~Comparators();

    bool comparePrefix(string ip1, string mask1, string ip2, string mask2);
    bool compareTime(string time1, string time2);
private:
    uint32_t ipToBinary(const string& ip);
    long long timeToSeconds(const string& timeStr);
};

Comparators::Comparators()
{
}

Comparators::~Comparators()
{
}

bool Comparators::comparePrefix(string ip1, string mask1, string ip2, string mask2)
{
	uint32_t ip1Binary = ipToBinary(ip1);
	uint32_t ip2Binary = ipToBinary(ip2);
    int cidr1 = stoi(mask1);
    uint32_t mask1Binary = cidr1 ? (~0U << (32 - cidr1)) : 0;
    int cidr2 = stoi(mask2);
    uint32_t mask2Binary = cidr2 ? (~0U << (32 - cidr2)) : 0;
	uint32_t prefix1 = ip1Binary & mask1Binary;
	uint32_t prefix2 = ip2Binary & mask2Binary;
	if (prefix1 == prefix2)
	{
		return cidr1 < cidr2;
	}
	else
	{
		return prefix1 < prefix2;
	}
}
bool Comparators::compareTime(string time1, string time2)
{
	long long time1Seconds = timeToSeconds(time1);
	long long time2Seconds = timeToSeconds(time2);
    return time1Seconds < time2Seconds;
}

uint32_t Comparators::ipToBinary(const string& ip) {
    ds::amt::ImplicitSequence<int> octets;
    stringstream ss(ip);
    string token;

    while (getline(ss, token, '.')) {
        octets.insertLast().data_ = stoi(token);

    }

    if (octets.size() < 4) return 0;
    //cout << octets.access(0)->data_ << endl;

    return ((octets.access(0)->data_) << 24) | ((octets.access(1)->data_) << 16) | ((octets.access(2)->data_) << 8) | (octets.access(3)->data_);
}

long long Comparators::timeToSeconds(const string& timeStr) {
    long long totalSeconds = 0;
    long long num = 0;
    int colonCount = 0;

    for (size_t i = 0; i < timeStr.size(); ++i) {
        char c = timeStr[i];

        if (isdigit(c)) {
            num = num * 10 + (c - '0');
        }
        else if (c == ':') {
            // pre HH:MM:SS format
            totalSeconds = totalSeconds * 60 + num;
            num = 0;
            colonCount++;
        }
        else {
            // pre format w, d, h, m, s
            switch (c) {
            case 'w': totalSeconds += num * 7 * 24 * 3600; break;
            case 'd': totalSeconds += num * 24 * 3600; break;
            case 'h': totalSeconds += num * 3600; break;
            case 'm': totalSeconds += num * 60; break;
            case 's': totalSeconds += num; break;
            default: break;
            }
            num = 0;
        }
    }

    // pridaj posledne cislo pre HH:MM:SS ak je
    if (colonCount > 0)
        totalSeconds = totalSeconds * 60 + num;
    else
        totalSeconds += num;

    return totalSeconds;
}