#include <gtest/gtest.h>
#include <iostream>

#include "tritSet.h"

TEST(TritSetTests, MainTest)
{
    TritSet set(1000);

    size_t allocLength = set.capacity();
    ASSERT_TRUE(allocLength >= 1000 * 2 / 8 / sizeof(unsigned int));

    set[1000'000'000] = Trit::Unknown;
    ASSERT_TRUE(allocLength == set.capacity());

    ASSERT_FALSE(set[2000'000'000] == Trit::True);
    ASSERT_TRUE(allocLength == set.capacity());

    set[1000'000'000] = Trit::True;
    ASSERT_TRUE(allocLength < set.capacity());

    allocLength = set.capacity();
    set[1000'000'000] = Trit::Unknown;
    set[1000'000] = Trit::False;
    ASSERT_TRUE(allocLength == set.capacity());

    set.shrink();
    ASSERT_TRUE(allocLength > set.capacity());

    TritSet setA(1000);
    setA[0] = Trit::True;
    TritSet setB(2000);
    setB[0] = Trit::True;

    TritSet setC = setA & setB;
    ASSERT_TRUE(setC.capacity() == setB.capacity());

    TritSet setD = ~setA;
    ASSERT_TRUE(setD.capacity() == setA.capacity());
    ASSERT_TRUE(setD[1000] == ~(Trit)setA[1000]);
}

TEST(TritSetTests, GetterSetterTests)
{
    TritSet set(1000);
    for (size_t i = 0; i < 2000; i++)
    {
        ASSERT_TRUE(set[i] == Trit::Unknown);
    }

    set[0] = Trit::False;
    ASSERT_TRUE(set[0] == Trit::False);
    for (size_t i = 1; i < 1000; i++)
    {
        ASSERT_TRUE(set[i] == Trit::Unknown);
    }

    set[1000] = Trit::True;
    ASSERT_TRUE(set[1000] == Trit::True);

    set[1000] = Trit::False;
    ASSERT_TRUE(set[1000] == Trit::False);


    // same for getAt / setAt


    TritSet setB(1000);
    for (size_t i = 0; i < 2000; i++)
    {
        ASSERT_TRUE(setB.getAt(i) == Trit::Unknown);
    }

    setB.setAt(0, Trit::False);
    ASSERT_TRUE(setB.getAt(0) == Trit::False);
    for (size_t i = 1; i < 1000; i++)
    {
        ASSERT_TRUE(setB.getAt(i) == Trit::Unknown);
    }

    setB.setAt(1000, Trit::True);
    ASSERT_TRUE(setB.getAt(1000) == Trit::True);

    setB.setAt(1000, Trit::False);
    ASSERT_TRUE(setB.getAt(1000) == Trit::False);
}

TEST(TritSetTests, CapacityTests)
{
    TritSet set(0);
    ASSERT_TRUE(set.capacity() == 0);

    set[1] = Trit::Unknown;
    ASSERT_TRUE(set.capacity() == 0);

    set[1] = Trit::True;
    ASSERT_TRUE(set.capacity() == 1);

    set[1] = Trit::Unknown;
    ASSERT_TRUE(set.capacity() == 1);

    set[10000000] = Trit::True;
    size_t cap = set.capacity();

    ASSERT_TRUE(cap >= 10000000 * 2 / 8 / sizeof(unsigned int));
    set[10000001] = Trit::True;
    ASSERT_TRUE(cap == set.capacity());

    set.shrink();
    ASSERT_TRUE(cap == set.capacity());

    set[10000000] = Trit::Unknown;
    set[10000001] = Trit::Unknown;
    set.shrink();
    ASSERT_TRUE(set.capacity() == 0);
}

TEST(TritSetTests, LengthTests)
{
    TritSet set(1000);
    ASSERT_TRUE(set.length() == 0);

    set[0] = Trit::False;
    ASSERT_TRUE(set.length() == 1);

    set[0] = Trit::Unknown;
    ASSERT_TRUE(set.length() == 0);

    set[1] = Trit::False;
    set[10] = Trit::True;
    set[10000] = Trit::True;
    ASSERT_TRUE(set.length() == 10001);
    set[10000] = Trit::Unknown;
    ASSERT_TRUE(set.length() == 11);
    set[10] = Trit::Unknown;
    ASSERT_TRUE(set.length() == 2);
}

TEST(TritSetTests, MaxTritsCountTests)
{
    TritSet set(10);
    ASSERT_TRUE(set.maxTritsCount() == 10);

    set[10] = Trit::Unknown;
    ASSERT_TRUE(set.maxTritsCount() == 10);

    set[10] = Trit::False;
    ASSERT_TRUE(set.maxTritsCount() == 11);

    set[10] = Trit::Unknown;
    ASSERT_TRUE(set.maxTritsCount() == 11);

    set[0] = Trit::True;
    set.shrink();
    ASSERT_TRUE(set.maxTritsCount() == 1);

    set[10000] = Trit::False;
    ASSERT_TRUE(set.maxTritsCount() == 10001);
    set.shrink();
    ASSERT_TRUE(set.maxTritsCount() == 10001);
}

TEST(TritSetTests, CardinalityTests)
{
    TritSet set(1000);

    ASSERT_TRUE(1000 == set.cardinality(Trit::Unknown));
    ASSERT_TRUE(0 == set.cardinality(Trit::True));
    ASSERT_TRUE(0 == set.cardinality(Trit::False));
    auto map = set.cardinality();
    ASSERT_TRUE(1000 == map[Trit::Unknown]);
    ASSERT_TRUE(0 == map[Trit::True]);
    ASSERT_TRUE(0 == map[Trit::False]);

    for (size_t i = 0; i < 999; i++)
    {
        set[i] = Trit::True;
    }
    ASSERT_TRUE(0 == set.cardinality(Trit::Unknown));
    ASSERT_TRUE(999 == set.cardinality(Trit::True));
    ASSERT_TRUE(0 == set.cardinality(Trit::False));
    map = set.cardinality();
    ASSERT_TRUE(0 == map[Trit::Unknown]);
    ASSERT_TRUE(999 == map[Trit::True]);
    ASSERT_TRUE(0 == map[Trit::False]);

    set[998] = Trit::Unknown;
    ASSERT_TRUE(0 == set.cardinality(Trit::Unknown));
    ASSERT_TRUE(998 == set.cardinality(Trit::True));
    ASSERT_TRUE(0 == set.cardinality(Trit::False));
    map = set.cardinality();
    ASSERT_TRUE(0 == map[Trit::Unknown]);
    ASSERT_TRUE(998 == map[Trit::True]);
    ASSERT_TRUE(0 == map[Trit::False]);
}

TEST(TritSetTests, ShrinkTests)
{
    TritSet set(0);

    set.shrink();
    ASSERT_TRUE(set.maxTritsCount() == 0);

    set[10] = Trit::True;
    set.shrink();
    ASSERT_TRUE(set.maxTritsCount() == 11);

    set[11] = Trit::True;
    set.shrink();
    ASSERT_TRUE(set.maxTritsCount() == 12);

    set[1000] = Trit::Unknown;
    set.shrink();
    ASSERT_TRUE(set.maxTritsCount() == 12);

    set[1000] = Trit::False;
    set.shrink();
    ASSERT_TRUE(set.maxTritsCount() == 1001);
    set[1000] = Trit::Unknown;
    set.shrink();
    ASSERT_TRUE(set.maxTritsCount() == 12);
    set.shrink();
    ASSERT_TRUE(set.maxTritsCount() == 12);
}

TEST(TritSetTests, AssignmentTests)
{
    // =
    {
        TritSet setA(10);
        TritSet setB(11);

        setA[0] = Trit::True;
        setB[0] = Trit::False;
        setB[10] = Trit::False;

        setA = setB;

        ASSERT_TRUE(setA.capacity() == setB.capacity());
        ASSERT_TRUE(setA.maxTritsCount() == setB.maxTritsCount());
        ASSERT_TRUE(setA.length() == setB.length());
        for (size_t i = 0; i < 11; i++)
        {
            ASSERT_TRUE(setA[i] == setB[i]);
        }

        TritSet setC(0);
        setA = setC;
        ASSERT_TRUE(setA.maxTritsCount() == setC.maxTritsCount());

        setC = setB;
        ASSERT_TRUE(setB.maxTritsCount() == setC.maxTritsCount());
    }

    // &=
    {
        TritSet setA(10);
        TritSet setB(10);
        setA[0] = Trit::True;
        setB[0] = Trit::False;
        setA &= setB;
        ASSERT_TRUE(setA[0] == Trit::False);
        for (size_t i = 1; i < 10; i++)
        {
            ASSERT_TRUE(setA[i] == Trit::Unknown);
        }

        setA &= setA;
        ASSERT_TRUE(setA[0] == Trit::False);
        for (size_t i = 1; i < 10; i++)
        {
            ASSERT_TRUE(setA[i] == Trit::Unknown);
        }

        TritSet set1(6);
        TritSet set2(6);

        // possible combs  TT TF TU FU UU FF
        // expected result T  F  U  F  U  F
        vector<pair<pair<Trit, Trit>, Trit>> v =
                {
                        {{Trit::True,    Trit::True},    Trit::True},
                        {{Trit::True,    Trit::False},   Trit::False},
                        {{Trit::True,    Trit::Unknown}, Trit::Unknown},
                        {{Trit::False,   Trit::Unknown}, Trit::False},
                        {{Trit::Unknown, Trit::Unknown}, Trit::Unknown},
                        {{Trit::False,   Trit::False},   Trit::False},
                };

        for (size_t i = 0; i < 6; i++)
        {
            set1[i] = v[i].first.first;
            set2[i] = v[i].first.second;
        }

        setA = set1;
        setB = set2;

        setA &= setB;

        for (size_t i = 0; i < 6; i++)
        {
            ASSERT_TRUE(setA[i] == v[i].second);
        }

        // swap

        setA = set2;
        setB = set1;

        setA &= setB;

        for (size_t i = 0; i < 6; i++)
        {
            ASSERT_TRUE(setA[i] == v[i].second);
        }
    }

    // |=
    {
        TritSet setA(10);
        TritSet setB(10);
        setA[0] = Trit::True;
        setB[0] = Trit::False;
        setA |= setB;
        ASSERT_TRUE(setA[0] == Trit::True);
        for (size_t i = 1; i < 10; i++)
        {
            ASSERT_TRUE(setA[i] == Trit::Unknown);
        }

        setA |= setA;
        ASSERT_TRUE(setA[0] == Trit::True);
        for (size_t i = 1; i < 10; i++)
        {
            ASSERT_TRUE(setA[i] == Trit::Unknown);
        }

        setA[100] = Trit::True;
        setA |= setB;
        ASSERT_TRUE(setA.maxTritsCount() == 101);
        Trit t = setA[100];
        ASSERT_TRUE(setA[100] == Trit::True);

        TritSet set1(6);
        TritSet set2(6);

        // possible combs  TT TF TU FU UU FF
        // expected result T  T  T  U  U  F
        vector<pair<pair<Trit, Trit>, Trit>> v =
                {
                        {{Trit::True,    Trit::True},    Trit::True},
                        {{Trit::True,    Trit::False},   Trit::True},
                        {{Trit::True,    Trit::Unknown}, Trit::True},
                        {{Trit::False,   Trit::Unknown}, Trit::Unknown},
                        {{Trit::Unknown, Trit::Unknown}, Trit::Unknown},
                        {{Trit::False,   Trit::False},   Trit::False},
                };

        for (size_t i = 0; i < 6; i++)
        {
            set1[i] = v[i].first.first;
            set2[i] = v[i].first.second;
        }

        setA = set1;
        setB = set2;

        setA |= setB;

        for (size_t i = 0; i < 6; i++)
        {
            ASSERT_TRUE(setA[i] == v[i].second);
        }

        // swap

        setA = set2;
        setB = set1;

        setA |= setB;

        for (size_t i = 0; i < 6; i++)
        {
            ASSERT_TRUE(setA[i] == v[i].second);
        }
    }
}

TEST(TritSetTests, TrimTests)
{
    TritSet set(0);

    set.trim(0);
    ASSERT_TRUE(set.maxTritsCount() == 0);

    set.trim(10);
    ASSERT_TRUE(set.maxTritsCount() == 0);

    set[11] = Trit::True;
    ASSERT_TRUE(set.maxTritsCount() == 12);
    set.trim(10);
    ASSERT_TRUE(set.maxTritsCount() == 11);
    ASSERT_TRUE(set[11] == Trit::Unknown);
}

TEST(TritSetTests, BitwiseOpsTests)
{
    // &
    {
        TritSet setA(10);
        TritSet setB(10);
        setA[0] = Trit::True;
        setB[0] = Trit::False;
        TritSet setC = setA & setB;
        ASSERT_TRUE(setC.capacity() == setA.capacity());
        ASSERT_TRUE(setC[0] == Trit::False);
        for (size_t i = 1; i < 10; i++)
        {
            ASSERT_TRUE(setC[i] == Trit::Unknown);
        }
        setA[100] = Trit::False;

        setC = setA & setB;

        ASSERT_TRUE(setC.maxTritsCount() == setA.maxTritsCount());
        ASSERT_TRUE(setC[100] == Trit::False);

        TritSet set1(6);
        TritSet set2(6);

        // possible combs  TT TF TU FU UU FF
        // expected result T  F  U  F  U  F
        vector<pair<pair<Trit, Trit>, Trit>> v =
                {
                        {{Trit::True,    Trit::True},    Trit::True},
                        {{Trit::True,    Trit::False},   Trit::False},
                        {{Trit::True,    Trit::Unknown}, Trit::Unknown},
                        {{Trit::False,   Trit::Unknown}, Trit::False},
                        {{Trit::Unknown, Trit::Unknown}, Trit::Unknown},
                        {{Trit::False,   Trit::False},   Trit::False},
                };

        for (size_t i = 0; i < 6; i++)
        {
            set1[i] = v[i].first.first;
            set2[i] = v[i].first.second;
        }

        setC = set1 & set2;

        for (size_t i = 0; i < 6; i++)
        {
            ASSERT_TRUE(setC[i] == v[i].second);
        }

        // swap

        setC = set2 & set1;

        for (size_t i = 0; i < 6; i++)
        {
            ASSERT_TRUE(setC[i] == v[i].second);
        }
    }

    // |
    {
        TritSet setA(10);
        TritSet setB(10);
        setA[0] = Trit::True;
        setB[0] = Trit::False;
        TritSet setC = setA | setB;
        ASSERT_TRUE(setC[0] == Trit::True);
        for (size_t i = 1; i < 10; i++)
        {
            ASSERT_TRUE(setC[i] == Trit::Unknown);
        }

        setA[100] = Trit::True;
        setC = setA | setB;
        ASSERT_TRUE(setC.maxTritsCount() == 101);
        ASSERT_TRUE(setC[100] == Trit::True);

        TritSet set1(6);
        TritSet set2(6);

        // possible combs  TT TF TU FU UU FF
        // expected result T  T  T  U  U  F
        vector<pair<pair<Trit, Trit>, Trit>> v =
                {
                        {{Trit::True,    Trit::True},    Trit::True},
                        {{Trit::True,    Trit::False},   Trit::True},
                        {{Trit::True,    Trit::Unknown}, Trit::True},
                        {{Trit::False,   Trit::Unknown}, Trit::Unknown},
                        {{Trit::Unknown, Trit::Unknown}, Trit::Unknown},
                        {{Trit::False,   Trit::False},   Trit::False},
                };

        for (size_t i = 0; i < 6; i++)
        {
            set1[i] = v[i].first.first;
            set2[i] = v[i].first.second;
        }

        setA = set1;
        setB = set2;

        setC = setA | setB;

        for (size_t i = 0; i < 6; i++)
        {
            ASSERT_TRUE(setC[i] == v[i].second);
        }

        // swap

        setA = set2;
        setB = set1;

        setC = setA | setB;

        for (size_t i = 0; i < 6; i++)
        {
            ASSERT_TRUE(setC[i] == v[i].second);
        }
    }
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
