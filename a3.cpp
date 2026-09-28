#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <stdexcept>

struct ListNode
{
public:
    ListNode(int value, ListNode* prev = nullptr, ListNode* next = nullptr)
        : value(value), prev(prev), next(next)
    {
        if (prev != nullptr) prev->next = this;
        if (next != nullptr) next->prev = this;
    }

public:
    int value;
    ListNode* prev;
    ListNode* next;
};


class List
{
public:
    List()
        : m_head(new ListNode(static_cast<int>(0))), m_size(0),
        m_tail(new ListNode(0, m_head))
    {
    }

    virtual ~List()
    {
        Clear();
        delete m_head;
        delete m_tail;
    }

    bool Empty() { return m_size == 0; }

    unsigned long Size() { return m_size; }

    void PushFront(int value)
    {
        new ListNode(value, m_head, m_head->next);
        ++m_size;
    }

    void PushBack(int value)
    {
        new ListNode(value, m_tail->prev, m_tail);
        ++m_size;
    }

    int PopFront()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_head->next->next);
        int ret = node->value;
        delete node;
        return ret;
    }

    int PopBack()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_tail);
        int ret = node->value;
        delete node;
        return ret;
    }

    void Clear()
    {
        auto current = m_head->next;
        while (current != m_tail)
        {
            current = current->next;
            delete extractPrev(current);
        }
    }

private:
    ListNode* extractPrev(ListNode* node)
    {
        auto target = node->prev;
        target->prev->next = target->next;
        target->next->prev = target->prev;
        --m_size;
        return target;
    }

private:
    ListNode* m_head;
    ListNode* m_tail;
    unsigned long m_size;
};


TEST_CASE("Empty")
{
    List list;

    SECTION("new list is empty")
    {
        REQUIRE(list.Empty() == true);
    }

    SECTION("list with elements is not empty")
    {
        list.PushFront(1);
        REQUIRE(list.Empty() == false);
    }

    SECTION("list after clearing is empty")
    {
        list.PushBack(1);
        list.PushBack(2);
        list.Clear();
        REQUIRE(list.Empty() == true);
    }
}

TEST_CASE("Size")
{
    List list;

    SECTION("new list has size 0")
    {
        REQUIRE(list.Size() == 0);
    }

    SECTION("size grows with PushFront")
    {
        list.PushFront(1);
        REQUIRE(list.Size() == 1);
        list.PushFront(2);
        REQUIRE(list.Size() == 2);
        list.PushFront(3);
        REQUIRE(list.Size() == 3);
    }

    SECTION("size grows with PushBack")
    {
        list.PushBack(1);
        REQUIRE(list.Size() == 1);
        list.PushBack(2);
        REQUIRE(list.Size() == 2);
        list.PushBack(3);
        REQUIRE(list.Size() == 3);
    }

    SECTION("size decreases with PopFront")
    {
        list.PushBack(1);
        list.PushBack(2);
        list.PopFront();
        REQUIRE(list.Size() == 1);
        list.PopFront();
        REQUIRE(list.Size() == 0);
    }

    SECTION("size decreases with PopBack")
    {
        list.PushBack(1);
        list.PushBack(2);
        list.PopBack();
        REQUIRE(list.Size() == 1);
        list.PopBack();
        REQUIRE(list.Size() == 0);
    }
}

TEST_CASE("Clear")
{
    List list;

    SECTION("clear on empty list keeps size 0")
    {
        list.Clear();
        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
    }

    SECTION("clear removes all elements")
    {
        list.PushFront(1);
        list.PushFront(2);
        list.PushFront(3);
        list.PushBack(4);
        list.PushBack(5);
        REQUIRE(list.Size() == 5);

        list.Clear();
        REQUIRE(list.Size() == 0);
        REQUIRE(list.Empty() == true);
    }

    SECTION("list is usable after clear")
    {
        list.PushBack(1);
        list.PushBack(2);
        list.Clear();

        list.PushBack(42);
        REQUIRE(list.Size() == 1);
        REQUIRE(list.PopBack() == 42);
    }
}
