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

    EXPECT_THROW(
        ocean.add_object(std::make_unique<Stone>(), 3, 4),
        std::runtime_error
    );
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


TEST(CoralTest, DiesWhenLifetimeReached)
{
    Ocean ocean(5, 5);

    ocean.add_object(std::make_unique<Coral>(2, 10), 2, 2);

    EXPECT_FALSE(ocean.cell(2, 2).empty());

    ocean.update(); // age_ = 1, живет
    EXPECT_FALSE(ocean.cell(2, 2).empty());

    ocean.update(); // age_ = 2 >= lifetime -> умирает
    EXPECT_TRUE(ocean.cell(2, 2).empty()); 
}

TEST(CoralTest, DoesNotReproduceIfReproductionTimeIsZeroOrNegative)
{
    Ocean ocean(5, 5);
    
    ocean.add_object(std::make_unique<Coral>(5, 0), 2, 2);

    ocean.update();

    int occupied = 0;
    for (int y = 0; y < ocean.height(); ++y) 
    {
        for (int x = 0; x < ocean.width(); ++x) 
        {
            if (!ocean.cell(x, y).empty()) ++occupied;
        }
    }
    EXPECT_EQ(occupied, 1);
}

TEST(CoralTest, ReproducesOnlyOnSpecificIntervals)
{
    Ocean ocean(5, 5);
    
    ocean.add_object(std::make_unique<Coral>(10, 3), 2, 2);

    ocean.update(); // такт 1
    
    int occupied_step1 = 0;
    for (int y = 0; y < ocean.height(); ++y) 
    {
        for (int x = 0; x < ocean.width(); ++x) 
        {
            if (!ocean.cell(x, y).empty()) ++occupied_step1;
        }
    }
    EXPECT_EQ(occupied_step1, 1);

    ocean.update(); // такт 2
    ocean.update(); // такт 3 -> размножение
    
    int occupied_step3 = 0;
    for (int y = 0; y < ocean.height(); ++y) 
    {
        for (int x = 0; x < ocean.width(); ++x) 
        {
            if (!ocean.cell(x, y).empty()) ++occupied_step3;
        }
    }
    EXPECT_EQ(occupied_step3, 2);
}

TEST(CoralTest, DoesNotReproduceWhenNoFreeNeighbors)
{
    Ocean ocean(3, 3);  
    
    for (int y = 0; y < 3; ++y) 
    {
        for (int x = 0; x < 3; ++x) 
        {
            if (x == 1 && y == 1) {
                ocean.add_object(std::make_unique<Coral>(10, 1), x, y);
            } else {
                ocean.add_object(std::make_unique<Stone>(), x, y);
            }
        }
    }

    ocean.update();

    int occupied = 0;
    for (int y = 0; y < ocean.height(); ++y) 
    {
        for (int x = 0; x < ocean.width(); ++x) 
        {
            if (!ocean.cell(x, y).empty()) ++occupied;
        }
    }
    EXPECT_EQ(occupied, 9);
}


TEST(PreyTest, DiesAfterLifetime)
{
    Ocean ocean(5, 5);
    ocean.add_object(std::make_unique<Prey>(1, 100, 5), 2, 2);

    ocean.update();
    EXPECT_TRUE(ocean.cell(2, 2).empty());
}

TEST(PreyTest, DoesNotReproduceIfReproductionTimeIsZeroOrNegative)
{
    Ocean ocean(5, 5);
    ocean.add_object(std::make_unique<Prey>(5, 0, 5), 2, 2);

    ocean.update();

    int occupied = 0;
    for (int y = 0; y < ocean.height(); ++y) 
    {
        for (int x = 0; x < ocean.width(); ++x) 
        {
            if (!ocean.cell(x, y).empty()) ++occupied;
        }
    }
    EXPECT_EQ(occupied, 1);
}

TEST(PreyTest, ReproducesOnlyOnSpecificIntervals)
{
    Ocean ocean(5, 5);
    ocean.add_object(std::make_unique<Prey>(10, 3, 10), 2, 2);

    ocean.update();
    
    int occupied_step1 = 0;
    for (int y = 0; y < ocean.height(); ++y) 
    {
        for (int x = 0; x < ocean.width(); ++x) 
        {
            if (!ocean.cell(x, y).empty()) ++occupied_step1;
        }
    }
    EXPECT_EQ(occupied_step1, 1);

    ocean.update(); 
    ocean.update(); 
    
    int occupied_step3 = 0;
    for (int y = 0; y < ocean.height(); ++y) 
    {
        for (int x = 0; x < ocean.width(); ++x) 
        {
            if (!ocean.cell(x, y).empty()) ++occupied_step3;
        }
    }
    EXPECT_EQ(occupied_step3, 2);
}

TEST(PreyTest, DoesNotReproduceWhenNoFreeNeighbors)
{
    Ocean ocean(3, 3);  
    
    for (int y = 0; y < ocean.height(); ++y) 
    {
        for (int x = 0; x < ocean.width(); ++x) 
        {
            if (x == 1 && y == 1) 
            {
                ocean.add_object(std::make_unique<Prey>(10, 1, 10), x, y);
            } 
            else 
            {
                ocean.add_object(std::make_unique<Stone>(), x, y);
            }
        }
    }

    ocean.update();

    int occupied = 0;
    for (int y = 0; y < ocean.height(); ++y) 
    {
        for (int x = 0; x < ocean.width(); ++x) 
        {
            if (!ocean.cell(x, y).empty()) ++occupied;
        }
    }
    EXPECT_EQ(occupied, 9);
}

TEST(PreyTest, MoveOrNot)
{
    Ocean ocean(5, 5);

    ocean.add_object(std::make_unique<Prey>(10, 999, 10), 2, 2);

    EXPECT_FALSE(ocean.cell(2, 2).empty());

    ocean.update();
    EXPECT_TRUE(ocean.cell(2, 2).empty());

    int prey_count = 0;
    for (int y = 0; y < ocean.height(); ++y)
    {
        for (int x = 0; x < ocean.width(); ++x)
        {
            const auto* obj = ocean.cell(x, y).object();
            if (obj && obj->symbol() == 'F')
            {
                ++prey_count;
            }
        }
    }
    
    EXPECT_EQ(prey_count, 1);
}


TEST(PredatorTest, DiesAfterLifetime)
{
    Ocean ocean(5, 5);
    ocean.add_object(std::make_unique<Predator>(1, 100, 5), 2, 2);

    ocean.update();

    EXPECT_TRUE(ocean.cell(2, 2).empty());
}

TEST(PredatorTest, DoesNotReproduceIfReproductionTimeIsZeroOrNegative)
{
    Ocean ocean(5, 5);
    ocean.add_object(std::make_unique<Predator>(10, 0, 10), 2, 2);

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

    EXPECT_EQ(occupied, 1);
}

TEST(PredatorTest, ReproducesOnlyOnSpecificIntervals)
{
    Ocean ocean(5, 5);
    ocean.add_object(std::make_unique<Predator>(10, 3, 10), 2, 2);

    ocean.update(); // такт 1

    int occupied_step1 = 0;
    for (int y = 0; y < ocean.height(); ++y)
    {
        for (int x = 0; x < ocean.width(); ++x)
        {
            if (!ocean.cell(x, y).empty())
                ++occupied_step1;
        }
    }
    EXPECT_EQ(occupied_step1, 1);

    ocean.update();
    ocean.update(); 

    int occupied_step3 = 0;
    for (int y = 0; y < ocean.height(); ++y)
    {
        for (int x = 0; x < ocean.width(); ++x)
        {
            if (!ocean.cell(x, y).empty())
                ++occupied_step3;
        }
    }
    EXPECT_EQ(occupied_step3, 2);
}

TEST(PredatorTest, DoesNotReproduceWhenNoFreeNeighbors)
{
    Ocean ocean(3, 3);

    for (int y = 0; y < ocean.height(); ++y)
    {
        for (int x = 0; x < ocean.width(); ++x)
        {
            if (x == 1 && y == 1)
            {
                ocean.add_object(std::make_unique<Predator>(10, 1, 10), x, y);
            }
            else
            {
                ocean.add_object(std::make_unique<Stone>(), x, y);
            }
        }
    }

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

    EXPECT_EQ(occupied, 9);
}

TEST(PredatorTest, MoveOrNot)
{
    Ocean ocean(5, 5);

    ocean.add_object(std::make_unique<Predator>(10, 999, 10), 2, 2);

    EXPECT_FALSE(ocean.cell(2, 2).empty());

    ocean.update();
    EXPECT_TRUE(ocean.cell(2, 2).empty());

    int predator_count = 0;
    for (int y = 0; y < ocean.height(); ++y)
    {
        for (int x = 0; x < ocean.width(); ++x)
        {
            const auto* obj = ocean.cell(x, y).object();
            if (obj && obj->symbol() == 'P')
            {
                ++predator_count;
            }
        }
    }
    
    EXPECT_EQ(predator_count, 1);
}