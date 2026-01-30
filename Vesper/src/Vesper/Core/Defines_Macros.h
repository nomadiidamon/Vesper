#pragma once


#define BIT(x) (1 << x)
/// @brief Binds a member function as an event handler.
#define VZ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
/// @brief Binds the given member function of the Application class as an event handler.
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)


/// @namespace Vesper
/// @brief The main namespace for the Vesper engine.
namespace Vesper
{
	/// @brief A smart pointer type representing exclusive ownership of an object.
	///
	/// @tparam T The type of object to manage.
	/// @note This is an alias for std::unique_ptr.
	template <typename T>
	using Scope = std::unique_ptr<T>;

	/// @brief Creates a Scope (unique_ptr) for the given type and constructor arguments.
	///
	/// @tparam T The type of object to create.
	/// @tparam Args The types of constructor arguments.
	/// @param args The constructor arguments.
	/// @return A Scope (unique_ptr) managing the created object.
	template <typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}


	/// @brief A smart pointer type representing shared ownership of an object.
	///
	/// @tparam T The type of object to manage.
	/// @note This is an alias for std::shared_ptr.
	template <typename T>
	using Ref = std::shared_ptr<T>;

	/// @brief Creates a Ref (shared_ptr) for the given type and constructor arguments.
	///
	/// @tparam T The type of object to create.
	/// @tparam Args The types of constructor arguments.
	/// @param args The constructor arguments.
	/// @return A Ref (shared_ptr) managing the created object.
	template <typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}