#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <tuple>

namespace __bits_hidden__
{

	template <std::size_t _Size, auto _Value = 1>
	constexpr auto pick_type()
	{
		if constexpr (_Size >  0u && _Size <=  8u)
			return byte{_Value};
		if constexpr (_Size >  8u && _Size <= 16u)
			return word{_Value};
		if constexpr (_Size > 16u && _Size <= 32u)
			return dword{_Value};
		if constexpr (_Size > 32u && _Size <= 64u)
			return qword{_Value};
		static_assert(_Size > 0u, "Can't pick type bigger than smaller then 1bit");
		static_assert(_Size <= 64u, "Can't pick type bigger than bigger then 64bits");
	}

	template <std::size_t _Size>
	constexpr auto pick_posone()
	{
		return pick_type<_Size, 1>();
	}

	template <std::size_t _Size>
	constexpr auto pick_negone()
	{
		if constexpr (_Size >  0u && _Size <=  8u)
			return byte{0xff >> (8 - _Size)};
		if constexpr (_Size >  8u && _Size <= 16u)
			return word{0xffff >> (16 - _Size)};
		if constexpr (_Size > 16u && _Size <= 32u)
			return dword{0xfffffffful >> (32 - _Size)};
		if constexpr (_Size > 32u && _Size <= 64u)
			return qword{0xffffffffffffffffull >> (64 - _Size)};
		static_assert(_Size > 0u, "Can't pick type bigger than smaller then 1bit");
		static_assert(_Size <= 64u, "Can't pick type bigger than bigger then 64bits");
	}

	template <byte _Offset, byte _Length>
	constexpr auto mask ()
	{		
		static_assert(_Length != 0 && _Offset+_Length <= 64u);
		constexpr auto _1 = pick_type<_Offset+_Length, 1>();
		using value_type = decltype(pick_type<_Offset+_Length, 1>());
		if constexpr (_Length == sizeof(_1) * 8u)
			return pick_negone<_Length>();
		else
			return value_type(((_1 << _Length) - _1) << _Offset);
	}

	template<typename T> using ei = std::enable_if_t<std::is_integral_v<T>, int>;


	template <typename Q, std::size_t _Offset, std::size_t _L0, std::size_t... _Length>
	struct Packer
	{
		template <typename _Arg0, typename... _Args>
		static constexpr auto pack(_Arg0 arg0, _Args...args)
		{

			if constexpr (sizeof...(_Length) > 0 && sizeof...(_Args) > 0)
				return ((Q(arg0) & pick_negone<_L0>()) << _Offset) | Packer<Q, _Offset + _L0, _Length...>::pack(args...);
			else
				return Q(arg0) << _Offset;
		}
	};

	template <typename Q, std::size_t _L0, std::size_t... _Length>
	struct Unpacker
	{
		template <typename _Arg0, typename... _Args>
		static constexpr auto unpack(Q q, _Arg0&& arg0, _Args&&...args)
		{
			arg0 = q & mask<0, _L0>();
			if constexpr (sizeof...(_Args) && sizeof...(_Length))
				Unpacker<Q, _Length...>::unpack(q >> _L0, std::forward<_Args>(args)...);
		}
		template <typename _Color, std::size_t... _Index>
		static constexpr auto unpack_tuple(Q q, _Color&& u, std::index_sequence<_Index...>)
		{
			Unpacker<Q, _L0, _Length...>::unpack(q, std::get<_Index>(u)...);
		}
	};

}

namespace bits
{
	template <std::size_t... _Length, typename... _Args>
	constexpr auto pack(_Args... args)
	{
		using type = decltype(__bits_hidden__::pick_type<(0+...+_Length), 0>());
		return __bits_hidden__::Packer<type, 0, _Length...>::pack(args...);
	}

	template <std::size_t... _Length, typename Q, typename... _Args>
	constexpr auto unpack(Q q, _Args&&... args)
	{
		using type = decltype(__bits_hidden__::pick_type<(0+...+_Length), 0>());
		__bits_hidden__::Unpacker<Q, _Length...>::unpack(q, std::forward<_Args>(args)...);
	}

	template <std::size_t... _Length, typename Q>
	constexpr auto unpack_as_tuple(Q q)
	{
		std::tuple result {__bits_hidden__::pick_type<_Length, 0>()...};
		__bits_hidden__::Unpacker<Q, _Length...>::unpack_tuple(q, result, std::make_index_sequence<sizeof...(_Length)>{});
		return result;
	}

	template <std::size_t _Length, typename _Value>
	constexpr auto extract(_Value value, std::size_t offset)
	{
		using value_type = decltype(__bits_hidden__::mask<0, _Length>());
		constexpr const auto m = __bits_hidden__::mask<0, _Length>();
		return value_type (value >> offset) & m;
	}

	template <std::size_t _Length, std::size_t _Offset, typename _Value>
	constexpr auto extract(_Value value)
	{
		using value_type = decltype(__bits_hidden__::mask<0, _Length>());
		constexpr const auto m = __bits_hidden__::mask<0, _Length>();
		return value_type (value >> _Offset) & m;
	}

	template <auto _Mask, typename _ValueA, typename _ValueB>
	constexpr auto splice_mask(_ValueA&& dst, _ValueB&& src)
	{
		return (dst & ~_Mask)|(src & _Mask);
	}

	template <auto _Offset, auto _Size, typename _ValueA, typename _ValueB>
	constexpr auto splice(_ValueA&& dst, _ValueB&& src)
	{		
		constexpr auto _Mask = __bits_hidden__::mask<_Offset, _Size>();
		using type = decltype(_Mask);
		return splice_mask<_Mask>(dst, type(src) << _Offset);
	}


}
