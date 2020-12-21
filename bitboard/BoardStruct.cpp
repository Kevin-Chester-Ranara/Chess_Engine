#include "BoardStruct.h"

void BoardStruct::InitFileRankBoard()
{
	for (int i = 0; i < squarenum; i++)
	{
		FileSq[i] = (int)Square::OFFBOARD;
		RankSq[i] = (int)Square::OFFBOARD;
	}
	for (int i = (int)Rank::r8; i >= (int)Rank::r1; i--)
	{
		for (int j = (int)File::a; j <= (int)File::h; j++)
		{
			int sq = (21 + j) + (i * 10);
			FileSq[sq] = j;
			RankSq[sq] = i;
		}
	}
}

void BoardStruct::Settingbit(U64* bb,int sq64)
{
	*bb |= SetMask[sq64];
}

void BoardStruct::UpdateBoard()
{
	for (int i = 0; i < squarenum; i++)
	{
		int p = pieces[i];
		if(p!=(int)Pieces::Empty && p!=(int)Square::OFFBOARD)
		{
			Color c = ofpieces[p];
			if (bigpieceb[p] == true)
			{
				bigpiece[(int)c]++;
			}
			if (majpieceb[p] == true)
			{
				majpiece[(int)c]++;
			}
			if (minpieceb[p] == true)
			{
				minpiece[(int)c]++;
			}
			material[(int)c] += pieceval[p];
			piecelist[p][piecenum[p]] = i;
			piecenum[p]++;

			if (p == (int)Pieces::wK || p == (int)Pieces::bK)
			{
				KingSq[(int)c] = i;
			}
			if (p == (int)Pieces::wP)
			{
				Settingbit(&pawns[(int)c], sq120[i]);
				Settingbit(&pawns[int(Color::Both)], sq120[i]);
			}
			else if (p == (int)Pieces::bP)
			{
				Settingbit(&pawns[(int)c], sq120[i]);
				Settingbit(&pawns[int(Color::Both)], sq120[i]);
			}


		}
	}
}

bool BoardStruct::CheckBoard()
{
	int temp_piecenum[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };
	int temp_bigpiece[2] = { 0,0 };
	int temp_majpiece[2] = { 0,0 };
	int temp_minpiece[2] = { 0,0 };
	int temp_material[2] = { 0,0 };
	U64 temp_pawns[3] = { 0ULL,0ULL,0ULL };

	temp_pawns[(int)Color::White] = pawns[(int)Color::White];
	temp_pawns[(int)Color::Black] = pawns[(int)Color::Black];
	temp_pawns[(int)Color::Both] = pawns[(int)Color::Both];

	int sq120s = 0;
	for (int i = (int)Pieces::bP; i <= (int)Pieces::wK; i++)
	{
		for (int j = 0; j < piecenum[i]; j++)
		{
			sq120s = piecelist[i][j];
			assert(pieces[sq120s] == i);
		}
	}
	for (int i = 0; i < 64; i++)
	{
		Color c;
		sq120s = sq64[i];
		int tpiece = pieces[sq120s];
		temp_piecenum[tpiece]++;
		//checking number of pieces in each set
		if (bigpieceb[tpiece] == true)
		{
			c = ofpieces[tpiece];
			temp_bigpiece[(int)c]++;
		}
		if (majpieceb[tpiece] == true)
		{
			c = ofpieces[tpiece];
			temp_majpiece[(int)c]++;
		}
		if (minpieceb[tpiece] == true)
		{
			c = ofpieces[tpiece];
			temp_minpiece[(int)c]++;
		}
		if (pawnpiece[tpiece] == true)
		{
			c = ofpieces[tpiece];
		}
		else if (tpiece == 0)
		{
			continue;
		}
		temp_material[(int)c] += pieceval[tpiece];
	}
	//check number of pieces
	for (int i = (int)Pieces::bP; i <= (int)Pieces::wK; i++)
	{
		assert(temp_piecenum[i] == piecenum[i]);
	}
	int pcount = 0;
	pcount = CountBit(pawns[(int)Color::White]);
	assert(pcount == piecenum[(int)Pieces::wP]);
	pcount = CountBit(pawns[(int)Color::Black]);
	assert(pcount == piecenum[(int)Pieces::bP]);
	pcount = CountBit(pawns[(int)Color::Both]);
	int k = (piecenum[(int)Pieces::wP] + piecenum[(int)Pieces::bP]);
	assert(pcount == (piecenum[(int)Pieces::wP] + piecenum[(int)Pieces::bP]));

	while (temp_pawns[(int)Color::White])
	{
		sq120s = PopBit(&temp_pawns[(int)Color::White]);
		assert(pieces[sq64[sq120s]] == (int)Pieces::wP);
	}
	while (temp_pawns[(int)Color::Black])
	{
		sq120s = PopBit(&temp_pawns[(int)Color::Black]);
		assert(pieces[sq64[sq120s]] == (int)Pieces::bP);
	}
	while (temp_pawns[(int)Color::Both])
	{
		sq120s = PopBit(&temp_pawns[(int)Color::Both]);
		assert(pieces[sq64[sq120s]] == (int)Pieces::wP || pieces[sq64[sq120s]] == (int)Pieces::bP);
	}

	assert(temp_material[(int)Color::White] == material[(int)Color::White] && temp_material[(int)Color::Black] == material[(int)Color::Black]);
	assert(temp_bigpiece[(int)Color::White] == bigpiece[(int)Color::White] && temp_bigpiece[(int)Color::Black] == bigpiece[(int)Color::Black]);
	assert(temp_majpiece[(int)Color::White] == majpiece[(int)Color::White] && temp_majpiece[(int)Color::Black] == majpiece[(int)Color::Black]);
	assert(temp_minpiece[(int)Color::White] == minpiece[(int)Color::White] && temp_minpiece[(int)Color::Black] == minpiece[(int)Color::Black]);

	assert(side == (int)Color::White || side == (int)Color::Black);
	assert(pieces[KingSq[(int)Color::White]] == (int)Pieces::wK && pieces[KingSq[(int)Color::Black]] == (int)Pieces::bK);
	assert(GeneratePositionKey() == poskey);

	return true;
}

void BoardStruct::SetArray()
{
	for (int i = 0; i < squarenum; i++)
	{
		sq120[i] = i;
	}
	for (int i = 0; i < 64; i++)
	{
		sq64[i] = i;
		SetMask[i] = 0ULL;
		ClearMask[i] = 0ULL;
	}
	for (int i = 0; i < 64; i++)
	{
		SetMask[i] |= 1ULL << i;
		ClearMask[i] = ~SetMask[i];
	}
	int count = 0;
	for (int i = (int)File::a; i <= (int)File::h; i++)
	{
		for (int j = (int)Rank::r1; j <= (int)Rank::r8; j++)
		{
			int num = (21 + j) + (i * 10);
			sq120[num] = count;// array with offboard
			sq64[count] = num;
			count++;
		}
	}
}

void BoardStruct::ResetBoard()
{
	int index = 0;
	for (int i = 0; i < squarenum; i++)
	{
		pieces[i] = (int)Square::OFFBOARD;
	}
	for (int i = 0; i < 64; i++)
	{
		pieces[sq64[i]] = (int)Pieces::Empty;
	}

	for (int i = 0; i < 2; i++)
	{
		bigpiece[i] = 0;
		minpiece[i] = 0;
		majpiece[i] = 0;
		material[i] = 0;
		pawns[i] = 0ULL;
	}
	for (int i = 0; i < 13; i++)
	{
		piecenum[i] = 0;
	}
	side = (int)Color::Both;
	KingSq[0] = 0;
	KingSq[1] = 0;
	enpas = (int)Square::NO_SQ;
	fiftymove = 0;
	ply = 0;
	hisply = 0;;
	poskey = 0ULL;

}

U64 BoardStruct::RandomGenerator()
{
	std::random_device rd;
	std::default_random_engine rng(rd());
	std::uniform_int_distribution<unsigned long long> dist(0, 0xffffffffffffffff);
	return dist(rng);
}

void BoardStruct::InitHashKeys()
{
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < squarenum; j++)
		{
			piecekeys[i][j] = RandomGenerator();
		}
	}
	sidekey = RandomGenerator();

	for (int i = 0; i < 16; i++)
	{
		castlekey[i] = RandomGenerator();
	}

	
}

int BoardStruct::PopBit(U64* bb)
{
	U64 b = *bb ^ (*bb - 1);
	unsigned int fold = (unsigned)((b & 0xffffffff) ^ (b >> 32));
	*bb &= (*bb - 1);
	return BitTable[(fold * 0x783a9b23) >> 26];
}

int BoardStruct::CountBit(U64 bb)
{
	int c = 0;
	for (; bb; c++, bb &= (bb - 1));
	return c;
}

bool BoardStruct::DoesKnightAtt(int sq)
{
	for (int i = 0; i < 8; i++)
	{
		if (side == (int)Color::White)
		{
			if (pieces[sq + KnightTiles[i]] == (int)Pieces::bN)
			{
				return true;
			}
		}
		else if (side == (int)Color::Black)
		{
			if (pieces[sq + KnightTiles[i]] == (int)Pieces::wN)
			{
				return true;
			}
		}
	}
	return false;
}

U64 BoardStruct::GeneratePositionKey()
{
	U64 finalkey = 0ULL;
	int p = 0;
	for (int i = 0; i < squarenum; i++)
	{
		p = pieces[i];
		if (p != (int)Pieces::Empty && p != (int)Square::OFFBOARD)
		{
			finalkey ^= piecekeys[p][i];
		}
	}
	if (side == (int)Color::White)
	{
		finalkey ^= sidekey;
	}
	if (enpas != (int)Square::NO_SQ)
	{
		assert(enpas >= 0 && enpas <= squarenum);
		finalkey ^= piecekeys[(int)Pieces::Empty][enpas];
	}
	assert(castleperm >= 0 && castleperm <= 15);
	finalkey ^= castlekey[castleperm];

	return finalkey;
}

int BoardStruct::FENparsing(const char* str)
{
	ResetBoard();
	//int count = 1;
	int p = 0;
	int file = (int)File::a;
	int ranggo = (int)Rank::r8;
	int indexo = 0;
	
	while (*str && ranggo >= (int)Rank::r1)
	{
		int count = 1;
		switch (*str)
		{
		case 'p':
			p = (int)Pieces::bP;
			break;
		case 'r':
			p = (int)Pieces::bR;
			break;
		case 'n':
			p = (int)Pieces::bN;
			break;
		case 'b':
			p = (int)Pieces::bB;
			break;
		case 'q':
			p = (int)Pieces::bQ;
			break;
		case 'k':
			p = (int)Pieces::bK;
			break;
		case 'P':
			p = (int)Pieces::wP;
			break;
		case 'R':
			p = (int)Pieces::wR;
			break;
		case 'N':
			p = (int)Pieces::wN;
			break;
		case 'B':
			p = (int)Pieces::wB;
			break;
		case 'Q':
			p = (int)Pieces::wQ;
			break;
		case 'K':
			p = (int)Pieces::wK;
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
			p = (int)Pieces::Empty;
			count = *str - '0';
			break;
		case ' ':
		case '/':
			ranggo--;
			file = (int)File::a;
			str++;
			count = 1;
			continue;
			

		default:
			break;
		}
		for (int i = 0; i < count; i++)
		{
			indexo = ranggo * 8 + file;
			if (p != (int)Pieces::Empty)
			{
				pieces[sq64[indexo]] = p;
				std::cout << pieces[sq64[indexo]] << " " << sq64[indexo] << std::endl;
			}
			file++;
		}
		str++;
	}
	std::cout << std::endl;
	assert(*str == 'w' || *str == 'b');
	side = (*str == 'w') ? (int)Color::White : (int)Color::Black;
	str += 2;

	for (int i = 0; i < 4; i++)
	{
		if (*str == ' ')
		{
			break;
		}
		switch (*str)
		{
		case 'K':
			castleperm |= CastlePermission::WKCA;
			break;
		case 'Q':
			castleperm |= CastlePermission::WQCA;
			break;
		case 'k':
			castleperm |= CastlePermission::BKCA;
			break;
		case 'q':
			castleperm |= CastlePermission::BQCA;
			break;
		default:
			break;
		}
		str++;
	}
	str++;

	assert(castleperm >= 0 && castleperm <= 15);
	if (*str != '-')
	{
		file = str[0] - 'a';
		ranggo = str[1] - '0' - 1;
		assert(file > (int)File::a && file < (int)File::h);
		assert(ranggo > (int)Rank::r1 && ranggo < (int)Rank::r8);
		enpas = (21 + file) + (ranggo * 10);
	}
	poskey = GeneratePositionKey();
	return 0;
}

bool BoardStruct::IsKnight(int piece)
{
	return ((piece == (int)Pieces::bN) || (piece == (int)Pieces::wN));
}

bool BoardStruct::IsRookorQueen(int piece)
{
	return ((piece == (int)Pieces::bQ) || (piece == (int)Pieces::wQ) || (piece == (int)Pieces::bR) 
		|| (piece == (int)Pieces::wR));
}

bool BoardStruct::IsBishopOrQueen(int piece)
{
	return ((piece == (int)Pieces::bQ) || (piece == (int)Pieces::wQ) || (piece == (int)Pieces::bB)
		|| (piece == (int)Pieces::wB));
}

void BoardStruct::PrintBoard()
{
	int piece = 0;
	for (int i = (int)Rank::r8; i >= 0; i--)
	{
		std::cout << i + 1;
		for (int j = (int)File::a; j <= (int)File::h; j++)
		{
			int num = (21 + j) + (i * 10);
			int p = pieces[num];
			std::cout << " " << piecechar[pieces[num]] << " ";
		}
		std::cout << std::endl;
	}

	for (int i = 0; i < 8; i++)
	{
		char l = 'a' + i;
		std::cout << " " << l << " ";
	}
	std::cout<<std::endl;

	std::cout << sidechar[side] << std::endl;
	std::cout << enpas << std::endl;
	std::cout << "Castle Perm " << (castleperm & (int)CastlePermission::WKCA ? "K" : "-") <<
		(castleperm & (int)CastlePermission::WQCA ? "Q" : "-") << (castleperm & CastlePermission::BKCA ? "k" : "-") <<
		(castleperm & CastlePermission::BQCA ? "q" : "-") << std::endl;
	std::cout << std::endl;
	std::cout << "PosKey:" << poskey << std::endl;
}
