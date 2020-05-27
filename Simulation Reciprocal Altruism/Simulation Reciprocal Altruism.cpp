// Simulation Reciprocal Altruism.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Simulation Reciprocal Altruism.h"
#include<time.h>


int main()
{
	//Requests starting parameters of simulation.
	std::cout << "Insert the amount of starting cooperators \n";
	std::cin >> InitAmountCoop;
	std::cout << " \n" << "Insert the amount of starting Tit-for-tatters \n";
	std::cin >> InitAmountTFT;
	std::cout << " \n" << "Insert the amount of starting Deflectors \n";
	std::cin >> InitAmountDefl;
	std::cout << " \n" << "Finally, how many rounds should this simulation run for? \n";
	std::cin >> AmountRounds;

	Agents.reserve(10*(InitAmountCoop + InitAmountDefl + InitAmountTFT));
	//Initializes Agents and AliveAgents
	//Seems to work
		for (int i = 0; i < InitAmountCoop; ++i)
		{
			
			Agents.push_back(Agent(Agent::Strategy::Cooperator));
			AliveAgents.push_back(&Agents[i]);

		}
		int arraySize = Agents.size();
		for (int i = 0; i < InitAmountDefl; ++i)
		{
			Agents.push_back(Agent(Agent::Strategy::Deflector));
			AliveAgents.push_back(&Agents[i + arraySize]);
		}
		arraySize = Agents.size();
		for (int i = 0; i < InitAmountTFT; ++i)
		{
			Agents.push_back(Agent(Agent::Strategy::TFT));
			AliveAgents.push_back(&Agents[i + arraySize]);
		}
	

	//Round-Organizer
	for (int Round = 0; Round < AmountRounds; ++Round)
	{
		//Handles meetings
		for (int i = 0; i < std::floor(AliveAgents.size()*nMeetingsProportion); ++i)
		{
			//FIX THIS
			std::uniform_int_distribution<std::mt19937::result_type> distAlive(0, AliveAgents.size() - 1); // distribution in range [0, to top of vector]

			unsigned int FirstCandidateNumber = distAlive(rng);
			unsigned int SecondCandidateNumber = distAlive(rng);

			while (FirstCandidateNumber == SecondCandidateNumber)
			{
				SecondCandidateNumber = distAlive(rng);
			}

			bool WillFirstCoop = (AliveAgents[FirstCandidateNumber]->WillCooperate(AliveAgents[SecondCandidateNumber]));
			bool WillSecondCoop = (AliveAgents[SecondCandidateNumber]->WillCooperate(AliveAgents[FirstCandidateNumber]));
			//The actual meeting
			switch (WillFirstCoop)
			{
			case true:
				if (WillSecondCoop == true)
				{
					AliveAgents[FirstCandidateNumber]->Score += CooperateValue;
					AliveAgents[SecondCandidateNumber]->Score += CooperateValue;
				}
				else 
				{
					AliveAgents[FirstCandidateNumber]->Score += LoserValue;
					AliveAgents[SecondCandidateNumber]->Score += WinnerValue;
				}
				break;
			case false:
				if (WillSecondCoop == true)
				{
					AliveAgents[FirstCandidateNumber]->Score += WinnerValue;
					AliveAgents[SecondCandidateNumber]->Score += LoserValue;
				}
				else
				{
					AliveAgents[FirstCandidateNumber]->Score += DeflectorValue;
					AliveAgents[SecondCandidateNumber]->Score += DeflectorValue;
				}
				break;
			}
			//Remembering system for TFTs
			if (AliveAgents[FirstCandidateNumber]->GetStrategy() == Agent::Strategy::TFT)
			{
				switch (WillSecondCoop)
				{
				case true:
					AliveAgents[FirstCandidateNumber]->RemoveNaughty(AliveAgents[SecondCandidateNumber]);
				case false:
					AliveAgents[FirstCandidateNumber]->AddNaughty(AliveAgents[SecondCandidateNumber]);
				}
			}
			if (AliveAgents[SecondCandidateNumber]->GetStrategy() == Agent::Strategy::TFT)
			{
				switch (WillFirstCoop)
				{
				case true:
					AliveAgents[SecondCandidateNumber]->RemoveNaughty(AliveAgents[FirstCandidateNumber]);
				case false:
					AliveAgents[SecondCandidateNumber]->AddNaughty(AliveAgents[FirstCandidateNumber]);
				}
			}
		}
		//Handles deaths
		for (unsigned int i = 0; i < AliveAgents.size(); ++i)
		{
			if (AliveAgents[i]->GetScore() <= DeathScore)
			{
				//swaps that pointer with last one
				std::swap(AliveAgents[i], AliveAgents[AliveAgents.size() - 1]);
				AliveAgents.pop_back();

			}
		}

		//Handles reproductions
		for (unsigned int i = 0; i < AliveAgents.size(); ++i)
		{
			if (AliveAgents[i]->GetScore() > ReproduceScore)
			{
				unsigned int TempSize = Agents.size();
				AliveAgents[i]->Score = Agent::StartScore;
				Agent agent = AliveAgents[i]->ReturnCopy();
				Agents.push_back(agent);
				AliveAgents.push_back(&Agents[TempSize]);
			}
		}

		//Tally different types and output them
		int AmountCoop = 0;
		int AmountDefl = 0;
		int AmountTFT = 0;
		int AmountBogus = 0;
		for (unsigned int i = 0; i < AliveAgents.size(); ++i)
		{
			switch ((*AliveAgents[i]).GetStrategy())
			{
			case Agent::Strategy::Cooperator:
				AmountCoop++;

			case Agent::Strategy::Deflector:
				AmountDefl++;

			case Agent::Strategy::TFT:
				AmountTFT++;

			case Agent::Strategy::Bogus:
				AmountBogus++;

			}


		}
		std::cout << AmountCoop << "/" << AmountTFT << "/" << AmountDefl << "/" << AmountBogus << "\n";
	}
	std::cout << "Thank you for using our simulation";
	int WaitForInput;
	std::cin >> WaitForInput;
	return 0;
}

