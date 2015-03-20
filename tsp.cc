#include "tsp.h"

// TSP
TSP::TSP(DistanceTable distances) 
{
	this->distances = distances;
	this->shortestDistance = 0.0;

	// init the current order
	for(unsigned int i = 0; i < distances.size(); i++)
	{
		this->currentOrder.push_back(i);
	}
}

// RANDOM INT FROM INTERVAL
int TSP::RandomIntFromInterval(int min, int max)
{
	return rand() % (max - min) + min;
}

// RANDOM DOUBLE FROM INTERVAL
double TSP::RandomDouble()
{
	return ((double)rand()/(double)(RAND_MAX)) * 1;
}

// GET TOTAL DISTANCE
double TSP::GetTotalDistance(Order order)
{
	double distance;

	for(unsigned int i = 0; i < order.size(); i++)
	{
		int a = order[i];
		int b = order[i + 1];
		distance += this->distances[a][b];
	}

	if(order.size() > 1)
	{
		int last = order[order.size() - 1];
		distance += this->distances[last][0];
	}

	return distance;
}

// GET NEXT ARRANGEMENT
Order TSP::GetNextArrangement(Order order)
{
	Order newOrder;

	for(unsigned int i = 0; i < order.size(); i++)
	{
		newOrder.push_back(i);
	}

	// we will only rearrange two points by random
	// starting point should always be zero - so zero should not be included
	int firstRandomIdx = this->RandomIntFromInterval(1, newOrder.size() - 2);
	int secondRandomIdx = this->RandomIntFromInterval(1, newOrder.size() - 2);

	int dummy = newOrder[firstRandomIdx];
	newOrder[firstRandomIdx] = newOrder[secondRandomIdx];
	newOrder[secondRandomIdx] = dummy;

	return newOrder; 
}

// ANNEAL
void TSP::Anneal()
{
	int iteration = -1;

	double temperature = 10000.0;
	double distanceDelta = 0.0;
	double coolingRate = 0.9999;
	double absoluteTemperature = 0.00001;

	double distance = this->GetTotalDistance(this->currentOrder);

	while(temperature > absoluteTemperature)
	{
		this->nextOrder = this->GetNextArrangement(this->currentOrder);
		distanceDelta = this->GetTotalDistance(this->nextOrder) - distance;

		// if the new order has a smaller distance
		// or if the new order has a larger distance but satisfies the Boltzman condition 
		// then accept the arrangement
		if((distanceDelta < 0) || (distanceDelta >= 0 && std::exp(-distanceDelta / temperature) > this->RandomDouble()))
		{
			for(unsigned int i = 0; i < this->nextOrder.size(); i++)
			{
				this->currentOrder[i] = this->nextOrder[i];
			}

			distance = distanceDelta + distance;
		}

		// cool down temperature
		temperature *= coolingRate;

		iteration++;
	}

	this->shortestDistance = distance;
}

// GET CURRENT ORDER
Order TSP::GetCurrentOrder()
{
	return this->currentOrder;
}

// GET SHORTEST DISTANCE
double TSP::GetShortestDistance()
{
	return this->shortestDistance;
}