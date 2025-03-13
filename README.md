# SO2(P) – Problem ucztujących filozofów

## **1. Instrukcja uruchomienia projektu w Linuxie** ##

### 1.1 Sklonuj repozytorium z GitHub ###
Wykorzystaj komendę : ```git clone https://github.com/d3vi1337/SO-2-PROJEKT```

### 1.2 Przejdź do katalogu, w którym znajduje się projekt ###
Wykorzystaj komendę : ```cd SO-2-PROJEKT```

### 1.3 Skompiluj projekt za pomocą Makefile ###
Wykorzystaj komendę : `make`                                                                                         
Ewentualnie skompiluj projekt ręcznie                                                                                
Wykorzystaj komendę : ```g++ -Wall -std=c++20 -pthread dining_ph_problem_P1.cpp -o dining_philosophers```

### 1.4 Uruchom skompilowany program ###
Wykorzystaj komendę : ```./dining_philosophers```

### 1.5 Wyjście z programu ###
Wykorzystaj skrót klawiszowy : ```CTRL + C```

## **2. Opis problemu** ##

### 2.1 Problem jedzących filozofów (**dining philosophers problem**) ###
Zadanie to zostało początkowo w innej wersji zaprezentowane przez *E. Dijkstre*. Dotyczy ono problemu synchronizacji procesów w środowisku współbieżnym.
    
Każdy z filozofów może znajdować się w jednym z trzech stanów:
- **THINKING** (filozof rozmyśla)
- **HUNGRY** (filozof jest głodny)
- **EATING** (filozof je)

Filozofowie w celu jedzenia mogą korzystać wyłącznie z lewego i prawego widelca, natomiast nie mogą się
oni ze sobą komunikować. Rozwiązanie problemu musi więc zapewniać, że filozofowie nigdy nie zakleszczą
się (**deadlock**) oraz nie będą oni zagłodzeni (**starvation**). Dodatkowo należy wyeliminować występowanie wyścigów (**race condition**) w sekcji krytycznej. Program implementuje rozwiązanie w C++ do klasycznego problemu jedzących filozofów wykorzystując mechanizmy synchronizacji tj. **muteksy** oraz **semafory zliczające**.

### 2.2 Zakleszczenie (**Deadlock**) ###
Sytuacja, w której mnoga liczba wątków czeka na dostęp do zasobu, który finalnie nigdy nie zostanie 
zostanie zapewniony, ponieważ wątki blokują dostęp do zasobu innym wątkom. 

W zadanym problemie, sytuacja taka może wystąpić, gdy każdy z filofów podniesie po jednym widelcu, 
przez co oczekując na drugi widelec nigdy go nie dostanie, co prowadzi do deadlocka. 

Program zapobiega zakleszczeniu sprawdzając czy dany filozof ma jednoczesny dostęp zarówno do 
lewego jak i prawego widelca.

### 2.3 Głodzenie (**Starvation**) ###
Sytuacja, w której wątek nie dostaje dostępu do danego zasobu, z racji na to, że inne procesy stale 
go uzyskują przed nim, przez co może on go nigdy nie uzyskać.

W zadanym problemie taka sytuacja może wystąpić, gdy niektórzy filozofowie częściej dostawaliby 
widelce, przez co inny filozof mógłby nigdy nie jeść.

Program zapobiega głodzeniu dzięki sprawdzeniu po zakończeniu jedzenia czy sąsiedzi danego filozofa mogą zacząć
jeść. Dzięki czemu filozof, który wcześniej nie mógł jeść, może zostać sprawdzony ponownie - jeżeli obecnie 
sąsiedzi nie jedzą to na jego semaforze jest wykonywana funkcja release przez co przechodzi on do jedzenia.

### 2.4 Wyścig (**Race condition**) ###
Sytuacja, w której wątki mogą uzyskać jednocześnie dostęp do współdzielonego zasobu, modyfikując 
go w tym samym czasie, co prowadzi do niezdeterminowanych rezultatów. 

W zadanym problemie wyścig może wystąpic w sekcji krytycznej, jeżeli kilku filozofów próbowałoby 
zmienić jednocześnie swój stan. Prowadziłoby to do nieprzewidywanych sytuacji.

Program zapobiega występowaniu wyścigów dzięki zabezpieczeniu sekcji krytycznej muteksami, przez 
co wyłącznie jeden filozof naraz może zmieniać swój stan.

## **3. Wątki i sekcje krytyczne w programie** ##

### 3.1 Wątki (**Threads**) ###
Liczba uruchomionych wątków jest określona przez wprowadzony argument liczby filozofów do programu (**N**).
Więc liczba filozofów **=** liczba wątków.
Każdy wątek reprezentuje osobnego filozofa, który przechodzi przez cykl - myślenie, bycie głodnym oraz jedzenie:
Każdy z wątków jest uruchamiany poprzez wywołanie funkcji **philosopher** podając za argument liczbę filozofów, funkcja ta reprezentuje zachowanie filozofa:
- Myślenie: filozof myśli
- Bycie głodnym: filozof staje się głodny
- Próba podniesienia widelców: sprawdzane jest czy filozof może uzyskać widelce
- Jedzenie: Jeżeli może jeść to przystępuje do tego, w innym wypadku czeka na kolej
- Odkładanie widelców: Filozof odkłada widelce, znowu myśli i sprawdza czy sąsiedzi mogą jeść

### Sekcje krytyczne (**Critical Sections**) ###
Sekcje krytyczne to miejsca w programie, w którym korzysta się z współdzielonego zasobu.
W celu zabezpieczeniu sekcji krytycznych, czyli zapewnieniu wzajemnego wykluczania (**mutual exclusion**) w programie wykorzystano muteksy - czyli mechanizmy synchronizacji.
Sekcje krytyczne w programie występują w dwóch miejscach:

- Funkcja *take_fork*: występuje w niej m.in. modyfikacja współdzielonej tablicy stanów filozofów *state[]*.
  - Rozwiązanie: Na początku funkcji zastosowany został *mutex1.lock()*, natomiast po wykonaniu *test* mutex jest zwalniany za pomocą *mutex1.unlock()*. Dzięki zastosowaniu muteksowi sekcja krytyczna jest zabezpieczona, tylko jeden wątek (filozof) może edytować wspólny zasób *state* co zapobiega wyścigom.

- Funkcja *put_fork*: występuje w niej również modyfikacja współdzielonej tablicy stanów filozofów *state[]*.
  - Rozwiązanie: Podobnie jak w poprzedniej funkcji zastosowano muteks dla sekcji krytycznej, dzięki czemu tablica *state* może być w danej chwili wyłącznie modyfikowana przez jeden wątek.

- Funkcja *test*: równiez modyfikuje tablicę *state*. Lecz wywołania funkcji odbywają się wyłącznie w funkcjach *take_fork* oraz *put_fork*, które zostały już zabepieczone muteksami.
