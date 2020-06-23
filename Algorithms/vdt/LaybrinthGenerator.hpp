//
// Created by phoenixflower on 6/18/20.
//

#ifndef LANDESSDEVCORE_LAYBRINTHGENERATOR_HPP
#define LANDESSDEVCORE_LAYBRINTHGENERATOR_HPP
#include <random>
#include <algorithm>
#include "Primitives/General/Matrix.hpp"
#include <Primitives/General/StaticArray.hpp>
namespace LD
{
    enum Direction {
        NORTH,
        EAST,
        SOUTH,
        WEST

    }; // enum;

    struct Node {
        bool walls[4];
        bool inMaze;

        Node() : inMaze(false) {
            walls[NORTH] = true;
            walls[EAST] = true;
            walls[SOUTH] = true;
            walls[WEST] = true;

        } // Node::Node() : inMaze(false);

    }; // struct Node;


    struct Edge {
        int from, to;
        Direction dir;
        int weight;

    }; // struct Edge;


    template<LD::UInteger Width, LD::UInteger Height>
    class LabrynthGenerator
    {
    private:
        int width, height, debug;
        //std::vector<Node> mazeGrid;
        LD::StaticArray<Node,Width*Height> mMazeGrid;
        //PDP::Matrix<char> Map;
        LD::ImmutableMatrix<char,Width,Height> mMap;
    public:
        LabrynthGenerator():LabrynthGenerator(0,0,false){};
        //LabrynthGenerator(const int width,const int height, const int debug);

        void operator()()
        {
            std::default_random_engine engine((int)(time(nullptr)));
            std::uniform_int_distribution<int> weightDist(100, 999);


            LD::StaticArray<Edge,Width*Height> edgeVect;

            for(LD::UInteger i = 0; i < this->mMazeGrid.size(); i++)
            {
                if(i % width != (width - 1)) {
                    edgeVect.push_back(Edge());
                    edgeVect.back().from = i;
                    edgeVect.back().to = i + 1;
                    edgeVect.back().dir = EAST;

                } // if(i % width != 2);
                if(i < (this->mMazeGrid.GetSize() - Width))
                {
                    edgeVect.push_back(Edge());
                    edgeVect.back().from = i;
                    edgeVect.back().to = i + width;
                    edgeVect.back().dir = SOUTH;

                } // if(i < (mazeGrid.size() - width));

            } // for(int i = 0; i < mazeGrid.size(); i++);


            std::sort(edgeVect.begin(), edgeVect.end(), [](const Edge& a, const Edge& b){ return a.weight < b.weight; });


            for(Edge& n : edgeVect)
            {
                n.weight = weightDist(engine); /* Assign random weights */
            } // for(Edge& n : edgeVect);

            this->mMazeGrid[0].inMaze = true;
            int nodeCount = 1;

            /* Do prims! (Well, sort of) */
            while(nodeCount < this->mMazeGrid.GetSize()) {
                for(Edge& n : edgeVect) {
                    //std::cout << ".";
                    if((this->mMazeGrid[n.from].inMaze && !this->mMazeGrid[n.to].inMaze) or (this->mMazeGrid[n.to].inMaze && !this->mMazeGrid[n.from].inMaze))
                    {
                        if(debug >= 1) {
                            //std::cout << "\n" << nodeCount << ": " << n.from << "->" << n.to << " (weight " << n.weight << ")";
                            if(debug >= 2)
                            {
                                //std::cout << "\n";

                            } // if(debug >= 2);

                        } // if(debug);

                        this->mMazeGrid[n.to].inMaze = true;
                        this->mMazeGrid[n.from].inMaze = true;
                        nodeCount++;

                        if(n.dir == EAST) {
                            this->mMazeGrid[n.from].walls[EAST] = false;
                            this->mMazeGrid[n.to].walls[WEST] = false;

                        } // if(n.dir == EAST);
                        else if(n.dir == SOUTH) {
                            this->mMazeGrid[n.from].walls[SOUTH] = false;
                            this->mMazeGrid[n.to].walls[NORTH] = false;

                        } // else if(n.dir == SOUTH);

                        break;

                    } // if(mazeGrid[n.from].inMaze && !mazeGrid[n.to].inMaze);

                } // for(Edge& n : edgeVect);

                if(debug >= 2)
                {
                    for(Node n : this->mMazeGrid) {
                        //std::cout << n.inMaze << "; ";

                    } // for(Node n : mazeGrid);
                    //std::cout << "\n";

                } // if(debug);

            } // while(nodeCount < (width * height));


        }
        void Print();
    };
}
#endif //LANDESSDEVCORE_LAYBRINTHGENERATOR_HPP
