#include <iostream>
#include <loco/utils/common.hpp>

class Interface {
    LOCO_NO_COPY_NO_MOVE_NO_ASSIGN(Interface)
 public:
    Interface() = default;

    virtual ~Interface() = default;

    virtual auto Draw() -> void { std::cout << "virtual-draw" << std::endl; }

    virtual auto Move() -> void { std::cout << "virtual-move" << std::endl; }
};

class DerivedA : public Interface {
    LOCO_NO_COPY_NO_MOVE_NO_ASSIGN(DerivedA)
 public:
    DerivedA() = default;

    ~DerivedA() override = default;

    auto Draw() -> void override { std::cout << "derivedA-draw" << std::endl; }

    auto Move() -> void override { std::cout << "derivedA-move" << std::endl; }
};

class DerivedB : public Interface {
    LOCO_NO_COPY_NO_MOVE_NO_ASSIGN(DerivedB)
 public:
    DerivedB() = default;

    ~DerivedB() override = default;

    auto Draw() -> void override { std::cout << "derivedB-draw" << std::endl; }

    auto Move() -> void override { std::cout << "derivedB-move" << std::endl; }
};

auto main() -> int {
    auto obj_a = std::make_unique<DerivedA>();
    auto obj_b = std::make_unique<DerivedB>();
    Interface* obj_poly = obj_a.get();
    obj_poly->Draw();
    obj_poly->Move();
    obj_poly = obj_b.get();
    obj_poly->Draw();
    obj_poly->Move();

    //// {
    ////     DerivedA d_a_1;
    ////     DerivedA d_a_2;
    ////     // These shouldn't compile
    ////     DerivedA d_a_copy = d_a_1; // No copy constructor
    ////     d_a_2 = d_a_1; // No assignment operator
    //// }
    return 0;
}
