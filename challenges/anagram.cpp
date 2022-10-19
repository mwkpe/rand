union Anagram_bits
{
  Anagram_bits() : all_fields{} {}
  struct
  {
    std::uint32_t a : 2;
    std::uint32_t b : 2;
    std::uint32_t c : 2;
    std::uint32_t d : 2;
    std::uint32_t e : 2;
    std::uint32_t f : 2;
    std::uint32_t g : 2;
    std::uint32_t h : 2;
    std::uint32_t i : 2;
    std::uint32_t j : 2;
    std::uint32_t k : 2;
    std::uint32_t l : 2;
    std::uint32_t m : 2;
    std::uint32_t n : 2;
    std::uint32_t o : 2;
    std::uint32_t p : 2;

    std::uint32_t q : 2;
    std::uint32_t r : 2;
    std::uint32_t s : 2;
    std::uint32_t t : 2;
    std::uint32_t u : 2;
    std::uint32_t v : 2;
    std::uint32_t w : 2;
    std::uint32_t x : 2;
    std::uint32_t y : 2;
    std::uint32_t z : 2;
    std::uint32_t len : 5;
    std::uint32_t overflow : 7;
  } bits;
  struct
  {
    std::uint64_t value;
  } all_fields;
};


struct Anagram_key
{
  explicit Anagram_key(std::string_view word)
  {
    static const std::size_t ascii_offset = 65;

    key.bits.len = word.size();

    for (char letter : word) {
      switch (letter - ascii_offset) {
        case 0: { if (++key.bits.a == 0) key.bits.overflow += 1; } break;
        case 1: { if (++key.bits.b == 0) key.bits.overflow += 2; } break;
        case 2: { if (++key.bits.c == 0) key.bits.overflow += 3; } break;
        case 3: { if (++key.bits.d == 0) key.bits.overflow += 4; } break;
        case 4: { if (++key.bits.e == 0) key.bits.overflow += 5; } break;
        case 5: { if (++key.bits.f == 0) key.bits.overflow += 6; } break;
        case 6: { if (++key.bits.g == 0) key.bits.overflow += 7; } break;
        case 7: { if (++key.bits.h == 0) key.bits.overflow += 8; } break;
        case 8: { if (++key.bits.i == 0) key.bits.overflow += 9; } break;
        case 9: { if (++key.bits.j == 0) key.bits.overflow += 10; } break;
        case 10: { if (++key.bits.k == 0) key.bits.overflow += 11; } break;
        case 11: { if (++key.bits.l == 0) key.bits.overflow += 12; } break;
        case 12: { if (++key.bits.m == 0) key.bits.overflow += 13; } break;
        case 13: { if (++key.bits.n == 0) key.bits.overflow += 14; } break;
        case 14: { if (++key.bits.o == 0) key.bits.overflow += 15; } break;
        case 15: { if (++key.bits.p == 0) key.bits.overflow += 16; } break;
        case 16: { if (++key.bits.q == 0) key.bits.overflow += 17; } break;
        case 17: { if (++key.bits.r == 0) key.bits.overflow += 18; } break;
        case 18: { if (++key.bits.s == 0) key.bits.overflow += 19; } break;
        case 19: { if (++key.bits.t == 0) key.bits.overflow += 20; } break;
        case 20: { if (++key.bits.u == 0) key.bits.overflow += 21; } break;
        case 21: { if (++key.bits.v == 0) key.bits.overflow += 22; } break;
        case 22: { if (++key.bits.w == 0) key.bits.overflow += 23; } break;
        case 23: { if (++key.bits.x == 0) key.bits.overflow += 24; } break;
        case 24: { if (++key.bits.y == 0) key.bits.overflow += 25; } break;
        case 25: { if (++key.bits.z == 0) key.bits.overflow += 26; } break;
        default:;
      }
    }
  }

  Anagram_bits key;
  bool operator==(const Anagram_key &other) const {
    return other.key.all_fields.value == key.all_fields.value;
  }
};


struct Anagram_key_hash_func
{
  std::size_t operator()(const Anagram_key& k) const
  {
    return std::hash<std::uint64_t>{}(k.key.all_fields.value);
  }
};


std::vector<std::vector<std::string>> findAnagrams(std::vector<std::string>& wordlist)
{
  [[maybe_unused]] double speed;

  std::vector<std::vector<std::string>> quickly;
  quickly.reserve(50000);

  std::unordered_map<Anagram_key, std::vector<std::string>, Anagram_key_hash_func> anagram_map;
  anagram_map.reserve(300000);

  for (auto& word : wordlist) {
    anagram_map[Anagram_key{word}].push_back(std::move(word));
  }

  for (auto& anagrams : anagram_map) {
    if (anagrams.second.size() > 1) {
      quickly.push_back(std::move(anagrams.second));
    }
  }

  return quickly;
}
