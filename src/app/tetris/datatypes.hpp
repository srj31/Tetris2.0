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
        : filled(filled),color(color), character(character){};

    char character = 'A';
    bool filled    = false;
    ImU32 color    = ImGui::GetColorU32(ImGuiCol_Text);
};

using Board = std::array<std::array<Cell, 10>, 22>;