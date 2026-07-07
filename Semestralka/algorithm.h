#pragma once

#include <string>
#include <libds/amt/implicit_sequence.h>
//#include <algorithm>
#include "node.h"
#include "libds/heap_monitor.h."

using namespace std;
using namespace ds::amt;

template<typename T>
class Algorithm
{
public:
    Algorithm();
    ~Algorithm();

    template<typename Iterator>
    ImplicitSequence<Node*>& spracujDataPodlaPredikatu(Iterator begin, Iterator end, std::function<bool(Node*)> predikat, std::function<void(Node*)> akcia);

    void vypisSekvenciu();
	void vlozDoSekvencie(Node* node) { this->novaImplicitnaSekvencia->insertLast().data_ = node; }

private:
    ImplicitSequence<Node*>* novaImplicitnaSekvencia = new ImplicitSequence<Node*>();
};

template<typename T>
Algorithm<T>::Algorithm()
{
}

template<typename T>
Algorithm<T>::~Algorithm()
{
    delete this->novaImplicitnaSekvencia;
    this->novaImplicitnaSekvencia = nullptr;
}

template<typename T>
void Algorithm<T>::vypisSekvenciu()
{
    int i = 0;
    cout << endl;
    for (auto it = this->novaImplicitnaSekvencia->begin(); it != this->novaImplicitnaSekvencia->end(); ++it)
    {
        Node* zaznam = *it;
        cout << i+1;
        zaznam->vypisInfo();
        ++i;
    }
}

template<typename T>
template<typename Iterator>
ImplicitSequence<Node*>& Algorithm<T>::spracujDataPodlaPredikatu(Iterator begin, Iterator end, std::function<bool(Node*)> predikat, std::function<void(Node*)> akcia)
{
    this->novaImplicitnaSekvencia->clear();
    auto it = begin;
    while (it != end)
    {
        Node* zaznam = *it;
        if (predikat(zaznam))
        {
			akcia(zaznam);
            //this->novaImplicitnaSekvencia->insertLast().data_ = zaznam;
        }
        ++it;
    }
    return *this->novaImplicitnaSekvencia;
}

