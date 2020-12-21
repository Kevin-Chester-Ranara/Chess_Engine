#pragma once
/*
* hexadecimal
*  a full four occupied bit (1111) is F and empty bit is 0
* (10>) designated by numeric number (>10) ten designated by letters
* ex. 0x8F ->	(1000 1111) | 0xBF -> (1011 1111) | 0x70 -> (0111 0000)
*  we need 7 bit to describe the 21 to 98 squares  (1 2 4 8 16 32 64)-> (0111 1111)
*  we also need  4 bit to describe the capture pieces since pieces are numbered 0 to 13 (1 2 4 8) -> (1111)
ex.
							
* 0000 0000 0000 0000 0000 0111 1111 -> 0x7f (FROMSQ)
* 0000 0000 0000 0011 1111 1000 0000 ->   TO >> 7 TOSQ
* 0000 0000 0011 1100 0000 0000 0000 -> CAPTURE >> 14, 0xF
* 0000 0000 0100 0000 0000 0000 0000 ->ENPAS 0x40000
* 0000 0000 1000 0000 0000 0000 0000 -> PAWN START 0x80000
* 0000 1111 0000 0000 0000 0000 0000 -> PROMOTED PIECE-> 0xF
* 0001 0000 0000 0000 0000 0000 0000 -> CASTLE 0x1000000
*/
#include "BoardStruct.h"
#include <string>
#include <vector>
#include <iostream>
#include <vector>
#define FROMSQ(m) ((m) & 0x7f)
#define TOSQ(m) ((m>>7) & 0x7f)
#define CAPTURE(m) ((m >> 14) & 0xf)
#define PROMOTED(m) ((m >> 20) & 0xf)
#define ENPASSMOVE 0x40000
#define PAWNSTARTMOVE 0x80000
#define MOVECASTLE 0x1000000
#define MOVECAPTURE 0x7C000 //combine capture and enpas
#define MOVEPROMOTION 0xF00000
#define MOVE(from,to,capture,promoted,flag) ((from) | (to<<7) | (capture<<14) | (promoted<<20) | flag)

class Moves
{
public:
	class PieceMove
	{
	public:
		PieceMove(int move, int score, BoardStruct& pos)
			:
			move(move),
			score(score),
			pos(pos)
		{
		}
		char* printsqmove();
		char* printsq(int sq); //returns file and rank ex. b1
	public:
		int move;
		int score;
		BoardStruct& pos;
	};
	Moves(BoardStruct& pos)
		:
		pos(pos),
		count(0)
	{}
	void AddMove(int from, int to, int capture, int Promoted, int flag);
	void AddCaptureMove(int from, int to, int capture, int Promoted, int flag);
	void AddEnPassMove(int from, int to, int capture, int Promoted, int flag);
	void AddWhitePawn();
	void AddMoveSlider(); //bishop rook,queen
	void AddMoveNonSlider(); // knight,king
	void AddBlackPawn();
	void PrintMovelist();
public:
	BoardStruct& pos;
	std::vector<PieceMove> list;
	int count;
	std::vector<bool> SliderPiece = { false,false,true,false,true,true,false,false,true,false,true,true,false };
	std::vector<std::vector<int>> piecedir =
	{
		{0},
		{0},
		{-1,-10,1,10},
		{-8,-19,-21,-12,8,19,21,12},
		{-9,-11,11,9},
		{-1,-10,-9,-11,1,10,11,9},
		{-1,-10,-9,-11,1,10,11,9},
		{0},
		{-1,-10,1,10},
		{-8,-19,-21,-12,8,19,21,12},
		{-9,-11,11,9},
		{-1,-10,-9,-11,1,10,11,9},
		{-1,-10,-9,-11,1,10,11,9}
	};
};
