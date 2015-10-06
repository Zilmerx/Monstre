#include <ostream>
#include <istream>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <class T, class U>
bool est_entre_demi_ouvert(const T &val, const U &seuilmin, const U &seuilmax)
{
	return seuilmin <= val && val < seuilmax;
}
template <class T, class U>
bool est_entre_inclusif(const T &val, const U &seuilmin, const U &seuilmax)
{
	return seuilmin <= val && val <= seuilmax;
}

class ContrainteNonRespectee { };

template <class T, class Pred>
const T& valider_contrainte(Pred pred, const T &val)
{
	return !pred(val) ? throw ContrainteNonRespectee{} : val;
}

class Monstre
{
	string nom_;
public:
	Monstre(const string &nom)
		: nom_{ nom }
	{
	}
	string nom() const
	{
		return nom_;
	}
	virtual ostream& hurler(ostream&) const = 0;
	virtual void mettre_a_jour(ostream&, istream&) = 0;
	virtual ~Monstre() = default;
	//
	// VOTRE CODE ICI
	//
	virtual unique_ptr<Monstre> clone() const = 0;
};

ostream& operator<<(ostream &os, const Monstre &monstre)
{
	return monstre.hurler(os);
}

bool operator==(const Monstre &a, const Monstre &b)
{
	return a.nom() == b.nom();
}
bool operator!=(const Monstre &a, const Monstre &b)
{
	return !(a == b);
}

class Bestiole
	: public Monstre
{
	double agacement_; // [0..1)
public:
	Bestiole(const string &nom, double agacement)
		: Monstre{ nom },
		agacement_{
		valider_contrainte([](const double &val) {
			return est_entre_demi_ouvert(val, 0.0, 1.0);
		}, agacement)
	}
	{
	}
	friend void RendreMoinsAchalant(Bestiole&);
	friend void RendrePlusAchalant(Bestiole&);
	double agacement() const
	{
		return agacement_;
	}
	ostream& hurler(ostream &os) const override
	{
		return os << "Je suis " << nom() << " la bestiole, achalant a " << agacement() * 100 << "%";
	}
	void mettre_a_jour(ostream &out, istream &in) override
	{
		out << R"(Vos options :
   1) rendre plus achalant
   2) rendre moins achalant
Votre choix: )";
		int choix;
		while (in >> choix && !est_entre_inclusif(choix, 1, 2))
			out << R"(Vos options :
   1) rendre plus achalant
   2) rendre moins achalant
Votre choix: )";
		switch (choix)
		{
		case 1:
			RendrePlusAchalant(*this);
			break;
		case 2:
			RendreMoinsAchalant(*this);
			break;
		}
	}
	//
	// VOTRE CODE ICI
	//
	unique_ptr<Monstre> clone() const override
	{
		return make_unique<Bestiole>(*this);
	}
};

class Bibitte
	: public Monstre
{
	int puanteur_; // [1 .. 100]
	double mechancete_; // [0..1)
public:
	Bibitte(const string &nom, int puanteur, double mechancete)
		: Monstre{ nom },
		puanteur_{
		valider_contrainte([](const int &val) {
			return est_entre_inclusif(val, 1, 100);
		}, puanteur)
	},
	mechancete_{
		valider_contrainte([](const double &val) {
			return est_entre_demi_ouvert(val, 0.0, 1.0);
		}, mechancete)
	}
		{
	}
		friend void Laver(Bibitte&);
		friend void Salir(Bibitte&);
		int puanteur() const
		{
			return puanteur_;
		}
		double mechancete() const
		{
			return mechancete_;
		}
		ostream& hurler(ostream &os) const override
		{
			return os << "Je suis " << nom() << " la bibitte; puant a " << puanteur() << "% et mechant a " << mechancete() * 100 << "%";
		}
		void mettre_a_jour(ostream &out, istream &in) override
		{
			out << R"(Vos options :
   1) laver
   2) salir
Votre choix: )";
			int choix;
			while (in >> choix && !est_entre_inclusif(choix, 1, 2))
				out << R"(Vos options :
   1) laver
   2) salir
Votre choix: )";
			switch (choix)
			{
			case 1:
				Laver(*this);
				break;
			case 2:
				Salir(*this);
				break;
			}
		}
		//
		// VOTRE CODE ICI
		//
		unique_ptr<Monstre> clone() const override
		{
			return make_unique<Bibitte>(*this);
		}
};

void Laver(Bibitte &b)
{
	b.puanteur_ = 1;
}
void Salir(Bibitte &b)
{
	b.puanteur_ = min(100, b.puanteur_ + 5);
}
void RendreMoinsAchalant(Bestiole &b)
{
	b.agacement_ -= b.agacement_ * 0.5;
}
void RendrePlusAchalant(Bestiole &b)
{
	b.agacement_ += (1.0 - b.agacement_) * 0.5;
}


template <class T>
unique_ptr<T> modifier_potentiellement(unique_ptr<T> p, ostream &out, istream &in)
{
	//
	// VOTRE CODE ICI
	//

	unique_ptr<Monstre> clone = p->clone();

	clone->mettre_a_jour(out, in);

	int choix;
	do
	{
		out << R"(Souhaitez-vous sauvegarder les changements ? :
1) Oui
2) Non
Votre choix: )";
	} while (in >> choix && !est_entre_inclusif(choix, 1, 2));

	switch (choix)
	{
	case 1:
		return clone;
		break;
	case 2:
		return p;
		break;
	default:
		return p;
	}
}


class y
{
public:
	int x;
};

#include <iostream>

int main()
{
	vector<unique_ptr<Monstre>> v;
	v.push_back(make_unique<Bibitte>("Joe", 30, 0.75));
	v.push_back(make_unique<Bestiole>("Fred", 0.23));
	v.push_back(make_unique<Bestiole>("Bill", 0.8));
	v.push_back(make_unique<Bibitte>("Zebda", 66, 0.11));
	v.push_back(make_unique<Bestiole>("Guy", 0.99));
	for (auto & p : v)
	{
		cout << "Avant: " << *p << '\n';
		p = modifier_potentiellement(std::move(p), cout, cin);
		cout << "Apres: " << *p << '\n';
	}
}