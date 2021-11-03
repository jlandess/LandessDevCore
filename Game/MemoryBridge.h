//
// Created by phoenixflower on 6/18/20.
//

#ifndef LANDESSDEVCORE_MEMORYBRIDGE_H
#define LANDESSDEVCORE_MEMORYBRIDGE_H
/**
 *
 * namespace AI
{
    namespace Games
    {
        class MemoryBridge
        {
        private:
            Foundation::Matrix<char> Bridge;
            Foundation::RayTracing Tracer;
            Math::Random::CRandomMersenne Generator;
        public:
            MemoryBridge(const unsigned long length = 10);
            void Generate(const unsigned long additionalCollisions = 0);
            const Foundation::Matrix<char> & GetMap() const;
        };
    }
}
 * namespace AI
{
    namespace Games
    {
        MemoryBridge::MemoryBridge(const unsigned long length):Bridge(length,((length/2.0)-2.0)),Generator((int)time2posix(time(nullptr)))
        {

        }
        void MemoryBridge::Generate(const unsigned long additionalCollisions)
        {
            //unsigned long numberOfCollisionPoints = (this->Bridge.ColumnSize()+this->Bridge.RowSize())%5;
            //numberOfCollisionPoints+=numberOfCollisionPoints/5;
            //numberOfCollisionPoints+=additionalCollisions;
            unsigned long numberOfCollisionPoints = ((this->Bridge.RowSize()-1)*(this->Bridge.ColumnSize()-1))/(this->Bridge.RowSize()/2);
            numberOfCollisionPoints+=additionalCollisions;
            long numberOfPlacedCollisions = 0;


            std::vector<CPoint> collisionPoints;
            while (numberOfPlacedCollisions < numberOfCollisionPoints)//calculate the collision points
            {
                unsigned long x = this->Generator.IRandom(0, (int)this->Bridge.RowSize()-1);
                unsigned long y = this->Generator.IRandom(0, (int)this->Bridge.ColumnSize()-1);

                if (this->Bridge[x][y] != 'X')
                {
                    this->Bridge[x][y] = 'X';
                    CPoint currentCollisionPoint;
                    currentCollisionPoint.x = (int)x;
                    currentCollisionPoint.y = (int)y;
                    collisionPoints.push_back(currentCollisionPoint);
                    numberOfPlacedCollisions++;
                }
            }
            unsigned int randomColumnEnding = this->Generator.IRandom(0, (int)this->Bridge.ColumnSize()-1);
            this->Bridge[this->Bridge.RowSize()-1][randomColumnEnding];

            CPoint ending;
            ending.x = (int)this->Bridge.RowSize()-1;
            ending.y = randomColumnEnding;



            unsigned int secondaryColumnEnding = this->Generator.IRandom(0, (int)this->Bridge.ColumnSize()-1);

            CPoint beginning;
            beginning.x = 0;
            beginning.y = secondaryColumnEnding;


            do
            {

                this->Tracer.FindRoute(this->Bridge, beginning, ending);

                if (this->Tracer.GetRoute().size() == 0)
                {
                    unsigned int randomCollisionPointToRemove = this->Generator.IRandom(0, (int)collisionPoints.size()-1);

                    CPoint currentCollisionPoint;
                    currentCollisionPoint.x = collisionPoints[randomCollisionPointToRemove].x;
                    currentCollisionPoint.y = collisionPoints[randomCollisionPointToRemove].y;

                    this->Bridge[currentCollisionPoint.x][currentCollisionPoint.y] = '\0';
                }
            }while (this->Tracer.GetRoute().size() == 0);





            for (int n = 0; n<this->Tracer.GetRoute().size(); n++)
            {
                this->Bridge[this->Tracer.GetRoute()[n].x][this->Tracer.GetRoute()[n].y] = 'O';
            }
            std::cout << this->Bridge;

        }
        const Foundation::Matrix<char> & MemoryBridge::GetMap() const
        {
            return this->Bridge;
        }
    }
}
 */
#endif //LANDESSDEVCORE_MEMORYBRIDGE_H
