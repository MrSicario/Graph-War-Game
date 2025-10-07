#include "game_grid.hpp"
#include <algorithm>
#include <cmath>
#include <set>
#include <tuple>
#include <array>

GameGrid::GameGrid(int width, int height) 
    : gridWidth(width), gridHeight(height) {
    grid.resize(height, std::vector<Point>(width));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x] = Point(x, y);
        }
    }
}

bool GameGrid::placePoint(int x, int y, const sf::Color& color) {
    if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) {
        return false;
    }
    
    if (grid[y][x].occupied) {
        return false;
    }
    
    grid[y][x].occupied = true;
    grid[y][x].color = color;
    
    return true;
}

bool GameGrid::canPlacePoint(int x, int y) const {
    if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) {
        return false;
    }
    return !grid[y][x].occupied;
}

Point GameGrid::getPointAt(int x, int y) const {
    if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) {
        return Point();
    }
    return grid[y][x];
}

bool GameGrid::startDrag(int gx, int gy, const sf::Color& color) {
    if (gx < 0 || gx >= gridWidth || gy < 0 || gy >= gridHeight) return false;
    Point p = getPointAt(gx, gy);
    if (!p.occupied || p.color != color) return false;
    dragging = true;
    dragStartPoint = p;
    dragCurrentScreen = gridToScreen(gx, gy);
    return true;
}

void GameGrid::updateDragScreen(float sx, float sy) {
    if (!dragging) return;
    dragCurrentScreen = {sx, sy};
}

bool GameGrid::endDrag(int gx, int gy, const sf::Color& color) {
    if (!dragging) return false;
    dragging = false;
    if (gx < 0 || gx >= gridWidth || gy < 0 || gy >= gridHeight) return false;
    Point target = getPointAt(gx, gy);
    if (!target.occupied || target.color != color) return false;

    Point a = dragStartPoint;
    Point b = target;
    int dx = b.x - a.x;
    int dy = b.y - a.y;

    auto sign = [](int v){ return (v>0) - (v<0); };

    if (a.y == b.y && a.x != b.x) {
        int step = sign(dx);
        for (int x = a.x; x != b.x; x += step) {
            Point p1 = getPointAt(x, a.y);
            Point p2 = getPointAt(x + step, a.y);
            if (!(p1.occupied && p2.occupied && p1.color == color && p2.color == color)) return false;
            if (!edgeExists(p1, p2, color)) addEdge(p1, p2, color);
        }
        return true;
    }
    if (a.x == b.x && a.y != b.y) {
        int step = sign(dy);
        for (int y = a.y; y != b.y; y += step) {
            Point p1 = getPointAt(a.x, y);
            Point p2 = getPointAt(a.x, y + step);
            if (!(p1.occupied && p2.occupied && p1.color == color && p2.color == color)) return false;
            if (!edgeExists(p1, p2, color)) addEdge(p1, p2, color);
        }
        return true;
    }
    if (std::abs(dx) == std::abs(dy) && dx != 0) {
        if (edgeExists(a, b, color)) return false;
        addEdge(a, b, color);
        return true;
    }

    return false;
}

std::vector<Graph> GameGrid::detectRectangles(const sf::Color& color) const {
    std::vector<Graph> rectangles;

    for (int y1 = 0; y1 < gridHeight; ++y1) {
        for (int x1 = 0; x1 < gridWidth; ++x1) {
            const auto &p1 = grid[y1][x1];
            if (!(p1.occupied && p1.color == color)) continue;
            for (int y2 = y1 + 1; y2 < gridHeight; ++y2) {
                for (int x2 = x1 + 1; x2 < gridWidth; ++x2) {
                    const auto &p2 = grid[y2][x2];
                    if (!(p2.occupied && p2.color == color)) continue;
                    const auto &p3 = grid[y1][x2];
                    const auto &p4 = grid[y2][x1];
                    if (!(p3.occupied && p3.color == color && p4.occupied && p4.color == color)) continue;

                    int width = x2 - x1;
                    int height = y2 - y1;
                    if (width != height &&
                        hasHorizontalPath(y1, x1, x2, color) &&
                        hasHorizontalPath(y2, x1, x2, color) &&
                        hasVerticalPath(x1, y1, y2, color) &&
                        hasVerticalPath(x2, y1, y2, color)) {
                        rectangles.emplace_back(std::vector<Point>{p1, p3, p2, p4}, color, "rectangle");
                    }
                }
            }
        }
    }

    return rectangles;
}

std::vector<Graph> GameGrid::detectRightTriangles(const sf::Color& color) const {
    std::vector<Graph> triangles;
    std::set<std::tuple<int,int,int,int,int,int>> uniq;

    for (int y0 = 0; y0 < gridHeight; ++y0) {
        for (int x0 = 0; x0 < gridWidth; ++x0) {
            const auto &P = grid[y0][x0];
            if (!(P.occupied && P.color == color)) continue;

            for (int x2 = x0 + 1; x2 < gridWidth; ++x2) {
                const auto &B = grid[y0][x2];
                if (!(B.occupied && B.color == color)) continue;
                if (!hasHorizontalPath(y0, x0, x2, color)) continue;

                for (int y3 = 0; y3 < y0; ++y3) {
                    const auto &C = grid[y3][x0];
                    if (!(C.occupied && C.color == color)) continue;
                    if (!hasVerticalPath(x0, y3, y0, color)) continue;

                    if (!edgeExistsBetween(B.x, B.y, C.x, C.y, color)) continue;

                    std::array<std::pair<int,int>,3> pts = {{{P.x,P.y},{B.x,B.y},{C.x,C.y}}};
                    std::sort(pts.begin(), pts.end());
                    auto key = std::make_tuple(pts[0].first, pts[0].second,
                                               pts[1].first, pts[1].second,
                                               pts[2].first, pts[2].second);
                    if (uniq.insert(key).second) {
                        triangles.emplace_back(std::vector<Point>{P, B, C}, color, "triangle_rectangle");
                    }
                }
            }
        }
    }

    return triangles;
}

std::vector<Graph> GameGrid::getAllGraphs(const sf::Color& color) const {
    std::vector<Graph> allGraphs;

    auto rectangles = detectRectangles(color);
    auto triangles = detectRightTriangles(color);

    allGraphs.insert(allGraphs.end(), rectangles.begin(), rectangles.end());
    allGraphs.insert(allGraphs.end(), triangles.begin(), triangles.end());

    return allGraphs;
}

bool GameGrid::edgeExistsBetween(int x1, int y1, int x2, int y2, const sf::Color& color) const {
    for (const auto& e : edges) {
        if (e.color != color) continue;
        bool m1 = (e.a.x == x1 && e.a.y == y1 && e.b.x == x2 && e.b.y == y2);
        bool m2 = (e.a.x == x2 && e.a.y == y2 && e.b.x == x1 && e.b.y == y1);
        if (m1 || m2) return true;
    }
    return false;
}

bool GameGrid::hasHorizontalPath(int y, int x1, int x2, const sf::Color& color) const {
    if (x2 < x1) std::swap(x1, x2);
    for (int x = x1; x < x2; ++x) {
        if (!edgeExistsBetween(x, y, x+1, y, color)) return false;
    }
    return true;
}

bool GameGrid::hasVerticalPath(int x, int y1, int y2, const sf::Color& color) const {
    if (y2 < y1) std::swap(y1, y2);
    for (int y = y1; y < y2; ++y) {
        if (!edgeExistsBetween(x, y, x, y+1, color)) return false;
    }
    return true;
}

void GameGrid::draw(sf::RenderWindow& window) {
    drawGrid(window);
    drawPoints(window);
    drawGraphs(window);

    if (dragging) {
        sf::Vertex line[2];
        line[0].position = gridToScreen(dragStartPoint.x, dragStartPoint.y);
        line[0].color = dragStartPoint.color;
        line[1].position = dragCurrentScreen;
        line[1].color = dragStartPoint.color;
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }
}

void GameGrid::drawGrid(sf::RenderWindow& window) {
    sf::RectangleShape line;
    line.setFillColor(sf::Color(100, 100, 100));
    
    for (int x = 0; x <= gridWidth; ++x) {
        line.setSize(sf::Vector2f(2, gridHeight * CELL_SIZE));
        line.setPosition({GRID_OFFSET_X + x * CELL_SIZE, GRID_OFFSET_Y});
        window.draw(line);
    }
    
    for (int y = 0; y <= gridHeight; ++y) {
        line.setSize(sf::Vector2f(gridWidth * CELL_SIZE, 2));
        line.setPosition({GRID_OFFSET_X, GRID_OFFSET_Y + y * CELL_SIZE});
        window.draw(line);
    }
}

void GameGrid::drawPoints(sf::RenderWindow& window) {
    sf::CircleShape point(8);
    point.setOrigin({8, 8});
    
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            if (grid[y][x].occupied) {
                point.setFillColor(grid[y][x].color);
                point.setPosition(gridToScreen(x, y));
                window.draw(point);
            }
        }
    }
}

void GameGrid::drawGraphs(sf::RenderWindow& window) {
    for (const auto& e : edges) {
        sf::Vertex line[2];
        line[0].position = gridToScreen(e.a.x, e.a.y);
        line[0].color = e.color;
        line[1].position = gridToScreen(e.b.x, e.b.y);
        line[1].color = e.color;
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }
}

sf::Vector2f GameGrid::gridToScreen(int x, int y) const {
    return sf::Vector2f(
        GRID_OFFSET_X + x * CELL_SIZE,
        GRID_OFFSET_Y + y * CELL_SIZE
    );
}

sf::Vector2i GameGrid::screenToGrid(float x, float y) const {
    float relX = x - GRID_OFFSET_X;
    float relY = y - GRID_OFFSET_Y;
    
    int gridX = static_cast<int>(std::round(relX / CELL_SIZE));
    int gridY = static_cast<int>(std::round(relY / CELL_SIZE));
    
    gridX = std::max(0, std::min(gridWidth, gridX));
    gridY = std::max(0, std::min(gridHeight, gridY));
    
    return sf::Vector2i(gridX, gridY);
}

bool GameGrid::isRectangle(const std::vector<Point>& points) const {
    if (points.size() != 4) return false;
    return false;
}

bool GameGrid::isRightTriangle(const std::vector<Point>& points) const {
    if (points.size() != 3) return false;
    return false;
}

std::vector<std::vector<Point>> GameGrid::findConnectedComponents(const sf::Color& color) const {
    std::vector<std::vector<Point>> components;
    return components;
}

bool GameGrid::areAdjacent(const Point& p1, const Point& p2) const {
    int dx = std::abs(p1.x - p2.x);
    int dy = std::abs(p1.y - p2.y);
    return (dx == 1 && dy == 0) || (dx == 0 && dy == 1);
}

bool GameGrid::edgeExists(const Point& a, const Point& b, const sf::Color& color) const {
    for (const auto& e : edges) {
        bool same = ((e.a.x == a.x && e.a.y == a.y && e.b.x == b.x && e.b.y == b.y) ||
                     (e.a.x == b.x && e.a.y == b.y && e.b.x == a.x && e.b.y == a.y));
        if (same && e.color == color) return true;
    }
    return false;
}

void GameGrid::addEdge(const Point& a, const Point& b, const sf::Color& color) {
    edges.push_back({a, b, color});
}