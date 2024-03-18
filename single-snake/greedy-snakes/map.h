#pragma once
namespace GreedySnake {
    class Map {
    public:
        Map();
        Map(int h);
        Map(int w, int h);
        Map(int w, int h, int x, int y);
        int getTop() const;
        int getBottom() const;
        int getLeft() const;
        int getRight() const;
        void initMap();

    private:
        int top;
        int bottom;
        int left;
        int right;
    };
}