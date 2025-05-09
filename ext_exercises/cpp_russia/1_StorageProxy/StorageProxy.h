#pragma once

#include <cstddef>
#include <atomic>
#include <unordered_map>
#include <any>
#include <vector>

namespace storageProxy
{

  using id_index_t = std::size_t;
  namespace details
  {
    struct type_index final
    {
      [[nodiscard]] static id_index_t next() noexcept 
      {
        static std::atomic<id_index_t> value{};
        return value++;
      }
    };
  }

  template <typename Type>
  struct type_index final
  {
      [[nodiscard]] static id_index_t value() noexcept
      {
        static const id_index_t value = details::type_index::next();
        return value;
      }

      [[nodiscard]] constexpr operator id_index_t() const noexcept
      {
        return value();
      }
  };


  using pool_type = std::unordered_map<id_index_t, std::any>;
  
  template <typename T>
  concept DecayType = std::same_as<T, std::decay_t<T>>;
  
  class registry
  {
    private:
      template <typename T>
      [[nodiscard]] auto& storage(id_index_t id = type_index<T>())
      {
        using storage_t = std::vector<T>;
        auto& st = m_pool[id];  // any for type T
        if (!st.has_value()) {
          st.emplace<storage_t>();
        }
        return std::any_cast<storage_t &>(st); // std::vector<T>&
      }
      
    public:
      template<typename T, typename... Args>
      requires std::same_as<T, std::decay_t<T>>
      auto& emplace(Args&&... args)
      {
        return storage<T>().emplace_back(T{std::forward<Args>(args)...});
      }

      template <typename T>
      requires std::same_as<T, std::decay_t<T>>
      auto& emplace(T&& arg)
      {
        return storage<T>().emplace_back(std::forward<T>(arg));
      }

    
    private: 
      template <typename T>
      [[nodiscard]] const auto* storage(id_index_t id = type_index<T>()) const
      {
        using storage_t = std::vector<T>;
        if (const auto it = m_pool.find(id); it != m_pool.cend())
        {
          auto& store_ref = std::any_cast<const storage_t&>(it->second);
          return &store_ref;
        }
        return static_cast<const storage_t*>(nullptr);
      }

      template <typename T>
      [[nodiscard]] bool has() const
      {
        auto *cpool = storage<std::decay_t<T>>();
        return cpool && !cpool->empty();
      }

    public:
      template <typename... T>
      requires (std::same_as<T, std::decay_t<T>> && ...)
      [[nodiscard]] bool all_of() const
      {
        return (has<T>() && ...);
      }

      template <typename... T>
      requires (std::same_as<T, std::decay_t<T>> || ...)
      [[nodiscard]] bool any_of() const
      {
        return (has<T>() || ...);
      }

      template <typename T>
      [[nodiscard]] const auto& all() const
      {
        return *storage<T>();
      }

      template <typename... T>
      [[nodiscard]] decltype(auto) last() const
      {
        if constexpr (sizeof...(T) == 1u) {
          return (storage<T>()->back(), ...);
        }
        else {
          return std::forward_as_tuple(last<T>()...);
        }
      }

    private:
      pool_type m_pool;
  };
}
