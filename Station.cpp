/*
AUTORE: ALBU COSMIN ANDREI
*/

#include "Station.h"
#include <iostream>

Station::Rail::Rail()   //costruttore di default
{
	isTaken = false;
	trainOccuping = nullptr;
	direction = INVALID_DIRECTION;
	canHasOccupantTrain = false; //non so che binario sia quindi non posso metterci treni
}

Station::Rail::Rail(int _direction)
{
	isTaken = false;
	trainOccuping = nullptr;
	direction = _direction;
	canHasOccupantTrain = false; //non so che binario sia quindi non posso metterci treni
}

int Station::Rail::getDirection() const
{
	return direction;
}

bool Station::Rail::railTaken() const
{
	if (!canHasOccupantTrain)
	{
		throw RailCannotHaveOccupantTrain();
	}
	return isTaken;
}

const Train* Station::Rail::getOccupantTrain() const
{
	if (!canHasOccupantTrain)
	{
		throw RailCannotHaveOccupantTrain();
	}

	return trainOccuping;
}

void Station::Rail::setOccupantTrain(const Train* _trainOccuping)
{
	if (!canHasOccupantTrain)
	{
		throw RailCannotHaveOccupantTrain();
	}

	if (trainOccuping != nullptr)
	{
		throw MoreTrainsOnARailError();
	}

	trainOccuping = _trainOccuping;
	isTaken = true;
}

void Station::Rail::removeOccupantTrain()
{
	if (!canHasOccupantTrain)
	{
		throw RailCannotHaveOccupantTrain();
	}

	if (trainOccuping != nullptr)
	{
		trainOccuping = nullptr;
	}
	isTaken = false;
}

Station::StdRail::StdRail(int _direction) : Rail(_direction)
{
	canHasOccupantTrain = true;
}

Station::TransitRail::TransitRail(int _direction) : Rail(_direction)
{
	//intentionally left blank
}

Station::Station(std::string _stationIdentifier, int _frontDistance)
{
	stationIdentifier = _stationIdentifier;
	frontDistance = _frontDistance;
	backDistance = -1;  //-1 NON E' UN VALORE VALIDO 
	outwardPark = new Park;  //inizializzo un parcheggio vuoto, ossia senza treni: chiamo il costruttore di default Park()
	returnPark = new Park;  //inizializzo un parcheggio vuoto, ossia senza treni: chiamo il costruttore di default Park()
	outwardRails = nullptr;  //un'istanza di Station non ha binari. Station e' un concetto astratto che
	returnRails = nullptr;   //nel concreto si divide in MainStation e SecondaryStation
}

void Station::addInPark(const Train* train, int arrivingDirection)
{
	if (arrivingDirection != 0 && arrivingDirection != 1)  //non e' ne' andata ne' ritorno: non va bene.
	{
		throw WrongDirectionException();
	}
	else if (arrivingDirection == 0)   //andata
	{
		outwardPark->addTrain(const_cast<Train*>(train));
	}
	else  //arrivingDirection == 1   //ritorno
	{
		returnPark->addTrain(const_cast<Train*>(train));
	}
}

Station::~Station()
{
	delete outwardPark;
	outwardPark = nullptr;

	delete returnPark;
	returnPark = nullptr;

	if (outwardRails != nullptr)
	{
		delete[] outwardRails;
		outwardRails = nullptr;
	}
	if (returnRails != nullptr)
	{
		delete[] returnRails;
		returnRails = nullptr;
	}
}
