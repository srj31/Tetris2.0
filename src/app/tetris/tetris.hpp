#include "app_base/app_base.hpp"
#include "tetromino.hpp"
#include "datatypes.hpp"

class Tetris : public AppBase<Tetris>
{
  public:
    Tetris(){};
    virtual ~Tetris() = default;

    virtual void StartUp()
    {
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
        // draw the game board

        for (int row = 2; row < board.size(); ++row)
        {
            for (int col = 0; col < board[0].size(); ++col)
            {
                ImVec2 top_left(col * CELL_SIZE, row * CELL_SIZE);
                ImVec2 bottom_right((col + 1) * CELL_SIZE, (row + 1) * CELL_SIZE);

                ImDrawList* draw = ImGui::GetBackgroundDrawList();
                draw->AddRect(top_left, bottom_right, ImGui::GetColorU32(ImGuiCol_Text));

                if (GetCell(row, col).filled == true)
                {
                    draw->AddRectFilled(top_left, bottom_right, GetCell(row, col).color);
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
    const int CELL_SIZE = 30;
};