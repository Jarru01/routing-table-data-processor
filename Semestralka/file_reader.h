#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "node.h"

#include <libds/amt/implicit_sequence.h>
#include <libds/adt/table.h>
#include "libds/heap_monitor.h."


using namespace std;
using namespace ds::amt;
using namespace ds::adt;

class FileReader
{

public:
    FileReader();
    ~FileReader();

    void nacitajZaznamy();
    void vytvorSekvenciu(ImplicitSequence<Node*>& sekvenciaRt);
	void vytvorHierarchiu(MultiWayExplicitHierarchy<Node*>& viacCestnaHierarchia);
    void vytvorTabulku(SortedSequenceTable<string, Node*>& tabulkaRt);

private:
    void nacitajSubor(string cestaSuboru, ImplicitSequence<Node*>& implicitnaSekvencia);

    ImplicitSequence<Node*>* rt = new ImplicitSequence<Node*>();
    ImplicitSequence<Node*>* oktet1 = new ImplicitSequence<Node*>();
    ImplicitSequence<Node*>* oktet2 = new ImplicitSequence<Node*>();
    ImplicitSequence<Node*>* oktet3 = new ImplicitSequence<Node*>();
    ImplicitSequence<Node*>* oktet4 = new ImplicitSequence<Node*>();
};


FileReader::FileReader() 
{
}

FileReader::~FileReader()
{
    for (int i = this->rt->size() - 1; i >= 0; i--)
    {
        delete this->rt->access(i)->data_;
    }
    for (int i = this->oktet1->size() - 1; i >= 0; i--)
    {
        delete this->oktet1->access(i)->data_;
        //cout << "uvolnujem1" << endl;
    }
    for (int i = this->oktet2->size() - 1; i >= 0; i--)
    {
        delete this->oktet2->access(i)->data_;
        //cout << "uvolnujem2" << endl;
    }
    for (int i = this->oktet3->size() - 1; i >= 0; i--)
    {
        delete this->oktet3->access(i)->data_;
        //cout << "uvolnujem3" << endl;
    }
    for (int i = this->oktet4->size() - 1; i >= 0; i--)
    {
        delete this->oktet4->access(i)->data_;
        //cout << "uvolnujem4" << endl;
    }


    delete this->rt;
    this->rt = nullptr;
    delete this->oktet1;
	this->oktet1 = nullptr;
	delete this->oktet2;
	this->oktet2 = nullptr;
	delete this->oktet3;
	this->oktet3 = nullptr;
	delete this->oktet4;
	this->oktet4 = nullptr;


}

void FileReader::nacitajZaznamy()
{
    this->nacitajSubor("C:\\Users\\Jarru\\Desktop\\RT.csv", *this->rt);
}

void FileReader::nacitajSubor(string cestaSuboru, ImplicitSequence<Node*>& implicitnaSekvencia)
{

    fstream input_file(cestaSuboru);
    if (!input_file.is_open()) {
        std::cout << "Subor sa nepodarilo otvorit!" << endl;
    }

    string line;
    getline(input_file, line); //Preskocenie prvej neplatnej hodnoty


    while (getline(input_file, line))
    {
        stringstream ss(line);
        string field;
        Node* router = new Node();

        getline(ss, field, ';');
        //router->setSource(field);

        getline(ss, field, ';');
        router->setLifetime(field);

        getline(ss, field, ';');
        router->setAddress(field);

        getline(ss, field, ';');
        router->setMask(field);

        getline(ss, field, ';');
        //router->setMetric(field);

        getline(ss, field, ';');
        router->setNextHop(field);

        implicitnaSekvencia.insertLast().data_ = router;
    }
}

void FileReader::vytvorSekvenciu(ImplicitSequence<Node*>& sekvenciaRt)
{
    for (int i = 0; i < this->rt->size(); i++)
    {
		Node* router = this->rt->access(i)->data_;
        sekvenciaRt.insertLast().data_ = router;
    }
	
}

void FileReader::vytvorHierarchiu(MultiWayExplicitHierarchy<Node*>& viacCestnaHierarchia)
{
    for (int i = 0; i < this->rt->size(); i++) {
		Node* router = this->rt->access(i)->data_;

        std::stringstream ss(this->rt->access(i)->data_->getAddress());
        std::string octetStr;
        auto* current = viacCestnaHierarchia.accessRoot();
		//cout << viacCestnaHierarchia.degree(current) << endl;
        int octetCislo = 0;
        

        while (std::getline(ss, octetStr, '.')) {
            ++octetCislo;
            bool found = false;
            Node* foundNode = nullptr;
            //skontroluje ci dieta s tymto oktetom uz existuje
            for (size_t i = 0; i < current->sons_->size(); i++) {
				//cout << "Checking child " << i << " :" << current->sons_->size() << endl;
				auto* child = viacCestnaHierarchia.accessSon(*current, i);
                //cout << current.sons_->size() << endl;
                if (child && child->data_ && child->data_->getOctet() == octetStr) {
                    current = child;
					foundNode = child->data_;
                    found = true;
					//cout << "Found existing node for " << octetStr << endl;
                    break;
                }

            }

            // ak sa nenajde, vytvori noveho syna
            if (!found) {
                size_t newIndex = current->sons_->size();

                auto& newNode = viacCestnaHierarchia.emplaceSon(*current, newIndex);
				Node* naVkladanie = new Node();
                //std::cout << "Allocated node for " << octetStr << std::endl;
				naVkladanie->setOctet(octetStr);
                
                if (octetCislo == 1) {
                    this->oktet1->insertLast().data_ = naVkladanie;
                    //cout << "vkladam" << endl;
                }
				else if (octetCislo == 2) {
                    this->oktet2->insertLast().data_ = naVkladanie;

				}
				else if (octetCislo == 3) {
                    this->oktet3->insertLast().data_ = naVkladanie;

				}
                else if (octetCislo == 4) {
                    this->oktet4->insertLast().data_ = naVkladanie;

                }
				newNode.data_ = naVkladanie;
                current = &newNode;
                foundNode = naVkladanie;
            }
			if (octetCislo == 4) {
                if (foundNode->getRoutingEntries() == nullptr) {
					IS<Node*>* entries = new IS<Node*>();
					foundNode->setRoutingEntries(entries);
					//cout << "vytvaram novy routing entries" << i << endl;
                }
                else {
					//cout << "uz existuje" << endl;
                }
				foundNode->getRoutingEntries()->insertLast().data_ = router;
			}
        }
    }
}

void FileReader::vytvorTabulku(SortedSequenceTable<string, Node*>& tabulkaRt)
{
    for (int i = 0; i < this->rt->size(); i++)    
    {
        Node** prvaDuplicitna = nullptr;

        if (tabulkaRt.tryFind(this->rt->access(i)->data_->getNextHop(), prvaDuplicitna))
        {
            Node* sucasnaDuplicitna = *prvaDuplicitna;
            while (sucasnaDuplicitna->getDuplicitnyZaznam() != nullptr) {
                sucasnaDuplicitna = sucasnaDuplicitna->getDuplicitnyZaznam();
            }
            sucasnaDuplicitna->setDuplicitnyZaznam(new Node(*this->rt->access(i)->data_));
        }
        else {
            tabulkaRt.insert(this->rt->access(i)->data_->getNextHop(), this->rt->access(i)->data_);
        }
    }
}