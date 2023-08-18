#pragma once
#include "datatypes.hpp"

class Tetromino
{
  public:
    Tetromino(){};
    virtual ~Tetromino() = default;

    // Return the occupied spaces
    void GetGrid();

    int GetRowOnBoard(int relativeRow) const
    {
        return row_offset + relativeRow;
    }

    int GetColOnBoard(int relativeCol) const
    {
        return col_offset + relativeCol;
    }

    Cell GetPieceCell(int relRow, int relCol) const
    {
        return shape[relRow][relCol];
    }

    // Rotate the pieces
    void RotateLeft(Board const& board)
    {
        std::array<std::array<Cell, 5>, 5> newShape;
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 5; ++j)
            {
                newShape[4 - j][i] = shape[i][j];
            }
        }
        shape = newShape;
    }
    void RotateRight(Board const& board)
    {
        std::array<std::array<Cell, 5>, 5> newShape;
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 5; ++j)
            {
                newShape[j][4 - i] = shape[i][j];
            }
        }
        shape = newShape;
    }

    // move the pieces around if it is possible to move
    void MoveLeft(Board const& board)
    {
        col_offset--;
        if (CheckCollision(board) == true)
        {
            col_offset++;
        }
    }
    void MoveRight(Board const& board)
    {
        col_offset++;
        if (CheckCollision(board) == true)
        {
            col_offset--;
        }
    }

    bool MoveDownAndCheck(Board const& board)
    {
        row_offset++;
        if (CheckCollision(board) == true)
        {
            row_offset--;
            return false;
        }

        return true;
    }

    // make the item drop down
    void Fall(Board const& board)
    {
        bool canFall = true;
        while (canFall)
        {
            canFall &= MoveDownAndCheck(board);
        }
    }
    int row_offset = -1;
    int col_offset = -1;

  private:
    using Shape = std::array<std::array<Cell, 5>, 5>;
    Shape GetRandomShape()
    {
        auto colorVec = ImVec4(rand() % 256 / 255.f, rand() % 256 / 255.f, rand() % 256 / 255.f, 0.8f);
        char ch       = char(rand() % 26 + 65);
        auto color    = ImGui::GetColorU32(colorVec);

        return {{{{Cell(false), Cell(false), Cell(false), Cell(false), Cell(false)}},
                 {{Cell(false), Cell(false), Cell(false), Cell(false), Cell(false)}},
                 {{Cell(false), Cell(true, color, ch), Cell(true, color, ch), Cell(true, color, ch), Cell(false)}},
                 {{Cell(false), Cell(false), Cell(false), Cell(false), Cell(false)}},
                 {{Cell(false), Cell(false), Cell(false), Cell(false), Cell(false)}}}};
    }

    bool CheckFilled(int row, int col)
    {
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 5; ++j)
            {
                if (row_offset + i == row && col_offset + j == col && shape[i][j].filled == true)
                    return true;
            }
        }
        return false;
    }

    bool CheckCollision(Board const& board)
    {
        for (size_t row = 2; row < board.size(); ++row)
        {
            for (size_t col = 0; col < board[0].size(); ++col)
            {

                if (board[row][col].filled == true && CheckFilled(row, col) == true)
                    return true;
            }
        }

        return CheckNotInBounds(board);
    }

    bool CheckNotInBounds(Board const& board)
    {
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 5; ++j)
            {
                if (shape[i][j].filled)
                {
                    if (row_offset + i >= 22 || col_offset + j >= 10 || col_offset + j < 0)
                        return true;
                }
            }
        }
        return false;
    }

    Rotation rotation = Rotation::One;
    Shape shape       = GetRandomShape();
};