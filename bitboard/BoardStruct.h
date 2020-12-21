#pragma once
#define U64 unsigned long long
#include <assert.h>
#include <random>
#include <iostream>
class BoardStruct
{
public:
	enum class Color
	{
		White,
		Black,
		Both
	};
	enum class Pieces
	{
		Empty, bP, bR, bN, bB, bQ, bK, wP, wR, wN, wB, wQ, wK
	};
	enum class Rank
	{
		r1,
		r2,
		r3,
		r4,
		r5,
		r6,
		r7,
		r8
	};
	enum class File
	{
		a,b,c,d,e,f,g,h
	};
	enum class Square
	{
		A1 = 21, B1, C1, D1, E1, F1, G1, H1,
		A2 = 31, B2, C2, D2, E2, F2, G2, H2,
		A3 = 41, B3, C3, D3, E3, F3, G3, H3,
		A4 = 51, B4, C4, D4, E4, F4, G4, H4,
		A5 = 61, B5, C5, D5, E5, F5, G5, H5,
		A6 = 71, B6, C6, D6, E6, F6, G6, H6,
		A7 = 81, B7, C7, D7, E7, F7, G7, H7,
		A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD
	};
	enum CastlePermission
	{
		WKCA = 1,
		WQCA = 2,
		BKCA = 4,
		BQCA = 8
	};
	const int BitTable[64] = 
	{
		63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
		51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
		26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
		58, 20, 37, 17, 36, 8
	};
	// array of tiles for piece movement
	int RookTiles[4] = { -1,-10,1,10 };
	int BishopTiles[4] = { -9,-11,9,11 };
	int KnightTiles[8] = { -8,-12,-19,-22,8,12,19,22 };
	int KingTiles[8] = { -1,-9,-11,-10,1,9,11,10 };
	BoardStruct()
	{
		SetArray();
		InitFileRankBoard();
		ResetBoard();
		InitHashKeys();
	}
	void InitFileRankBoard();
	void Settingbit(U64* bb,int sq64);
	void UpdateBoard();
	bool CheckBoard();
	void SetArray();
	void ResetBoard();
	U64 RandomGenerator();
	void InitHashKeys();
	int PopBit(U64* bb);
	int CountBit(U64 bb);
	bool DoesKnightAtt(int sq); 
	U64 GeneratePositionKey();
	int FENparsing(const char *str);
	bool IsKnight(int piece);
	bool IsRookorQueen(int piece);
	bool IsBishopOrQueen(int piece);
	void PrintBoard();
public:
	const static int squarenum = 120;
	int FileSq[squarenum];
	int RankSq[squarenum];
	int sq64[64]; // number of squares
	int sq120[squarenum]; // number of squares plus the offboard
	int pieces[squarenum]; //represents the board 0-120 and also the pieces in it 
	U64 pawns[3];
	int KingSq[2];
	int dummy;
public:
	int enpas;
	int fiftymove;
	int ply;
	int hisply;
	int side;
	int castleperm;
	U64 piecekeys[13][squarenum];
	U64 sidekey;
	U64 castlekey[16];
	U64 poskey;
	U64 SetMask[64];
	U64 ClearMask[64];
	int piecenum[13]; // array of pieces (so we can track the number of each piece) /  number of each piece left in the board 
	int bigpiece[2]; // array of big pieces (all pieces except pawn) only 2 index since only colour black and white
	int majpiece[2]; //array of major piece (rook, queen, king)
	int minpiece[2]; //(knight , bishop)
	int material[2]; //score
	int piecelist[13][10]; // array used to track the pieces location on the board (will be used to update the board);
	//10 is the number of pieces that can be promoted from a pawn
	// ex. piecelist[bQ][2] = a4; (a 2nd black Queen is in a4)
	static constexpr bool bigpieceb[13] = { false,false,true,true,true,true,true,false,true,true,true,true,true };
	static constexpr bool majpieceb[13] = { false,false,true,false,false,true,true,false,true,false,false,true,true };
	static constexpr bool minpieceb[13] = { false,false,false,true,true,false,false,false,false,true,true,false,false };
	static constexpr bool pawnpiece[13] = { false,true,false,false,false,false,false,true,false,false,false,false,false };
	Color ofpieces[13] = { Color::Both,Color::Black,Color::Black,Color::Black,Color::Black,Color::Black,Color::Black,
		Color::White,Color::White,Color::White,Color::White,Color::White,Color::White};
	int pieceval[13] = { 0,100,500,325,325,1000,50000,100,500,325,325,1000,50000 };
	const char* piecechar = ".PRNBQKprnbqk";
	const char* sidechar = "wb-";
	const char* rankchar = "12345678";
	const char* filechar = "abcdefgh";
};