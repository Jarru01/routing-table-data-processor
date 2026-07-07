#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <libds/amt/implicit_sequence.h>
//#include <algorithm>
#include "libds/heap_monitor.h."


using namespace std;
using namespace ds::amt;

class Node
{
public:
    Node();
    ~Node();

    Node(const Node& other);

    bool operator==(const Node& other) const {
        return this->address == other.address;
    }

    void setLifetime(string time);
    void setAddress(string address);
    void setMask(string mask);
    void setNextHop(string nextHop);
    void setOctet(string octet);
    void setRoutingEntries(IS<Node*>* entries) { this->routingEntries = entries; }
    IS<Node*>* getRoutingEntries() const { return this->routingEntries; }

    void vypisInfo();
    void setDuplicitnyZaznam(Node* duplicitnyZaznam);
    Node* getDuplicitnyZaznam();

    string getLifetime();
    string getAddress();
    string getMask();
    string getNextHop();
    string getOctet();

    

private:

    string lifetime;
    string address;
    string mask;
    string nextHop;
    string octet;
    IS<Node*>* routingEntries = nullptr;
    Node* duplicitnyZaznam = nullptr;
    void zrusDuplicitneZaznamy();
};


Node::Node(const Node& other)
{
    this->lifetime = other.lifetime;
    this->address = other.address;
    this->mask = other.mask;
    this->nextHop = other.nextHop;

    if (other.duplicitnyZaznam != nullptr) {
        this->duplicitnyZaznam = new Node(*other.duplicitnyZaznam);
    }
    else {
        this->duplicitnyZaznam = nullptr;
    }
}


Node::Node()
{
}

Node::~Node()
{
    if (routingEntries) {
        delete routingEntries;
        routingEntries = nullptr;
    }
	this->zrusDuplicitneZaznamy();
    
}


void Node::setLifetime(string time)
{
    this->lifetime = time;
}

void Node::setAddress(string address)
{
    this->address = address;
}

void Node::setMask(string mask)
{
    this->mask = mask;
}


void Node::setNextHop(string nextHop)
{
    this->nextHop = nextHop;
}

inline void Node::setOctet(string octet)
{
	this->octet = octet;
}

void Node::vypisInfo()
{
    std::cout << ".........................." << endl;
    std::cout << "lifetime: " << this->getLifetime() << endl;
    std::cout << "address: " << this->getAddress() << endl;
    std::cout << "mask: " << this->getMask() << endl;
    std::cout << "nextHop: " << this->getNextHop() << endl;
}

inline void Node::setDuplicitnyZaznam(Node* duplicitnyZaznam)
{
    this->duplicitnyZaznam = duplicitnyZaznam;
}

inline Node* Node::getDuplicitnyZaznam()
{
    return this->duplicitnyZaznam;
}

string Node::getLifetime()
{
	if (this->lifetime.empty()) {
		return "00:00:00";
	}
	else {
		return this->lifetime;
	}
}

string Node::getAddress()
{
    return this->address;
}

string Node::getMask()
{
    if (this->mask.empty()) {
        return "32";
    }
    else {
        return this->mask;
    }
    
} 

string Node::getNextHop()
{
    return this->nextHop;
}

inline string Node::getOctet()
{
    return this->octet;
}

void Node::zrusDuplicitneZaznamy()
{
   Node* current = this->duplicitnyZaznam;
   while (current != nullptr) {
       Node* next = current->getDuplicitnyZaznam();
       current->setDuplicitnyZaznam(nullptr);
       delete current;
       current = next;
   }
   this->duplicitnyZaznam = nullptr;
}