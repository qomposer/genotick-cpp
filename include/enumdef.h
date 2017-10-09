
#pragma once

#include <stdexcept>

#define UNROLL_CUSTOM_ENUM_MEMBERS(enumName, enumValue, ...) constexpr static enum_type enumName{enum_type::enumName};
#define UNROLL_CUSTOM_ENUM_VALUES(enumName, enumValue, ...) enum_type::enumName,
#define UNROLL_CUSTOM_ENUM_NAMES(enumName, enumValue, ...) #enumName,
#define UNROLL_CUSTOM_ENUM_VALUE(enumName, enumValue, ...) enumName enumValue,
#define UNROLL_CUSTOM_ENUM_META(enumName, enumValue, meta, ...) meta,
#define DEFINE_CUSTOM_ENUM_CLASS(clazz, underlying_t, list, meta_t) \
class clazz { \
public: \
	using ordinal_type = size_t; \
	using underlying_type = underlying_t; \
	using string_type = const char*; \
	using meta_type = meta_t; \
private: \
	enum class enum_type : underlying_type { list(UNROLL_CUSTOM_ENUM_VALUE) }; \
	constexpr static enum_type m_values[] = { list(UNROLL_CUSTOM_ENUM_VALUES) }; \
	constexpr static string_type m_names[] = { list(UNROLL_CUSTOM_ENUM_NAMES) }; \
	constexpr static meta_type m_metas[] = { list(UNROLL_CUSTOM_ENUM_META) }; \
	enum_type m_value; \
public: \
	list(UNROLL_CUSTOM_ENUM_MEMBERS); \
	constexpr clazz(enum_type value) : m_value(value) {} \
	constexpr clazz(const class clazz& other) : clazz(other.m_value) {} \
	constexpr static ordinal_type count() noexcept { \
		return sizeof(m_values) / sizeof(enum_type); \
	} \
	constexpr static class clazz getByOrdinal(ordinal_type ordinal) { \
		if (ordinal < count()) \
			return clazz(m_values[ordinal]); \
		throw std::exception("invalid ordinal number"); \
	} \
	constexpr static class clazz getByValue(underlying_type value) { \
		class clazz instance(static_cast<enum_type>(value)); \
		if (instance.ordinal() < instance.count()) \
			return instance; \
		throw std::exception("invalid enum value"); \
	} \
	static ordinal_type ordinalOf(const class clazz& instance) noexcept { \
		return instance.ordinal(); \
	} \
	static underlying_type valueOf(const class clazz& instance) noexcept { \
		return instance.value(); \
	} \
	static string_type nameOf(const class clazz& instance) noexcept { \
		return instance.name(); \
	} \
	static meta_type metaOf(const class clazz& instance) noexcept { \
		return instance.meta(); \
	} \
	constexpr ordinal_type ordinal() const { \
		for (ordinal_type i = 0, c = count(); i < c; ++i) \
			if (m_value == m_values[i]) \
				return i; \
		throw std::exception("invalid enum value"); \
	} \
	constexpr underlying_type value() const noexcept { \
		return static_cast<underlying_type>(m_value); \
	} \
	constexpr string_type name() const { \
		return m_names[ordinal()]; \
	} \
	constexpr meta_type meta() const { \
		return m_metas[ordinal()]; \
	} \
	constexpr bool operator==(const class clazz& other) const noexcept { \
		return m_value == other.m_value; \
	} \
	constexpr bool operator!=(const class clazz& other) const noexcept { \
		return m_value != other.m_value; \
	} \
	constexpr bool operator<(const class clazz& other) const noexcept { \
		return m_value < other.m_value; \
	} \
	constexpr bool operator>(const class clazz& other) const noexcept { \
		return m_value > other.m_value; \
	} \
	constexpr bool operator<=(const class clazz& other) const noexcept { \
		return m_value <= other.m_value; \
	} \
	constexpr bool operator>=(const class clazz& other) const noexcept { \
		return m_value >= other.m_value; \
	} \
};

/*
This is an enum class macro for enums with ordinal and count awareness.
Additionally it can store meta data as defined in the macro.

enum class FruitColor { Red, Orange, Yellow, Brown };

struct FruitMeta
{
	constexpr FruitMeta(const char* const description, const FruitColor color)
		: description(description)
		, color(color)
	{}
	const char* const description;
	const FruitColor color;
};

#define FRUIT_ENUM_LIST(e) \
	e( Apple     , = 44 , (FruitMeta("Apples are rare"         , FruitColor::Red   )) ) \
	e( Orange    , = 55 , (FruitMeta("Oranges are orange"      , FruitColor::Orange)) ) \
	e( Banana    , = 66 , (FruitMeta("Bananas are long"        , FruitColor::Yellow)) ) \
	e( Pineapple ,      , (FruitMeta("Pineapples are the tasty", FruitColor::Brown )) ) \

DEFINE_CUSTOM_ENUM_CLASS(Fruit, int, FRUIT_ENUM_LIST, FruitMeta)

void sample()
{
	auto orangeOrdinal = Fruit::ordinalOf(Fruit::Orange); // 1
	auto orangeValue = Fruit::valueOf(Fruit::Orange);     // 55
	auto orangeName = Fruit::nameOf(Fruit::Orange);       // "Orange"
	auto orangeMeta = Fruit::metaOf(Fruit::Orange);       // FruitMeta

	// Fruit illegalFruit = 0;                            // compile error
	// Fruit illegalFruit = Fruit::getByOrdinal(5);       // illegal, exception

	Fruit fruit = Fruit::Banana;                          // fruit is Banana
	size_t fruitOrdinal = fruit.ordinal();                // 2
	int fruitValue = fruit.value();                       // 66
	const char* fruitName = fruit.name();                 // "Banana"
	auto description = fruit.meta().description;          // "Bananas are long"
	auto color = fruit.meta().color;                      // FruitColor::Yellow

	Fruit fruit2(Fruit::Apple);

	bool same = (fruit == fruit2);                        // false

	int valueOf = Fruit::valueOf(Fruit::Pineapple);       // 67
	size_t count = Fruit::count();                        // 4

	for (size_t ordinal = 0; ordinal < count; ++ordinal)
	{
		Fruit fruitByOrdinal = Fruit::getByOrdinal(ordinal);
	}
}

The Meta data cannot be omitted.
If there is no meta data necessary, just specify a meaningless literal type like so:

#define DUMMY_ENUM_LIST(e) \
	e(Enum1 , , false) \
	e(Enum2 , , false) \
	e(Enum3 , , false) \
	e(Enum4 , , false) \

DEFINE_CUSTOM_ENUM_CLASS(DummyEnum, int, DUMMY_ENUM_LIST, bool)

*/
