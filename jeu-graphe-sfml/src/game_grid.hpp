#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <set>

struct Point {
    int x, y;
    sf::Color color;
    bool occupied;
    
    Point(int x = 0, int y = 0) : x(x), y(y), color(sf::Color::Transparent), occupied(false) {}
    
    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

struct Graph {
    std::vector<Point> points;
    sf::Color color;
    std::string type;
    
    Graph(const std::vector<Point>& pts, const sf::Color& col, const std::string& t)
        : points(pts), color(col), type(t) {}
};

struct Edge {
    Point a;
    Point b;
    sf::Color color;
};

class GameGrid {
public:
    GameGrid(int width = 10, int height = 10);
    
    bool placePoint(int x, int y, const sf::Color& color);
    bool canPlacePoint(int x, int y) const;
    Point getPointAt(int x, int y) const;

    bool startDrag(int gx, int gy, const sf::Color& color);
    void updateDragScreen(float sx, float sy);
    bool endDrag(int gx, int gy, const sf::Color& color);

    std::vector<Graph> detectRectangles(const sf::Color& color) const;
    std::vector<Graph> detectRightTriangles(const sf::Color& color) const;
    std::vector<Graph> getAllGraphs(const sf::Color& color) const;
    
    void connectAdjacentPoints(const sf::Color& color);
    bool areAdjacent(const Point& p1, const Point& p2) const;
    
    void draw(sf::RenderWindow& window);
    void drawGrid(sf::RenderWindow& window);
    void drawPoints(sf::RenderWindow& window);
    void drawGraphs(sf::RenderWindow& window);
    
    sf::Vector2f gridToScreen(int x, int y) const;
    sf::Vector2i screenToGrid(float x, float y) const;
    
    int getWidth() const { return gridWidth; }
    int getHeight() const { return gridHeight; }
    const std::vector<std::vector<Point>>& getGrid() const { return grid; }

private:
    int gridWidth, gridHeight;
    std::vector<std::vector<Point>> grid;
    std::vector<Graph> detectedGraphs;
    std::vector<Edge> edges;

    bool dragging = false;
    Point dragStartPoint;
    sf::Vector2f dragCurrentScreen = {0.f, 0.f};

    static constexpr float CELL_SIZE = 40.0f;
    static constexpr float GRID_OFFSET_X = 100.0f;
    static constexpr float GRID_OFFSET_Y = 100.0f;
    
    bool isRectangle(const std::vector<Point>& points) const;
    bool isRightTriangle(const std::vector<Point>& points) const;
    std::vector<std::vector<Point>> findConnectedComponents(const sf::Color& color) const;

    bool edgeExistsBetween(int x1, int y1, int x2, int y2, const sf::Color& color) const;
    bool hasHorizontalPath(int y, int x1, int x2, const sf::Color& color) const;
    bool hasVerticalPath(int x, int y1, int y2, const sf::Color& color) const;

    bool edgeExists(const Point& a, const Point& b, const sf::Color& color) const;
    void addEdge(const Point& a, const Point& b, const sf::Color& color);
};