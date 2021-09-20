#include <iostream>
#include <tinyutils/common.hpp>

class Foo {
 public:
    Foo(const std::string& name, const float& pos_x, const float& pos_y)
        : m_Name(name), m_X(pos_x), m_Y(pos_y) {}

    ~Foo() = default;

    std::string name() const { return m_Name; }

    float x() const { return m_X; }

    float y() const { return m_Y; }

 private:
    std::string m_Name;
    float m_X, m_Y;

    ADD_CLASS_SMART_POINTERS(Foo);
};

int main() {
    Foo::ptr foo_obj_1 = Foo::Create("foo_obj_1", 10.0f, 10.0f);
    Foo::uptr foo_obj_2 = Foo::CreateUnique("foo_obj_2", 20.0f, 20.0f);

    std::cout << "foo(1).name   : " << foo_obj_1->name() << std::endl;
    std::cout << "foo(1).x      : " << foo_obj_1->x() << std::endl;
    std::cout << "foo(1).y      : " << foo_obj_1->y() << std::endl;

    std::cout << "foo(2).name   : " << foo_obj_2->name() << std::endl;
    std::cout << "foo(2).x      : " << foo_obj_2->x() << std::endl;
    std::cout << "foo(2).y      : " << foo_obj_2->y() << std::endl;

    return 0;
}
