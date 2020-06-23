//
// Created by phoenixflower on 6/18/20.
//

#ifndef LANDESSDEVCORE_RAYTRACING_HPP
#define LANDESSDEVCORE_RAYTRACING_HPP
#include "Primitives/General/Matrix.hpp"
#include "Primitives/General/Vec2.hpp"
#include "Primitives/General/StaticArray.hpp"
#define WAVE_SOURCE	2
namespace LD
{
    template<typename T>
    class RayTracingState
    {
    private:
        LD::BasicVec2D<T> mLocation;
    public:

        inline constexpr RayTracingState() noexcept :mLocation{0,0}
        {

        }

        inline constexpr RayTracingState(const LD::BasicVec2D<T> & location) noexcept :mLocation{location}
        {

        }

        const LD::BasicVec2D<T> & Location() const noexcept
        {
            return this->mLocation;
        }

        LD::BasicVec2D<T> & Location() noexcept
        {
            return this->mLocation;
        }
    };
    class RayTracer
    {

        template<LD::UInteger Width, LD::UInteger Height>
        LD::BasicVec2D<int> FindNear(const LD::ImmutableMatrix<char,Width,Height> &map, int x, int y, int n) const
        {
            if ( y > 0 && map[x][y - 1] == n )
                return LD::BasicVec2D<int>(x, y - 1);
            else if  ( y < map.ColumnSize() - 1 && map[x][y + 1] == n )//( y < m_BoardSize - 1 && map[x][y + 1] == n )
                return LD::BasicVec2D<int>(x, y + 1);
            else if ( x > 0 && map[x - 1][y] == n )
                return LD::BasicVec2D<int>(x - 1, y);
            else// if ( x < m_BoardSize - 1 && map[x + 1][y] == n )
                return LD::BasicVec2D<int>(x + 1, y);
        }

        template<LD::UInteger Width, LD::UInteger Height>
        bool SetNear(const LD::ImmutableMatrix<char,Width,Height> &map, int x, int y, int n) const
        {
            bool bStatus = false;
            if ( y > 0 && map[x][y - 1] == 0 )
            {
                map[x][y - 1] = n;
                bStatus = true;
            }
            if  ( y < map.GetHeight() - 1 && map[x][y + 1] == 0 )//( y < m_BoardSize - 1 && map[x][y + 1] == 0 )
            {
                map[x][y + 1] = n;
                bStatus = true;
            }
            if ( x > 0 && map[x - 1][y] == 0 )
            {
                map[x - 1][y] = n;
                bStatus = true;
            }
            if ( x < map.GetWidth() - 1 && map[x + 1][y] == 0 )//( x < m_BoardSize - 1 && map[x + 1][y] == 0 )
            {
                map[x + 1][y] = n;
                bStatus = true;
            }
            return bStatus;
        }

        template<LD::UInteger Width, LD::UInteger Height>
        bool NearBy(const LD::ImmutableMatrix<char,Width,Height> &map, int x, int y, int n, LD::BasicVec2D<int> &pt) const
        {
            if ( y > 0 && map[x][y - 1] == n )
            {
                pt.X() = x; pt.Y() = y - 1;
                return true;
            }
            if ( y < map.ColumnSize() - 1 && map[x][y + 1] == n )//( y < m_BoardSize - 1 && map[x][y + 1] == n )
            {
                pt.X() = x; pt.Y() = y + 1;
                return true;
            }
            if ( x > 0 && map[x - 1][y] == n )
            {
                pt.X() = x - 1; pt.Y() = y;
                return true;
            }
            if ( x < map.RowSize() - 1 && map[x + 1][y] == n )//( x < m_BoardSize - 1 && map[x + 1][y] == n )
            {
                pt.X() = x + 1; pt.Y() = y;
                return true;
            }
            return false;
        }



        //LD::StaticArray<LD::BasicVec2D<LD::UInteger>,Width*Height> mRoute;
    public:

        template<typename T,LD::UInteger Width, LD::UInteger Height>
        LD::BasicVec2D<T> GetNextPoint(const LD::ImmutableMatrix<char,Width,Height> & m_Board,
                                        LD::RayTracingState<T> & state,
                                        const LD::BasicVec2D<T> & end) const noexcept
        {

            int i, j, k = WAVE_SOURCE;
            LD::BasicVec2D<LD::Integer> pt1, pt2;
            bool isMet = false, isNotFinished = true;


            LD::ImmutableMatrix<char,Width,Height> map;

            for ( int i = 0; i < m_Board.GetWidth(); i++ )
                for ( int j = 0; j < m_Board.GetHeight(); j++ )
                    map[i][j] = m_Board[i][j] ? 1 : 0;


            // Mark wave sources
            map[state.Location().X()][state.Location().Y()] = k;
            map[end.X()][end.Y()] = -k;

            for ( ; isNotFinished && !isMet; k++ )
            {
                isNotFinished = false;
                for ( i = 0; i < m_Board.RowSize() && !isMet; i++ )
                {
                    for ( j = 0; j < m_Board.ColumnSize() && !isMet; j++ )
                    {
                        if ( map[i][j] == k )
                        {
                            if ( NearBy(map, i, j, -k, pt2) || NearBy(map, i, j, -k - 1, pt2) )
                            {
                                // Two waves met
                                isMet = true;
                                pt1.X() = i;
                                pt1.Y() = j;
                            }
                            else
                                isNotFinished |= SetNear(map, i, j, k + 1);
                        }

                        else if ( map[i][j] == -k )
                        {
                            if ( NearBy(map, i, j, k, pt1) || NearBy(map, i, j, k + 1, pt1) )
                            {
                                // Two waves met
                                isMet = true;
                                pt2.X() = i;
                                pt2.Y() = j;
                            }
                            else
                            {
                                isNotFinished |= SetNear(map, i, j, -k - 1);
                            }
                        }
                    }

                }
            }

            if (isMet)
            {
                LD::BasicVec2D<LD::Integer> buffer[2] = {FindNear(map, pt2.X(), pt2.Y(), map[pt2.X()][pt2.Y()] + 1),
                                                         FindNear(map, pt1.X(), pt1.Y(), map[pt1.X()][pt1.Y()] - 1)};

                state.Location() = buffer[map[pt1.X()][pt1.Y()] > WAVE_SOURCE];
            }
            /*
            if (map[pt2.X()][pt2.Y()] < -WAVE_SOURCE )
            {
                pt2 = FindNear(map, pt2.X(), pt2.Y(), map[pt2.X()][pt2.Y()] + 1);
                state.Location() = pt2;
            }else if(map[pt1.X()][pt1.Y()] > WAVE_SOURCE)
            {
                pt1 = FindNear(map, pt1.X(), pt1.Y(), map[pt1.X()][pt1.Y()] - 1);
                state.Location() = pt1;
            }
             */
            /**
             *
             * // Extract route
            if ( isMet )
            {

                m_Route.push_back(glm::vec3(pt1.x,pt1.y,0));
                m_Route.push_back(glm::vec3(pt2.x,pt2.y,0));
                while ( map[pt2.x][pt2.y] < -WAVE_SOURCE )
                {
                    pt2 = FindNear(map, pt2.x, pt2.y, map[pt2.x][pt2.y] + 1);
                    m_Route.push_back(glm::vec3(pt2.x,pt2.y,0));
                }
                while ( map[pt1.x][pt1.y] > WAVE_SOURCE )
                {
                    pt1 = FindNear(map, pt1.x, pt1.y, map[pt1.x][pt1.y] - 1);
                    m_Route.insert(m_Route.begin(), glm::vec3(pt1.x,pt1.y,0));
                }
                return true;
            }
            return false;
             *
             */
            return {};
        }

    };
}
#endif //LANDESSDEVCORE_RAYTRACING_HPP
