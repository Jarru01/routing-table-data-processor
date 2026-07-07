#pragma once

#include <string>
#include <libds/amt/implicit_sequence.h>
//#include <algorithm>
#include "node.h"

template<typename T>
class Predicates
{

public:
    Predicates();
    ~Predicates();

    bool matchWithAddress(string address, string mask);
    bool matchLifetime(string lifetime);
    bool matchNextHop(string nextHop);
    void setHladanyRetazec(string retazec);
    string getHladanyRetazec();

private:
    string hladanyRetazec;
    uint32_t ipToBinary(const string& ip);
    long long timeToSeconds(const string& timeStr);
};

template<typename T>
string Predicates<T>::getHladanyRetazec()
{
    return this->hladanyRetazec;
}

template<typename T>
Predicates<T>::Predicates()
{

}

template<typename T>
Predicates<T>::~Predicates()
{
    this->hladanyRetazec = "";
}

template<typename T>
void Predicates<T>::setHladanyRetazec(string retazec)
{
    this->hladanyRetazec = retazec;
}

template<typename T>
bool Predicates<T>::matchWithAddress(string address, string mask)
{

    uint32_t ip1Binary = this->ipToBinary(address);
    uint32_t ip2Binary = this->ipToBinary(this->hladanyRetazec);
    int cidr = stoi(mask);
    uint32_t maskBinary = cidr ? (~0U << (32 - cidr)) : 0;
    
    /*cout << "Address: " << address << " (" << ip1Binary << ")" << endl;
    cout << "HladanyRetazec: " << this->hladanyRetazec << " (" << ip2Binary << ")" << endl;
    cout << "Mask: " << mask << " (" << maskBinary << ")" << endl;
    string yesno;
	if ((ip1Binary & maskBinary) == (ip2Binary & maskBinary)) {
		yesno = "ano";
	}
	else
	{
		yesno = "nie";
	}
    cout << yesno << endl;*/


    return (ip1Binary & maskBinary) == (ip2Binary & maskBinary);
}

template<typename T>
bool Predicates<T>::matchLifetime(string lifetime)
{
    size_t dashPos = this->hladanyRetazec.find('-');
    string minTimeStr = this->hladanyRetazec.substr(0, dashPos);
    string maxTimeStr = this->hladanyRetazec.substr(dashPos + 1);

    long long minSeconds = timeToSeconds(minTimeStr);
    long long maxSeconds = timeToSeconds(maxTimeStr);

    if (minSeconds > maxSeconds) {
        swap(minSeconds, maxSeconds);
    }

    long long t = timeToSeconds(lifetime);
    return (t >= minSeconds && t <= maxSeconds); 
}


template<typename T>
bool Predicates<T>::matchNextHop(string nextHop)
{
	//cout << nextHop << endl;
	//cout << this->hladanyRetazec << endl;
	if (nextHop == this->hladanyRetazec)
	{
		//cout << "Nasla sa zhoda" << endl;
		return true;
	}
	else
	{
        //cout << "neNasla sa zhoda" << endl;
		return false;
	}
}
template<typename T>
uint32_t Predicates<T>::ipToBinary(const string& ip) {
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
template<typename T>
long long Predicates<T>::timeToSeconds(const string& timeStr) {
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