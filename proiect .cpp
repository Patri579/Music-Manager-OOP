#include <algorithm>
#include <exception>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

using namespace std;

class EroareMuzicala : public exception {
protected:
  string mesaj; // Mesajul cu eroarea
public:
  EroareMuzicala(const string &m) : mesaj(m) {}

  const char *what() const noexcept override { // Suprascriu mettoda what()
    return mesaj.c_str();
  }
}


class EroareRating : public EroareMuzicala {
private:
  int ratingInvalid;

public:
  EroareRating(const string &m, int rating)
      : EroareMuzicala(m), ratingInvalid(rating) {
    // Construim mesajul folosind std::to_string
    this->mesaj = m + " Valoarea introdusa a fost: " + to_string(ratingInvalid);
  }
};

class EroareIndex : public EroareMuzicala {
private:
  int indexInvalid;

public:
  EroareIndex(const string &m, int index)
      : EroareMuzicala(m), indexInvalid(index) {
    this->mesaj = m + " Indexul accesat: " + to_string(indexInvalid);
  }
};

class Logger {
private:
  vector<string> mesaje;

  Logger() {} // Constructorul este privat

public:
  static Logger &getInstance() {
    static Logger instance;
    return instance;
  }

  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  void log(const string &mesaj) { mesaje.push_back(mesaj); }

  void afiseazaJurnal() const {
    cout << "\n=== JURNALUL APLICATIEI (SINGLETON) ===\n";
    if (mesaje.empty()) {
      cout << "Nicio actiune inregistrata.\n";
    } else {
      for (const string &m : mesaje) {
        cout << " -> " << m << "\n";
      }
    }
    cout << "=======================================\n";
  }
};

class IObject {
protected:
  const int id;
  static int contor_id;

public:
  IObject() : id(++contor_id) {}
  virtual ~IObject() noexcept = default;

  int getId() const noexcept { return id; }
  virtual string toString() const = 0;

  bool operator==(const IObject &other) const { return this->id == other.id; }

  IObject &operator=(const IObject &other) { return *this; }

  friend ostream &operator<<(ostream &out, const IObject &obj) {
    out << obj.toString();
    return out;
  }
};

int IObject::contor_id = 0;
class Persoana
    : public IObject { // Clasa abstracta(nu poate fi instantiata direct
protected:
  string nume; // Protected pentru ca clasele derivate sa aiba acces la el

public:
  Persoana() : nume("Necunoscut") { // Constructor fara parametrii
    cout << "[Persoana] Constructor apelat\n";
  }

  Persoana(const string &n) : nume(n) { // costructor cu parametrii
    cout << "[Persoana] Constructor apelat\n";
  }

  Persoana(const Persoana &p) : nume(p.nume) {} // Constructor de copiere

  Persoana &
  operator=(const Persoana &p) { // Supraincarcarea operatorului de atribuire
    if (this == &p)
      return *this;
    this->nume = p.nume;
    return *this;
  }
  virtual ~Persoana() noexcept { cout << "[Persoana] Destructor apelat\n"; }
  string getNume() const { return this->nume; } // getter
  void setNume(string numeNou) {                // Setter
    this->nume = numeNou;
  }
  virtual void AfiseazaTip() const = 0;
  virtual void ActiuneSpecifica() const = 0;
};

class Artist : public Persoana {
private:
  int varsta;
  int nr_premii;
  static int nr_artisti;

public:
  Artist() : Persoana("Necunoscut") { // Constructor fara parametrii

    varsta = 0;
    nr_premii = 0;
    nr_artisti++;
  }
  Artist(const string &nume, int varsta,
         int nr_premii)
      : Persoana(nume) { // Constructor cu toti parametrii
    this->varsta = varsta;
    this->nr_premii = nr_premii;
    nr_artisti++;
    Logger::getInstance().log("A fost inregistrat artistul: " + nume);
  }
  Artist(const string &nume,
         int varsta)
      : Persoana(
            nume) { // Constructor cu 2 parametri(se considera ca nu au premii)
    this->varsta = varsta;
    this->nr_premii = 0;
    nr_artisti++;
    Logger::getInstance().log("A fost inregistrat artistul: " + nume);
  }
  Artist(const string &nume)
      : Persoana(nume) { // constructor cu un singur parametru
    this->varsta = 18;   // Consideram ca e la inceput de parcurs muzical
    this->nr_premii = 0;
    nr_artisti++;
    Logger::getInstance().log("A fost inregistrat artistul: " + nume);
  }
  Artist(const Artist &a) : Persoana(a) { // Constructorul de copiere
    this->varsta = a.varsta;
    this->nr_premii = a.nr_premii;
    nr_artisti++;
    Logger::getInstance().log("A fost inregistrat artistul: " + nume);
  }
  string toString() const override {
    return "[ID: " + to_string(getId()) + "] Artist: " + nume +
           " | Varsta: " + to_string(varsta) +
           " | Premii: " + to_string(nr_premii) + "\n";
  }
  Artist &operator=(const Artist &a) { // Suprascrierea operatorului =
    if (this == &a)
      return *this;
    Persoana::operator=(
        a); // folosim suprascrierea din clasa de baza pentru nume
    this->varsta = a.varsta;
    this->nr_premii = a.nr_premii;

    return *this;
  }
  ~Artist() noexcept { // Destructor
    varsta = 0;
    nr_premii = 0;
    nr_artisti--;
  }
  void AfiseazaTip() const override { // Override la functiile virtuale
    cout << "[Tip: Artist Muzical]\n";
  }

  void ActiuneSpecifica() const override {
    cout << nume << " compune si inregistreaza piese.\n";
  }
  char operator[](int index) { // returneaza o litera din numele artistului
    if (index >= 0 && index < this->nume.length())
      return this->nume[index];
    throw EroareIndex("Litera nu exista in nume!", index);
  }
  Artist &operator++() { // Operatorul++ prefixat
    this->varsta++;      // Artistul imbatraneste un an
    return *this;
  }
  Artist operator+(int premii_noi) { // Adauga premii noi
    Artist copie = *this;
    copie.nr_premii += premii_noi;
    return copie;
  }
  Artist operator-(int deScazut)
      const { // Scade din premii daca se dovedeste furt de melodii
    Artist copie = *this;
    copie.nr_premii -= deScazut;
    if (copie.nr_premii < 0)
      copie.nr_premii = 0;
    return copie;
  }
  bool
  operator==(const Artist &a) const { // Verifica daca artistii au acelasi nume
    return this->nume == a.nume;
  }
  bool operator>(const Artist &a) const { // Verifica daca varsta e mai mare
    return this->varsta > a.varsta;
  }
  friend istream &operator>>(istream &in, Artist &a);
  int getVarsta() const { return this->varsta; } // Getteri
  static int getNrArtisti() { return nr_artisti; }
  void setVarsta(int v) { this->varsta = v; }
};
Artist operator+(int premii_noi, Artist a) { // comutativitatea
  return a + premii_noi;
}
istream &operator>>(istream &in, Artist &a) {
  cout << "Nume artist: ";
  string tempNume; // Nu mai folosim char buffer[100]!

  in >> ws;
  getline(in, tempNume);

  a.setNume(tempNume);

  cout << "Varsta artist: ";
  in >> a.varsta;
  cout << "Numar premii: ";
  in >> a.nr_premii;
  return in;
}
int Artist::nr_artisti = 0;

class Melodie : public IObject {
private:
  string titlu;
  vector<int> ratinguri; // Am ales std::vectorpentru ca ofera acces aleator
                         // prin index in O(1) si pentru caadaugarea de elemente
                         // noi la final(push_back) se face tot in O(1)
  Artist cantaret;

public:
  Melodie() : titlu("Necunoscut"), cantaret() {
    cout << "[Melodie] Constructor apelat\n";
  }

  Melodie(const string &titlu, const vector<int> &ratinguriAlese,
          const Artist &a)
      : titlu(titlu), cantaret(a) {
    for (int nota : ratinguriAlese) {
      if (nota < 1 || nota > 10)
        throw EroareRating("Rating invalid!", nota);
      this->ratinguri.push_back(nota);
    }
    Logger::getInstance().log("A fost creata o melodie cu titlul: " + titlu);
    cout << "[Melodie] Constructor apelat\n";
  }

  Melodie(const string &t) : titlu(t), cantaret() {
    cout << "[Melodie] Constructor apelat\n";
    Logger::getInstance().log("A fost creata o melodie cu titlul: " + titlu);
  }

  ~Melodie() noexcept { cout << "[Melodie] Destructor apelat\n"; }

  string getTitlu() const { return this->titlu; }
  int getNrRatinguri() const { return this->ratinguri.size(); }
  void setTitlu(const string &t) { this->titlu = t; }

  string toString() const override {
    string rez = "[ID: " + to_string(getId()) + "] Titlu: " + titlu + "\n";
    rez += "Nr de ratinguri: " + to_string(ratinguri.size()) + "\n";
    if (!ratinguri.empty()) {
      rez += "Ratinguri: ";
      for (int nota : ratinguri)
        rez += to_string(nota) + " ";
      rez += "\n";
    }
    rez += "Artist:\n" + cantaret.toString(); // Aici fol toString-ul artistului
    return rez;
  }

  int &operator[](int index) {
    if (index >= 0 && index < this->ratinguri.size())
      return this->ratinguri[index];
    throw EroareIndex("Ratingul cautat nu exista in vector!", index);
  }

  Melodie &operator++() {
    for (size_t i = 0; i < this->ratinguri.size(); i++)
      if (this->ratinguri[i] < 10)
        this->ratinguri[i]++;
    return *this;
  }

  Melodie operator+(int notaNoua) {
    Melodie copie = *this;
    if (notaNoua < 1 || notaNoua > 10) {
      throw EroareRating("Rating invalid la adaugarea unei note noi!",
                         notaNoua);
    }
    copie.ratinguri.push_back(notaNoua);
    return copie;
  }

  Melodie operator-(int valoare) {
    Melodie copie = *this;
    for (size_t i = 0; i < copie.ratinguri.size(); i++) {
      copie.ratinguri[i] -= valoare;
      if (copie.ratinguri[i] < 1)
        copie.ratinguri[i] = 1;
    }
    return copie;
  }

  bool operator==(const Melodie &m) const {
    return (this->titlu == m.titlu &&
            this->ratinguri.size() == m.ratinguri.size());
  }

  bool operator<(const Melodie &m) const {
    return this->ratinguri.size() < m.ratinguri.size();
  }

  friend istream &operator>>(istream &in, Melodie &m);
};

Melodie operator+(int nota, Melodie m) { return m + nota; }

istream &operator>>(istream &in, Melodie &m) {
  cout << "Introduceti titlul: ";
  in >> ws;
  getline(in, m.titlu);

  int nr_citit;
  cout << "Introduceti nr de ratinguri: ";
  in >> nr_citit;

  m.ratinguri.clear();

  if (nr_citit > 0) {
    for (int i = 0; i < nr_citit; i++) {
      cout << "Rating [" << i << "]: ";
      int nota_introdusa;
      in >> nota_introdusa;
      if (nota_introdusa < 1 || nota_introdusa > 10) {
        throw EroareRating("Rating invalid introdus de la tastatura!",
                           nota_introdusa);
      }
      m.ratinguri.push_back(nota_introdusa);
    }
  }
  cout << "Introduceti datele artistului:" << endl;
  in >> m.cantaret;
  return in;
}

class Playlist : public IObject {
private:
  string nume_playlist;
  vector<Melodie> lista_melodii;

public:
  Playlist(const string &nume, const vector<Melodie> &vectorMelodii)
      : nume_playlist(nume), lista_melodii(vectorMelodii) {
    Logger::getInstance().log("A fost generat un playlist nou: " +
                              nume_playlist);
  }

  Playlist() : nume_playlist("Playlist nou") {}

  Playlist(const string &nume) : nume_playlist(nume) {
    Logger::getInstance().log("A fost generat un playlist nou: " +
                              nume_playlist);
  }

  ~Playlist() noexcept {}

  string toString() const override {
    string rez = "--- [ID: " + to_string(getId()) +
                 "] PLAYLIST: " + nume_playlist + " ---\n";
    rez += "Numar melodii actuale: " + to_string(lista_melodii.size()) + "\n";
    if (!lista_melodii.empty()) {
      int index = 1;
      for (const Melodie &m : lista_melodii) {
        rez += to_string(index++) + ". " + m.toString();
      }
    } else {
      rez += "[Playlistul este gol]\n";
    }
    rez += "----------------------------\n";
    return rez;
  }

  Melodie &operator[](int index) {
    if (index >= 0 && index < lista_melodii.size()) {
      return this->lista_melodii[index];
    }
    throw EroareIndex("Melodia nu exista in acest playlist!", index);
  }

  Playlist &operator++() {
    this->lista_melodii.push_back(Melodie());
    return *this;
  }

  Playlist operator+(const Melodie &m) {
    Playlist copie = *this;
    copie.lista_melodii.push_back(m);
    return copie;
  }

  Playlist operator-(int nrDeSters) {
    Playlist copie = *this;
    while (nrDeSters > 0 && !copie.lista_melodii.empty()) {
      copie.lista_melodii.pop_back();
      nrDeSters--;
    }
    return copie;
  }

  bool operator==(const Playlist &p) const {
    return (this->lista_melodii.size() == p.lista_melodii.size() &&
            this->nume_playlist == p.nume_playlist);
  }

  bool operator<(const Playlist &p) const {
    return this->lista_melodii.size() < p.lista_melodii.size();
  }

  void SorteazaMelodiiDescrescator() {
    sort(lista_melodii.begin(), lista_melodii.end(),
         [](const Melodie &m1, const Melodie &m2) { return m1 < m2; });
  }

  friend istream &operator>>(istream &in, Playlist &p);

  string getNumePlaylist() const { return this->nume_playlist; }
  int getNrMelodii() const { return this->lista_melodii.size(); }
  void setNumePlaylist(const string &numeNou) { this->nume_playlist = numeNou; }

  void StergeMelodieDupaTitlu(const string &titluDeSters) {
    lista_melodii.erase(remove_if(lista_melodii.begin(), lista_melodii.end(),
                                  [&titluDeSters](const Melodie &m) {
                                    return m.getTitlu() == titluDeSters;
                                  }),
                        lista_melodii.end());
  }
};

Playlist operator+(const Melodie &m, Playlist p) { return p + m; }

istream &operator>>(istream &in, Playlist &p) {
  cout << "Introdu numele noului playlist: ";
  in >> ws;
  getline(in, p.nume_playlist);

  int nr_citit;
  cout << "Cate melodii vrei sa adaugi acum? ";
  in >> nr_citit;

  p.lista_melodii.clear();

  if (nr_citit > 0) {
    for (int i = 0; i < nr_citit; i++) {
      cout << "Melodia " << i + 1 << ":" << endl;
      Melodie m_temp;
      in >> m_temp;
      p.lista_melodii.push_back(m_temp);
    }
  }
  return in;
}

class Utilizator : public Persoana {
private:
  list<Playlist> playlisturi; // Am ales std::list pentru ca, fiind o lista
                              // dublu inlantuita, permite stergeri si inserari
                              // in timp O(1) (dupa ce elementul a fost gasit).
  //  Este ideala aici deoarece folosim des
  // metoda remove_if (ex. pentru stergerea playlisturilor goale), evitand
  // astfel realocari si  de memorie
  bool premium;

public:
  Utilizator() : Persoana("Anonim"), premium(false) {}

  Utilizator(const string &nume, const list<Playlist> &lista, bool premium)
      : Persoana(nume), playlisturi(lista), premium(premium) {}

  Utilizator(const string &nume) : Persoana(nume), premium(false) {}

  Utilizator(const string &nume, int nr) : Persoana(nume), premium(false) {
    for (int i = 0; i < nr; i++) {
      playlisturi.push_back(Playlist());
    }
  }

  ~Utilizator() noexcept override {}

  void AfiseazaTip() const override { cout << "[Tip: Utilizator]\n"; }

  void ActiuneSpecifica() const override {
    cout << nume << " Asculta piese.\n";
  }

  int getNrPlaylisturi() const { return this->playlisturi.size(); }
  bool getEstePremium() const { return this->premium; }
  void setEstePremium(bool status) { this->premium = status; }

  string toString() const override {
    string rez = "\n=== [ID: " + to_string(getId()) +
                 "] PROFIL UTILIZATOR: " + nume + " ===\n";
    rez += "Statut: ";
    rez += (premium ? "PREMIUM (Fara reclame)" : "Standard (Cu reclame)");
    rez +=
        "\nNumar playlist-uri salvate: " + to_string(playlisturi.size()) + "\n";

    if (!playlisturi.empty()) {
      int i = 1;
      for (const Playlist &p : playlisturi) {
        rez += "\n[Playlist-ul " + to_string(i++) + "]\n" + p.toString();
      }
    } else {
      rez += "Niciun playlist.\n";
    }
    rez += "========================================\n";
    return rez;
  }

  Playlist &operator[](int index) {
    if (index >= 0 && index < playlisturi.size()) {
      auto it = next(playlisturi.begin(), index);
      return *it;
    }
    throw EroareIndex("Playlistul cautat nu exista la acest utilizator!",
                      index);
  }

  Utilizator &operator++() {
    this->playlisturi.push_back(Playlist());
    return *this;
  }

  Utilizator operator+(const Playlist &p) {
    Utilizator copie = *this;
    copie.playlisturi.push_back(p);
    return copie;
  }

  Utilizator operator-(int nrDeSters) {
    Utilizator copie = *this;
    while (nrDeSters > 0 && !copie.playlisturi.empty()) {
      copie.playlisturi.pop_back();
      nrDeSters--;
    }
    return copie;
  }

  bool operator==(const Utilizator &u) const {
    return (this->nume == u.nume &&
            this->playlisturi.size() == u.playlisturi.size() &&
            this->premium == u.premium);
  }

  bool operator<(const Utilizator &u) const {
    return this->playlisturi.size() < u.playlisturi.size();
  }

  bool CautaPlaylist(const string &numeCautat) {
    auto it = find_if(playlisturi.begin(), playlisturi.end(),
                      [&numeCautat](const Playlist &p) {
                        return p.getNumePlaylist() == numeCautat;
                      });
    return it != playlisturi.end();
  }

  void StergePlaylisturiFaraMelodii() {
    playlisturi.remove_if(
        [](const Playlist &p) { return p.getNrMelodii() == 0; });
  }

  friend istream &operator>>(istream &in, Utilizator &u);
};

Utilizator operator+(const Playlist &p, Utilizator u) { return u + p; }

istream &operator>>(istream &in, Utilizator &u) {
  cout << "Introdu numele utilizatorului: ";
  string tempNume;

  in >> ws;
  getline(in, tempNume);
  u.setNume(tempNume);

  cout << "Are cont premium? (1 pt Da, 0 pt Nu): ";
  in >> u.premium;

  int nr_citit;
  cout << "Cate playlist-uri are? ";
  in >> nr_citit;

  u.playlisturi.clear();

  if (nr_citit > 0) {
    for (int i = 0; i < nr_citit; i++) {
      cout << "\n--- Playlist " << i + 1 << " ---\n";
      Playlist p_temp;
      in >> p_temp;
      u.playlisturi.push_back(p_temp);
    }
  }
  return in;
}

class CreatorContinut : public Utilizator { // Nivelul 3 al mostenirii
private:
  float venit_din_reclame;

public:
  CreatorContinut()
      : Utilizator("Creator_Necunoscut"), venit_din_reclame(0.0f) {
    cout << "[CreatorContinut] Constructor apelat\n";
  }

  CreatorContinut(const string &nume, int nr_playlisturi, float venit)
      : Utilizator(nume, nr_playlisturi), venit_din_reclame(venit) {
    cout << "[CreatorContinut] Constructor apelat\n";
  }

  ~CreatorContinut() noexcept override {
    cout << "[CreatorContinut] Destructor apelat\n";
  }

  void AfiseazaTip() const override {
    Utilizator::AfiseazaTip();
    cout << "   -> Mai exact: Creator de Continut Monetizat!\n";
  }

  void ActiuneSpecifica() const override {
    cout << nume
         << " posteaza melodii noi si genereaza venituri din reclame!\n";
  }

  void RetrageBani() {
    cout << ">>> " << nume << " a retras suma de " << venit_din_reclame
         << " RON in contul bancar!\n";
    venit_din_reclame = 0;
  }
  string toString() const override {
    string rez = Utilizator::toString();
    rez += "   -> [Creator de Continut Monetizat] Venit: " +
           to_string(venit_din_reclame) + " RON\n";
    return rez;
  }
};
class DemonstratieUnwinding {
public:
  DemonstratieUnwinding() {
    cout << "--> [Unwinding] Obiect creat pe stiva (Resursa alocata)\n";
  }
  ~DemonstratieUnwinding() {
    cout << "--> [Unwinding] Destructor apelat automat (Resursa eliberata)! ";
  }
};
template <typename T> void afiseazaElement(const T &element) {
  cout << "=> " << element << "\n";
}

// Am specializat functia pentru tipul std::string deoarece vreau ca mesajele
// text sa aiba un prefix specific ("Melodia momentului"), tratandu-le diferit
// fata de numere
template <> void afiseazaElement<string>(const string &text) {
  cout << "=> Melodia momentului " << text << "\n";
}

template <typename T> T gasesteMaxim(const T &a, const T &b) {
  if (a > b) {
    return a;
  }
  return b;
}

template <typename T, int N> class TopMuzical {
private:
  T elemente[N]; // Dimensiunea colectiei este fixata de N(paramtru non tip)
  int contor;

public:
  TopMuzical() : contor(0) {}

  void adauga(const T &elem) {
    if (contor >= N) {
      throw EroareMuzicala(
          "Topul este plin! Nu mai poti adauga elemente in acest clasament.");
    }
    elemente[contor++] = elem;
  }

  void afiseazaClasament() const {
    cout << "--- CLASAMENT TOP " << N << " ---\n";
    for (int i = 0; i < contor; i++) {
      cout << "Locul " << i + 1 << ": " << elemente[i];
    }
  }

  template <typename U> void interactioneazaCu(const TopMuzical<U, N> &altTop) {
    cout << "[Metoda Template] Schimb de statistici intre clasamente "
            "diferite.\n";
  }
};

class IPlayerAudio {
public:
  virtual void redaMuzica() const = 0;
  virtual ~IPlayerAudio() = default;
};

class IReclama {
public:
  virtual void afiseazaReclama() const = 0;
  virtual ~IReclama() = default;
};

class PlayerCalitateSlaba : public IPlayerAudio {
public:
  void redaMuzica() const override {
    cout << "Redare muzica la calitate 128kbps...\n";
  }
};

class ReclamaAudio : public IReclama {
public:
  void afiseazaReclama() const override {
    cout << "Reclama: Cumpara Premium pentru muzica fara intreruperi!\n";
  }
};

class PlayerCalitateInalta : public IPlayerAudio {
public:
  void redaMuzica() const override {
    cout << "Redare muzica la calitate Lossless/FLAC...\n";
  }
};

class FaraReclama : public IReclama {
public:
  void afiseazaReclama() const override {
    cout << "Nicio reclama. Bucura-te de muzica!\n";
  }
};

class IFabricaAbonament {
public:
  virtual IPlayerAudio *crearePlayer() const = 0;
  virtual IReclama *creareReclama() const = 0;
  virtual ~IFabricaAbonament() = default;
};

class FabricaStandard : public IFabricaAbonament {
public:
  IPlayerAudio *crearePlayer() const override {
    return new PlayerCalitateSlaba();
  }
  IReclama *creareReclama() const override { return new ReclamaAudio(); }
};

class FabricaPremium : public IFabricaAbonament {
public:
  IPlayerAudio *crearePlayer() const override {
    return new PlayerCalitateInalta();
  }
  IReclama *creareReclama() const override { return new FaraReclama(); }
};

class ClientAplicatie {
private:
  IPlayerAudio *player;
  IReclama *reclama;

public:
  // Primeste fabrica prin Dependency Injection (constructor)
  ClientAplicatie(const IFabricaAbonament &fabrica) {
    player = fabrica.crearePlayer();
    reclama = fabrica.creareReclama();
  }

  void ruleazaExperienta() const {
    player->redaMuzica();
    reclama->afiseazaReclama();
  }

  ~ClientAplicatie() {
    delete player;
    delete reclama;
  }
};

int main() {
  map<int, Persoana *> bazaDeDate; // Am ales std::map pentru a stoca perechi de
                                   // tip (ID -> Pointer Catre Obiect).
  // Acesta pastreaza elementele sortate dupa cheie (ID) si ne garanteaza ca nu
  // exista ID-uri duplicate. Operatiile CRUD principale (cautarea unui ID,
  // inserarea si stergerea) se realizeaza eficient in timp O(log N).
  int optiune;

  afiseazaElement(100);
  afiseazaElement(string("Metanoia"));

  Artist art1("Drake", 20, 2);
  Artist art2("Michael", 35, 10);
  Artist veteran = gasesteMaxim(art1, art2);
  cout << "Artistul mai in varsta este: " << veteran.getNume() << "\n";

  TopMuzical<Melodie, 3> top3Melodii;
  top3Melodii.adauga(Melodie("Piesa de pe Locul 1"));
  top3Melodii.adauga(Melodie("Piesa de pe Locul 2"));
  top3Melodii.afiseazaClasament();
  do {
    cout << "\n=========================================" << endl;
    cout << "      MANAGER MUZICAL (PROIECT 3)        " << endl;
    cout << "=========================================" << endl;
    cout << "1. Create -> Adauga un Utilizator Simplu" << endl;
    cout << "2. Create -> Adauga un Creator de Continut" << endl;
    cout << "3. Create -> Adauga un Artist" << endl;
    cout << "4. Read   -> Afiseaza toti" << endl;
    cout << "5. Update -> Plateste Creator dupa ID" << endl;
    cout << "6. Delete -> Sterge persoana dupa ID" << endl;
    cout << "7. Demonstreaza Stack Unwinding" << endl;
    cout << "8. Raport -> Numara Artistii" << endl;
    cout << "9. Jurnal -> Afiseaza istoricul aplicatiei (Singleton)" << endl;
    cout << "10. Demonstreaza Abstract Factory (Experienta Abonament)" << endl;
    cout << "0. Iesire" << endl;
    cout << "=========================================" << endl;
    cout << "Alege o optiune: ";

    if (!(cin >> optiune)) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Input invalid!\n";
      continue;
    }

    try {
      switch (optiune) {
      case 1: {
        cout << "\n[ADAUGARE UTILIZATOR]\n";
        Utilizator *u = new Utilizator();
        cin >> *u;
        bazaDeDate[u->getId()] = u;
        cout << "Utilizator adaugat cu ID-ul: " << u->getId() << "!\n";
        break;
      }
      case 2: {
        cout << "\n[ADAUGARE CREATOR DE CONTINUT]\n";
        string bufferNume;
        cout << "Introdu numele creatorului: ";
        cin >> ws;
        getline(cin, bufferNume);

        int nrPlaylisturi;
        cout << "Cate playlist-uri are? ";
        cin >> nrPlaylisturi;

        float venit;
        cout << "Ce venit initial a generat din reclame? (RON): ";
        cin >> venit;
        CreatorContinut *cc =
            new CreatorContinut(bufferNume, nrPlaylisturi, venit);
        bazaDeDate[cc->getId()] = cc;
        cout << "Creator adaugat cu ID-ul: " << cc->getId() << "!\n";
        break;
      }
      case 3: {
        cout << "\n[ADAUGARE ARTIST]\n";
        Artist *a = new Artist();
        cin >> *a;
        bazaDeDate[a->getId()] = a;
        cout << "Artist adaugat cu ID-ul: " << a->getId() << "!\n";
        break;
      }
      case 4: {
        cout << "\n[Afisare baza de date]\n";
        if (bazaDeDate.empty())
          cout << "Nu exista inregistrari.\n";

        for_each(bazaDeDate.begin(), bazaDeDate.end(),
                 [](const pair<const int, Persoana *> &element) {
                   auto [id, p] = element; // Structured bindinh
                   cout << "\n--- ID Inregistrare Map: " << id << " ---\n";
                   p->AfiseazaTip();
                   p->ActiuneSpecifica();
                   cout << *p;
                 });
        break;
      }
      case 5: {
        cout << "\n[Plata creatori de continut]\n";
        int id_cautat;
        cout << "Introdu ID-ul persoanei pe care vrei sa o platesti: ";
        cin >> id_cautat;

        auto it = bazaDeDate.find(id_cautat);
        if (it != bazaDeDate.end()) {
          CreatorContinut *cc = dynamic_cast<CreatorContinut *>(it->second);
          if (cc != nullptr) {
            cc->RetrageBani();
          } else {
            cout << "Eroare: Persoana gasita nu este un Creator de Continut!\n";
          }
        } else {
          cout << "Nu s-a gasit nicio persoana cu acest ID!\n";
        }
        break;
      }
      case 6: {
        cout << "\n[STERGERE PERSOANA]\n";
        int id_stergere;
        cout << "Introdu ID-ul persoanei de sters: ";
        cin >> id_stergere;

        auto it = bazaDeDate.find(id_stergere);
        if (it != bazaDeDate.end()) {
          delete it->second;
          bazaDeDate.erase(it);
          cout << "Persoana stearsa cu succes!\n";
        } else {
          cout << "Nu s-a gasit nicio persoana cu acest ID!\n";
        }
        break;
      }
      case 7: {
        cout << "\n[DEMONSTRATIE STACK UNWINDING]\n";
        DemonstratieUnwinding demo;
        throw EroareMuzicala("Eroare fortata pentru a testa Unwinding-ul");
      }
      case 8: {
        cout << "\n[RAPORT]\n";
        int numarArtisti =
            count_if(bazaDeDate.begin(), bazaDeDate.end(),
                     [](const pair<const int, Persoana *> &element) {
                       auto [id, p] = element;
                       return dynamic_cast<Artist *>(p) != nullptr;
                     });
        cout << "Avem inregistrati " << numarArtisti
             << " artisti muzicali in aplicatie.\n";
        break;
      }
      case 9: {
        Logger::getInstance().afiseazaJurnal();
        // Logger log2 = Logger::getInstance();// Eroare compliare(nu se pot
        // crea 2 instante)
        break;
      }
      case 10: {
        cout << "\n[ABSTRACT FACTORY]\n";
        cout << "1. Ruleaza Experienta Standard\n";
        cout << "2. Ruleaza Experienta Premium\n";
        cout << "Alege tipul de abonament simulat: ";
        int optAbonament;
        cin >> optAbonament;

        IFabricaAbonament *fabricaReala = nullptr;

        if (optAbonament == 1) {
          fabricaReala = new FabricaStandard();
        } else if (optAbonament == 2) {
          fabricaReala = new FabricaPremium();
        } else {
          cout << "Optiune invalida.\n";
          break;
        }

        ClientAplicatie client(
            *fabricaReala); // DEPENDENCY INJECTION: Clientul nu stie ce fel de
                            // fabrica primeste
        client.ruleazaExperienta();
        delete fabricaReala;
        break;
      }
      case 0:
        break;
      default:
        cout << "Optiune invalida!\n";
        break;
      }
    } catch (const EroareRating &e) {
      cout << "\n[EROARE RATING] Ai gresit o nota: " << e.what() << endl;
      cout << "Operatiunea a fost anulata. Incearca din nou.\n";
    } catch (const EroareIndex &e) {
      cout << "\n[EROARE INDEX] Cautare gresita: " << e.what() << endl;
      cout << "Operatiunea a fost anulata. Incearca din nou.\n";
    } catch (const EroareMuzicala &e) {
      cout << "\n[EROARE MUZICALA GENERALA]: " << e.what() << endl;
    } catch (...) {
      cout << "\n[EROARE NECUNOSCUTA] S-a intamplat ceva grav!\n";
    }

  } while (optiune != 0);
  for (auto &pereche : bazaDeDate) {
    delete pereche.second;
  }
  bazaDeDate.clear();

  return 0;
}