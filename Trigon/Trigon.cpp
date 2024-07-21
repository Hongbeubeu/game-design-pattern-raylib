// main.cpp
#include <map>
#include <string>
#include <optional>
#include <functional>
#include <iostream>

#include "raylib.h"
#include "map"
#include "raymath.h"

using namespace std;

// Define a comparison functor for Vector2
struct Vector2Comparer {
	bool operator()(const Vector2& a, const Vector2& b) const {
		if (a.x < b.x) return true;
		if (a.x > b.x) return false;
		return a.y < b.y;
	}
};


class Tile
{
public:
	explicit Tile(const Vector2 position, const Texture2D& texture, const Color color) : position(position), texture(texture), color(color) {}

	void Draw() const
	{
		DrawTextureEx(texture, position, 0, 1, color);
	}

	bool operator==(const Tile& other) const
	{
		const bool positionEqual = position.x == other.position.x && position.y == other.position.y;
		const bool textureEqual = texture.id == other.texture.id;
		return positionEqual && textureEqual;
	}

	bool operator!=(const Tile& other) const
	{
		const bool positionEqual = position.x != other.position.x || position.y != other.position.y;
		const bool textureEqual = texture.id != other.texture.id;
		return positionEqual || textureEqual;
	}

	Vector2 GetPosition() const
	{
		return position;
	}

	void ChangeColor(const Color c)
	{
		color = c;
	}

private:
	Vector2 position;
	Texture2D texture;
	Color color;
};

class Grid
{
public:
	Grid(const Vector2 size, const float spacing, const Vector2 cellSize, const Vector2 offset) : size(size), cellSize(cellSize), offset(offset), spacing(spacing)
	{
	}

	Vector2 GetGridPosition(const Tile& tile) const
	{
		for (auto [fst, snd] : tiles)
		{
			if (snd == &tile)
			{
				return fst;
			}
		}
		return Vector2{ 0, 0 };
	}

	Vector2 ScreenPositionToGridPosition(const Vector2 screenPosition) const
	{
		const auto x = static_cast<int>(screenPosition.x / (cellSize.x + spacing));
		const auto y = static_cast<int>(screenPosition.y / (cellSize.y + spacing));
		return Vector2{ static_cast<float>(x), static_cast<float>(y) };
	}

	Vector2 GridPositionToScreenPosition(const Vector2 gridPosition) const
	{
		return Vector2{ (gridPosition.x * (cellSize.x + spacing)) + offset.x, (gridPosition.y * (cellSize.y + spacing)) + offset.y };
	}

	Tile* GetTileByGridPosition(const Vector2 gridPosition)
	{
		for (auto [fst, snd] : tiles)
		{
			if (Vector2Equals(fst, gridPosition))
			{
				return snd;
			}
		}
		return nullptr;
	}

	Tile* GetTileByMousePosition(const Vector2 mousePosition)
	{
		if (!IsInsideGrid(mousePosition))
		{
			return nullptr;
		}

		const auto screenPosition = Vector2Subtract(mousePosition, offset);
		const auto gridPosition = ScreenPositionToGridPosition(screenPosition);
		return GetTileByGridPosition(gridPosition);
	}

	void Draw() const
	{
		for (auto [fst, snd] : tiles)
		{
			snd->Draw();
		}
	}
	void GenerateGrid(const Texture2D& texture)
	{
		for (int x = 0; x < static_cast<int>(size.x); x++)
		{
			for (int y = 0; y < static_cast<int>(size.y); y++)
			{
				const auto gridPosition = Vector2{ static_cast<float>(x), static_cast<float>(y) };
				auto tile = new Tile(GridPositionToScreenPosition(gridPosition), texture, RED);
				tiles.emplace(gridPosition, tile);
			}
		}
	}

	void ClearGrid()
	{
		for (auto [fst, snd] : tiles)
		{
			delete snd;
		}
		tiles.clear();
	}

	bool IsInsideGrid(const Vector2 mousePosition) const
	{
		const auto [x, y] = Vector2Subtract(mousePosition, offset);
		const float maxX = size.x * (cellSize.x + spacing);
		const float maxY = size.y * (cellSize.y + spacing);
		if (x < 0 || y < 0 || x > maxX || y > maxY)
		{
			return false;
		}
		return true;
	}
private:
	Vector2 size = {};
	Vector2 cellSize = {};
	Vector2 offset = {};
	float spacing = 0;
	map<Vector2, Tile*, Vector2Comparer> tiles;

};


int main() {
	// Initialize the Raylib window
	constexpr int screenWidth = 800;
	constexpr int screenHeight = 600;
	InitWindow(screenWidth, screenHeight, "Hexagon with Triangle Grid");
	const Texture2D texture = LoadTexture("Tile.png");
	constexpr auto gridSize = Vector2{ 10, 10 };
	constexpr Vector2 cellSize = { 32, 32 };
	constexpr float spacing = 4;
	constexpr Vector2 centerGrid = { ((cellSize.x + spacing) * gridSize.x - spacing) / 2, ((cellSize.y + spacing) * gridSize.y - spacing) / 2 };
	constexpr Vector2 centerScreen = { static_cast<float>(screenWidth) / 2, static_cast<float>(screenHeight) / 2 };
	constexpr Vector2 offset = { centerScreen.x - centerGrid.x, centerScreen.y - centerGrid.y };
	Grid grid(gridSize, spacing, cellSize, offset);
	grid.GenerateGrid(texture);
	Tile* selectedTile = nullptr;
	// Main game loop
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		grid.Draw();

		const Vector2 mousePosition = GetMousePosition();

		string text = "Mouse Position: " + to_string(mousePosition.x) + ", " + to_string(mousePosition.y);
		DrawText(text.c_str(), 0, 500, 20, BLACK);

		string textMousePositionOffset = "Mouse Position Offset: " + to_string(mousePosition.x - offset.x) + ", " + to_string(mousePosition.y - offset.y);
		DrawText(textMousePositionOffset.c_str(), 0, 520, 20, BLACK);

		string gridPosition = "Grid Position: " + to_string(grid.ScreenPositionToGridPosition(Vector2Subtract(mousePosition, offset)).x) + ", " + to_string(grid.ScreenPositionToGridPosition(Vector2Subtract(mousePosition, offset)).y);
		DrawText(gridPosition.c_str(), 0, 540, 20, BLACK);

		if (const auto& currentTile = grid.GetTileByMousePosition(mousePosition); currentTile != nullptr)
		{
			if (selectedTile == nullptr)
			{
				selectedTile = currentTile;
				selectedTile->ChangeColor(BLUE);

			}
			else if (selectedTile != currentTile)
			{
				selectedTile->ChangeColor(RED);
				selectedTile = currentTile;
				selectedTile->ChangeColor(BLUE);
			}
		}
		else if (selectedTile != nullptr)
		{
			selectedTile->ChangeColor(RED);
			selectedTile = nullptr;
		}
		EndDrawing();
	}
	grid.ClearGrid();
	// De-initialize Raylib
	CloseWindow();

	return 0;
}
