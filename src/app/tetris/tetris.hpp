#include "app_base/app_base.hpp"
#include "datatypes.hpp"
#include "tetromino.hpp"
#include <thread>

class Tetris : public AppBase<Tetris>
{
  public:
    Tetris(){};
    virtual ~Tetris() = default;

    virtual void StartUp()
    {
        needsNewBlock = true;
        for (int row = 0; row < board.size(); ++row)
        {
            for (int col = 0; col < board[0].size(); ++col)
            {
                board[row][col] = Cell();
            }
        }
    }

    void Update()
    {
        ImDrawList* draw = ImGui::GetBackgroundDrawList();
        for (int row = 2; row < board.size(); ++row)
        {
            for (int col = 0; col < board[0].size(); ++col)
            {
                ImVec2 top_left(col * CELL_SIZE, row * CELL_SIZE);
                ImVec2 bottom_right((col + 1) * CELL_SIZE, (row + 1) * CELL_SIZE);

                draw->AddRect(top_left, bottom_right, ImGui::GetColorU32(ImGuiCol_Text));

                if (GetCell(row, col).filled == true)
                {
                    draw->AddRectFilled(top_left, bottom_right, GetCell(row, col).color);
                }
            }
        }

        if (needsNewBlock)
        {
            Tetromino newPiece;
            curPiece = newPiece;
            DrawPiece(curPiece, draw);
            needsNewBlock = false;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            needsNewBlock |= !curPiece.MoveDownAndCheck(board);
            DrawPiece(curPiece, draw);
            if (needsNewBlock)
            {
                AddPieceToBoard(curPiece);
            }
        }
    }

    void AddPieceToBoard(Tetromino const& piece)
    {
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if (piece.GetPieceCell(i, j).filled == true)
                {
                    int row = piece.GetRowOnBoard(i);
                    int col = piece.GetColOnBoard(j);
                    board[row][col] = piece.GetPieceCell(i,j);
                }
            }
        }
    }

    void DrawPiece(Tetromino const& piece, ImDrawList* draw)
    {
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if (piece.GetPieceCell(i, j).filled == true)
                {
                    int row = piece.GetRowOnBoard(i);
                    int col = piece.GetColOnBoard(j);
                    ImVec2 top_left(col * CELL_SIZE, row * CELL_SIZE);
                    ImVec2 bottom_right((col + 1) * CELL_SIZE, (row + 1) * CELL_SIZE);
                    draw->AddRectFilled(top_left, bottom_right, piece.GetPieceCell(i, j).color);
                }
            }
        }
    }

    Cell GetCell(int row, int col)
    {
        return board[row][col];
    }

    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureMouse)
        {
        }
    }

    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureMouse)
        {
        }
    }

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureKeyboard)
        {
        }
    }

  private:
    Board board;
    bool needsNewBlock = false;
    int canMove        = 0;
    Tetromino curPiece;
    const int CELL_SIZE = 30;
};