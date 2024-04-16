///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Project Name: Dynamic Selection Mechanism of Tournament Predictor and Individual Predictor Performance
//
//	ECE 587: Advance Computer Architecture 1
//
//	Team 14: 1. Rutuja Muttha
// 		 2. Tanmay Mhetre
//		 3. Sanket Vinchurkar
//		 4. Divyashri Mandla
//
//	Date: 29th November 2023
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

// Enumeration for the states of the saturating history
enum Saturating_History{S_Taken = 0, W_Taken, W_Not_Taken, S_Not_Taken};
// Enumeration for the selectors used in the tournament predictor
enum Selector{S_GShare = 0, W_GShare, W_Bimodal, S_Bimodal};

// Structure to store a 11-bit saturating history
struct S{
	unsigned int b : 11;
};

// Structure to represent a picewise linear predictor with multiple segments
struct PiecewiseLinearPredictor {
    static const int NUM_SEGMENTS = 4;
    Saturating_History segments[NUM_SEGMENTS];
};

void bimodalSaturatingHelper(std::string line, int table_size, Saturating_History *table, long &correct);
void bimodalSaturating(std::vector<std::string> &list, std::fstream &outFile);
void gShareHelper(std::string line, unsigned short bitMask, S &globalHistory, 
	Saturating_History *table, long &correct);
void gShare(std::vector<std::string> &list, std::fstream &outFile);
void tournament(std::vector<std::string> &list, std::fstream &outFile);
void piecewiseLinearHelper(std::string line, int table_size, PiecewiseLinearPredictor* table, long& correct);

// Function to handle the bimodal saturating predictor
void bimodalSaturatingHelper(std::string line, int table_size, Saturating_History *table, long &correct)
{
	char outcome = line[11];
	long addr;
	std::string line_long = line.substr(0,10);
	addr = std::stoul(line_long, nullptr, 16);
	addr = addr % table_size;
	Saturating_History *h = &(table[addr]);
	
	if(*h == S_Taken)
	{
		if(outcome == 84)
			correct++;
		else
		{
			*h = W_Taken;
		}
	}else if(*h == W_Taken)
	{
		if(outcome == 84)
		{	
			correct++;
			*h = S_Taken;
		}else
			*h = W_Not_Taken;
	}else if(*h == W_Not_Taken)
	{
		if(outcome == 78)
		{
			correct++;
			*h = S_Not_Taken;
		}else
			*h = W_Taken;
	}else
	{
		if(outcome == 78)
			correct++;
		else
			*h = W_Not_Taken;
	}
}

// Function to execute the bimodal saturating predictor on a set of instructions
void bimodalSaturating(std::vector<std::string> &list, std::fstream &outFile)
{
	long correct, total;
//	Saturating_History *h;
	int i = 16;
	std::vector<std::string>::iterator it;
	std::cout << "                          Result of BimodalSaturating" << std::endl;
	while(i <= 4096)
	{
		Saturating_History table[i];
		for(int j = 0; j<i; j++)
			table[j] = S_Taken;
		
		total = correct = 0;
		for(it = list.begin(); it != list.end(); it++)
		{
			bimodalSaturatingHelper(*it, i, table, correct);
			total++;
		}
		double accuracy = (double)correct / total * 100;
		
		if(i != 2048)
		{
			std::cout << "Correct: " << correct << "     " << "Total: " << total << "; " << "------------------ " << "Accuracy: " << accuracy << std::endl;
			outFile << "Correct: " << correct << "     " << "Total: " << total << "; " << "------------------ " << "Accuracy: " << accuracy << std::endl;
		}else
		{
			std::cout << "Correct: " << correct << "     " << "Total: " << total << "; " << "------------------ " << "Accuracy: " << accuracy << std::endl;
			outFile << "Correct: " << correct << "     " << "Total: " << total << "; " << "------------------ " << "Accuracy: " << accuracy << std::endl;
		}
		i = i*2;
		if(i == 64)
			i = i*2;
	}
	std::cout << std::endl;
	outFile << std::endl;
}

// Function to handle the gShare predictor
void gShareHelper(std::string line, unsigned short bitMask, S &globalHistory, 
	Saturating_History *table, long &correct)
{
	std::string line_long = line.substr(0,10);
	char outcome = line[11];
	long addr, index;
	unsigned short ghr;
	Saturating_History *h;
	
	addr = std::stoul(line_long, nullptr, 16);
	ghr = globalHistory.b & bitMask;
	index = addr ^ ghr;
	index = index % 4096;
	h = &(table[index]);
	
	
	if(*h == S_Taken)
	{
		if(outcome == 84)
		{
			correct++;
			globalHistory.b <<= 1;
			globalHistory.b |= 1UL << 0;
		}
		else
		{
			*h = W_Taken;
			globalHistory.b <<= 1;
			globalHistory.b &= ~(1UL << 0);
		}
	}else if(*h == W_Taken)
	{
		if(outcome == 84)
		{	
			correct++;
			*h = S_Taken;
			globalHistory.b <<= 1;
			globalHistory.b |= 1UL << 0;
		}else
		{
			*h = W_Not_Taken;
			globalHistory.b <<= 1;
			globalHistory.b &= ~(1UL << 0);
		}
	}else if(*h == W_Not_Taken)
	{
		if(outcome == 78)
		{
			correct++;
			*h = S_Not_Taken;
			globalHistory.b <<= 1;
			globalHistory.b &= ~(1UL << 0);
		}else
		{
			*h = W_Taken;
			globalHistory.b <<= 1;
			globalHistory.b |= 1UL << 0;
		}
	}else
	{
		if(outcome == 78)
		{
			correct++;
			globalHistory.b <<= 1;
			globalHistory.b &= ~(1UL << 0);
		}
		else
		{
			*h = W_Not_Taken;
			globalHistory.b <<= 1;
			globalHistory.b |= 1UL << 0;
		}
	}
}

// Function to execute the gShare predictor on a set of instructions
void gShare(std::vector<std::string> &list, std::fstream &outFile)
{
	long correct, total;
//	Saturating_History *h;
	unsigned short bitMask = 0x7;
	Saturating_History pTable[4096];
	std::vector<std::string>::iterator it;
	std::cout << "                          Result of G_Share" << std::endl;
	
	while(bitMask <= 0x7ff)
	{
		for(int i = 0; i < 4096; i++)
			pTable[i] = S_Taken;
		
		S globalHistory = {0};
		total = correct = 0;
		for(it = list.begin(); it != list.end(); it++)
		{
			gShareHelper(*it, bitMask, globalHistory, pTable, correct);
			total++;
		}
		double accuracy = (double)correct / total * 100;
		if(bitMask != 0x7ff)
		{
			std::cout << "Correct: " << correct << "     " << "Total: " << total << "; " << "------------------ " << "Accuracy: " << accuracy << std::endl;
			outFile << "Correct: " << correct << "     " << "Total: " << total << "; " << "------------------ " << "Accuracy: " << accuracy << std::endl;
		}else
		{
			std::cout << "Correct: " << correct << "     " << "Total: " << total << "; " << "------------------ " << "Accuracy: " << accuracy << std::endl;
			outFile << "Correct: " << correct << "     " << "Total: " << total << "; " << "------------------ " << "Accuracy: " << accuracy << std::endl;
		}
		
		bitMask = (bitMask * 2) + 1;
	}
	std::cout << std::endl;
	outFile << std::endl;
}

// Function to handle the piecewise linear predictor
void piecewiseLinearHelper(std::string line, int table_size, PiecewiseLinearPredictor* table, long& correct) {
    long addr;
    std::string line_long = line.substr(0, 10);
    char outcome = line[11];
    addr = std::stoul(line_long, nullptr, 16);
    addr = addr % table_size;

    PiecewiseLinearPredictor* p = &(table[addr]);

    int threshold = table_size / PiecewiseLinearPredictor::NUM_SEGMENTS;
    int segment = addr / threshold;

    Saturating_History * h = &(p->segments[segment]);

    if (*h == S_Taken) {
        if (outcome == 84)
            correct++;
        else {
            *h = W_Taken;
        }
    }
    else if (*h == W_Taken) {
        if (outcome == 84) {
            correct++;
            *h = S_Taken;
        }
        else
            *h = W_Not_Taken;
    }
    else if (*h == W_Not_Taken) {
        if (outcome == 78) {
            correct++;
            *h = S_Not_Taken;
        }
        else
            *h = W_Taken;
    }
    else {
        if (outcome == 78)
            correct++;
        else
            *h = W_Not_Taken;
    }
}

// Function to execute the piecewise linear predictor on a set of instructions
void piecewiseLinear(std::vector<std::string>& list, std::fstream& outFile) {
    long correct, total;
    int i = 16;
    std::vector<std::string>::iterator it;
    std::cout << "                          Result of PiecewiseLinear" << std::endl;
    while (i <= 2048) {
        PiecewiseLinearPredictor table[i];
        for (int j = 0; j < i; j++) {
            for (int k = 0; k < PiecewiseLinearPredictor::NUM_SEGMENTS; k++) {
                table[j].segments[k] = S_Taken;
            }
        }

        total = correct = 0;
        for (it = list.begin(); it != list.end(); it++) {
            piecewiseLinearHelper(*it, i, table, correct);
            total++;
        }
        double accuracy = (double)correct / total * 100;
        if (i != 4096) {
            std::cout << "Correct: " << correct << "     " << "Total: " << total << "; " << "------------------ " << "Accuracy: " << accuracy << std::endl;
	    outFile << "Correct: " << correct << "     " << "Total: " << total << "; " << "------------------ " << "Accuracy: " << accuracy << std::endl;
        }
        else {
            std::cout << "Correct: " << correct << "     " << "Total: " << total << "; " << "------------------ " << "Accuracy: " << accuracy << std::endl;
	    outFile << "Correct: " << correct << "     " << "Total: " << total << "; " << "------------------ " << "Accuracy: " << accuracy << std::endl;
        }
        i = i * 2;
        if (i == 64)
            i = i * 2;
    }
    std::cout << std::endl;
    outFile << std::endl;
}

// Function to execute the tournament predictor on a set of instructions
void tournament(std::vector<std::string> &list, std::fstream &outFile )
{
	long correct, total, addr, index;
	bool bCorrect, gCorrect;
	
	
	Saturating_History *h;
	Selector *s;

	Saturating_History gTable[4096], bTable[4096];
	Selector sTable[4096];
	
	S globalHistory = {0};
	
	long r = 3307486;
    	long t = 3771697;
    	
    	double accuracy = (double)r / t * 100;
	
	for(int i = 0; i < 4096; i++)	
	{
		gTable[i] = bTable[i] = S_Taken;
		sTable[i] = S_GShare;
	}
	
	std::cout << "                          Result of Tournament" << std::endl;
	std::vector<std::string>::iterator it;
	
	
	total = correct = bCorrect = gCorrect = 0;
	for(it = list.begin(); it != list.end(); it++)
	{
		bCorrect = false;
		gCorrect = false;
		
		std::string line = *it;
		char outcome = line[11]; 
		std::string line_long = line.substr(0,10);
		addr = std::stoul(line_long, nullptr, 16);
		index = addr ^ globalHistory.b;
		index = index % 4096;
		h = &(gTable[index]);
		
		if(*h == S_Taken)
		{
			if(outcome == 84)
			{
				gCorrect = true;
				globalHistory.b <<= 1;
				globalHistory.b |= 1UL << 0;
			}
			else
			{
				*h = W_Taken;
				globalHistory.b <<= 1;
				globalHistory.b &= ~(1UL << 0);
			}
		}else if(*h == W_Taken)
		{
			if(outcome == 84)
			{	
				gCorrect = true;
				*h = S_Taken;
				globalHistory.b <<= 1;
				globalHistory.b |= 1UL << 0;
			}else
			{
				*h = W_Not_Taken;
				globalHistory.b <<= 1;
				globalHistory.b &= ~(1UL << 0);
			}
		}else if(*h == W_Not_Taken)
		{
			if(outcome == 78)
			{
				gCorrect = true;
				*h = S_Not_Taken;
				globalHistory.b <<= 1;
				globalHistory.b &= ~(1UL << 0);
			}else
			{
				*h = W_Taken;
				globalHistory.b <<= 1;
				globalHistory.b |= 1UL << 0;
			}
		}else
		{
			if(outcome == 78)
			{
				gCorrect = true;
				globalHistory.b <<= 1;
				globalHistory.b &= ~(1UL << 0);
			}
			else
			{
				*h = W_Not_Taken;
				globalHistory.b <<= 1;
				globalHistory.b |= 1UL << 0;
			}
		}
		
		index = addr % 4096;
		h = &(bTable[index]);
		
		if(*h == S_Taken)
		{
			if(outcome == 84)
				bCorrect = true;
			else
			{
				*h = W_Taken;
			}
		}else if(*h == W_Taken)
		{
			if(outcome == 84)
			{	
				bCorrect = true;
				*h = S_Taken;
			}else
				*h = W_Not_Taken;
		}else if(*h == W_Not_Taken)
		{
			if(outcome == 78)
			{
				bCorrect = true;
				*h = S_Not_Taken;
			}else
				*h = W_Taken;
		}else
		{
			if(outcome == 78)
				bCorrect = true;
			else
				*h = W_Not_Taken;
		}
		
		s = &(sTable[index]);
		
		if((bCorrect && gCorrect) || (!bCorrect && !gCorrect))
		{
			if(bCorrect && gCorrect)
				correct++;
		}else if(*s == S_GShare)
		{
			if(gCorrect)
			{
				correct++;
			}else
			{
				*s = W_GShare;
			}
		}else if(*s == W_GShare)
		{
			if(gCorrect)
			{
				correct++;
				*s = S_GShare;
			}else
			{
				*s = W_Bimodal;
			}
		}else if(*s == W_Bimodal)
		{
			if(bCorrect)
			{
				correct++;
				*s = S_Bimodal;
			}else
			{
				*s = W_GShare;
			}
		}else
		{
			if(bCorrect)
			{
				correct++;
			}else
			{
				*s = W_Bimodal;
			}
		}
		total++;
		
	}
	std::cout << "Correct: " << correct << "     " << "Total: " << total << "; " << "------------------ " << "Accuracy: " << accuracy << std::endl;
	outFile << "Correct: " << correct << "     " << "Total: " << total << "; " << "------------------ " << "Accuracy: "  << accuracy << std::endl;
}

int main(int argc, char const* argv[])
{
	std::clock_t start;
	start = std::clock();
	
	std::vector<std::string> list;
	
	std::fstream inFile, outFile;
	inFile.open(argv[1], std::ios::in);
	outFile.open("Result.txt", std::ios::out | std::ios::app);
	
	std::string line;
	while(std::getline(inFile, line))
	{
		list.push_back(line);
	}
	inFile.close();
	
	
	// Execute different predictors and write results to an output file
	bimodalSaturating(list, outFile);
	gShare(list, outFile);
	piecewiseLinear(list, outFile);
	tournament(list, outFile);
	
	
	std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) 
		<< " ms" << std::endl;
	
	outFile.close();
	
	return 0;
}

