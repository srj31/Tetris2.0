#include "app_base/app_base.hpp"
#include "datatypes.hpp"
#include "tetromino.hpp"
#include <thread>
#include <vector>

class Tetris : public AppBase<Tetris>
{
  public:
    Tetris(){};
    virtual ~Tetris() = default;

    virtual void StartUp()
    {
        needsNewBlock = true;
        for (size_t row = 0; row < board.size(); ++row)
        {
            for (size_t col = 0; col < board[0].size(); ++col)
            {
                board[row][col] = Cell();
            }
        }
        curPiece  = Tetromino();
        nextPiece = Tetromino();
    }

    void Update()
    {
        ImGuiIO& io      = ImGui::GetIO();
        float delta_time = io.DeltaTime;

        DrawBoard();
        if (!gameOver)
        {
            ImDrawList* draw = ImGui::GetBackgroundDrawList();
            ImVec2 topLeft(22 * CELL_SIZE, 10 * CELL_SIZE);
            draw->AddText(ImGui::GetFont(), ImGui::GetFontSize() + 40, topLeft, ImGui::GetColorU32(ImGuiCol_Text),
                          std::to_string(score).c_str());

            if (needsNewBlock)
            {
                curPiece  = nextPiece;
                nextPiece = Tetromino();
                DrawPiece(curPiece, draw);
                needsNewBlock = false;
            }
            else
            {
                if (left_move)
                {
                    curPiece.MoveLeft(board);
                    left_move = false;
                }

                if (right_move)
                {
                    curPiece.MoveRight(board);
                    right_move = false;
                }

                if (left_rotate)
                {
                    curPiece.RotateLeft(board);
                    left_rotate = false;
                }

                if (right_rotate)
                {
                    curPiece.RotateRight(board);
                    right_rotate = false;
                }

                if (down)
                {
                    curPiece.Fall(board);
                    time_till_fall = 0;
                    down           = false;
                }

                if (time_till_fall - delta_time < 0)
                {
                    time_till_fall = 1.0 - (1.0 * score / 10000.0);
                    needsNewBlock |= !curPiece.MoveDownAndCheck(board);
                    CheckGameOver();
                    if (needsNewBlock)
                    {
                        AddPieceToBoard(curPiece);
                    }
                    std::vector<uint32_t> rowsToBeDeleted = FindCompleteLines();
                    CalculateScore(rowsToBeDeleted);
                    DeleteAndMoveRows(rowsToBeDeleted);
                }
                else
                {
                    time_till_fall -= delta_time;
                }
                DrawPiece(curPiece, draw);
            }
        }
    }

    void DrawBoard()
    {

        ImDrawList* draw = ImGui::GetBackgroundDrawList();
        for (size_t row = 2; row < board.size(); ++row)
        {
            for (size_t col = 0; col < board[0].size(); ++col)
            {
                ImVec2 top_left(col * CELL_SIZE, row * CELL_SIZE);
                ImVec2 bottom_right((col + 1) * CELL_SIZE, (row + 1) * CELL_SIZE);

                draw->AddRect(top_left, bottom_right, ImGui::GetColorU32(ImGuiCol_Text));

                if (GetCell(row, col).filled == true)
                {
                    draw->AddText(ImGui::GetFont(), ImGui::GetFontSize() + 20, top_left, GetCell(row, col).color,
                                  new char(GetCell(row, col).character));
                }
            }
        }
    }

    void CalculateScore(std::vector<uint32_t> const& rows)
    {
        if (rows.size() > 0)
            score += rows.size() * 500 + 1000;
    }

    void DeleteAndMoveRows(std::vector<uint32_t>& rows)
    {
        int curRow = board.size() - 1;
        for (size_t row = board.size() - 1; row >= 2; --row)
        {

            if (!rows.empty() && row == rows.back())
            {
                rows.pop_back();
                continue;
            }
            for (size_t col = 0; col < board[0].size(); ++col)
            {
                board[curRow][col] = board[row][col];
            }
            curRow--;
        }
        while (curRow >= 2)
        {
            for (size_t col = 0; col < board[0].size(); ++col)
            {
                board[curRow][col] = Cell(false);
            }
            curRow--;
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
                    int row         = piece.GetRowOnBoard(i);
                    int col         = piece.GetColOnBoard(j);
                    board[row][col] = piece.GetPieceCell(i, j);
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
                auto const cell = piece.GetPieceCell(i, j);
                if (cell.filled == true)
                {
                    int row = piece.GetRowOnBoard(i);
                    int col = piece.GetColOnBoard(j);
                    ImVec2 top_left(col * CELL_SIZE + CELL_SIZE / 8, row * CELL_SIZE + CELL_SIZE / 8);
                    ImVec2 bottom_right((col + 1) * CELL_SIZE, (row + 1) * CELL_SIZE);
                    draw->AddText(ImGui::GetFont(), ImGui::GetFontSize() + 20, top_left, cell.color,
                                  new char(cell.character));
                }
            }
        }
    }

    std::vector<uint32_t> FindCompleteLines()
    {
        std::vector<uint32_t> res;
        for (size_t row = 2; row < board.size(); ++row)
        {
            bool isComplete = true;
            for (size_t col = 0; col < board[0].size(); ++col)
            {
                isComplete &= board[row][col].filled;
            }
            if (isComplete)
                res.push_back(row);
        }
        return res;
    }

    void CheckGameOver()
    {
        for (size_t row = 0; row < 2; ++row)
        {
            for (size_t col = 0; col < board[0].size(); ++col)
            {
                if (board[row][col].filled == true)
                {
                    gameOver = true;
                    return;
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
            if (key == GLFW_KEY_A && action == GLFW_PRESS)
            {
                left_move = true;
            }
            if (key == GLFW_KEY_A && action != GLFW_PRESS)
            {
                left_move = false;
            }

            if (key == GLFW_KEY_D && action == GLFW_PRESS)
            {
                right_move = true;
            }
            if (key == GLFW_KEY_D && action != GLFW_PRESS)
            {
                right_move = false;
            }

            if (key == GLFW_KEY_Q && action == GLFW_PRESS)
            {
                left_rotate = true;
            }
            if (key == GLFW_KEY_Q && action != GLFW_PRESS)
            {
                left_rotate = false;
            }

            if (key == GLFW_KEY_E && action == GLFW_PRESS)
            {
                right_rotate = true;
            }
            if (key == GLFW_KEY_E && action != GLFW_PRESS)
            {
                right_rotate = false;
            }

            if (key == GLFW_KEY_S && action == GLFW_PRESS)
            {
                down = true;
            }
            if (key == GLFW_KEY_S && action != GLFW_PRESS)
            {
                down = false;
            }
        }
    }

  private:
    Board board;
    bool needsNewBlock = false;
    int canMove        = 0;
    Tetromino curPiece;
    Tetromino nextPiece;
    bool gameOver        = false;
    const int CELL_SIZE  = 30;
    float time_till_fall = 1.0;
    long long int score  = 0;

    static bool left_move;
    static bool right_move;
    static bool left_rotate;
    static bool right_rotate;
    static bool down;
};

bool Tetris::left_move    = false;
bool Tetris::right_move   = false;
bool Tetris::left_rotate  = false;
bool Tetris::right_rotate = false;
bool Tetris::down         = false;