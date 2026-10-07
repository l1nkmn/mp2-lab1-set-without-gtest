// ============================================================
// tests_tset.cpp — тесты для класса TSet
// Сборка: g++ -std=c++17 tests_tset.cpp tset.cpp tbitfield.cpp -o tests
// ============================================================
#include <iostream>
#include <sstream>
#include <cassert>
#include <string>
#include "tset.h"

// ---------- 1. Базовый ввод/вывод ----------
void test_basic_io() {
    std::istringstream in("{1,2,3}");
    TSet s(10);
    in >> s;
    std::ostringstream out;
    out << s;
    std::cout << "basic_io: " << out.str() << "\n";
    assert(out.str() == "{ 1, 2, 3,}");
}

// ---------- 2. Пустое множество ----------
void test_empty_input() {
    std::istringstream in("{}");
    TSet s(10);
    in >> s;
    std::ostringstream out;
    out << s;
    std::cout << "empty_input: " << out.str() << "\n";
    assert(out.str() == "{}");
}

// ---------- 3. Ввод с пробелами ----------
void test_input_with_spaces() {
    std::istringstream in("{ 1 , 2 , 3 }");
    TSet s(10);
    in >> s;
    std::ostringstream out;
    out << s;
    std::cout << "with_spaces: " << out.str() << "\n";
    assert(out.str() == "{ 1, 2, 3,}");
}

// ---------- 4. IsMember ----------
void test_ismember() {
    TSet s(10);
    s.InsElem(2);
    s.InsElem(5);
    assert(s.IsMember(2) == 1);
    assert(s.IsMember(5) == 1);
    assert(s.IsMember(3) == 0);
    assert(s.IsMember(0) == 0);
}

// ---------- 5. InsElem / DelElem ----------
void test_insert_delete() {
    TSet s(10);
    s.InsElem(4);
    assert(s.IsMember(4) == 1);
    s.DelElem(4);
    assert(s.IsMember(4) == 0);
    s.DelElem(4);
    assert(s.IsMember(4) == 0);
}

// ---------- 6. Объединение ----------
void test_union() {
    TSet a(10), b(10);
    a.InsElem(1); 
    a.InsElem(2);
    b.InsElem(2); 
    b.InsElem(3);
    TSet c = a + b;
    std::cout << c;
    assert(c.IsMember(1) == 1);
    assert(c.IsMember(2) == 1);
    assert(c.IsMember(3) == 1);
    assert(c.IsMember(4) == 0);
}

// ---------- 7. Пересечение ----------
void test_intersection() {
    TSet a(10), b(10);
    a.InsElem(1); a.InsElem(2); a.InsElem(3);
    b.InsElem(2); b.InsElem(3); b.InsElem(4);
    TSet c = a * b;
    assert(c.IsMember(1) == 0);
    assert(c.IsMember(2) == 1);
    assert(c.IsMember(3) == 1);
    assert(c.IsMember(4) == 0);
}

// ---------- 8. Дополнение ----------
void test_complement() {
    TSet a(5);
    a.InsElem(1);
    a.InsElem(3);
    TSet c = ~a;
    assert(c.IsMember(0) == 1);
    assert(c.IsMember(1) == 0);
    assert(c.IsMember(2) == 1);
    assert(c.IsMember(3) == 0);
    assert(c.IsMember(4) == 1);
}

// ---------- 9. Разность с элементом ----------
void test_difference_element() {
    TSet a(10);
    a.InsElem(1); a.InsElem(2);
    TSet c = a - 1;
    assert(c.IsMember(1) == 0);
    assert(c.IsMember(2) == 1);
}

// ---------- 10. Сравнение ----------
void test_equality() {
    TSet a(10), b(10);
    a.InsElem(1); a.InsElem(2);
    b.InsElem(2); b.InsElem(1);
    assert((a == b) != 0);
    b.InsElem(3);
    assert((a == b) == 0);
    assert((a != b) != 0);
}

// ---------- 11. Копирование ----------
void test_copy() {
    TSet a(10);
    a.InsElem(1); a.InsElem(5);
    TSet b = a;
    assert(b.IsMember(1) == 1);
    assert(b.IsMember(5) == 1);
    b.InsElem(7);
    assert(a.IsMember(7) == 0);
}

// ---------- 12. Присваивание ----------
void test_assignment() {
    TSet a(10), b(10);
    a.InsElem(2); a.InsElem(4);
    b = a;
    assert(b.IsMember(2) == 1);
    assert(b.IsMember(4) == 1);
    a = a; // самоприсваивание
    assert(a.IsMember(2) == 1);
}

// ---------- 13. Границы ----------
void test_boundaries() {
    TSet s(5);
    s.InsElem(0);
    s.InsElem(4);
    assert(s.IsMember(0) == 1);
    assert(s.IsMember(4) == 1);

    bool thrown = false;
    try { s.InsElem(5); }
    catch (...) { thrown = true; }
    assert(thrown);

    thrown = false;
    try { s.InsElem(-1); }
    catch (...) { thrown = true; }
    assert(thrown);
}

// ---------- 14. Граница слова (32 бита) ----------
void test_word_boundary() {
    TSet s(64);
    s.InsElem(0);
    s.InsElem(31);
    s.InsElem(32);
    s.InsElem(63);
    assert(s.IsMember(0) == 1);
    assert(s.IsMember(31) == 1);
    assert(s.IsMember(32) == 1);
    assert(s.IsMember(63) == 1);
    TSet c = ~s;
    assert(c.IsMember(0) == 0);
    assert(c.IsMember(31) == 0);
    assert(c.IsMember(32) == 0);
    assert(c.IsMember(63) == 0);
    assert(c.IsMember(1) == 1);
}

// ---------- 15. Дополнение за пределами BitLen ----------
void test_complement_extra_bits() {
    // BitLen = 5, MemLen = 1 (32-битное слово).
    // Биты 5..31 должны быть равны 0 в дополнении,
    // иначе вывод покажет лишние элементы.
    TSet a(5);
    TSet c = ~a;
    std::ostringstream out;
    out << c;
    std::cout << "complement(empty, len=5): " << out.str() << "\n";
    // Ожидаем ровно 5 элементов: 0,1,2,3,4
    assert(out.str() == "{ 0, 1, 2, 3, 4,}");
}

// ---------- 16. Ввод без пробелов и с пробелами ----------
void test_input_variants() {
    {
        std::istringstream in("{0}");
        TSet s(10);
        in >> s;
        assert(s.IsMember(0) == 1);
        for (int i = 1; i < 10; ++i) assert(s.IsMember(i) == 0);
    }
    {
        std::istringstream in("  {  7  }  ");
        TSet s(10);
        in >> s;
        assert(s.IsMember(7) == 1);
        for (int i = 0; i < 10; ++i) if (i != 7) assert(s.IsMember(i) == 0);
    }
}

// ---------- 17. Многократный ввод подряд ----------
void test_multiple_inputs() {
    std::istringstream in("{1,2} {3,4}");
    TSet a(10), b(10);
    in >> a >> b;
    assert(a.IsMember(1) == 1 && a.IsMember(2) == 1);
    assert(a.IsMember(3) == 0 && a.IsMember(4) == 0);
    assert(b.IsMember(3) == 1 && b.IsMember(4) == 1);
    assert(b.IsMember(1) == 0 && b.IsMember(2) == 0);
}

// ---------- 18. Симметричная разность (через операции) ----------
void test_symmetric_difference() {
    TSet a(10), b(10);
    a.InsElem(1); a.InsElem(2);
    b.InsElem(2); b.InsElem(3);
    TSet c = (a + b) - 2; // (a ∪ b) \ {2}
    assert(c.IsMember(1) == 1);
    assert(c.IsMember(2) == 0);
    assert(c.IsMember(3) == 1);
}

int main() {
    test_basic_io();
    //test_empty_input();
    //test_input_with_spaces();
    test_ismember();
    test_insert_delete();
    test_union();
    test_intersection();
    test_complement();
    test_difference_element();
    test_equality();
    test_copy();
    test_assignment();
    test_boundaries();
    test_word_boundary();
    test_complement_extra_bits();
    test_input_variants();
    test_multiple_inputs();
    test_symmetric_difference();

    std::cout << "All tests passed!\n";
    return 0;
}