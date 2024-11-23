//
//    構文テスト
//
//    Copyright (c) 2022-2024 udonrobo
//

#include <Arduino.h>
#include <Udon/Types/ArrayView.hpp>

__attribute__((unused)) static void testConstructor()
{
    Udon::ArrayView<int> defaultConstructor{};

    int array[10];

    Udon::ArrayView<int> arrayRef{ array };
    Udon::ArrayView<int> arrayPointer{ array, 10 };
}

__attribute__((unused)) static void testMethod()
{
    Udon::ArrayView<int> arrayView{};
    arrayView.size();
    arrayView.data();
    arrayView[0];
    arrayView.at(0);
    arrayView.front();
    arrayView.back();
    arrayView.empty();
    arrayView.clear();
    arrayView.fill(0);
    arrayView.begin();
    arrayView.end();
    arrayView.cbegin();
    arrayView.cend();
    arrayView.rbegin();
    arrayView.rend();
    arrayView.crbegin();
    arrayView.crend();
}

__attribute__((unused)) static void testConstMethod()
{
    const Udon::ArrayView<int> arrayView{};
    arrayView.size();
    arrayView.data();
    arrayView[0];
    arrayView.at(0);
    arrayView.front();
    arrayView.back();
    arrayView.empty();
    arrayView.begin();
    arrayView.end();
    arrayView.cbegin();
    arrayView.cend();
    arrayView.rbegin();
    arrayView.rend();
    arrayView.crbegin();
    arrayView.crend();
}
