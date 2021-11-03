//
// Created by phoenixflower on 6/18/20.
//

#ifndef LANDESSDEVCORE_MINESWEEPER_H
#define LANDESSDEVCORE_MINESWEEPER_H
/*
namespace AI
{
    namespace Generator
    {
        class MineSweeper
        {
        private:
            Foundation::Matrix<char> Map;

            unsigned int NumberOfMines;

            Math::Random::CRandomMersenne MineGenerator;


        public:
            MineSweeper(const int width, const int height, const int numberOfMines);

            const Foundation::Matrix<char> & GetMap() const;

            const std::vector<char> & operator [] (const int index) const;

            std::vector<char> & operator [] (const int index);

            void GenerateMap()
            {
                this->Map.resize(this->Map.RowSize(), this->Map.ColumnSize());
                int minecounter = 0, X, Y;
                do
                {
                     X = this->MineGenerator.IRandom(0, (int)this->Map.RowSize());
                     Y = this->MineGenerator.IRandom(0, (int)this->Map.ColumnSize());
                     if (this->Map[X][Y] !=BOMB)
                     {
                         this->Map[X][Y] = BOMB; //Assign the bomb
                         minecounter++;
                      }
                } while (minecounter < this->NumberOfMines);
            }


        };
    }
}
 */
#endif //LANDESSDEVCORE_MINESWEEPER_H
