/*
AUTORE: ALBU COSMIN ANDREI
*/

#ifndef Station_h
#define Station_h
#ifndef Park_h
#define Park_h

#include <iostream>
#include "Train.h"
#include <vector>

//classi da lanciare per i binari (classe Rail)
class MoreTrainsOnARailError {};   //se si prova a inserire un treno su un binario in cui c'e' gia' un treno
class RailWithNoDirectionException {};  //se si chiede la direzione a un binario senza direzione
class NoTrainOnTheRailException {};  //se si chiede il treno a un binario senza treno
class RailCannotHaveOccupantTrain {};  //se si cerca di accedere,mettere o togliere un treno occupante un binario che non puo' averne


//classi da lanciare per le stazioni (classe Station)
class WrongStationException {}; //se alla richiesta di un dato membro con le funzioni di accesso, questo e' nullptr
class WrongDirectionException {};  //se alle funzioni vengono passate direzioni errate, ossia !=0 e !=1
class WrongTrainException {};    //se alle funzioni vengono passati treni errati
class  NumberTrainsTransitingNegativeError {}; //se stanno transitando un numero negativo di treni
class TrainsCannotTransitInThisStationError {};  //se un treno sta cercando di transitare in una stazione in cui non is puo' transitare

class Station
{
public:
	//costruttori
	Station(std::string _stationIdentifier, int _frontDistance);
	// _frontDistance e' la distanza dalla stazione origine

	//funzioni di accesso
	std::string getIdentifier() const { return stationIdentifier; };
	int getFrontDistance() const { return frontDistance; };
	int getBackDistance() const { return backDistance; };
	//_backDistance e' la distanza dalla stazione capolinea

	//modificatori
	void setBackDistance(int newBackDistance) { backDistance = newBackDistance; };

	//funzioni per la comunicazione con treni e linea feroviaria
	virtual void departure(const Train* train, int arrivingDirection) = 0;  //funzione chiamata quando un treno deve partire dalla stazione

	virtual int railRequest(const Train* train, int arrivingDirection) = 0;   //funzione chiamata quando un treno si sta avvicinando alla 
				//stazione				   //restituisce 0-fermata , 1-transito, 2-parcheggio   
	virtual bool startRequest(const Train* train, int arrivingDirection) = 0;  //funzione chiamata quando un treno deve iniziare il suo 
																			  //viaggio
	void addInPark(const Train* train, int arrivingDirection);  //mette un treno nel parcheggio

	virtual void moveTrainsToRail(const int arrivingDirection) = 0; //controlla se ci sono binari liberi e eventualmente prende treni
																	// dal parcheggio
	virtual bool trainsAreTransiting(int arrivingDirection) const = 0;  //true se nella stazione stanno transitando treni

	virtual void transitEnded(int arrivingDirection) = 0;  //il treno passato come argomento ha finito di transitare in stazione

	//distruttore
	~Station();

protected:
	//classi annidate
	class Rail
	{
	public:

		//costruttori
		Rail();   //costruttore di default: binario senza verso; utilizzata nei costruttori di Station
		Rail(int _direction);  //verso del binario; 0 - andata , 1 - ritorno

		//funzioni di accesso
		int getDirection() const;  //resituisce il verso del binario
		bool railTaken() const;  //il binario e' occupato? Restituisce true se e' occupato
		const Train* getOccupantTrain() const; //restituisce il treno occupato, non piu' const; nullptr se non c'e' nessun treno
		bool firstTrainInPark();

		//modificatori
		void setOccupantTrain(const Train*);  //associa al binario un treno
		void removeOccupantTrain();  //rimuove il treno dal binario

	protected:
		//variabili
		int direction;  //verso binario, uguale a INVALID_DIRECTION 
		bool isTaken;   //true se il binario e' occupato
		bool canHasOccupantTrain;  //true se il binario puo' avere un treno occupante


		//costanti
		const Train* trainOccuping; //treno che occupa il binario; nullptr se il binario e' libero; const perche' il binario non puo'
								//modificare il treno
		static constexpr int INVALID_DIRECTION = -1;
	};

	class StdRail : public Rail
	{
	public:
		//costruttori
		StdRail(int _direction);    //verso del binario; 0 - andata , 1 - ritorno

		//funzioni di accesso
		double getSpeedLimit() const { return SPEED_LIMIT; };

	private:
		//costanti
		static constexpr double SPEED_LIMIT = 80.0; // km/h
	};

	class TransitRail : public Rail
	{
	public:
		//costruttori
		TransitRail(int _direction);  //verso del binario; 0 - andata , 1 - ritorno
	};

	class Park
	{
	public:

		//costruttori
		Park();   //costruttore di default; parcheggio vuoto

		//funzioni di controllo
		bool isEmpty();   //true se non ci sono treni in parcheggio
		bool trainInPark(Train*); //restituisce true se il treno passato come argomento e' nel parcheggio

		//funzioni per la comunicazione 
		void addTrain(Train* train);  //inserisci un treno nel parcheggio
		Train* giveNextTrain();  //restituisce il prossimo treno e toglie il treno stesso dal parcheggio
		void removeTrain(Train* train);  //toglie un treno dal parcheggio
		bool firstTrainInPark();  //restituisce isInPark del treno con piu' priorita' in parcheggio

	private:
		//variabili
		std::vector<Train*> trains;  //vettore di puntatori a treni

		//funzioni di accesso
		int indexNextTrain();  //restituisce l'indice del prossimo treno che uscira' dal parcheggio
	};

	//costanti
	static constexpr int OUTWARD_DIRECTION = 0;
	static constexpr int RETURN_DIRECTION = 1;

	//dati membro
	std::string stationIdentifier;
	int frontDistance;  //distanza della stazione dall'origine al capolinea in km;
	int backDistance;   //distanza della stazione dal capolinea all'origine in km;
	Park* outwardPark;   //puntatore a un parcheggio di verso andata associato alla stazione 
	Park* returnPark;   //puntatore a un parcheggio di verso ritorno associato alla stazione
	Rail* outwardRails;  //puntatore a un array di binari, verso andata
	Rail* returnRails;   //puntatore a un array di binari, verso ritorno

	//controlla se l'stanza D e' di tipo B
	template<typename D, typename B>
	bool isType(const B& b)
	{
		return typeid(D).hash_code() == typeid(b).hash_code();
	}
};

class MainStation : public Station
{
public:

	//costruttori
	MainStation(std::string _stationIdentifier, int _frontDistance);

	//funzioni per la comunicazione con treni e linea feroviaria
	void departure(const Train* train, int arrivingDirection) override;

	int railRequest(const Train* train, int arrivingDirection) override;

	bool startRequest(const Train* train, int arrivingDirection) override;

	void moveTrainsToRail(const int arrivingDirection) override;

	bool trainsAreTransiting(int arrivingDirection) const override;

	void transitEnded(int arrivingDirection) override;

	//distruttore
	~MainStation();

private:

	//costanti
	static constexpr int RAILS_PER_SIDE = 2;
};

class SecondaryStation : public Station
{
public:

	//costruttori
	SecondaryStation(std::string _stationIdentifier, int _frontDistance);

	//funzioni per la comunicazione con treni e linea feroviaria
	void departure(const Train* train, int arrivingDirection) override;

	int railRequest(const Train* train, int arrivingDirection) override;

	bool startRequest(const Train* train, int arrivingDirection) override;

	void moveTrainsToRail(const int arrivingDirection) override;

	bool trainsAreTransiting(int arrivingDirection) const override;

	void transitEnded(int arrivingDirection) override;


	//distruttore
	~SecondaryStation();


private:

	//variabili
	int outwardTrainsTransiting;  //numeri di treni che stanno transitando in stazione, verso andata
	int returnTrainsTransiting;  //numeri di treni che stanno transitando in stazione, verso ritorno

	//costanti
	static constexpr int RAILS_PER_SIDE = 3;
	static constexpr int TRANSIT_INDEX = 2;  //la stazione secondaria ha tre binari: il terzo e' di transito; indice = 2
};

#endif  //Station_h   
#endif  //Park_h



