#include "node.h"
#include "terminal.h"
#include "algorithm.h"
#include "file_reader.h"

using namespace std;
using namespace ds::amt;


int main()
{
	initHeapMonitor();

	FileReader citac;
	citac.nacitajZaznamy();

	Terminal terminal;

	ImplicitSequence<Node*>* rt = new ImplicitSequence<Node*>();
	citac.vytvorSekvenciu(*rt);

	MultiWayExplicitHierarchy<Node*>* viacCestnaHierarchia = new MultiWayExplicitHierarchy<Node*>();
	viacCestnaHierarchia->emplaceRoot().data_ = nullptr;
	citac.vytvorHierarchiu(*viacCestnaHierarchia);

	SortedSequenceTable<string, Node*>* tabulkaRt = new SortedSequenceTable<string, Node*>();
	citac.vytvorTabulku(*tabulkaRt);

	string volbaUrovne;
	bool aplikaciaBezi = true;

	while (aplikaciaBezi)
	{
		cout << "Zvolte si uroven, ktoru si prajete zobrazit." << endl << "------------------------" << endl;
		cout << "1. Uroven jedna" << endl;
		cout << "2. Uroven dva - hierarchia" << endl;
		cout << "3. Uroven tri - tabulka" << endl;
		cout << "4. Ukonci aplikaciu" << endl;
		cout << "Zadajte volbu: ";
		cin >> volbaUrovne;

		if (volbaUrovne == "1")
		{
			terminal.spustiTerminalUrovenJedna(*rt);
		}
		else if (volbaUrovne == "2")
		{
			terminal.spustiTerminalUrovenDva(*viacCestnaHierarchia);

		}
		else if (volbaUrovne == "3")
		{
			terminal.spustiTerminalUrovenTri(*tabulkaRt);

		}
		else if (volbaUrovne == "4")
		{
			aplikaciaBezi = false;
			delete rt;
			rt = nullptr;
			delete viacCestnaHierarchia;
			viacCestnaHierarchia = nullptr;
			delete tabulkaRt;
			tabulkaRt = nullptr;
		}
		else {
			cout << endl << "Neplatny index, skuste to znovu." << endl;
		}
	}
}
