#include "tuple.h"
#include <iostream>
#include <format>
#include <functional>
#include <chrono>

class from_closure
{
	size_t index;
	template<size_t... indexes, typename... Ts>
	void* from_impl(my::tuple<Ts...>& tuple, std::index_sequence<indexes...>)
	{
		static std::function<void* (void)> call_list[my::tuple<Ts...>::size] = {
			[&]() -> void* { return static_cast<void*>(&(my::get<indexes>(tuple))); }...
		};
		if (my::tuple<Ts...>::size <= index) return nullptr;
		return call_list[index]();
	}
public:
	from_closure(size_t index) : index(index) {}

	template<typename TupleType>
	void* from(TupleType& tuple)
	{
		return from_impl(tuple, std::make_index_sequence<TupleType::size>());
	}
};

from_closure get(size_t index)
{
	return from_closure(index);
}

union result
{
	int result1;
	char result2;
	double result3;
};

int main()
{
	auto t = my::make_tuple(1, 'a', 'b', 'c', 'd', 2.0);
	std::wcout << std::format(
		L"my::tuple {{\n   elem[0]: {}\n   elem[1]: {}\n   elem[2]: {}\n   elem[3]: {}\n   elem[4]: {}\n   elem[5]: {}\n  }}",
		my::get<0>(t),
		my::get<1>(t),
		my::get<2>(t),
		my::get<3>(t),
		my::get<4>(t),
		my::get<5>(t));
	std::wcout << std::endl;
	std::wcout << std::format(L"get<double> -> {}\n", my::get<double>(t));
	std::wcout << std::format(L"sizeof(int)={}\nsizeof(char)={}\nsizeof(double)={}\nsizeof(tuple)={}\n",
		sizeof(int),
		sizeof(char),
		sizeof(double),
		sizeof(t));
	std::wcout << *static_cast<char*>(get(2).from(t));

	//{
	//	using namespace std::chrono;

	//	microseconds time_total = microseconds::zero();
	//	constexpr auto max_loop = 1000;

	//	for (int loop = 0; loop < max_loop; ++loop)
	//	{
	//		auto start = system_clock::now();

	//		for (int count = 0; count < 100000; ++count)
	//		{
	//			get(count % (t.size + 1)).from(t);
	//		}

	//		auto end = system_clock::now();
	//		auto time_pasted = duration_cast<microseconds>(end - start);
	//		time_total += time_pasted;
	//		std::cout << time_pasted << " ";
	//		if (loop % 10 == 9) std::cout << std::endl;
	//	}

	//	std::cout << std::format("Average duration: {}\n", time_total / max_loop);
	//}

	return 0;
}