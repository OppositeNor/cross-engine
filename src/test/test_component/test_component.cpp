#include "../unit_test/unit_test.h"
#include "ce/component/component.h"
#include <memory>

void UnitTest::TestComponent0()
{
    auto component = std::make_shared<Component>(nullptr);
    auto component_2 = std::make_shared<Component>(nullptr);
    component->AddChild(component_2);
    EXPECT_VALUES_EQUAL(component->GetChildren()[0].lock().get(), component_2.get());
    EXPECT_VALUES_EQUAL(component_2->GetParent().lock().get(), component.get());
    component->RemoveChild(component_2.get());
    EXPECT_VALUES_EQUAL(component->GetChildren().size(), 0);
    EXPECT_VALUES_EQUAL(component_2->GetParent().lock().get(), nullptr);
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
