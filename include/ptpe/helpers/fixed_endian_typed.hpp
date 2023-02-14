/**
 * @file fixed_endian_typed.hpp
 * @author Mattis Kieffer (mattis.kieffer@hotmail.de)
 * @brief Endian-Safe wrapper templates
 * @version 0.1
 * @date 2022-11-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef PTPE_FIXED_ENDIAN_TYPED_HPP
#define PTPE_FIXED_ENDIAN_TYPED_HPP
#include <ptpe/interface/platform.hpp>
namespace ptpe
{
  /**
   * @brief Helper to determine machine endianness
   *
   */
  class FindEndian
  {
    union Finder
    {
      int i;
      char c[sizeof(int)];
    };
    static constexpr Finder ef{.i = 1};

  public:
    static const bool isBig;
  };

  enum class Endianness
  {
    BIG,
    LITTLE
  };

  /**
   * @brief constant defining the host machines endianness
   *
   */
  const Endianness HostEndianness = FindEndian::isBig ? Endianness::BIG : Endianness::LITTLE;


  template <typename T, Endianness storageByteOrder>
  class FixedEndian
  {

    [[nodiscard]] static constexpr T swap(const T &arg) noexcept
    {
      if constexpr(sizeof(T)==2){
        return __builtin_bswap16(arg);
      }
      else if constexpr(sizeof(T)==4){
        return __builtin_bswap32(arg);
      }
      else if constexpr(sizeof(T)==8){
        return __builtin_bswap64(arg);
      }
      // default byteswap implementation if no builtin can be used
      T ret;
      char *dst = reinterpret_cast<char *>(&ret);
      const char *src = reinterpret_cast<const char *>(&arg + 1);

      for (size_t i = 0; i < sizeof(T); i++)
        *dst++ = *--src;

      return ret;
    };

    T value; ///< the underlying value stored in the desired storage byte order
  public:
    FixedEndian() {}
    constexpr FixedEndian(const T &t) : value(storageByteOrder == HostEndianness ? t : swap(t)) {}
    constexpr operator T() const { return storageByteOrder == HostEndianness ? value : swap(value); }
  } PACKED_STRUCT;

  template <typename T>
  class BigEndian : public FixedEndian<T, Endianness::BIG>
  {
  public:
    BigEndian() {}
    BigEndian(const T &t) : FixedEndian<T, Endianness::BIG>(t) {}
  } PACKED_STRUCT;

  template <typename T>
  class LittleEndian : public FixedEndian<T, Endianness::LITTLE>
  {
  public:
    LittleEndian() {}
    LittleEndian(const T &t) : FixedEndian<T, Endianness::LITTLE>(t) {}
  } PACKED_STRUCT;
};


#endif