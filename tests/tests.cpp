#include "Coral.h"
#include "Ocean.h"
#include "Predator.h"
#include "Prey.h"
#include "Stone.h"

#include <gtest/gtest.h>

TEST(OceanTest, CreatesCorrectSize)
{
    Ocean ocean(10, 20);

    EXPECT_EQ(ocean.width(), 10);
    EXPECT_EQ(ocean.height(), 20);
}

TEST(OceanTest, CanPlaceObject)
{
    Ocean ocean(10, 10);

    ocean.add_object(std::make_unique<Stone>(), 3, 4);

    EXPECT_FALSE(ocean.cell(3, 4).empty());
    EXPECT_EQ(ocean.cell(3, 4).object()->symbol(), '#');
}

TEST(OceanTest, CannotPlaceTwoObjectsInSameCell)
{
    Ocean ocean(10, 10);

    ocean.add_object(std::make_unique<Stone>(), 3, 4);

    try{
        ocean.add_object(std::make_unique<Stone>(), 3, 4);
    }
    catch (std::exception &e)
    {
        std::cout << "exception happened" << std::endl;
    }
}

TEST(OceanTest, RejectsOutOfBoundsObject)
{
    Ocean ocean(10, 10);

    EXPECT_THROW(
        ocean.add_object(std::make_unique<Stone>(), -1, 0),
        std::out_of_range);
}

TEST(ObjectTest, StoneDoesNotMove)
{
    Ocean ocean(5, 5);
    ocean.add_object(std::make_unique<Stone>(), 2, 2);

    ocean.update();

    EXPECT_NE(ocean.cell(2, 2).object(), nullptr);
    EXPECT_EQ(ocean.cell(2, 2).object()->symbol(), '#');
}

TEST(PreyTest, DiesAfterLifetime)
{
    Ocean ocean(5, 5);
    ocean.add_object(std::make_unique<Prey>(1, 100), 2, 2);

    ocean.update();

    EXPECT_TRUE(ocean.cell(2, 2).empty());
}

TEST(CoralTest, Reproduces)
{
    Ocean ocean(5, 5);
    ocean.add_object(std::make_unique<Coral>(1), 2, 2);

    ocean.update();

    int occupied = 0;
    for (int y = 0; y < ocean.height(); ++y)
    {
        for (int x = 0; x < ocean.width(); ++x)
        {
            if (!ocean.cell(x, y).empty())
                ++occupied;
        }
    }

    EXPECT_EQ(occupied, 2);
}


// TODO: Add more tests