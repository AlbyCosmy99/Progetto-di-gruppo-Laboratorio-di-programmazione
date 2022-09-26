/*
AUTORE: ALBU COSMIN ANDREI
*/

#include "Station.h"
#include <iostream>

MainStation::MainStation(std::string _stationIdentifier, int _frontDistance) : Station(_stationIdentifier, _frontDistance)
{
	outwardRails = new Rail[RAILS_PER_SIDE];
	for (int i = 0; i < RAILS_PER_SIDE; i++)
	{
		outwardRails[i] = StdRail(OUTWARD_DIRECTION);
	}
	returnRails = new Rail[RAILS_PER_SIDE];
	for (int i = 0; i < RAILS_PER_SIDE; i++)
	{
		returnRails[i] = StdRail(RETURN_DIRECTION);
	}
}

void MainStation::moveTrainsToRail(const int arrivingDirection)
{
	Rail* consideredRails = nullptr;

	if (arrivingDirection != 0 && arrivingDirection != 1)  //non e' ne' andata ne' ritorno: non va bene.
	{
		throw WrongDirectionException();
	}
	else if (arrivingDirection == 0)   //andata
	{
		consideredRails = outwardRails;
	}
	else  //arrivingDirection == 1   //ritorno
	{
		consideredRails = returnRails;
	}

	bool freeRail = false;
	int indexFreeRail = -1;

	for (int i = 0; i < RAILS_PER_SIDE; i++)
	{
		if (!consideredRails[i].railTaken())
		{
			freeRail = true;
			indexFreeRail = i;
			break;
		}
	}

	if (freeRail)
	{
		Park* consideredPark = nullptr;

		if (arrivingDirection == 0)   //andata
		{
			consideredPark = outwardPark;
		}
		else  //arrivingDirection == 1   //ritorno
		{
			consideredPark = returnPark;
		}

		if (!consideredPark->isEmpty())//se frontDistance==0 o backDistance==0 allora e' una stazione capolinea
		{
			if (consideredPark->firstTrainInPark())
			{
				Train* parkTrain = consideredPark->giveNextTrain();
				consideredRails[indexFreeRail].setOccupantTrain(parkTrain);  //metto il primo treno del parcheggio in stazione
				parkTrain->start();   //il treno esce dal parcheggio verso la stazione
			}
		}
	}
}

bool MainStation::startRequest(const Train* train, int arrivingDirection)
{
	Rail* consideredRails = nullptr;

	if (arrivingDirection != 0 && arrivingDirection != 1)  //non e' ne' andata ne' ritorno: non va bene.
	{
		throw WrongDirectionException();
	}
	else if (arrivingDirection == 0)   //andata
	{
		consideredRails = outwardRails;
	}
	else  //arrivingDirection == 1   //ritorno
	{
		consideredRails = returnRails;
	}

	bool freeRail = false;
	int indexFreeRail = -1;

	for (int i = 0; i < RAILS_PER_SIDE; i++)
	{
		if (!consideredRails[i].railTaken())
		{
			freeRail = true;
			indexFreeRail = i;
			break;
		}
	}

	if (!freeRail)
	{
		return false;  //non ci sono binari liberi
	}

	//se arriva qui c'e' almeno un binario libero e quindi metto il treno richiesto dal parcheggio nel binario libero

	Park* consideredPark = nullptr;

	if (arrivingDirection == 0)   //andata
	{
		consideredPark = outwardPark;
	}
	else  //arrivingDirection == 1   //ritorno
	{
		consideredPark = returnPark;
	}

	consideredPark->removeTrain(const_cast<Train*>(train));   //rimuovo il treno dal parcheggio
	consideredRails[indexFreeRail].setOccupantTrain(train);  //metto il treno su un binario libero in stazione
	return true;  //ho messo un treno dal parcheggio in un binario in stazione
}

bool MainStation::trainsAreTransiting(int arrivingDirection) const
{
	throw TrainsCannotTransitInThisStationError();//in una stazione principale non possono transitare treni
}

void MainStation::transitEnded(int arrivingDirection)
{
	throw TrainsCannotTransitInThisStationError();//in una stazione principale non possono transitare treni
}

int MainStation::railRequest(const Train* train, int arrivingDirection) //funzione chiamata quando il treno richiede un binario
{
	//tutti i treni si fermano in stazione

	Rail* consideredRails = nullptr;

	if (arrivingDirection != 0 && arrivingDirection != 1)  //non e' ne' andata ne' ritorno: non va bene.
	{
		throw WrongDirectionException();
	}
	else if (arrivingDirection == 0)   //andata
	{
		consideredRails = outwardRails;
	}
	else  //arrivingDirection == 1   //ritorno
	{
		consideredRails = returnRails;
	}

	bool freeRail = false;
	int indexFreeRail = -1;

	for (int i = 0; i < RAILS_PER_SIDE; i++)
	{
		if (!consideredRails[i].railTaken())
		{
			freeRail = true;
			indexFreeRail = i;
			break;
		}
	}

	if (freeRail)
	{
		consideredRails[indexFreeRail].setOccupantTrain(train);
		return 0;
	}

	//se arriva qui non ci sono binari liberi e quindi metto il treno in parcheggio

	Park* consideredPark = nullptr;

	if (arrivingDirection == 0)   //andata
	{
		consideredPark = outwardPark;
	}
	else  //arrivingDirection == 1   //ritorno
	{
		consideredPark = returnPark;
	}

	consideredPark->addTrain(const_cast<Train*>(train));
	return 2;
}

void MainStation::departure(const Train* train, int arrivingDirection)
{
	Rail* consideredRails = nullptr;
	bool removed = false;
	int indexFreeRail = -1;

	if (arrivingDirection != 0 && arrivingDirection != 1)  //non e' ne' andata ne' ritorno: non va bene.
	{
		throw WrongDirectionException();
	}
	else if (arrivingDirection == 0)   //andata
	{
		consideredRails = outwardRails;
	}
	else  //arrivingDirection == 1   //ritorno
	{
		consideredRails = returnRails;
	}

	for (int i = 0; i < RAILS_PER_SIDE; i++)
	{
		if (consideredRails[i].getOccupantTrain() != nullptr && *(consideredRails[i].getOccupantTrain()) == *train)//operator== tra const Train
		{
			consideredRails[i].removeOccupantTrain();
			removed = true;
			indexFreeRail = i;
			break;
		}
	}

	if (!removed)  //se il treno non e' stato rimosso dai binari vuol dire che il treno passato come argomento non e' presente in stazione
	{
		throw WrongTrainException();
	}

	//se arriva qui, il treno e' stato rimosso dai binari
	//se il parcheggio non e' vuoto, chiedo un treno per metterlo nel binario appena liberato

	Park* consideredPark = nullptr;

	if (arrivingDirection == 0)   //andata
	{
		consideredPark = outwardPark;
	}
	else  //arrivingDirection == 1   //ritorno
	{
		consideredPark = returnPark;
	}

	if (!consideredPark->isEmpty() && (!(frontDistance == 0 && arrivingDirection == 0) || !(backDistance == 0 && arrivingDirection == 1)))//se frontDistance==0 o backDistance==0 allora e' una stazione capolinea
	{
		if (consideredPark->firstTrainInPark())
		{
			Train* parkTrain = consideredPark->giveNextTrain();
			consideredRails[indexFreeRail].setOccupantTrain(parkTrain);  //metto il primo treno del parcheggio in stazione
			parkTrain->start();   //il treno esce dal parcheggio verso la stazione
		}
	}
}

MainStation::~MainStation()
{
	delete outwardPark;
	outwardPark = nullptr;

	delete returnPark;
	returnPark = nullptr;

	delete[] outwardRails;
	outwardRails = nullptr;

	delete[] returnRails;
	returnRails = nullptr;
}
