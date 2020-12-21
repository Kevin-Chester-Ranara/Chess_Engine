//#include<bit>
#include<iostream>
#include <random>
#include <string>
#include <stdlib.h>
#include "Move.h"
#define U64 unsigned long long
int sq64[64];
int sq120[120];
U64 SetMask[64];
U64 ClearMask[64];
void SetArray()
{
	int k = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			int num = (21 + j) + (i * 10);
			sq120[k] = num;
			sq64[num] = k;
			k++;
		}
	}
}
const int table[64] = { 63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
		51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
		26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
		58, 20, 37, 17, 36, 8
};
int pop_bit(U64* bb)
{
	U64 b = *bb ^ (*bb - 1);
	unsigned int fold = (unsigned)((b & 0xffffffff) ^ (b >> 32));
	*bb &= (*bb - 1);
	unsigned int folding = (fold * 0x783a9b23) >> 26;
	std::cout << folding << std::endl;
	return table[(fold * 0x783a9b23) >> 26];
}
int CountBit(U64 bb)
{
	int r = 0;
	for (; bb; r++, bb &= (bb - 1))
	{
	}

	return r;
}
void Initia()
{
	for (int index = 0; index < 64; index++)
	{
		SetMask[index] = 0ULL;
		ClearMask[index] = 0ULL;
	}
	for (int index = 0; index < 64; index++)
	{
		SetMask[index] |= (1ULL << index);
		ClearMask[index] = ~SetMask[index];
	}
}

void print_board(U64 bb)
{
	U64 shift = 1ULL;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			int num = (21 + j) + (i * 10);
			U64 tt = shift << sq64[num];
			if (tt & bb)
			{
				std::cout << "x";
			}
			else
			{
				std::cout << "-";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
U64 RandomGen()
{
	/**std::random_device rd;
	std::default_random_engine rng(rd());
	std::uniform_int_distribution<unsigned long long> dist(0, 0xffffffffffffffff);
	return dist(rng);**/
	return (U64)std::rand() | (((U64)std::rand()) << 15) | (((U64)std::rand()) << 30) | (((U64)std::rand()) << 45) | (((U64)std::rand & 0xf) << 60);
}

int main()
{
	/**SetArray();
	U64 bb = 1ULL << 11;
	bb |= 1ULL << 25;
	bb |= 1ULL << 13;
	bb |= 1ULL << 1;

	print_board(bb);
	std::cout << CountBit(bb) << std::endl;
	std::cout << pop_bit(&bb) << std::endl;
	print_board(bb);
	Initia();
	/**for (int i = 0; i < 64; i++)
	{
		std::cout << i << std::endl;
		print_board(SetMask[i]);
		//std::cout << sq64[i] << std::endl;
	}
	std::cout << RandomGen() << std::endl;
	BoardStruct s;
	s.SetArray();**/
	const char* ppw = "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 2";
	const char* ppb = "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1";
	/**s.FENparsing(pp);
	s.UpdateBoard();
	for (int i = 7; i >= 0; i--)
	{
		for (int j = 0; j <= 7; j++)
		{
			int num = s.sq64[i * 8 + j];
			std::cout << s.pieces[num] << " ";
		}
		std::cout << std::endl;
	}
	s.PrintBoard();
	print_board(s.pawns[0]);**/
	BoardStruct bb;
	bb.FENparsing(ppb);
	bb.UpdateBoard();
	bb.CheckBoard();
	Moves m(bb);
	//m.AddWhitePawn();
	m.AddBlackPawn();
	m.PrintMovelist();
	return 0;
}