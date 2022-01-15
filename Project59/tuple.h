#pragma once
#ifndef MY_TUPLE
#define MY_TUPLE

#include <concepts>
#include <utility>

namespace
{
	template<std::size_t _index, typename _T>
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

	template<std::size_t... _indexes, typename... _Ts>
	class _tuple_impl<std::index_sequence<_indexes...>, _Ts...>
		: public _tuple_unit<_indexes, _Ts>... 
	{
	public:
		template<typename... _IniTs>
		_tuple_impl(_IniTs&&... _elem) : _tuple_unit<_indexes, _Ts>(std::forward<_IniTs>(_elem))... {}
	};

	//根据显式提供的index推断应转型到的包装类基类
	template<std::size_t _index, typename _T>
	_T& _get_impl(_tuple_unit<_index, _T>& _casted_tuple_impl)
	{
		return _casted_tuple_impl._get_elem();
	}

	//根据显式提供的Type推断应转型到的包装类基类
	template<typename _T, std::size_t _index>
	_T& _get_impl(_tuple_unit<_index, _T>& _casted_tuple_impl)
	{
		return _casted_tuple_impl._get_elem();
	}

	template<typename _T>
	std::false_type _test(void*);

	template<typename _T, std::size_t _index>
	std::true_type _test(_tuple_unit<_index, _T>* casted);
}

#include <iostream>

namespace my
{
	template<typename... _Ts>
	class tuple;

	template<typename... _IniTs>
	tuple(_IniTs&&... _elem) -> tuple<std::remove_reference_t<_IniTs>...>;

	template<typename... _Ts>
	class tuple
	{
		_tuple_impl<std::index_sequence_for<_Ts...>, _Ts...> _impl;
		template<std::size_t _index, typename _TupleTy>
		friend auto& get(_TupleTy&);
		template<typename _ElemTy, typename _TypleTy>
		friend auto& get(_TypleTy& _tuple);
	public:
		constexpr static std::size_t size = sizeof...(_Ts);
		template<typename... _IniTs>
		tuple(_IniTs&&... _elem) : _impl(std::forward<_IniTs>(_elem)...) {}
	};

	template<std::size_t _index, typename _TupleTy>
	auto& get(_TupleTy& _tuple)
	{
		static_assert(_index < _TupleTy::size, "index out of range.");
		return _get_impl<_index>(_tuple._impl);
	}

	template<typename _ElemTy, typename _TypleTy>
	auto& get(_TypleTy& _tuple)
	{
		static_assert(decltype(_test<_ElemTy>((decltype(_tuple._impl)*)nullptr))::value, "fail to deduce.");
		return _get_impl<_ElemTy>(_tuple._impl);
	}

	template<typename... _IniTs>
	auto make_tuple(_IniTs&&... _elem)
	{
		return tuple(std::forward<_IniTs>(_elem)...);
	}
}

#endif // !MY_TUPLE
