/*
AUTORE: ALBU COSMIN ANDREI
*/

#include "Station.h"
#include <iostream>

SecondaryStation::SecondaryStation(std::string _stationIdentifier, int _frontDistance) : Station(_stationIdentifier, _frontDistance)
{
	outwardRails = new Rail[RAILS_PER_SIDE];
	for (int i = 0; i < RAILS_PER_SIDE - 1; i++)
	{
		outwardRails[i] = StdRail(OUTWARD_DIRECTION);      //i primi RAILS_PER_SIDE-1 binari sono standard...
	}
	outwardRails[RAILS_PER_SIDE - 1] = TransitRail(OUTWARD_DIRECTION);   //...l'ultimo binario e' di transito

	returnRails = new Rail[RAILS_PER_SIDE];
	for (int i = 0; i < RAILS_PER_SIDE - 1; i++)
	{
		returnRails[i] = StdRail(RETURN_DIRECTION);     //i primi RAILS_PER_SIDE-1 binari sono standard...
	}
	returnRails[RAILS_PER_SIDE - 1] = TransitRail(RETURN_DIRECTION);   //...l'ultimo binario e' di transito
}

void SecondaryStation::moveTrainsToRail(const int arrivingDirection)
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

	for (int i = 0; i < RAILS_PER_SIDE - 1; i++)
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

bool SecondaryStation::startRequest(const Train* train, int arrivingDirection)
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

	for (int i = 0; i < RAILS_PER_SIDE - 1; i++)  //RAILS_PER_SIDE-1 perche' l'ultimo binario e' di transito (uno per verso)
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

	//se arriva qui c'e' almeno un binario libero e quindi metto un treno dal parcheggio nel binario libero

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

int SecondaryStation::railRequest(const Train* train, int arrivingDirection)
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

	bool stop = false;

	if (Station::isType<TrainReg>(*train))
	{
		stop = true;
	}

	if (stop)  //il treno vuole fermarsi in stazione, e' di tipo treno regionale
	{
		bool freeRail = false;
		int indexFreeRail = -1;

		for (int i = 0; i < RAILS_PER_SIDE - 1; i++)  //RAILS_PER_SIDE-1 perche' l'ultimo binario e' di transito (uno per verso)
		{
			if (!consideredRails[i].railTaken())
			{
				freeRail = true;
				indexFreeRail = i;
				break;
			}
		}

		if (freeRail)  //c'e' almeno un binario libero
		{
			consideredRails[indexFreeRail].setOccupantTrain(train);
			return 0;
		}

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

	//!stop  : il treno vuole transitare in stazione 

	if (arrivingDirection == 0)
	{
		outwardTrainsTransiting++;  //sta transitando un treno in piu' verso andata
	}
	else //arrivingDirection!=1
	{
		returnTrainsTransiting++;  //sta transitando un treno in piu' verso ritorno
	}
	return 1;  //il treno deve transitare
}

void SecondaryStation::departure(const Train* train, int arrivingDirection)
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

	for (int i = 0; i < RAILS_PER_SIDE - 1; i++)  //RAILS_PER_SIDE-1 perche' l'ultimo binario e' di transito (uno per verso)
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
	//se il parcheggio non e' vuoto, chiedo un treno per metterlo nel binario appena liberato, a meno che non sia in una stazione capolinea

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

bool SecondaryStation::trainsAreTransiting(int arrivingDirection) const
{
	if (arrivingDirection != 0 && arrivingDirection != 1)  //non e' ne' andata ne' ritorno: non va bene.
	{
		throw WrongDirectionException();
	}
	else if (arrivingDirection == 0)   //andata
	{
		if (outwardTrainsTransiting == 0)
		{
			return false;  //non stanno transitando treni nel verso di andata
		}
		else if (outwardTrainsTransiting > 0)
		{
			return true;  //stanno transitando treni nel verso di andata
		}
		else
		{
			throw NumberTrainsTransitingNegativeError();
		}
	}
	else  //arrivingDirection == 1   //ritorno
	{
		if (returnTrainsTransiting == 0)
		{
			return false;  //non stanno transitando treni nel verso di andata
		}
		else if (returnTrainsTransiting > 0)
		{
			return true;  //stanno transitando treni nel verso di andata
		}
		else
		{
			throw NumberTrainsTransitingNegativeError();
		}
	}
}

void SecondaryStation::transitEnded(int arrivingDirection)
{
	if (arrivingDirection != 0 && arrivingDirection != 1)  //non e' ne' andata ne' ritorno: non va bene.
	{
		throw WrongDirectionException();
	}
	else if (arrivingDirection == 0)   //andata
	{
		outwardTrainsTransiting--;  //sta transitando un treno in meno verso andata
	}
	else  //arrivingDirection == 1   //ritorno
	{
		returnTrainsTransiting--;  ////sta transitando un treno in meno verso ritorno
	}
}

SecondaryStation::~SecondaryStation()
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
