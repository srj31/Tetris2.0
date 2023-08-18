#pragma once

enum class Rotation
{
    One,
    Two,
    Three,
    Four
};

class Cell
{
  public:
    Cell(bool filled = false, ImU32 color = ImGui::GetColorU32(ImGuiCol_Text), char character = 'A')
        : filled(filled), color(color), character(character){};

    bool filled;
    ImU32 color;
    char character;
};

using Board = std::array<std::array<Cell, 10>, 22>;