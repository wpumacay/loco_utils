#include <iostream>
#include <loco/utils/common.hpp>

class Foo {
    LOCO_DEFINE_SMART_POINTERS(Foo)

 public:
    Foo(std::string&& name, const float& pos_x, const float& pos_y)
        : m_Name(std::move(name)), m_X(pos_x), m_Y(pos_y) {}

    auto name() const -> std::string { return m_Name; }

    auto x() const -> float { return m_X; }

    auto y() const -> float { return m_Y; }

 private:
    std::string m_Name;
    float m_X, m_Y;
};

auto main() -> int {
    constexpr float X_1 = 10.0F;
    constexpr float Y_1 = 10.0F;

    Foo::ptr foo_obj_1 = Foo::Create("foo_obj_1", X_1, Y_1);

    constexpr float X_2 = 20.0F;
    constexpr float Y_2 = 20.0F;
    Foo::uptr foo_obj_2 = Foo::CreateUnique("foo_obj_2", X_2, Y_2);

    std::cout << "foo(1).name   : " << foo_obj_1->name() << std::endl;
    std::cout << "foo(1).x      : " << foo_obj_1->x() << std::endl;
    std::cout << "foo(1).y      : " << foo_obj_1->y() << std::endl;

    std::cout << "foo(2).name   : " << foo_obj_2->name() << std::endl;
    std::cout << "foo(2).x      : " << foo_obj_2->x() << std::endl;
    std::cout << "foo(2).y      : " << foo_obj_2->y() << std::endl;

    return 0;
}
