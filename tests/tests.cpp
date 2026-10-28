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

TEST(CoralTest, DiesWhenLifetimeReached)
{
    Ocean ocean(5, 5);

    auto coral_ptr = std::make_unique<Coral>(10, 2);
    Coral* coral = coral_ptr.get();
    
    ocean.add_object(std::move(coral_ptr), 2, 2);

    EXPECT_TRUE(coral->is_alive());

    ocean.update();
    EXPECT_TRUE(coral->is_alive());

    ocean.update();
    EXPECT_FALSE(coral->is_alive());
}

TEST(CoralTest, DoesNotReproduceIfReproductionTimeIsZeroOrNegative)
{
    Ocean ocean(5, 5);
    
    ocean.add_object(std::make_unique<Coral>(0, 5), 2, 2);

    ocean.update();

    int occupied = 0;
    for (int y = 0; y < ocean.height(); ++y) {
        for (int x = 0; x < ocean.width(); ++x) {
            if (!ocean.cell(x, y).empty()) ++occupied;
        }
    }
    
    EXPECT_EQ(occupied, 1);
}

TEST(CoralTest, ReproducesOnlyOnSpecificIntervals)
{
    Ocean ocean(5, 5);
    
    ocean.add_object(std::make_unique<Coral>(3, 10), 2, 2);

    
    ocean.update();
    
    int occupied_step1 = 0;
    for (int y = 0; y < ocean.height(); ++y) {
        for (int x = 0; x < ocean.width(); ++x) {
            if (!ocean.cell(x, y).empty()) ++occupied_step1;
        }
    }
    EXPECT_EQ(occupied_step1, 1);

    ocean.update();
    
    ocean.update();
    
    int occupied_step3 = 0;
    for (int y = 0; y < ocean.height(); ++y) {
        for (int x = 0; x < ocean.width(); ++x) {
            if (!ocean.cell(x, y).empty()) ++occupied_step3;
        }
    }
    
    EXPECT_EQ(occupied_step3, 2);
}

TEST(CoralTest, DoesNotReproduceWhenNoFreeNeighbors)
{
    Ocean ocean(3, 3);  
    
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
            ocean.add_object(std::make_unique<Coral>(100, 100), x, y);
        }
    }

    ocean.add_object(std::make_unique<Coral>(1, 10), 1, 1);

    ocean.update();

    int occupied = 0;
    for (int y = 0; y < ocean.height(); ++y) {
        for (int x = 0; x < ocean.width(); ++x) {
            if (!ocean.cell(x, y).empty()) ++occupied;
        }
    }
    
    EXPECT_EQ(occupied, 9);
}


// TODO: Add more tests