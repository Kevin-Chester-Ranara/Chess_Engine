#include "Move.h"

char* Moves::PieceMove::printsq(int sq)
{
    static char* pp[2];
    *pp[0] = '1' + pos.RankSq[sq];
    *pp[1] = 'a' + pos.FileSq[sq];
    return *pp;
}

char* Moves::PieceMove::printsqmove()
{
    static char pp[5];
    pp[0] = 'a' + pos.FileSq[FROMSQ(move)];
    pp[1] = '1' + pos.RankSq[FROMSQ(move)];
    pp[2] = 'a' + pos.FileSq[TOSQ(move)];
    pp[3] = '1' + pos.RankSq[TOSQ(move)];
    if (PROMOTED(move))
    {
        char pr = 'q';
        if (pos.IsKnight(PROMOTED(move)))
        {
            pr = 'n';
        }
        else if (pos.IsBishopOrQueen(PROMOTED(move)) && !pos.IsRookorQueen(PROMOTED(move)))
        {
            pr = 'b';
        }
        else if (pos.IsRookorQueen(PROMOTED(move)) && !pos.IsBishopOrQueen(PROMOTED(move)))
        {
            pr = 'r';
        }
        pp[4] = pr;
        return pp;
    }
    else
        pp[4] = -1;
        return pp;
}


void Moves::AddMove(int from, int to, int capture, int Promoted, int flag)
{
    int move = MOVE(from, to, capture, Promoted, flag);
    list.emplace_back(move, 0, pos);
    count++;
}

void Moves::AddCaptureMove(int from, int to, int capture, int Promoted, int flag)
{
    int move = MOVE(from, to, capture, Promoted, flag);
    list.emplace_back(move, 0, pos);
    count++;
}

void Moves::AddEnPassMove(int from, int to, int capture, int Promoted, int flag)
{
    int move = MOVE(from, to, capture, Promoted, flag);
    list.emplace_back(move, 0, pos);
    count++;
}


void Moves::AddWhitePawn()
{
    assert(pos.side == (int)BoardStruct::Color::White);
    for (int i = 0; i < pos.piecenum[(int)BoardStruct::Pieces::wP]; i++)
    {
        int sq = pos.piecelist[(int)BoardStruct::Pieces::wP][i];
        assert(sq != (int)BoardStruct::Pieces::Empty);
        if (pos.pieces[sq + 10] == (int)BoardStruct::Pieces::Empty)
        {
            assert(sq + 10 != (int)BoardStruct::Pieces::Empty);
            if (pos.RankSq[sq] == (int)BoardStruct::Rank::r7)
            {
                AddMove(sq, sq + 10, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::wQ, 0);
                AddMove(sq, sq + 10, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::wR, 0);
                AddMove(sq, sq + 10, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::wB, 0);
                AddMove(sq, sq + 10, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::wN, 0);
            }
            else
            {
                AddMove(sq, sq + 10, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::Empty, 0);
            }
            if (pos.RankSq[sq] == (int)BoardStruct::Rank::r2 && pos.pieces[sq + 20] == (int)BoardStruct::Pieces::Empty)
            {
                AddMove(sq, sq + 20, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::Empty, PAWNSTARTMOVE);
            }
        }
        if (pos.pieces[sq + 9] != (int)BoardStruct::Square::OFFBOARD && pos.ofpieces[pos.pieces[sq + 9]] == BoardStruct::Color::Black)
        {
            assert(pos.pieces[sq + 9] != (int)BoardStruct::Square::OFFBOARD);
            if (pos.RankSq[sq] == (int)BoardStruct::Rank::r7)
            {
                AddMove(sq, sq + 9, pos.pieces[sq + 9], (int)BoardStruct::Pieces::wQ, 0);
                AddMove(sq, sq + 9, pos.pieces[sq + 9], (int)BoardStruct::Pieces::wR, 0);
                AddMove(sq, sq + 9, pos.pieces[sq + 9], (int)BoardStruct::Pieces::wB, 0);
                AddMove(sq, sq + 9, pos.pieces[sq + 9], (int)BoardStruct::Pieces::wN, 0);
            }
            else
            {
                AddMove(sq, sq + 9, pos.pieces[sq + 9], (int)BoardStruct::Pieces::Empty, 0);
            }
        }

        if (pos.pieces[sq + 11] != (int)BoardStruct::Square::OFFBOARD && pos.ofpieces[pos.pieces[sq + 11]] == BoardStruct::Color::Black)
        {
            assert(pos.pieces[sq + 11] != (int)BoardStruct::Square::OFFBOARD);
            if (pos.RankSq[sq] == (int)BoardStruct::Rank::r7)
            {
                AddMove(sq, sq + 11, pos.pieces[sq + 11], (int)BoardStruct::Pieces::wQ, 0);
                AddMove(sq, sq + 11, pos.pieces[sq + 11], (int)BoardStruct::Pieces::wR, 0);
                AddMove(sq, sq + 11, pos.pieces[sq + 11], (int)BoardStruct::Pieces::wB, 0);
                AddMove(sq, sq + 11, pos.pieces[sq + 11], (int)BoardStruct::Pieces::wN, 0);
            }
            else
            {
                AddMove(sq, sq + 11, pos.pieces[sq + 11], (int)BoardStruct::Pieces::Empty, 0);
            }
        }

        if ((sq + 9) == pos.enpas)
        {
            AddMove(sq, sq + 9, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::Empty, ENPASSMOVE);
        }
        if ((sq + 11) == pos.enpas)
        {
            AddMove(sq, sq + 11, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::Empty, ENPASSMOVE);
        }
    }
}

void Moves::AddMoveSlider()
{
    for (size_t i = 0; i < SliderPiece.size(); i++)
    {
        if (SliderPiece.at(i))
        {
            for (int j = 0; j < pos.piecenum[i]; j++)
            {
                int k = 0;
                int sq = pos.piecelist[i][j];
                for (int z = 0; z < piecedir.at(i).size(); z++)
                {
                    int to_sq = sq + piecedir.at(i).at(z);
                    while (pos.pieces[to_sq] != (int)BoardStruct::Square::OFFBOARD)
                    {
                        if (pos.pieces[to_sq] == (int)BoardStruct::Pieces::Empty)
                        {
                            AddMove(sq, to_sq, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::Empty, 0);
                        }
                        else if ((int)pos.ofpieces[pos.pieces[to_sq]] == (pos.side ^ 1))
                        {
                            AddMove(sq, to_sq, pos.pieces[to_sq], (int)BoardStruct::Pieces::Empty, 0);
                            break;
                        }
                        to_sq += piecedir.at(i).at(z);
                    }
                }
            }
        }
    }
}

void Moves::AddMoveNonSlider()
{
    //Knight
    assert(pos.side == (int)BoardStruct::Color::White || pos.side == (int)BoardStruct::Color::Black);
    int side = pos.side == (int)BoardStruct::Color::White ? (int)BoardStruct::Pieces::wN : (int)BoardStruct::Pieces::bN;
    for (size_t i = 0; i < pos.piecenum[side]; i++)
    {
        int sq = pos.piecelist[side][i];
        for (size_t i = 0; i < piecedir.at(side).size(); i++)
        {
            if (pos.pieces[sq + i] != (int)BoardStruct::Square::OFFBOARD)
            {
                if (pos.pieces[sq + i] == (int)BoardStruct::Pieces::Empty)
                {
                    AddMove(sq, sq + i, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::Empty, 0);
                }
                else if ((int)pos.ofpieces[pos.pieces[sq + i]] == (pos.side ^ 1))
                {
                    AddMove(sq, sq + i, pos.pieces[sq + i], (int)BoardStruct::Pieces::Empty, 0);
                }
                else
                    continue;
            }
        }
    }
    //King
    assert(pos.side == (int)BoardStruct::Color::White || pos.side == (int)BoardStruct::Color::Black);
    side = pos.side == (int)BoardStruct::Color::White ? (int)BoardStruct::
        Pieces::wK : (int)BoardStruct::Pieces::bK;
    for (size_t i = 0; i < pos.piecenum[side]; i++)
    {
        int sq = pos.piecelist[side][i];
        for (size_t i = 0; i < piecedir.at(side).size(); i++)
        {
            if (pos.pieces[sq + i] != (int)BoardStruct::Square::OFFBOARD)
            {
                if (pos.pieces[sq + i] == (int)BoardStruct::Pieces::Empty)
                {
                    AddMove(sq, sq + i, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::Empty, 0);
                }
                else if ((int)pos.ofpieces[pos.pieces[sq + i]] == (pos.side ^ 1))
                {
                    AddMove(sq, sq + i, pos.pieces[sq + i], (int)BoardStruct::Pieces::Empty, 0);
                }
                else
                    continue;
            }
        }
    }

}

void Moves::AddBlackPawn()
{
    assert(pos.side == (int)BoardStruct::Color::Black);
    for (int i = 0; i < pos.piecenum[(int)BoardStruct::Pieces::bP]; i++)
    {
        int sq = pos.piecelist[(int)BoardStruct::Pieces::bP][i];
        if (pos.pieces[sq - 10] == (int)BoardStruct::Pieces::Empty)
        {
            if (pos.RankSq[sq] == (int)BoardStruct::Rank::r2)
            {
                AddMove(sq, sq - 10, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::bQ, 0);
                AddMove(sq, sq - 10, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::bR, 0);
                AddMove(sq, sq - 10, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::bB, 0);
                AddMove(sq, sq - 10, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::bN, 0);
            }
            else
            {
                AddMove(sq, sq - 10, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::Empty, 0);
            }
            if (pos.RankSq[sq] == (int)BoardStruct::Rank::r7 && pos.pieces[sq - 20] == (int)BoardStruct::Pieces::Empty)
            {
                AddMove(sq, sq - 20, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::Empty, PAWNSTARTMOVE);
            }
        }
        if (pos.pieces[sq - 9] != (int)BoardStruct::Square::OFFBOARD && pos.ofpieces[pos.pieces[sq - 9]] == BoardStruct::Color::White)
        {
            assert(pos.pieces[sq - 9] != (int)BoardStruct::Square::OFFBOARD);
            if (pos.RankSq[sq] == (int)BoardStruct::Rank::r2)
            {
                AddMove(sq, sq - 9, pos.pieces[sq - 9], (int)BoardStruct::Pieces::bQ, 0);
                AddMove(sq, sq - 9, pos.pieces[sq - 9], (int)BoardStruct::Pieces::bR, 0);
                AddMove(sq, sq - 9, pos.pieces[sq - 9], (int)BoardStruct::Pieces::bB, 0);
                AddMove(sq, sq - 9, pos.pieces[sq - 9], (int)BoardStruct::Pieces::bN, 0);
            }
            else
            {
                AddMove(sq, sq - 9, pos.pieces[sq - 9], (int)BoardStruct::Pieces::Empty, 0);
            }
        }

        if (pos.pieces[sq - 11] != (int)BoardStruct::Square::OFFBOARD && pos.ofpieces[pos.pieces[sq - 11]] == BoardStruct::Color::White)
        {
            assert(pos.pieces[sq - 11] != (int)BoardStruct::Square::OFFBOARD);
            if (pos.RankSq[sq] == (int)BoardStruct::Rank::r2)
            {
                AddMove(sq, sq - 11, pos.pieces[sq - 11], (int)BoardStruct::Pieces::bQ, 0);
                AddMove(sq, sq - 11, pos.pieces[sq - 11], (int)BoardStruct::Pieces::bR, 0);
                AddMove(sq, sq - 11, pos.pieces[sq - 11], (int)BoardStruct::Pieces::bB, 0);
                AddMove(sq, sq - 11, pos.pieces[sq - 11], (int)BoardStruct::Pieces::bN, 0);
            }
            else
            {
                AddMove(sq, sq - 11, pos.pieces[sq - 11], (int)BoardStruct::Pieces::Empty, 0);
            }
        }

        if ((sq - 9) == pos.enpas)
        {
            AddMove(sq, sq - 9, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::Empty, ENPASSMOVE);
        }
        if ((sq - 11) == pos.enpas)
        {
            AddMove(sq, sq - 11, (int)BoardStruct::Pieces::Empty, (int)BoardStruct::Pieces::Empty, ENPASSMOVE);
        }
    }

}


void Moves::PrintMovelist()
{
    for (size_t i = 0; i < list.size(); i++)
    {
        std::cout << "Move " << i + 1 << ": " << list.at(i).printsqmove() << std::endl;
    }
}
