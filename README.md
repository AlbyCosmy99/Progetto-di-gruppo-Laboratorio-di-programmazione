# 🚉 Progetto di gruppo – Laboratorio di Programmazione

Progetto svolto in collaborazione con altri due colleghi per il corso di **Laboratorio di Programmazione** (Università di Padova).  
Durata: **2 settimane** – Periodo: **Gennaio 2021**.  

In questo repository è presente **solo il mio codice** (gestione della parte "Stazione").

---

## 📌 Obiettivo del progetto
Bisognava simulare e gestire un **sistema ferroviario** composto da:  
- 🚉 **Stazioni** (primarie e secondarie)  
- 🚂 **Treni**  
- 🛤️ **Linea ferroviaria**  

Ogni componente è stato sviluppato da un membro del gruppo:  
- Io mi sono occupato della **classe `Station` e derivate** (`MainStation`, `SecondaryStation`),  
- Un collega ha gestito i **Treni**,  
- Un altro la **Linea ferroviaria** e la comunicazione tra stazioni e treni.  

---

## 🛠️ Struttura del progetto

- `Station.h` – Header file della classe `Station` e relative eccezioni  
- `Station.cpp` – Implementazione della classe base `Station`  
- `MainStation.cpp` – Implementazione di una stazione principale (gestione binari, parcheggi, partenze/arrivi)  
- `SecondaryStation.cpp` – Implementazione di una stazione secondaria  

---

## ⚙️ Funzionalità implementate
- 🚉 Gestione binari di andata e ritorno  
- 🚂 Arrivo, partenza e sosta dei treni  
- 🅿️ Parcheggio dei treni quando non ci sono binari liberi  
- ❌ Eccezioni dedicate (`WrongTrainException`, `WrongDirectionException`, ecc.)  
- 🔄 Meccanismi di inserimento e rimozione dei treni dai binari con aggiornamento dinamico  

---

## 📚 Note finali
Questo progetto ha richiesto **coordinazione di gruppo** e divisione chiara dei ruoli.  
È stato utile per comprendere:  
- Programmazione a oggetti avanzata in C++  
- Uso di **ereditarietà e polimorfismo**  
- Gestione della memoria (puntatori, distruttori)  
- Modellazione software di sistemi complessi  
- Sviluppo software in un team
