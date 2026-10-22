#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Vesper/Events/ApplicationEvent.h"
#include "Vesper/Events/KeyEvent.h"
#include "Vesper/Events/MouseEvent.h"

using namespace Vesper;
using ::testing::_;
using ::testing::MockFunction;
using ::testing::Return;

TEST(Events, KeyPressedProperties)
{
	KeyPressedEvent e(65, 3);
	EXPECT_EQ(e.GetKeyCode(), 65);
	EXPECT_EQ(e.GetRepeatCount(), 3);
	EXPECT_EQ(e.GetEventType(), EventType::KeyPressed);
	EXPECT_STREQ(e.GetName(), "KeyPressed");
	EXPECT_TRUE(e.IsInCategory(EventCategoryKeyboard));
	EXPECT_TRUE(e.IsInCategory(EventCategoryInput));
	EXPECT_FALSE(e.IsInCategory(EventCategoryMouse));
	EXPECT_FALSE(e.Handled);
}

TEST(Events, KeyPressedToString)
{
	KeyPressedEvent e(65, 3);
	EXPECT_EQ(e.ToString(), "KeyPressedEvent: 65 (3 repeats)");
}

TEST(Events, MouseMovedProperties)
{
	MouseMovedEvent e(10.0f, 20.0f);
	EXPECT_FLOAT_EQ(e.GetX(), 10.0f);
	EXPECT_FLOAT_EQ(e.GetY(), 20.0f);
	EXPECT_EQ(e.GetEventType(), EventType::MouseMoved);
	EXPECT_TRUE(e.IsInCategory(EventCategoryMouse));
}

TEST(Events, MouseScrolledProperties)
{
	MouseScrolledEvent e(1.0f, -2.0f);
	EXPECT_FLOAT_EQ(e.GetXOffset(), 1.0f);
	EXPECT_FLOAT_EQ(e.GetYOffset(), -2.0f);
	EXPECT_EQ(e.GetEventType(), EventType::MouseScrolled);
}

TEST(Events, MouseButtonPressedProperties)
{
	MouseButtonPressedEvent e(1);
	EXPECT_EQ(e.GetMouseButton(), 1);
	EXPECT_EQ(e.GetEventType(), EventType::MouseButtonPressed);
	EXPECT_TRUE(e.IsInCategory(EventCategoryMouse));
}

TEST(Events, WindowResizeProperties)
{
	WindowResizeEvent e(1280, 720);
	EXPECT_EQ(e.GetWidth(), 1280u);
	EXPECT_EQ(e.GetHeight(), 720u);
	EXPECT_EQ(e.ToString(), "WindowResizeEvent: 1280, 720");
	EXPECT_TRUE(e.IsInCategory(EventCategoryApplication));
}

TEST(Events, ApplicationEventTypes)
{
	EXPECT_EQ(WindowCloseEvent().GetEventType(), EventType::WindowClose);
	EXPECT_EQ(AppTickEvent().GetEventType(), EventType::AppTick);
	EXPECT_EQ(AppUpdateEvent().GetEventType(), EventType::AppUpdate);
	EXPECT_EQ(AppRenderEvent().GetEventType(), EventType::AppRender);
}

TEST(EventDispatcher, DispatchesMatchingTypeAndMarksHandled)
{
	MockFunction<bool(KeyPressedEvent&)> handler;
	EXPECT_CALL(handler, Call(_)).WillOnce(Return(true));

	KeyPressedEvent e(1, 0);
	EventDispatcher dispatcher(e);
	EXPECT_TRUE(dispatcher.Dispatch<KeyPressedEvent>(handler.AsStdFunction()));
	EXPECT_TRUE(e.Handled);
}

TEST(EventDispatcher, HandlerReturningFalseLeavesEventUnhandled)
{
	MockFunction<bool(KeyPressedEvent&)> handler;
	EXPECT_CALL(handler, Call(_)).WillOnce(Return(false));

	KeyPressedEvent e(1, 0);
	EventDispatcher dispatcher(e);
	EXPECT_TRUE(dispatcher.Dispatch<KeyPressedEvent>(handler.AsStdFunction()));
	EXPECT_FALSE(e.Handled);
}

TEST(EventDispatcher, IgnoresMismatchedType)
{
	MockFunction<bool(MouseMovedEvent&)> handler;
	EXPECT_CALL(handler, Call(_)).Times(0);

	KeyPressedEvent e(1, 0);
	EventDispatcher dispatcher(e);
	EXPECT_FALSE(dispatcher.Dispatch<MouseMovedEvent>(handler.AsStdFunction()));
	EXPECT_FALSE(e.Handled);
}
