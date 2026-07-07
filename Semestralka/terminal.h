#pragma once

#include <iostream>
#include <string>
//#include <limits>
#include <fstream>

#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_hierarchy.h>
#include <libds/adt/table.h>
#include <libds/adt/sorts.h>

#include "node.h"
#include "algorithm.h"
#include "predicates.h"
#include "iterator_manager.h"
#include "comparators.h"
#include "libds/heap_monitor.h."


using namespace std;
using namespace ds::amt;
using namespace ds::adt;



class Terminal
{

public:
    Terminal();
    ~Terminal();

    void spustiTerminalUrovenJedna(ImplicitSequence<Node*>& sekvenciaRt);
	void spustiTerminalUrovenDva(MultiWayEH<Node*>& viacCestnaHierarchia);
	void spustiTerminalUrovenTri(SortedSequenceTable<string, Node*>& tabulkaRt);
private:
    string ziskajCestuKUzlu(MultiWayExplicitHierarchyBlock<Node*>* uzol);
    void usporiadajSekvenciu(ImplicitSequence<Node*>& sekvencia, string komparator);
    bool jePlatnaIp(const string& ip);
};

Terminal::Terminal()
{
}

Terminal::~Terminal()
{
}

void Terminal::spustiTerminalUrovenJedna(ImplicitSequence<Node*>& sekvenciaRt)
{
    bool terminalBezi = true;
    bool moznostBezi = false;

    string volbaPohybu = "";
    string cisloVolbyNaPredikat = "";
    string retazecNaPredikat = "";
    string pokracovanie = "";
	string usporiadat = "";

    Predicates<Node*> predikat;
    Algorithm<Node*> algoritmus;

    ImplicitSequence<Node*> sekvenciaNaSpracovanie;
    sekvenciaNaSpracovanie.assign(sekvenciaRt);

    cout << "Uroven 1" << endl;
    while (terminalBezi)
    {
        cout << endl;
        cout << "Zvolte si predikat, podla ktoreho chcete spracovat udaje." << endl << "--------------------------" << endl;
        cout << "1. matchWithAddress" << endl;
        cout << "2. matchLifetime" << endl;
        cout << "3. matchNextHop" << endl;
        cout << "4. Koniec." << endl;
		cout << "Zadajte volbu: ";
        cin >> volbaPohybu;

        if (volbaPohybu == "4")
        {
            terminalBezi = false;
        }
        else if (volbaPohybu == "1" || volbaPohybu == "2" || volbaPohybu == "3")
        {
            moznostBezi = true;
        }
        else
        {
            cout << "Neplatny Index skuste to znovu." << endl;
        }

        while (moznostBezi)
        {
            if (volbaPohybu == "1")
            {
                cout << "Zadana IP adresa sa zhoduje s adresou cielovej siete v prvych N bitoch zlava podla sietovej masky." << endl;
                cout << "Zadajte IP adresu: ";
                cin >> retazecNaPredikat;
                if (!jePlatnaIp(retazecNaPredikat)) {
                    cout << "Nespravny format IP adresy.\n";
                    moznostBezi = false;
                    break;
                }
                predikat.setHladanyRetazec(retazecNaPredikat);

                ImplicitSequence<Node*>& sekvenciaNaVypis = algoritmus.spracujDataPodlaPredikatu(sekvenciaNaSpracovanie.begin(), sekvenciaNaSpracovanie.end(), [&](Node* router)
                    {
                        return predikat.matchWithAddress(router->getAddress(), router->getMask());
                    }, [&](Node* router) {


						algoritmus.vlozDoSekvencie(router);
                        });
					//cout << "Zadali ste: " << retazecNaPredikat << endl;
                
			
                cout << "Chcete vyfiltrovane zaznamy usporiadat?(nie/lubovolny vstup - ano): ";
                cin >> usporiadat;
                if (usporiadat == "nie") {
                    algoritmus.vypisSekvenciu();
				}
                else {
					string volbaUsporiadania = "";
                    while (volbaUsporiadania != "1" && volbaUsporiadania != "2") {
						cout << "\nPodla coho chcete zaznamy usporiadat?" << endl;
						cout << "1. Podla adresy" << endl;
						cout << "2. Podla zivotnosti" << endl;
						cout << "Zadajte volbu: ";
						cin >> volbaUsporiadania;
                    }
                    if (volbaUsporiadania == "1") {
                        usporiadajSekvenciu(sekvenciaNaVypis, "1");
                        
                    }
                    else if (volbaUsporiadania == "2") {
                        usporiadajSekvenciu(sekvenciaNaVypis, "2");
                        
                    }
                    
                }

                moznostBezi = false;
            }
            else if (volbaPohybu == "2")
            {
                cout << "Zivotnost zaznamu v smerovacej tabulke je v zadanom casovom rozmedzi." << endl;
                cout << "Cas zadavajte vo formate XwXd/XdXh/h:m:s, a jednotlive casy oddelte pomocou '-', napriklad '2d3h-1d6h'" << endl;
                cout << "Zadajte casove rozmedzie: ";
                cin >> retazecNaPredikat;
                size_t dashPos = retazecNaPredikat.find('-');
                if (dashPos == string::npos) {
                    cerr << "Nespravny range format.\n";
					moznostBezi = false;
					break;
                }
                predikat.setHladanyRetazec(retazecNaPredikat);

                ImplicitSequence<Node*>& sekvenciaNaVypis = algoritmus.spracujDataPodlaPredikatu(sekvenciaNaSpracovanie.begin(), sekvenciaNaSpracovanie.end(), [&](Node* router)
                    {
                        return predikat.matchLifetime(router->getLifetime());
                    }, [&](Node* router) {


                        algoritmus.vlozDoSekvencie(router);
                        });

                cout << "Chcete vyfiltrovane zaznamy usporiadat?(nie/lubovolny vstup - ano): ";
                cin >> usporiadat;
                if (usporiadat == "nie") {
                    algoritmus.vypisSekvenciu();
				}
                else {
					string volbaUsporiadania = "";
                    while (volbaUsporiadania != "1" && volbaUsporiadania != "2") {
						cout << "\nPodla coho chcete zaznamy usporiadat?" << endl;
						cout << "1. Podla adresy" << endl;
						cout << "2. Podla zivotnosti" << endl;
						cout << "Zadajte volbu: ";
						cin >> volbaUsporiadania;
                    }
                    if (volbaUsporiadania == "1") {
                        usporiadajSekvenciu(sekvenciaNaVypis, "1");
                        
                    }
                    else if (volbaUsporiadania == "2") {
                        usporiadajSekvenciu(sekvenciaNaVypis, "2");
                        
                    }
                    
                }
                moznostBezi = false;
            }
            else if (volbaPohybu == "3")
            {
                cout << "Adresa suseda zaznamu v smerovacej tabulke je zhodna so zadanou adresou." << endl;
                cout << "Zadajte IP adresu: ";
                cin >> retazecNaPredikat;
                retazecNaPredikat = "via " + retazecNaPredikat;
                predikat.setHladanyRetazec(retazecNaPredikat);

                ImplicitSequence<Node*>& sekvenciaNaVypis = algoritmus.spracujDataPodlaPredikatu(sekvenciaNaSpracovanie.begin(), sekvenciaNaSpracovanie.end(), [&](Node* router)
                    {
                        return predikat.matchNextHop(router->getNextHop());
                    }, [&](Node* router) {


                        algoritmus.vlozDoSekvencie(router);
                        
                        });
                cout << "Chcete vyfiltrovane zaznamy usporiadat?(nie/lubovolny vstup - ano): ";
                cin >> usporiadat;
                if (usporiadat == "nie") {
                    algoritmus.vypisSekvenciu();
				}
                else {
					string volbaUsporiadania = "";
                    while (volbaUsporiadania != "1" && volbaUsporiadania != "2") {
						cout << "\nPodla coho chcete zaznamy usporiadat?" << endl;
						cout << "1. Podla adresy" << endl;
						cout << "2. Podla zivotnosti" << endl;
						cout << "Zadajte volbu: ";
						cin >> volbaUsporiadania;
                    }
                    if (volbaUsporiadania == "1") {
                        usporiadajSekvenciu(sekvenciaNaVypis, "1");
                        
                    }
                    else if (volbaUsporiadania == "2") {
                        usporiadajSekvenciu(sekvenciaNaVypis, "2");
                        
                    }
                    
                }
                moznostBezi = false;
            }
            else
            {
                cout << "Neplatny Index, skuste to znovu." << endl;
            }
        }
    }
}

void Terminal::spustiTerminalUrovenDva(MultiWayEH<Node*>& viacCestnaHierarchia)
{
    
    IteratorManager it(viacCestnaHierarchia);

    Algorithm<Node*> algoritmus;
    Predicates<Node*> predikat;
    bool running = true;
    string retazecNaPredikat = "";
	string usporiadat = "";

    while (running) {
        auto* vrchol = it.getAktualnyVrchol();

        int uroven = viacCestnaHierarchia.level(*vrchol);
        std::cout << "\n=== Aktualny vrchol ===\n";
        if (vrchol && vrchol->data_) {
            std::cout << uroven << ".Oktet: " << vrchol->data_->getOctet() << "\n";
            std::cout << "Parcialna IP: " << this->ziskajCestuKUzlu(vrchol) << "\n";
        }
        else {
            std::cout << "[KOREN - smerovacia tabulka]\n";
        }

        std::cout << "\n1. Prejdi na syna\n"
            << "2. Prejdi na rodica\n"
            << "3. Spusti algoritmus s predikatom\n"
            << "4. Koniec\n";
		std::cout << "Zadaj volbu: ";

        int volba = 0;
        string volbaIn = "";
        std::cin >> volbaIn;
        for (char c : volbaIn) {
            if (!isdigit(c)) {
                volba = -1;
                break;
            }
        }
		if (volba != -1) {
			volba = stoi(volbaIn);
		}

        switch (volba) {
        case 1: {
            size_t sonsCount = 0;
            if (vrchol != nullptr && vrchol->sons_ != nullptr) {
                sonsCount = vrchol->sons_->size();
            }
            if (sonsCount == 0) {
                std::cout << "Tento vrchol nema ziadnych synov.\n";
                break;
            }
            for (int i = 0; i < sonsCount; i++) {
                std::cout << i << ". syn: " << vrchol->sons_->access(i)->data_->data_->getOctet() << "\n";
            }
            std::cout << "Zadaj index syna (0.." << (sonsCount - 1) << "): ";

            int index = 0;
            string indexIn = "";
            std::cin >> indexIn;
            for (char c : indexIn) {
                if (!isdigit(c)) {
                    index = -1;
                    break;
                }
            }
            if (index != -1) {
                index = stoi(indexIn);
            }
            if (index >= 0 && index < vrchol->sons_->size()) {
                it.prejdiNaNizsiNode(index);
            }
            else {
                std::cout << "Neplatny index!\n";
            }
            break;
        }
        case 2:
            if (vrchol->parent_ != nullptr)
                it.prejdiNaVyssiNode();
            else
                std::cout << "Uz si v korenovom uzle.\n";
            break;

        case 3: {
            cout << endl;
            cout << "Zvolte si predikat, podla ktoreho chcete spracovat udaje." << endl << "--------------------------" << endl;
            cout << "1. matchWithAddress" << endl;
            cout << "2. matchLifetime" << endl;
            cout << "3. matchNextHop" << endl;
            cout << "4. Koniec." << endl;

			cout << "Zadajte volbu: ";

            int volba1 = 0;
            string volbaIn1 = "";
            std::cin >> volbaIn1;
            for (char c : volbaIn1) {
                if (!isdigit(c)) {
                    volba1 = -1;
                    break;
                }
            }
            if (volba1 != -1) {
                volba1 = stoi(volbaIn1);
            }

            switch (volba1) {
            case 1: {
                cout << "Zadana IP adresa sa zhoduje s adresou cielovej siete v prvych N bitoch zlava podla sietovej masky." << endl;
                cout << "Zadajte IP adresu: ";
                cin >> retazecNaPredikat;
                if (!jePlatnaIp(retazecNaPredikat)) {
                    cout << "Nespravny format IP adresy.\n";
                    break;
                }
                predikat.setHladanyRetazec(retazecNaPredikat);


                ImplicitSequence<Node*>& sekvenciaNaVypis = algoritmus.spracujDataPodlaPredikatu(it.getPreOrderBegin(), it.getPreOrderEnd(), [&](Node* router)
                    {
                        if (!router) {
                            return false;
                        }
                        if (router->getRoutingEntries() != nullptr) {
							//cout << "Zhoduje sa s adresou" << endl;
                            ImplicitSequence<Node*>* pomocnaSek;
                            pomocnaSek = router->getRoutingEntries();
                            for (int i = 0; i < pomocnaSek->size(); i++) {
                                Node* r = pomocnaSek->access(i)->data_;
                                if (predikat.matchWithAddress(r->getAddress(), r->getMask())) {
                                    return true;
									//cout << "Nasla sa zhoda" << endl;
                                }
                            }
                        }
                       return false;
                        
                    }, [&](Node* router) {

                        ImplicitSequence<Node*>* pomocnaSek;
                        pomocnaSek = router->getRoutingEntries();
                        for (int i = 0; i < pomocnaSek->size(); i++) {
                            Node* r = pomocnaSek->access(i)->data_;
                            if (predikat.matchWithAddress(r->getAddress(), r->getMask())) {
                                algoritmus.vlozDoSekvencie(r);
                                //cout << "vkladam";
                            }
                        }
                        });

                cout << "Chcete vyfiltrovane zaznamy usporiadat?(nie/lubovolny vstup - ano): ";
                cin >> usporiadat;
                if (usporiadat == "nie") {
                    algoritmus.vypisSekvenciu();
				}
                else {
					string volbaUsporiadania = "";
                    while (volbaUsporiadania != "1" && volbaUsporiadania != "2") {
						cout << "\nPodla coho chcete zaznamy usporiadat?" << endl;
						cout << "1. Podla adresy" << endl;
						cout << "2. Podla zivotnosti" << endl;
						cout << "Zadajte volbu: ";
						cin >> volbaUsporiadania;
                    }
                    if (volbaUsporiadania == "1") {
                        usporiadajSekvenciu(sekvenciaNaVypis, "1");
                        
                    }
                    else if (volbaUsporiadania == "2") {
                        usporiadajSekvenciu(sekvenciaNaVypis, "2");
                        
                    }
                    
                }
                break;
            }
            case 2: {
                cout << "Zivotnost zaznamu v smerovacej tabulke je v zadanom casovom rozmedzi." << endl;
                cout << "cas zadavajte vo formate XwXd/XdXh/h:m:s, a jednotlive casy oddelte pomocou '-', napriklad '2d3h-1d6h'" << endl;
                cout << "Zadajte casove rozmedzie: ";
                cin >> retazecNaPredikat;
                size_t dashPos = retazecNaPredikat.find('-');
                if (dashPos == string::npos) {
                    cerr << "Nespravny range format.\n";
                    break;
                }
                predikat.setHladanyRetazec(retazecNaPredikat);


                ImplicitSequence<Node*>& sekvenciaNaVypis = algoritmus.spracujDataPodlaPredikatu(it.getPreOrderBegin(), it.getPreOrderEnd(), [&](Node* router)
                    {
                        if (!router) {
                            return false;
                        }
                        if (router->getRoutingEntries() != nullptr) {
                            //cout << "Zhoduje sa s adresou" << endl;
                            ImplicitSequence<Node*>* pomocnaSek;
                            pomocnaSek = router->getRoutingEntries();
                            for (int i = 0; i < pomocnaSek->size(); i++) {
                                Node* r = pomocnaSek->access(i)->data_;
                                if (predikat.matchLifetime(r->getLifetime())) {
                                    return true;
                                    //cout << "Nasla sa zhoda" << endl;
                                }
                            }
                        }
                        return false;

                    }, [&](Node* router) {

                        ImplicitSequence<Node*>* pomocnaSek;
                        pomocnaSek = router->getRoutingEntries();
                        for (int i = 0; i < pomocnaSek->size(); i++) {
                            Node* r = pomocnaSek->access(i)->data_;
                            if (predikat.matchLifetime(r->getLifetime())) {
                                algoritmus.vlozDoSekvencie(r);
                                //cout << "vkladam";
                            }
                        }
                        });

                cout << "Chcete vyfiltrovane zaznamy usporiadat?(nie/lubovolny vstup - ano): ";
                cin >> usporiadat;
                if (usporiadat == "nie") {
                    algoritmus.vypisSekvenciu();
				}
                else {
					string volbaUsporiadania = "";
                    while (volbaUsporiadania != "1" && volbaUsporiadania != "2") {
						cout << "\nPodla coho chcete zaznamy usporiadat?" << endl;
						cout << "1. Podla adresy" << endl;
						cout << "2. Podla zivotnosti" << endl;
						cout << "Zadajte volbu: ";
						cin >> volbaUsporiadania;
                    }
                    if (volbaUsporiadania == "1") {
                        usporiadajSekvenciu(sekvenciaNaVypis, "1");
                        
                    }
                    else if (volbaUsporiadania == "2") {
                        usporiadajSekvenciu(sekvenciaNaVypis, "2");
                        
                    }
                    
                }
                break;
            }
                
            case 3: {
                cout << "Adresa suseda zaznamu v smerovacej tabulke je zhodna so zadanou adresou." << endl;
                cout << "Zadajte IP adresu: ";
                cin >> retazecNaPredikat;
                retazecNaPredikat = "via " + retazecNaPredikat;
                predikat.setHladanyRetazec(retazecNaPredikat);


                ImplicitSequence<Node*>& sekvenciaNaVypis = algoritmus.spracujDataPodlaPredikatu(it.getPreOrderBegin(), it.getPreOrderEnd(), [&](Node* router)
                    {
                        if (!router) {

                            return false;
                        }
                        if (router->getRoutingEntries() != nullptr) {
                            //cout << "Zhoduje sa s adresou" << endl;
                            ImplicitSequence<Node*>* pomocnaSek;
                            pomocnaSek = router->getRoutingEntries();
                            for (int i = 0; i < pomocnaSek->size(); i++) {
                                Node* r = pomocnaSek->access(i)->data_;
                                if (predikat.matchNextHop(r->getNextHop())) {
                                    return true;
                                    //cout << "Nasla sa zhoda" << endl;
                                }
                            }
                        }
                        return false;

                    }, [&](Node* router) {

                        ImplicitSequence<Node*>* pomocnaSek;
                        pomocnaSek = router->getRoutingEntries();
                        for (int i = 0; i < pomocnaSek->size(); i++) {
                            Node* r = pomocnaSek->access(i)->data_;
                            if (predikat.matchNextHop(r->getNextHop())) {
                                algoritmus.vlozDoSekvencie(r);
                                //cout << "vkladam";
                            }
                        }
                        });

                cout << "Chcete vyfiltrovane zaznamy usporiadat?(nie/lubovolny vstup - ano): ";
                cin >> usporiadat;
                if (usporiadat == "nie") {
                    algoritmus.vypisSekvenciu();
				}
                else {
					string volbaUsporiadania = "";
                    while (volbaUsporiadania != "1" && volbaUsporiadania != "2") {
						cout << "\nPodla coho chcete zaznamy usporiadat?" << endl;
						cout << "1. Podla adresy" << endl;
						cout << "2. Podla zivotnosti" << endl;
						cout << "Zadajte volbu: ";
						cin >> volbaUsporiadania;
                    }
                    if (volbaUsporiadania == "1") {
                        usporiadajSekvenciu(sekvenciaNaVypis, "1");
                        
                    }
                    else if (volbaUsporiadania == "2") {
                        usporiadajSekvenciu(sekvenciaNaVypis, "2");
                        
                    }
                    
                }
                break;
            }
                
            case 4: {
                break;
            }
                
            default:
                std::cout << "Neplatna volba.\n";
                break;
            }
            break;
            
        }
        case 4:
            running = false;
            break;
        default:
            std::cout << "Neplatna volba.\n";
            break;
        }
    }
}
void Terminal::spustiTerminalUrovenTri(SortedSequenceTable<string, Node*>& tabulkaRt)
{
    bool terminalBezi = true;
    bool nasloRt;

    Node** rt = nullptr;

    while (terminalBezi)
	{
		nasloRt = false;
        string volba;
        cout << "Zvolte akciu." << endl;
        cout << "1. Vyhladanie informacii o zaznamoch v tabulke podla zadanej next-hop ip adresy" << endl;
        cout << "2. Ukonci" << endl;
        cout << "Zadajte volbu: ";
        std::cin >> volba;
        if (volba == "1") {
            cout << "Zadajte next-hop ip adresu: ";
            string volba1;
            std::cin >> volba1;
            volba1 = "via " + volba1;
            if (tabulkaRt.tryFind(volba1, rt))
            {
                cout << "1";
                (*rt)->vypisInfo();
                nasloRt = true;
                Node** prvaDuplicitna = nullptr;

                if (tabulkaRt.tryFind((*rt)->getNextHop(), prvaDuplicitna))
                {
                    Node* sucasnaDuplicitna = *prvaDuplicitna;
                    int i = 2;
                    while (sucasnaDuplicitna->getDuplicitnyZaznam() != nullptr) {
                        
                        sucasnaDuplicitna = sucasnaDuplicitna->getDuplicitnyZaznam();
                        cout << i;
                        sucasnaDuplicitna->vypisInfo();
                        i++;
                    }
                }
            }
        }

		else if (volba == "2")
		{
            terminalBezi = false;
            nasloRt = true;
		}
		else
		{
            nasloRt = true;
			cout << "Neplatna volba." << endl;
		}

        if (nasloRt == false)
        {
            cout << "Zaznam so zadanym next-hopom sa v tabulke nenachadza." << endl;
        }
        
    }
}
string Terminal::ziskajCestuKUzlu(MultiWayExplicitHierarchyBlock<Node*>* uzol)
{
    std::string ip = "";

    while (uzol && uzol->data_) {
        const std::string& octet = uzol->data_->getOctet();

        if (!ip.empty()) {
            ip = octet + "." + ip; 
        }
        else {
            ip = octet;
        }

        uzol = static_cast<MultiWayExplicitHierarchyBlock<Node*>*>(uzol->parent_);
    }

    return ip;
}

void Terminal::usporiadajSekvenciu(ImplicitSequence<Node*>& sekvencia, string volbaKomparatora)
{;
    Comparators komparator;
    QuickSort<Node*> sorter;

    if (volbaKomparatora == "1")
    {
        sorter.sort(sekvencia, [&](Node* const& a, Node* const& b) {
			return komparator.comparePrefix(a->getAddress(), a->getMask(), b->getAddress(), b->getMask());
            });
    }
    else {
        sorter.sort(sekvencia, [&](Node* const& a, Node* const& b) {
			return komparator.compareTime(a->getLifetime(), b->getLifetime());
            });
    }

    for (int i = 0; i < sekvencia.size(); i++)
    {
        cout << i + 1;
        sekvencia.access(i)->data_->vypisInfo();
    }
}

bool Terminal::jePlatnaIp(const string& ip) {
    std::istringstream ss(ip);
    std::string token;
    int count = 0;

    while (std::getline(ss, token, '.')) {
        if (++count > 4) return false;

        //je numeric
        if (token.empty() || token.size() > 3) return false;
        for (char c : token) {
            if (!isdigit(c)) return false;
        }

        //skontroluje range
        int num = std::stoi(token);
        if (num < 0 || num > 255) return false;

		//zakaze nulu na zaciatku(001, 01)
        if (token[0] == '0' && token.size() > 1) return false;
    }

    return count == 4;
}
