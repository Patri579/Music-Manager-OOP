# Music Manager CLI (C++)

Aplicatie de tip Command Line Interface (CLI) dezvoltata in C++17 care simuleaza arhitectura backend a unei platforme de streaming muzical. Proiectul demonstreaza aplicarea practica a conceptelor de Programare Orientata pe Obiecte (OOP), Design Patterns, Programare Generica (Templates) si gestionarea sigura a resurselor (RAII & Exception Handling).

## Functionalitati Principale (Meniu Interactiv CRUD)

* **Gestiune Utilizatori si Artisti (Create / Read / Delete):** Adaugarea si stocarea polimorfica a utilizatorilor standard, creatorilor de continut si artistilor intr-o baza de date indexata in memorie (`std::map`).
* **Sistem de Monetizare (Update cu RTTI):** Identificarea dinamica la runtime a creatorilor de continut (`dynamic_cast`) si procesarea retragerii veniturilor generate din reclame.
* **Gestiune Playlist-uri si Melodii:** Crearea de playlist-uri personalizate, sistem de rating (note 1-10 validate), sortare descrescatoare a melodiilor si stergerea automata a playlist-urilor goale.
* **Simulare Abonamente (Standard vs. Premium):** Configurarea dinamica a calitatii audio (128kbps vs. Lossless/FLAC) si a prezentei reclamelor folosind sablonul Abstract Factory.
* **Jurnalizare Activitate:** Inregistrarea centralizata a evenimentelor din aplicatie prin intermediul unui Logger unic.
* **Raportare si Statistici:** Contorizarea entitatilor specifice folosind algoritmi STL si expresii lambda.

## Arhitectura si Concepte Tehnice Implementate

### 1. Programare Orientata pe Obiecte (OOP)
* **Mostenire Multi-Nivel (3 niveluri) si Interfete:**
  * `IObject` (interfata de baza cu generare automata de ID unic) -> `Persoana` (clasa abstracta) -> `Utilizator` -> `CreatorContinut`.
  * Ramura paralela: `IObject` -> `Persoana` -> `Artist`, respectiv `Melodie` si `Playlist`.
* **Polimorfism si Functii Virtuale Pure:** Apelarea dinamica a comportamentelor specifice (`AfiseazaTip()`, `ActiuneSpecifica()`, `toString()`) prin pointeri catre clasa de baza.
* **Supraincarcarea Operatorilor:** Implementare pentru operatorii de citire/afisare (`>>`, `<<`), indexare (`[]`), incrementare prefixata (`++`), aritmetici (`+`, `-` inclusiv comutativitate) si relationali (`==`, `<`, `>`).

### 2. Design Patterns
* **Singleton (`Logger`):** Implementare Meyers' Singleton (cu constructor privat si stergerea constructorului de copiere si a `operator=`) pentru urmarirea globala a actiunilor din aplicatie.
* **Abstract Factory + Dependency Injection (`IFabricaAbonament`):** Decuplarea logicii clientului (`ClientAplicatie`) de crearea concreta a familiilor de obiecte (`IPlayerAudio` si `IReclama`) pentru pachetele Standard si Premium.

### 3. Programare Generica (Templates)
* **Clasa Template cu Parametru Non-Tip:** `TopMuzical<typename T, int N>` pentru gestionarea clasamentelor de dimensiune fixata la compilare, incluzand metode template imbricate.
* **Functii Template si Specializare Explicita:** `gasesteMaxim<T>()` si `afiseazaElement<T>()`, cu specializare explicita pentru `std::string`.

### 4. STL (Standard Template Library) si C++ Modern
* **Containere alese pe criterii de complexitate:**
  * `std::map<int, Persoana*>` - Cautare, inserare si stergere dupa ID in timp O(log N), garantand unicitatea cheilor.
  * `std::list<Playlist>` - Stergere in timp constant O(1) a playlist-urilor goale fara realocari de memorie.
  * `std::vector<int>` / `std::vector<Melodie>` - Acces aleatoriu in O(1) prin index si memorie contigua.
* **Algoritmi STL si Lambda Expressions:** Utilizarea `std::sort`, `std::find_if`, `std::remove_if`, `std::count_if` si `std::for_each` impreuna cu Structured Bindings (C++17).
* **Memory Safety:** Utilizarea `std::unique_ptr` (`std::make_unique`) la citirea datelor pentru prevenirea memory leak-urilor in cazul aparitiei exceptiilor, alaturi de respectarea Rule of Three / Five.

### 5. Tratarea Exceptiilor (Exception Handling)
* **Ierarhie proprie de exceptii** derivata din `std::exception`:
  * `EroareMuzicala` (clasa de baza, suprascrie `what() const noexcept`)
  * `EroareRating` (aruncata la introducerea unor note in afara intervalului [1, 10])
  * `EroareIndex` (aruncata la accesarea unui index invalid in `operator[]`)
* **Stack Unwinding:** Demonstratie dedicata in meniu pentru evidentierea dezalocarii automate a obiectelor de pe stiva (RAII) la propagarea unei exceptii.

## Compilare si Rulare

Proiectul nu necesita biblioteci externe si poate fi compilat cu orice compilator compatibil C++17 (GCC, Clang, MSVC).

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o music_manager
./music_manager
