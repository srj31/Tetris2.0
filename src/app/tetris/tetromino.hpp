#include "datatypes.hpp"

class Tetromino
{
  public:
    Tetromino();
    virtual ~Tetromino() = default;

    // Return the occupied spaces
    void GetGrid();

    // Rotate the pieces
    void RotateLeft() {
        std::array<std::array<int, 5>, 5> newShape;
        for(int i =0 ;i < 5;++i) {
            for(int j = 0;j < 5;++j) {
                newShape[4-j][i] = shape[i][j];
            }
        }
        shape = std::move(newShape);
    }
    void RotateRight(){
        std::array<std::array<int, 5>, 5> newShape;
        for(int i =0 ;i < 5;++i) {
            for(int j = 0;j < 5;++j) {
                newShape[j][4-i] = shape[i][j];
            }
        }
        shape = std::move(newShape);
    }

    // move the pieces around if it is possible to move
    void MoveLeft(Board& board) {
        col_offset--;
    }
    void MoveRight(Board& board) {
        col_offset++;
    } 

    // make the item drop down
    void Fall();

  private:
    using Shape = std::array<std::array<Cell, 5>, 5>;
    Shape GetRandomShape()
    {
        return
        {
            {
                {{Cell(false), Cell(false), Cell(false), Cell(false), Cell(false)}},
                    {{Cell(false), Cell(false), Cell(false), Cell(false), Cell(false)}},
                    {{Cell(false), Cell(true), Cell(true), Cell(true), Cell(false)}},
                    {{Cell(false), Cell(false), Cell(false), Cell(false), Cell(false)}},
                {
                    {
                        Cell(false), Cell(false), Cell(false), Cell(false), Cell(false)
                    }
                }
            }
        };
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
    }

    bool CheckCollision(Board& board)
    {
        for (int row = 2; row < board.size(); ++row)
        {
            for (int col = 0; col < board[0].size(); ++col)
            {

                if (board[row][col].filled == true && CheckFilled(row, col) == true)
                    return true;
            }
        }
    }
    int row_offset = 0;
    int col_offset = 0;

    Rotation rotation = Rotation::One;
    Shape shape       = GetRandomShape();
};