#pragma once
#ifndef MY_TUPLE
#define MY_TUPLE

#include <concepts>

namespace
{
	template<size_t _index, typename _T>
	class _tuple_unit
	{
		_T _elem;
	public:
		template<typename _IniT>
		_tuple_unit(_IniT&& _ini_elem) : _elem(std::forward<_IniT>(_ini_elem)) {}

		_T& _get_elem() { return _elem; }
	};

	template<typename _index_sequence, typename... _Ts>
	class _tuple_impl;

	template<size_t... _indexes, typename... _Ts>
	class _tuple_impl<std::index_sequence<_indexes...>, _Ts...>
		: public _tuple_unit<_indexes, _Ts>... 
	{
	public:
		template<typename... _IniTs>
		_tuple_impl(_IniTs&&... _elem) : _tuple_unit<_indexes, _Ts>(std::forward<_IniTs>(_elem))... {}
	};

	//根据显式提供的index推断应转型到的包装类基类
	template<size_t _index, typename _T>
	_T& _get_impl(_tuple_unit<_index, _T>& _casted_tuple_impl)
	{
		return _casted_tuple_impl._get_elem();
	}
}

namespace my
{
	template<typename... _Ts>
	class tuple;

	template<size_t _index, typename _RetT, typename... _Ts> _RetT& get(tuple<_Ts...>&);

	template<typename... _IniTs>
	tuple(_IniTs&&... _elem) -> tuple<std::remove_reference_t<_IniTs>...>;

	template<typename... _Ts>
	class tuple
	{
		_tuple_impl<std::index_sequence_for<_Ts...>, _Ts...> _impl;
		template<size_t _index, typename... _Ts>
		friend auto& get(tuple<_Ts...>&);
	public:
		constexpr static size_t size = sizeof...(_Ts);
		template<typename... _IniTs>
		tuple(_IniTs&&... _elem) : _impl(std::forward<_IniTs>(_elem)...) {}
	};

	template<size_t _index, typename... _Ts>
	auto& get(tuple<_Ts...>& _tuple)
	{
		static_assert(_index >= 0 && _index < tuple<_Ts...>::size);
		return _get_impl<_index>(_tuple._impl);
	}

	template<typename... _IniTs>
	auto make_tuple(_IniTs&&... _elem)
	{
		return tuple(std::forward<_IniTs>(_elem)...);
	}
}

#endif // !MY_TUPLE
