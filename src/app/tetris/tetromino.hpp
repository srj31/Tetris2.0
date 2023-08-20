#pragma once
#include "datatypes.hpp"
#include <random>

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
        auto oldShape = shape;
        std::array<std::array<Cell, 5>, 5> newShape;
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 5; ++j)
            {
                newShape[4 - j][i] = shape[i][j];
            }
        }
        shape = newShape;
        if (CheckCollision(board) == true)
        {
            shape = oldShape;
        }
    }
    void RotateRight(Board const& board)
    {
        auto oldShape = shape;
        std::array<std::array<Cell, 5>, 5> newShape;
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 5; ++j)
            {
                newShape[j][4 - i] = shape[i][j];
            }
        }
        shape = newShape;
        if (CheckCollision(board) == true)
        {
            shape = oldShape;
        }
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
        auto color_vec = ImVec4(rand() % 256 / 255.f, rand() % 256 / 255.f, rand() % 256 / 255.f, 0.8f);
        char ch        = char(rand() % 26 + 65);
        auto color     = ImGui::GetColorU32(color_vec);
        static std::random_device randDev;
        static std::mt19937 twister(randDev());
        static std::uniform_int_distribution<int> dist;

        dist.param(std::uniform_int_distribution<int>::param_type(0, 3));
        auto rand                                                             = dist(twister);
        std::vector<std::vector<std::pair<int, int>>> possible_filled_pattern = {{{2, 1}, {2, 2}, {2, 3}, {2, 4}},
                                                                                 {{2, 1}, {2, 2}, {2, 3}, {1, 2}},
                                                                                 {{2, 1}, {2, 2}, {2, 3}, {1, 3}},
                                                                                 {{2, 1}, {2, 2}, {2, 3}, {1, 1}},
                                                                                 {{2, 2}}};

        Shape newShape = {{{{Cell(false), Cell(false), Cell(false), Cell(false), Cell(false)}},
                           {{Cell(false), Cell(false), Cell(false), Cell(false), Cell(false)}},
                           {{Cell(false), Cell(false), Cell(false), Cell(false), Cell(false)}},
                           {{Cell(false), Cell(false), Cell(false), Cell(false), Cell(false)}},
                           {{Cell(false), Cell(false), Cell(false), Cell(false), Cell(false)}}}};

        auto chosen_filled_pattern = possible_filled_pattern[rand];
        for (auto const& [row, col] : chosen_filled_pattern)
        {
            newShape[row][col] = Cell(true, color, ch);
        }

        return newShape;
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