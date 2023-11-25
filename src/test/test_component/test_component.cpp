#include "../unit_test/unit_test.h"
#include "ce/component/component.h"
#include <memory>

void UnitTest::TestComponent0()
{
    Component component = Component(nullptr);
    Component component_2 = Component(nullptr);
    component.AddChild(&component_2);
    EXPECT_VALUES_EQUAL(component.GetChildren()[0], &component_2);
    EXPECT_VALUES_EQUAL(component_2.GetParent(), &component);
    component.RemoveChild(&component_2);
    EXPECT_VALUES_EQUAL(component.GetChildren().size(), 0);
    EXPECT_VALUES_EQUAL(component_2.GetParent(), nullptr);
}

void UnitTest::TestComponent1()
{
    
}

void UnitTest::TestComponent2()
{

}

void UnitTest::TestComponent3()
{

}

void UnitTest::TestComponent4()
{

}
